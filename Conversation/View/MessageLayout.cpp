#include "MessageLayout.hpp"

#include "../../Core/Meta/Scheme.hpp"

#include <QtQml>
#include <QQmlContext>
#include <QQmlEngine>
#include <QtGlobal>

#include <QSGSimpleRectNode>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	MessageLayout::MessageLayout(QQuickItem* parent)
	:
		QQuickPaintedItem(parent),

		_component(nullptr),
		_incubator(this),
		_view(nullptr),
		_type(RenderLayout::FeedLayout),
		_side(Side::Center),
		_view_minimum_width(80),
		_view_minimum_height(40),
		_image_width(40),
		_status_height(20)
	{
		setHeight(_view_minimum_height + _status_height);
	}

	MessageLayout::~MessageLayout()
	{

	}

	void MessageLayout::paint(QPainter* painter)
	{
		if (!hasView()) {
			return ;
		}

		qreal status_padding(0);

		QPen pen;
		painter->setRenderHints(QPainter::Antialiasing, true);

		if (!_image.isNull()) {
			pen.setStyle(Qt::NoPen);
			painter->setPen(pen);
			QBrush brush(_image);
			painter->setBrush(brush);

			/// @todo красивенько приклеить картинку, у которой с одной стороны углы скошены, а с другой приклеены с _view
			if (_flag_set.testFlag(Flag::RightSideImage)) {
				painter->drawEllipse(width() - _image_width, 0, _image_width, _image_width);
			} else if (_flag_set.testFlag(Flag::LeftSideImage)) {
				painter->drawEllipse(0, 0, _image_width, _image_width);
				status_padding = _image_width + 4;
			}
		}

		QStringList status_list;
		if (hasModel()) {
			if (_flag_set.testFlag(Flag::StateLabel)) {
				if (getModel()->isFaulty()) {
					status_list << tr("Not sent");
					status_list << tr("Error").prepend("(").append("🚩)");
				} else if (getModel()->isGlobal()) {
					if (getModel()->inProgress()) {
						status_list << tr("Downloading...");
						status_list << QString(" %1%").arg(getModel()->getProgress());
					} else {
						status_list << tr("Sent");
						QStringList modification_list;
						if (_flag_set.testFlag(Flag::EditModifier) && (getModel()->getModificationDateTime() > getModel()->getPostDateTime())) {
							modification_list << tr("Edited");
						}
						if (_flag_set.testFlag(Flag::ReadModifier) && getModel()->isRead()) {
							modification_list << tr("Read");
						} else if (_flag_set.testFlag(Flag::ReceiveModifier) && getModel()->isReceived()) {
							modification_list << tr("Received");
						}
						if (!modification_list.isEmpty()) {
							status_list << modification_list.join(", ").prepend("(").append(")");
						}
					}
				} else if (getModel()->isLocal()) {
					if (getModel()->inProgress()) {
						status_list << tr("Uploading...");
						status_list << QString(" %1%").arg(getModel()->getProgress());
					} else if (getModel()->getProgress() == 100) {
						status_list << tr("Loaded");
					} else {
						status_list << tr("Not sent");
						status_list << tr("Error").prepend("(").append("🚩)");
					}
				}
			}
			if (_flag_set.testFlag(Flag::DateTimeLabel)) {
				status_list << getModel()->getActualDateTime().toString("d MMM hh:mm");
			}
		} else {
			status_list << tr("Loading...");
		}

		if (!status_list.isEmpty()) {
			pen.setStyle(Qt::SolidLine);
			pen.setColor(Qt::lightGray);
			pen.setCapStyle(Qt::RoundCap);

			painter->setPen(pen);
			painter->setFont(QFont(painter->font().family(), _status_height / 2));
			painter->drawText(QRectF{status_padding, height() - _status_height, width(), _status_height}, getSide() == Side::Right ? Qt::AlignRight : Qt::AlignLeft, status_list.join(' '));
		}
		align();
	}

	QSGNode* MessageLayout::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData* data)
	{
		return QQuickPaintedItem::updatePaintNode(node, data);

		if (!_flag_set.testFlag(Flag::BlurEffect)) {
			return QQuickPaintedItem::updatePaintNode(node, data);
		}

		QSGSimpleRectNode* n(static_cast<QSGSimpleRectNode *>(node));
		if (!n && (width() <= 0 || height() <= 0)) {
			return nullptr;
		} else if (!n) {
			n = new QSGSimpleRectNode();
			n->setColor(Qt::red);
		}
		n->setRect(0, 0, width(), height());
		return n;
	}

	bool MessageLayout::render(Cadabra::Queen::Core::Meta::Qd scheme, Cadabra::Queen::Conversation::RenderLayout type)
	{
		/// @note Если это re-render
		if (_view) {
			QObject::disconnect(_view, &MessageView::completed, this, &MessageLayout::viewCompleted);
			QObject::disconnect(_view, &MessageView::held, this, &MessageLayout::viewHeld);

			QObject::disconnect(_view, &MessageView::heightChanged, this, &MessageLayout::onViewHeightChanged);
			QObject::disconnect(_view, &MessageView::widthChanged, this, &MessageLayout::onViewWidthChanged);
			resetHeight();

			if (hasModel()) {
				QObject::disconnect(getModel(), &Model::AbstractMessage::modified, this, &MessageLayout::modified);
				QObject::disconnect(getModel(), &Model::AbstractMessage::imported, this, &MessageLayout::imported);
				QObject::disconnect(getModel(), &Model::AbstractMessage::delivered, this, &MessageLayout::onModelDelivered);
				QObject::disconnect(getModel(), &Model::AbstractMessage::progressed, this, &MessageLayout::onModelProgressed);
			}
			_view->setParent(nullptr);
			_view->setParentItem(nullptr);
			_view->setLayout(nullptr);
			delete _view;
			_view = nullptr;
		}

		if (_component) {
			delete _component;
			_component = nullptr;
		}

		_incubator.clear();

		QQmlEngine* qml_engine(QtQml::qmlEngine(this));
		if (nullptr == qml_engine) {
			return false;
		}

		const QQmlComponent::CompilationMode compilation_mode((scheme.isAny({0, -1}) || !_flag_set.testFlag(Flag::AsyncRender)) ? QQmlComponent::CompilationMode::PreferSynchronous : QQmlComponent::CompilationMode::Asynchronous);
		const Core::Meta::Scheme<Conversation::Meta::MessageRender>::Iterator r(Core::Meta::Scheme<Conversation::Meta::MessageRender>::Iterate(scheme));
		_component = new QQmlComponent(
										qml_engine,
										Core::Meta::Scheme<Conversation::Meta::MessageRender>::Exists(r) ? r->makeURL(type) : QUrl("qrc:/Conversation/View/Message/UnsupportedMessage.qml"),
										compilation_mode,
										this
		);
		_type = type;

		if (_component->isError()) {
			qCritical() << "Component error:" << _component->errors();
			return render(-1, type);
		} else if (_component->isLoading()) {
			qDebug() << "Component is loading...";
			QObject::connect(_component, &QQmlComponent::statusChanged, this, &MessageLayout::incubate);
			return true;
		} else if (!_component->isReady()) {
			qCritical() << "Component error: Unready";
			return render(0, type);
		} else if (QQmlComponent::CompilationMode::Asynchronous != compilation_mode) {
			if (attach(qobject_cast<MessageView*>(_component->create(QtQml::qmlContext(this))))) {
				return true;
			}
			qCritical() << "Component error: Unsupported scheme";
			return render(0, type);
		}
		qDebug() << "Component is ready?" << _component->status() << r->getName();
		return incubate(QQmlComponent::Status::Ready);
	}

	bool MessageLayout::load(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, Cadabra::Queen::Conversation::Meta::Message::Id id, QUuid uid, Cadabra::Queen::Conversation::Meta::Message::Id reply_id, Cadabra::Queen::Repository::Meta::Subject::Id subject_id, QJsonObject body, QDateTime post_time, QDateTime modification_time)
	{
		Model::AbstractMessage* model(_view ? _view->getModel() : nullptr);
		if (!model || !channel_application) {
			return false;
		} else if (!model->import(channel_application, id, uid, reply_id, subject_id, body, post_time, modification_time)) {
			return false;
		}

		if ((Side::Left == _side && _flag_set.testFlag(Flag::LeftSideImage)) || (Side::Right == _side && _flag_set.testFlag(Flag::RightSideImage))) {
			const QSharedPointer<Repository::Net::ObjectFile> image_file(channel_application->getDescriptor()->getRealm()->getHierarchy()->loadSubjectPicture(subject_id));
			if (!image_file->getData().isEmpty()) {
				_image = QImage::fromData(image_file->getData()).scaledToHeight(_image_width, Qt::SmoothTransformation);
			}
		}

		return true;
	}

	bool MessageLayout::incubate(QQmlComponent::Status status)
	{
		if (!_component || _component->isNull()) {
			qCritical() << "Component error: Not found";
			return render(0, _type);
		}
		do {
			if (_component->isError()) {
				qCritical() << "Component error:" << _incubator.errors();
				return render(-1, _type);
			} else if (_incubator.isError()) {
				qCritical() << "Incubator error:" << _incubator.errors();
				return render(-1, _type);
			} else if (QQmlComponent::Status::Ready == status) {
				qDebug() << "Component" << this << "is ready and going to be created";
				_component->create(_incubator, QtQml::qmlContext(this));
			} else {
				qCritical() << "Component error: Not ready";
				return render(0, _type);
			}
		} while (_component->isError() || _incubator.isError());
		return true;
	}

	bool MessageLayout::attach(MessageView* view)
	{
		qDebug() << "Attaching...";
		if (!view) {
			return false;
		}

		_view = view;
		_view->setParent(this);
		_view->setParentItem(this);
		_view->setLayout(this);

		/// @bug too late for completed
		QObject::connect(_view, &MessageView::completed, this, &MessageLayout::viewCompleted);
		QObject::connect(_view, &MessageView::held, this, &MessageLayout::viewHeld);

		QObject::connect(_view, &MessageView::heightChanged, this, &MessageLayout::onViewHeightChanged);
		QObject::connect(_view, &MessageView::widthChanged, this, &MessageLayout::onViewWidthChanged);

		setHeight(_view->height() + _status_height);
		_view->setY(0);
		_view->setX(0);
		align();

		if (hasModel()) {
			/// @todo Принять во внимание, что может случится modelChanged
			QObject::connect(getModel(), &Model::AbstractMessage::modified, this, &MessageLayout::modified);
			QObject::connect(getModel(), &Model::AbstractMessage::imported, this, &MessageLayout::imported);
			QObject::connect(getModel(), &Model::AbstractMessage::progressed, this, &MessageLayout::onModelProgressed);
			QObject::connect(getModel(), &Model::AbstractMessage::delivered, this, &MessageLayout::onModelDelivered);
		}

		emit rendered();
		return true;
	}

	void MessageLayout::align()
	{
		switch (getSide()) {
			case Side::Right:
				_view->setX(width() - _view->width() - (_flag_set.testFlag(Flag::RightSideImage) && !_image.isNull() ? (_image_width - 4) : 0));
				break;

			case Side::Center:
				_view->setX(_view->width() / 2);
				break;

			case Side::Left:
				_view->setX(_flag_set.testFlag(Flag::LeftSideImage) && !_image.isNull() ? (_image_width + 4) : 0);
				break;
		}
	}

	Cadabra::Queen::Conversation::Model::AbstractMessage* MessageLayout::getModel() const
	{		
		return _view ? _view->getModel() : nullptr;
	}

	Cadabra::Queen::Conversation::View::MessageView* MessageLayout::getView() const
	{
		return _view;
	}

	QDateTime MessageLayout::getActualDateTime() const
	{
		Cadabra::Queen::Conversation::Model::AbstractMessage* model(getModel());
		return nullptr == model ? QDateTime() : model->getActualDateTime();
	}

	const MessageLayout::Side& MessageLayout::getSide() const
	{
		return _side;
	}

	const QFlags<MessageLayout::Flag>& MessageLayout::getFlagSet() const
	{
		return _flag_set;
	}

	qreal MessageLayout::getViewMinimumWidth() const
	{
		return _view_minimum_width;
	}

	qreal MessageLayout::getViewMinimumHeight() const
	{
		return _view_minimum_height;
	}

	qreal MessageLayout::getImageWidth() const
	{
		return _image_width;
	}

	qreal MessageLayout::getStatusHeight() const
	{
		return _status_height;
	}

	void MessageLayout::setSide(Side value)
	{
		if (value != _side) {
			_side = value;
			emit sideChanged();
			update(QRect(0, 0, width(), height()));
		}
	}

	void MessageLayout::setFlagSet(Flags set)
	{
		if (_flag_set != set) {
			_flag_set = set;
			emit flagsetChanged();
			update(QRect(0, 0, width(), height()));
		}
	}

	void MessageLayout::setViewMinimumWidth(qreal value)
	{
		if (_view_minimum_width > value || _view_minimum_width < value) {
			_view_minimum_width = value;
			emit viewWidthMinimized();
		}
	}

	void MessageLayout::setViewMinimumHeight(qreal value)
	{
		if (_view_minimum_height > value || _view_minimum_height < value) {
			_view_minimum_height = value;
			emit viewHeightMinimized();
		}
	}

	void MessageLayout::setImageWidth(qreal value)
	{
		if (_image_width != value) {
			_image_width = value;
			emit imageWidthChanged();
		}
	}

	void MessageLayout::setStatusHeight(qreal value)
	{
		if (_status_height != value) {
			_status_height = value;
			emit statusHeightChanged();
		}
	}

	bool MessageLayout::hasModel() const
	{
		return getModel() != nullptr;
	}

	bool MessageLayout::hasView() const
	{
		return _view != nullptr;
	}

	void MessageLayout::onViewHeightChanged()
	{
		setHeight(_view->height() + _status_height);
		update(QRect(0, 0, width(), height()));
	}

	void MessageLayout::onViewWidthChanged()
	{
		align();
		update(QRect(0, 0, width(), height()));
	}

	void MessageLayout::onModelProgressed()
	{
		/// @todo Есть смысл отрисовывать только статус
		update(QRect(0, 0, width(), height()));
	}

	void MessageLayout::onModelDelivered()
	{
		/// @todo Есть смысл отрисовывать только статус
		update(QRect(0, 0, width(), height()));
	}

	MessageLayout::Incubator::Incubator(MessageLayout* layout)
	:
		QQmlIncubator(AsynchronousIfNested),

		_layout(layout)
	{

	}

	MessageLayout::Incubator::~Incubator()
	{

	}

	void MessageLayout::Incubator::statusChanged(QQmlIncubator::Status status)
	{
		switch (status) {
			case Status::Error:
				qCritical() << "Incubator error:" << errors();
				if (!_layout.isNull()) {
					_layout->render(-1, _layout->_type);
				}
				break;

			case Status::Ready:
				qDebug() << "Incubator status changed: Ready";
				if (!_layout.isNull() && !_layout->attach(qobject_cast<MessageView*>(object()))) {
					_layout->render(0, _layout->_type);
				}

			default:
				qDebug() << "Incubator status changed:" << status;
		}
	}
} } } }
