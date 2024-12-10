#include "QuizFlow.hpp"

#include "../../Meta/RiddleRender.hpp"
#include "../../Meta/RiddleInformation.hpp"

#include <QQmlEngine>
#include <QJsonObject>
#include <QStack>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Controller {
	QuizFlowAttachment* QuizFlow::qmlAttachedProperties(QObject* object)
	{
		return new QuizFlowAttachment(object);
	}

	QuizFlow::QuizFlow(QQuickItem* parent)
	:
		Repository::Controller::Application(parent),

		_list(nullptr),
		_delegate(nullptr),
		_active_riddle(nullptr),
		_component(nullptr),
		_incubator(*this)
	{

	}

	bool QuizFlow::activate(QQuickItem* item)
	{
		View::RiddleView* const view(qobject_cast<View::RiddleView*>(item));
		if (!view) {
			return false;
		}

		QMap<View::RiddleView*, int>::ConstIterator i(_riddle_map.find(view));
		if (_riddle_map.constEnd() == i) {
			qWarning("Riddle does not exist");
			return false;
		}

		_active_riddle = view;
		qDebug() << "Riddle activated:" << _active_riddle;
		emit activeChanged();
		return true;
	}

	bool QuizFlow::incubate(QuizFlow::Incubator::Method method)
	{
		if (_incubation_queue.isEmpty() || !_list || !_delegate) {
			return false;
		} else if (_component && Incubator::Method::Queue == method) {
			return false;
		} else if (_incubation_queue.head() >= _list->rowCount()) {
			return false;
		}
		QQmlComponent::CompilationMode compilation_mode(QQmlComponent::CompilationMode::PreferSynchronous);
		const QModelIndex index(_list->index(_incubation_queue.first(), 0));
		const QVariant delegate_variant(_list->data(index, Meta::RiddleInformation::Role::Delegate));
		if (_component && _component->parent() == this) {
			_component->deleteLater();
		}
		_component = delegate_variant.canConvert<QQmlComponent*>() ? delegate_variant.value<QQmlComponent*>() : nullptr;
		if (!_component) {
			QUrl scheme_url("qrc:/SmartCare/View/Riddle/UnsupportedRiddle.qml");
			if (Incubator::Method::Queue == method) {
				const QVariant scheme_variant(_list->data(index, Meta::RiddleInformation::Role::Scheme));
				const Core::Meta::Scheme<Meta::RiddleRender>::Iterator r
				(
					Core::Meta::Scheme<Meta::RiddleRender>::Iterate
					(
						scheme_variant.canConvert<Core::Meta::Qd>()
						? Core::Meta::Qd::Make(scheme_variant)
						: Core::Meta::Qd{0}
					)
				);
				if (Core::Meta::Scheme<Meta::RiddleRender>::Exists(r)) {
					scheme_url = r->makeURL();
					compilation_mode = QQmlComponent::CompilationMode::Asynchronous;
				}
			}
			_component = new QQmlComponent(QtQml::qmlEngine(_delegate), scheme_url, compilation_mode, this);
		}

		if (_component->isError()) {
			qCritical() << "Component error:" << _component->errors();
			return incubate(Incubator::Method::Cull);
		} else if (_component->isLoading()) {
			qDebug() << "Component is loading...";
			QObject::connect(_component, &QQmlComponent::statusChanged, this, &QuizFlow::onIncubated);
			return true;
		} else if (!_component->isReady()) {
			qCritical() << "Component error: Unready";
			return incubate(Incubator::Method::Skip);
		} else if (QQmlComponent::CompilationMode::Asynchronous != compilation_mode) {
			if (dequeue(qobject_cast<View::RiddleView*>(_component->create(QtQml::qmlContext(_delegate))))) {
				return true;
			}
			qCritical() << "Component error: Unsupported scheme";
			return incubate(Incubator::Method::Skip);
		}
		return onIncubated(QQmlComponent::Status::Ready);
	}

	bool QuizFlow::dequeue(View::RiddleView* view)
	{
		if (!view || !_list) {
			return false;
		}

		const bool first_riddle(_riddle_map.isEmpty());
		const int index(_incubation_queue.dequeue());
		_riddle_map[view] = index;
		view->setParent(this);
		view->setParentItem(_delegate);
		if (!first_riddle) {
			QQuickItem* container(_riddle_map.firstKey()->parentItem());
			if (container) {
				view->stackAfter(container->childItems().at(index - 1));
			}
		}
		emit stacked(view, index);

		const QVariant variant(_list->data(_list->index(index, 0), Meta::RiddleInformation::Role::Object));
		if (_component && _component->parent() == this) {
			_component->deleteLater();
		}
		_component = nullptr;
		incubate();
		if (view->getModel() && view->getType() != "Custom"
			&&
			!view->getModel()->parse(variant.canConvert<QJsonObject>() ? variant.toJsonObject() : QJsonObject{})) {
			qWarning() << "Model parse error";
			return false;
		}
		return true;
	}

	QAbstractItemModel* QuizFlow::getRiddleList()
	{
		return _list;
	}

	QQuickItem* QuizFlow::getDelegate()
	{
		return _delegate;
	}

	View::RiddleView* QuizFlow::getActive()
	{
		return _active_riddle;
	}

	void QuizFlow::setRiddleList(QAbstractItemModel* model)
	{
		if (_list == model) {
			return ;
		} else if (_list) {
			QObject::disconnect(_list, nullptr, this, nullptr);
			qDeleteAll(findChildren<View::RiddleView*>("", Qt::FindDirectChildrenOnly));
			_incubation_queue.clear();
		}

		_list = model;
		emit riddleListChanged();
		if (!_list) {
			return ;
		}

		QObject::connect(_list, &QAbstractItemModel::modelAboutToBeReset, this, &QuizFlow::onReset);
		QObject::connect(_list, &QAbstractItemModel::rowsInserted, this, &QuizFlow::onRiddleInserted);
		QObject::connect(_list, &QAbstractItemModel::rowsMoved, this, &QuizFlow::onRiddleMoved);
		QObject::connect(_list, &QAbstractItemModel::rowsRemoved, this, &QuizFlow::onRiddleRemoved);

		for (int r = 0; r < _list->rowCount(); ++r) {
			_incubation_queue.append(r);
		}
		incubate();
	}

	void QuizFlow::setDelegate(QQuickItem* delegate)
	{
		if (delegate != _delegate) {
			_delegate = delegate;
			if (_delegate && !_delegate->parentItem()) {
				_delegate->setParentItem(parentItem());
			}
			emit delegateChanged();
			incubate();
		}
	}

	bool QuizFlow::onIncubated(QQmlComponent::Status status)
	{
		if (!_component || _component->isNull()) {
			qCritical() << "Component not found";
			return incubate(Incubator::Method::Skip);
		}
		do {
			if (_component->isError()) {
				qCritical() << "Component error:" << _incubator.errors();
				return incubate(Incubator::Method::Cull);
			} else if (_incubator.isError()) {
				qCritical() << "Incubator error:" << _incubator.errors();
				return incubate(Incubator::Method::Cull);
			} else if (QQmlComponent::Status::Ready == status) {
				qDebug() << "Component is ready and going to be created..." << this;
				_component->create(_incubator, QtQml::qmlContext(this));
				return true;
			} else {
				qCritical() << "Component is not ready";
				return incubate(Incubator::Method::Skip);
			}
		} while (_component->isError() || _incubator.isError());
		return true;
	}

	void QuizFlow::onReset()
	{
		qDeleteAll(findChildren<View::RiddleView*>("", Qt::FindDirectChildrenOnly));
		_incubation_queue.clear();
		for (int r = 0; r < _list->rowCount(); ++r) {
			_incubation_queue.append(r);
		}
		incubate();
	}

	void QuizFlow::onRiddleInserted(const QModelIndex& parent, int first, int last)
	{
		for (int r = first; r <= last; ++r) {
			_incubation_queue.append(r);
		}
		incubate();
	}

	void QuizFlow::onRiddleMoved(const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row)
	{
		/// @todo
	}

	void QuizFlow::onRiddleRemoved(const QModelIndex& parent, int first, int last)
	{
		for (int i = first; i <= last; i++) {
			/// @todo Оптимизировать работу с _riddle_map
			View::RiddleView* view(_riddle_map.key(i, nullptr));

			if (view) {
				_riddle_map.remove(view);
				view->deleteLater();
			}
		}
	}

	QuizFlow::Incubator::Incubator(QuizFlow& flow)
	:
		QQmlIncubator(AsynchronousIfNested),

		_flow(flow)
	{

	}

	QuizFlow::Incubator::~Incubator()
	{

	}

	void QuizFlow::Incubator::statusChanged(QQmlIncubator::Status status)
	{
		switch (status) {
			case Status::Error:
				qCritical() << "Incubator error:" << errors();
				_flow.incubate(Method::Cull);
				break;

			case Status::Ready:
				if (!_flow.dequeue(qobject_cast<View::RiddleView*>(object()))) {
					_flow.incubate(Method::Skip);
				}

			default:
				qDebug() << "Incubator status changed:" << status;
		}
	}

	QuizFlowAttachment::QuizFlowAttachment(QObject* parent)
	:
		QObject(parent),

		_riddle_view(nullptr)
	{
		connect();
	}

	void QuizFlowAttachment::connect()
	{
		QObject::disconnect(this);
		removeEventFilter(_riddle_view);

		QStack<View::RiddleView*> view_stack;

		QObject* parent_object(parent());
		QQuickItem* parent_item(nullptr);

		while (parent_object) {
			parent_item = qobject_cast<QQuickItem*>(parent_object);

			if (parent_item) {
				break;
			}

			parent_object = parent_object->parent();
		}

		while (parent_item) {
			QObject::connect(parent_item, &QQuickItem::parentChanged, this, &QuizFlowAttachment::connect);
			view_stack.push(qobject_cast<View::RiddleView*>(parent_item));
			if (!view_stack.top()) {
				view_stack.pop();
			}
			parent_item = parent_item->parentItem();
		};
		_riddle_view = view_stack.isEmpty() ? nullptr : view_stack.top();
		if (!_riddle_view) {
			return ;
		}
		_riddle_view->installEventFilter(this);
		QuizFlow* flow(qobject_cast<QuizFlow*>(_riddle_view->parent()));
		if (flow) {
			QObject::connect(flow, &QuizFlow::activeChanged, this, &QuizFlowAttachment::activeChanged);
			QObject::connect(this, &QuizFlowAttachment::nextRequested, flow, &QuizFlow::nextRequested);
			QObject::connect(this, &QuizFlowAttachment::previousRequested, flow, &QuizFlow::previousRequested);
		}
	}

	bool QuizFlowAttachment::eventFilter(QObject* target, QEvent* event)
	{
		if (event->type() == QEvent::ParentChange) {
			connect();
		}
		return QObject::eventFilter(target, event);
	}

	bool QuizFlowAttachment::isActive() const
	{
		if (!_riddle_view) {
			return false;
		}
		QuizFlow* flow(qobject_cast<QuizFlow*>(_riddle_view->parent()));
		return flow ? flow->getActive() == _riddle_view : false;
	}
} } } }
