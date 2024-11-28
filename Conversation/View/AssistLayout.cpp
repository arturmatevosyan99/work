#include "AssistLayout.hpp"

#include "../../Core/Meta/Scheme.hpp"
#include "../Meta/AssistRender.hpp"

#include <QQmlContext>
#include <QQmlEngine>
#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	AssistLayout::AssistLayout(QQuickItem* parent)
	:
		QQuickItem(parent),
		_scheme(0),
		_view(nullptr)
	{

	}

	AssistLayout::~AssistLayout()
	{
	}

	void AssistLayout::render(Cadabra::Queen::Core::Meta::Qd scheme)
	{
		if (_view) {
			QObject::disconnect(_view, &AssistView::heightChanged, this, &AssistLayout::onViewHeightChanged);
			QObject::disconnect(_view, &AssistView::widthChanged, this, &AssistLayout::onViewWidthChanged);
			if (hasModel()) {
				QObject::disconnect(getModel(), &Model::AbstractAssist::imported, this, &AssistLayout::modelImported);
			}
			delete _view;
			_view = nullptr;
			emit viewChanged();
		}

		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Iterator r(Core::Meta::Scheme<Conversation::Meta::AssistRender>::Iterate(scheme));
		QQmlComponent component(
								qmlContext(this)->engine(),
								Core::Meta::Scheme<Conversation::Meta::AssistRender>::Exists(r) ? r->makeURL() : QUrl("qrc:/Conversation/View/Assist/UnsupportedAssist.qml"),
								QQmlComponent::CompilationMode::PreferSynchronous,
								this
		);
		_view = qobject_cast<AssistView*>(component.create());
		if (!_view) {
			if (component.isLoading()) {
				qDebug() << "QML is loading...";
			} else if (component.isError()) {
				qCritical() << "QML error during render is occured:" << component.errors();
				if (_scheme.value == -1) {
					return ;
				}
				_scheme = -1;
				return render(-1);
			} else {
				qCritical() << "Unsupported assist";
				if (_scheme.value == 0) {
					return ;
				}
				_scheme = 0;
				return render(0);
			}
		}
		emit viewChanged();

		_scheme = scheme;
		_view->setLayout(this);
		_view->setX(0);
		_view->setY(0);
		QObject::connect(_view, &AssistView::heightChanged, this, &AssistLayout::onViewHeightChanged); onViewHeightChanged();
		QObject::connect(_view, &AssistView::widthChanged, this, &AssistLayout::onViewWidthChanged); onViewWidthChanged();

		if (hasModel()) {
			QObject::connect(getModel(), &Model::AbstractAssist::imported, this, &AssistLayout::modelImported);
		}
		emit rendered();
	}

	void AssistLayout::reset()
	{
		render(0);
	}

	Cadabra::Queen::Conversation::Model::AbstractAssist* AssistLayout::getModel() const
	{		
		return _view ? _view->getModel() : nullptr;
	}

	Cadabra::Queen::Conversation::View::AssistView* AssistLayout::getView() const
	{
		return _view;
	}

	void AssistLayout::onViewWidthChanged()
	{
		setImplicitWidth(_view->width());
	}

	void AssistLayout::onViewHeightChanged()
	{
		setImplicitHeight(_view->height());
	}

	bool AssistLayout::hasModel() const
	{
		return getModel() != nullptr;
	}

	bool AssistLayout::hasView() const
	{
		return _view != nullptr;
	}
} } } }
