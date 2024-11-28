#include "SessionDescriptor.hpp"

#include "StatefullDescriptor.hpp"
#include "StatelessDescriptor.hpp"

#include "../AceEvent.hpp"
#include "../Action/HandleAction.hpp"
#include "../../Data/Entity/Session.hpp"

#include <QSharedPointer>
#include <QQmlEngine>
#include <QCoreApplication>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	SessionDescriptor::SessionDescriptor(Realm* realm, Session* session)
	:
		AbstractDescriptor(realm, QSharedPointer<Meta::Object>::create(Meta::Object::DomainId, Meta::Object::DomainId, "8660964310169681921", "Domain", false, true, "", 0, 0, 0)),

		_session(session),
		_pool_id(0),
		_pool_handle(0),
		_avatar_scheme("36446778833"), /// @todo Scheme
		_moniker("")
	{

	}

	SessionDescriptor::~SessionDescriptor()
	{
		if (hasRealm()) {
			getRealm()->deinitialize(false);
		}
		qDeleteAll(_cache);
	}

	QUrlQuery SessionDescriptor::makeMoniker() const
	{
		QUrlQuery retval;
		retval.addQueryItem("session_id", QString::number(_pool_id));
		retval.addQueryItem("session_handle", _pool_handle);

		return retval;
	}

	bool SessionDescriptor::identify(Meta::Session::Id id, Meta::Session::Handle handle, Meta::Subject::AvatarScheme scheme)
	{
		if (id == 0 || !hasRealm() || getRealm()->isInitialized()) {
			return false;
		}

		_pool_id = id;
		_pool_handle = handle;
		_avatar_scheme = scheme;

		return true;
	}

	bool SessionDescriptor::authorize(Meta::Session::Id id, Meta::Session::Handle handle, Meta::Subject::AvatarScheme scheme)
	{
		return identify(id, handle, scheme) ? reauthorize(0) : false;
	}

	bool SessionDescriptor::reauthorize(Meta::Session::Handle delta, bool increment)
	{
		if (_pool_id == 0 || !hasRealm() || getRealm()->isInitialized()) {
			return false;
		}

		if (delta) {
			if (increment) {
				if (!_pool_handle.increment(delta)) {
					return false;
				}
			} else {
				if (!_pool_handle.decrement(delta)) {
					return false;
				}
			}
		}
		_moniker = (!_pool_id || !_pool_handle) ? "" : QString("/%1/%2").arg(_pool_id).arg(static_cast<QString>(_pool_handle));

		if (!getRealm()->initialize(this)) {
			return false;
		}

		getRealm()->getPreference()->setOption("ace_host", getRealm()->getEndpoint()->getHost());
		getRealm()->getPreference()->setOption("ace_port", getRealm()->getEndpoint()->getPort());
		getRealm()->getPreference()->setOption("ace_secure", getRealm()->getEndpoint()->isSecured());
		getRealm()->getPreference()->setOption("ace_realm_session", QVariant::fromValue(Meta::Session::Pool(_pool_id, _pool_handle, _avatar_scheme)));

		return true;
	}

	bool SessionDescriptor::reauthorize(Meta::Session::Id id, Meta::Session::Handle handle, Meta::Subject::AvatarScheme scheme)
	{
		/// @note Тогда нужна чистая(первичная) авторизация
		if (!hasRealm() || !getRealm()->isInitialized()) {
			return false;
		}

		_avatar_scheme = scheme;
		_moniker = "";

		/// @note Почистим кэш открытых дескрипторов - он потерял актуальность
		_history_stack.clear();
		getRealm()->getPreference()->setOption("ace_realm_session_history", _history_stack);

		/// @note Чистим кэш если пул изменился(другой субъект)
		if (id != _pool_id) {
			qDeleteAll(_cache);
			_cache.clear();

			/// @note Деициниализируем Realm
			if (!getRealm()->deinitialize(true)) {
				return false;
			}

			_pool_id = id;
			_pool_handle = handle;

			/// @note Реавторизируемся под другим субъектом или выходим если это деавторизация(в 0)
			return !_pool_id ? true : reauthorize(0);
		}

		/// @note Всего лишь изменился pool_handle(новая сессия)
		_pool_handle = handle;
		_moniker = (!_pool_id || !_pool_handle) ? "" : QString("/%1/%2").arg(_pool_id).arg(static_cast<QString>(_pool_handle));
		getRealm()->getPreference()->setOption("ace_realm_session", QVariant::fromValue(Meta::Session::Pool(_pool_id, _pool_handle, _avatar_scheme)));

		if (_session->getFlagSet().testFlag(Session::Flag::AutoRettach)) {
			/// @note Перецепляем все открыте дескрипторы к новому пулу
			for (CacheMap::mapped_type& descriptor : _cache) {
				_session->reattach(descriptor);
			}
		} else {
			/// @todo Закроем все апплеты и их дескрипторы
			for (CacheMap::mapped_type& descriptor : _cache) {
				AceEvent event("Cadabra::Queen::Repository::Model::Handle", Meta::RealmTarget(Meta::Essence::Target::Object, 0));
				event.import(QJsonValue(static_cast<int>(HandleAction::Cause::Invalidation)));
				QCoreApplication::sendEvent(descriptor, &event);
				/// @xxx в AbstractApplication::close вызываем AbstractDescriptor->removeEventFilter
				descriptor->deleteLater();
			}
			_cache.clear();
		}
		return true;
	}

	bool SessionDescriptor::deauthorize()
	{
		reauthorize(0, 0, "");
		return (0 == _pool_id);
	}

	StatefullDescriptor* SessionDescriptor::handle(const Meta::Object::Id& object_id, const Meta::Object::Handle& application_handle, const Meta::Object::ApplicationScheme& application_scheme, const QString& name, bool singleton, const QVariant& option)
	{
		if (hasRealm() ? !getRealm()->isInitialized() : false) {
			return nullptr;
		}

		CacheMap::iterator retval
		(
			_cache.insert
			(
				object_id,
				new StatefullDescriptor
				(
					this,
					getRealm()->getHierarchy()->getObjectModel()->upsert
					(
						object_id,
						0,
						application_scheme,
						name,
						false,
						true,
						"",
						0,
						0,
						0
					),
					application_handle,
					singleton,
					option
				)
			)
		);

		/// @todo Избавиться в ближайшем будущем
		_history_stack.push_back(QVariant::fromValue(Meta::Session::Pool(object_id, application_handle, application_scheme)));
		getRealm()->getPreference()->setOption("ace_realm_session_history", _history_stack);
		/// @note ...вместо хранения истории в файле преференций
		Data::Session history_data;
		history_data.handle.setValue(_pool_handle.getValue());
		history_data.timestamp.setValue(QDateTime::currentDateTimeUtc());
		history_data.object_id.setValue(object_id);
		history_data.application_scheme.setValue(application_scheme);
		history_data.application_handle.setValue(application_handle.getValue());
		if (!getSession()->getRealm()->getDataProvider()->makeFactory().insert(&history_data, Core::Data::Factory::InsertionPolicy::Regular)) {
			qWarning() << "Could not save application attach in the session history";
		}
		/// @note When data is transferred from C++ to QML, the ownership of the data always remains with C++.
		/// The exception to this rule is when a QObject is returned from an explicit C++ method call: in this case, the QML engine assumes ownership of the object,
		/// unless the ownership of the object has explicitly been set to remain with C++ by invoking QQmlEngine::setObjectOwnership() with QQmlEngine::CppOwnership specified.
		QQmlEngine::setObjectOwnership(retval.value(), QQmlEngine::CppOwnership);
		return retval.value();
	}

	StatefullDescriptor* SessionDescriptor::handle(StatefullDescriptor* descriptor, const Meta::Object::Handle& former_handle)
	{
		if (!descriptor) {
			return nullptr;
		}
		/// @todo Избавиться в ближайшем будущем...
		_history_stack.push_back(QVariant::fromValue(Meta::Session::Pool(descriptor->getObjectId(), descriptor->getHandle(), descriptor->getScheme())));
		getRealm()->getPreference()->setOption("ace_realm_session_history", _history_stack);
		/// @note ...вместо хранения истории в файле преференций
		Data::Session history_data;
		history_data.handle.setValue(_pool_handle.getValue());
		history_data.timestamp.setValue(QDateTime::currentDateTimeUtc());
		history_data.object_id.setValue(descriptor->getObjectId());
		history_data.application_scheme.setValue(descriptor->getScheme());
		history_data.application_handle.setValue(descriptor->getHandle().getValue());
		if (!getSession()->getRealm()->getDataProvider()->makeFactory().insert(&history_data, Core::Data::Factory::InsertionPolicy::Regular)) {
			qWarning() << "Could not save application attach in the session history";
		}
		return descriptor;
	}

	void SessionDescriptor::marshal(AceEvent& event)
	{
		Meta::Object::Id target_id(0);
		if (0 == event.getTarget().id) { /// < Global in the target area
			target_id = 1;
		} else if (Meta::Essence::Target::Object == event.getTarget().target) { /// < Object(particular) specific
			target_id = event.getTarget().id;
		} else {
			return ;
		}

		QList<StatefullDescriptor*> target_list(_cache.values(target_id));
		for (int d = 0; d < target_list.size(); ++d) {
			QCoreApplication::sendEvent(target_list.at(d), &event);
		}
	}

	Model::Session* SessionDescriptor::getSession() const
	{
		return _session;
	}

	const QString& SessionDescriptor::getMoniker() const
	{
		return _moniker;
	}

	Meta::Session::Id SessionDescriptor::getPoolId() const
	{
		return _pool_id;
	}

	Meta::Session::Handle SessionDescriptor::getPoolHandle() const
	{
		return _pool_handle;
	}

	Model::SubjectKey SessionDescriptor::getSubjectKey()
	{
		return getRealm()->getHierarchy()->getSubjectModel()->getSubject(_pool_id).toWeakRef();
	}

	const Meta::Subject::Id& SessionDescriptor::getSubjectId() const
	{
		return _pool_id;
	}

	Meta::Subject::AvatarScheme SessionDescriptor::getAvatarScheme() const
	{
		return _avatar_scheme;
	}

	bool SessionDescriptor::hasCache() const
	{
		return !_cache.empty();
	}
} } } }
