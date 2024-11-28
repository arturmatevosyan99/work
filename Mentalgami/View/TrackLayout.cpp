#include "TrackLayout.hpp"

#include "../Meta/TrackRender.hpp"
#include "../Model/Track/VideoTrack.hpp"
#include "../Model/Track/WebTrack.hpp"
#include "../Model/Track/PDFTrack.hpp"
#include "../Model/Track/MSWordTrack.hpp"
#include "../Model/Track/MSExcelTrack.hpp"
#include "../Model/Track/HTMLTrack.hpp"
#include "../Model/Track/CalendarTrack.hpp"
#include "../Model/Track/TelegramTrack.hpp"

#include "../../Core/Meta/Scheme.hpp"

#include <QMap>
#include <QQmlContext>
#include <QQmlEngine>
#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace View {
	TrackLayout::TrackLayout(QQuickItem* parent)
	:
		QQuickItem(parent),

		_scheme(0),
		_view(nullptr)
	{

	}

	TrackLayout::~TrackLayout()
	{
	}

	bool TrackLayout::represent(const QString& content_type, TrackRenderLayout type)
	{
		/// @todo QQmlPropertyMap for specific
		static const QMap<QString, Core::Meta::Scheme<Meta::TrackRender>::Id> TypeMap = {
			{"video/x-cube", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::VideoTrack>()},
			{"application/html", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::WebTrack>()},
			{"application/pdf", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::PDFTrack>()},
			{"application/msword", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::MSWordTrack>()},
			{"application/vnd.m.ms-excel", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::MSExcelTrack>()},
			{"application/rtf", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::PDFTrack>()},
			{"application/x-telegram", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::TelegramTrack>()},
			{"text/html", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::HTMLTrack>()},
			{"text/calendar", Core::Meta::Scheme<Meta::TrackRender>::Ident<Model::CalendarTrack>()}
		};
		QMap<QString, Core::Meta::Scheme<Meta::TrackRender>::Id>::ConstIterator t(TypeMap.find(content_type));
		if (TypeMap.constEnd() == t) {
			return false;
		}
		render(t.value(), type);
		return true;
	}

	void TrackLayout::render(Cadabra::Queen::Core::Meta::Qd scheme, TrackRenderLayout type)
	{
		if (_view) {
			QObject::disconnect(_view, &TrackView::heightChanged, this, &TrackLayout::onViewHeightChanged);
			QObject::disconnect(_view, &TrackView::widthChanged, this, &TrackLayout::onViewWidthChanged);
			if (hasModel()) {
				QObject::disconnect(getModel(), &Model::AbstractTrack::imported, this, &TrackLayout::modelImported);
			}
			delete _view;
			_view = nullptr;
			emit viewChanged();
		}
		//Cadabra::Queen::Mentalgami::TrackLayout type
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Iterator r(Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Iterate(scheme));
		QQmlComponent component(
								qmlContext(this)->engine(),
								Core::Meta::Scheme<Meta::TrackRender>::Exists(r) ? r->makeURL(type) : QUrl("qrc:/Mentalgami/View/Track/UnsupportedTrack.qml"),
								QQmlComponent::CompilationMode::PreferSynchronous,
								this
		);
		_view = qobject_cast<TrackView*>(component.create(qmlContext(this)));
		if (!_view) {
			if (component.isLoading()) {
				qDebug() << "QML is loading...";
				/// @xxx Иначе крешнемся же на _view->...
				return ;
			} else if (component.isError()) {
				qCritical() << "QML error during render is occured:" << component.errors();
				if (_scheme.value == -1) {
					return ;
				}
				_scheme = -1;
				return render(-1, type);
			} else {
				qCritical() << "Unsupported track";
				if (_scheme.value == 0) {
					return ;
				}
				_scheme = 0;
				return render(0, type);
			}
		}

		_scheme = scheme;
		_view->setLayout(this);
		_view->setX(0);
		_view->setY(0);

		emit viewChanged();

		QObject::connect(_view, &TrackView::heightChanged, this, &TrackLayout::onViewHeightChanged); onViewHeightChanged();
		QObject::connect(_view, &TrackView::widthChanged, this, &TrackLayout::onViewWidthChanged); onViewWidthChanged();

		if (hasModel()) {
			QObject::connect(getModel(), &Model::AbstractTrack::imported, this, &TrackLayout::modelImported);
		}
		emit rendered();
	}

	Cadabra::Queen::Mentalgami::Model::AbstractTrack* TrackLayout::getModel() const
	{		
		return _view ? _view->getModel() : nullptr;
	}

	Cadabra::Queen::Mentalgami::View::TrackView* TrackLayout::getView() const
	{
		return _view;
	}

	Cadabra::Queen::Core::Meta::Qd TrackLayout::getScheme() const
	{
		return _scheme;
	}

	void TrackLayout::onViewWidthChanged()
	{
		setImplicitWidth(_view->width());
	}

	void TrackLayout::onViewHeightChanged()
	{
		setImplicitHeight(_view->height());
	}

	bool TrackLayout::hasModel() const
	{
		return getModel() != nullptr;
	}

	bool TrackLayout::hasView() const
	{
		return _view != nullptr;
	}
} } } }
