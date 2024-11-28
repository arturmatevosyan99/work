#include "Session.hpp"

#include "../Realm.hpp"
#include "../AceEvent.hpp"

#include "../Action/HandleAction.hpp"

#include "../Descriptor/SessionDescriptor.hpp"
#include "../Descriptor/StatefullDescriptor.hpp"

#include "../../../Core/System/Native.hpp"
#include "../../../Core/System/Alert.hpp"
#include "../../../Core/System/Alert/StateAlert.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Session::Session(QObject* parent)
	:
		AbstractApplication(parent),

		_authorization_phase(AuthorizationPhase::Anticipation),
		_active(false),
		_skeleton_name(Core::System::Native::Instance()->getSkeletonName())
	{
		resetPingInterval(45000);

		QObject::connect(this, &Session::authorized, this, &Session::onAuthorized);
		QObject::connect(this, &Session::validated, this, &Session::onValidated);
		QObject::connect(this, &Session::revalidated, this, &Session::onRevalidated);
		QObject::connect(this, &Session::invalidated, this, &Session::onInvalidated);

		QObject::connect(this, &Session::upstreamed, this, &Session::onUpstreamed);

		QObject::connect
		(
			Core::System::NotificationCenter::Instance(),
			&Core::System::NotificationCenter::registered,

			this,
			&Session::onNotificationRegistered
		);
	}

	Session::~Session()
	{
		derealize();
	}

	bool Session::open(Cadabra::Queen::Repository::Model::AbstractDescriptor*)
	{
		return false;
	}

	bool Session::close()
	{
		return false;
	}

	bool Session::realize(Cadabra::Queen::Repository::Model::Realm* realm)
	{
		if (!realm) {
			return false;
		} else if (isRealized()) {
			AbstractApplication::close();
			_descriptor.reset();
		}
		_descriptor.reset(new SessionDescriptor(realm, this));
		AbstractApplication::open(_descriptor.data());
		emit realized();
		return true;
	}

	bool Session::authorize(Cadabra::Queen::Repository::Meta::Session::Id id, Cadabra::Queen::Repository::Meta::Session::Handle handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme scheme)
	{
		if (!isRealized()) {
			return false;
		} else if (_descriptor->authorize(id, handle, scheme)) {
			emit authorized();
			return true;
		}

		return false;
	}

	void Session::derealize()
	{
		_upstream_onoff = false;
		AbstractApplication::close();
		_descriptor.reset();
	}

	bool Session::verify()
	{
		if (!isRealized() || getPoolId() <= 0) {
			return false;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		if (!call) {
			return false;
		}

		AuthorizationPhase target_phase(AuthorizationPhase::Verification);
		if (!_authorization_phase.transit(AuthorizationPhase::Anticipation, target_phase)) {
			return AuthorizationPhase::Verification == target_phase;
		}

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&Session::onVerified)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&Session::onVerified)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::ProfileInterface",
					"Cadabra::Ace::Repository::Model::Verify",
					{
					}
		);

		return true;
	}

	bool Session::validate(Cadabra::Queen::Repository::Meta::Session::Id id, Cadabra::Queen::Repository::Meta::Session::Handle handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme scheme)
	{
		if (!isRealized()) {
			return false;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall(true));
		if (!call) {
			return false;
		}

		AuthorizationPhase target_phase(AuthorizationPhase::Prolongation);
		if (!_authorization_phase.transit(AuthorizationPhase::Anticipation, target_phase)) {
			return AuthorizationPhase::Prolongation == target_phase;
		}

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&Session::onResignedIn)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&Session::onResignedIn)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AuthenticationInterface",
					"Cadabra::Ace::Repository::Model::ReSignIn",
					{
						{
							"frontend_agent",
							QJsonObject
							{
								{"family", 'Q'},
								{"serial", QJsonValue::fromVariant(Core::System::Native::Instance()->getFrameworkSerial())}
							}
						},
						{
							"frontend_engine",
							_skeleton_name
						},

						{"id", QString::number(id)},
						{"handle", handle.getValue()},
						{"scheme", scheme},
					}
		);
		_descriptor->identify(id, handle, scheme);
		return true;
	}

	bool Session::revalidate(Cadabra::Queen::Repository::Meta::Session::Id id, Cadabra::Queen::Repository::Meta::Session::Handle handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme scheme)
	{
		if (!isRealized()) {
			return false;
		} else if (_descriptor->reauthorize(id, handle, scheme)) {
			emit revalidated(true);
			return true;
		}
		return false;
	}

	bool Session::invalidate(bool silent)
	{
		if (!isRealized()) {
			return false;
		} else if (_descriptor->deauthorize()) {
			emit invalidated(silent);
			return true;
		}
		return false;
	}

	bool Session::prolongate(Cadabra::Queen::Repository::Model::Realm* realm)
	{
		if ((realm == nullptr && !isRealized()) || (realm != nullptr && !realize(realm))) {
			return false;
		}

		const QString preference_group(getRealm()->getPreference()->getLastGroup());
		if (preference_group.isEmpty() || !getRealm()->getPreference()->setGroup(preference_group)) {
			return false;
		}

		const QString ace_host(getRealm()->getPreference()->getOption("ace_host", "").toString());
		const int ace_port(getRealm()->getPreference()->getOption("ace_port", 0).toInt());
		const bool ace_secure(getRealm()->getPreference()->getOption("ace_secure", false).toBool());
		if (ace_host.isEmpty() || ace_port <= 0) {
			return false;
		}

		const QVariant var_pool(getRealm()->getPreference()->getOption("ace_realm_session"));
		if (!var_pool.canConvert<Meta::Session::Pool>()) {
			return false;
		}

		Meta::Session::Pool pool = var_pool.value<Meta::Session::Pool>();
		getRealm()->getEndpoint()->setHost(ace_host);
		getRealm()->getEndpoint()->setPort(ace_port);
		getRealm()->getEndpoint()->setSecure(ace_secure);

		Core::System::StateAlert* state(findChild<Core::System::StateAlert*>("SESSION_INACTIVE"));
		if (!state) {
			state = new Core::System::StateAlert
			(
				this,
				"SESSION_INACTIVE",
				tr("Reconnection..."),
				tr("Trying to prolongate session..."),
				this,
				Core::System::StateAlert::Level::Warning
			);
			if (_flag_set.testFlag(Flag::StubbornProlongation)) {
				QObject::connect(state, &Core::System::StateAlert::timerExpired, this, qOverload<>(&Session::prolongate));
			}
			Core::System::Alert::Instance()->enqueue(state);
		}

		return validate(pool.getId(), pool.getHandle(), pool.getScheme());
	}

	bool Session::prolongate()
	{
		return prolongate(nullptr);
	}

	Cadabra::Queen::Repository::Model::AbstractDescriptor* Session::handle(const Cadabra::Queen::Repository::Meta::Object::Id& object_id, const Cadabra::Queen::Repository::Meta::Object::Handle& object_handle, const Cadabra::Queen::Repository::Meta::Object::ApplicationScheme& application_scheme, const QString& name, bool singleton)
	{
		if (!hasDescriptor()) {
			return nullptr;
		}
		AbstractDescriptor* retval(_descriptor->handle(object_id, object_handle, application_scheme, name, singleton));
		return !retval ? nullptr : retval;
	}

	bool Session::attach(Cadabra::Queen::Repository::Model::AbstractApplication* application, Cadabra::Queen::Repository::Meta::Object::Id object_id, bool singleton_mode, QVariant option)
	{
		if (!application || object_id <= 0) {
			return false;
		}

		QVariant opt;
		if (option.canConvert<QJSValue>()) {
			opt =option.value<QJSValue>().toVariant();
		} else if (option.canConvert<QVariantMap>()) {
			opt = option;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			std::bind
			(
				(void(Session::*)(AbstractApplication*, QJsonValue))
				&Session::onAttached, this,
				application, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>

			(&Core::Net::CRPCall::excepted),
			std::bind
			(
				(void(Session::*)(AbstractApplication*, quint8, quint32, QString))
				&Session::onAttached, this,
				application, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AppendixInterface",
					"Cadabra::Ace::Repository::Model::Attach",
					{
						{"id", QString::number(object_id)},
						{"singleton", singleton_mode},
						{"option", QJsonValue::fromVariant(opt)}
					}
		);
		return true;
	}

	bool Session::reattach(Cadabra::Queen::Repository::Model::StatefullDescriptor* descriptor)
	{
		if (!descriptor) {
			return false;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			std::bind
			(
				(void(Session::*)(StatefullDescriptor*, QJsonValue))
				&Session::onReattached, this,
				descriptor, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			std::bind
			(
				(void(Session::*)(StatefullDescriptor*, quint8, quint32, QString))
				&Session::onReattached, this,
				descriptor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AppendixInterface",
					"Cadabra::Ace::Repository::Model::Attach",
					{
						{"id", QString::number(descriptor->getObjectId())},
						{"singleton", descriptor->isSingleton()}
					}
		);
		return true;
	}

	bool Session::detach(Cadabra::Queen::Repository::Model::AbstractApplication* application)
	{
		return false;
	}

	bool Session::subscribe(const QString& global_key, bool onoff)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&Session::onSubscribed)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&Session::onSubscribed)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AppendixInterface",
					"Cadabra::Ace::Repository::Model::Subscribe",
					{
						{"type", static_cast<char>(Meta::Session::SubscriptionKey::Global)},
						{"key", global_key},
						{"onoff", onoff}
					}
		);
		return true;
	}

	bool Session::subscribe(Meta::Session::SubscriptionKey target, Meta::Essence::Id target_id, bool onoff)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&Session::onSubscribed)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&Session::onSubscribed)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AppendixInterface",
					"Cadabra::Ace::Repository::Model::Subscribe",
					{
						{"type", static_cast<char>(target)},
						{"key", QString::number(target_id)},
						{"onoff", onoff}
					}
		);
		return true;
	}

	bool Session::launch(QSharedPointer<AbstractJob> job)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		if (!call) {
			return false;
		}

		QSet<QSharedPointer<AbstractJob>>::iterator j(_job_set.insert(job));
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			std::bind
			(
				(void(Session::*)(QSet<QSharedPointer<AbstractJob>>::iterator, QJsonValue))
				&Session::onLaunched, this,
				j, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			std::bind
			(
				(void(Session::*)(QSet<QSharedPointer<AbstractJob>>::iterator, quint8, quint32, QString))
				&Session::onLaunched, this,
				j, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AppendixInterface",
					"Cadabra::Ace::Repository::Model::Launch",
					{
						{
							{"scheme", QString::number(job->getScheme())},
							{"environment", job->makeEnvironment()},
							{"timeout", 0}
						}
					}
		);
		return true;
	}

	bool Session::signUpOTP(const QString& account, const QString& code, const QString& scheme_alias)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall(true));
		if (!call) {
			return false;
		}

		AuthorizationPhase target_phase(AuthorizationPhase::Registration);
		if (!_authorization_phase.transit(AuthorizationPhase::Anticipation, target_phase)) {
			return AuthorizationPhase::Registration == target_phase;
		}

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, QJsonValue))
				&Session::onSignedUp, this,
				SignMethod::OnetimePassword, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, quint8, quint32, QString))
				&Session::onSignedUp, this,
				SignMethod::OnetimePassword, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::RegistrationInterface",
					"Cadabra::Ace::Repository::Model::OTPSignUp",
					{
						{
							"frontend_agent",
							QJsonObject
							{
								{"family", 'Q'},
								{"serial", QJsonValue::fromVariant(Core::System::Native::Instance()->getFrameworkSerial())}
							}
						},
						{
							"frontend_engine",
							_skeleton_name
						},

						{"account", account},
						{"code", code},

						{"locale", QLocale::system().name()},
						{"timezone", QString::fromLatin1(QDateTime::currentDateTime().timeZone().id())},
						{"utc_offset", QDateTime::currentDateTime().offsetFromUtc()},

						{"machine_id", Core::System::Native::Instance()->getNativeIdentifier()},
						{"avatar_scheme", scheme_alias}
					}
		);

		return true;
	}

	bool Session::signInOTP(const QString& account, const QString& code)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall(true));
		if (!call) {
			return false;
		}

		AuthorizationPhase target_phase(AuthorizationPhase::Authentication);
		if (!_authorization_phase.transit(AuthorizationPhase::Anticipation, target_phase)) {
			return AuthorizationPhase::Authentication == target_phase;
		}

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, QJsonValue))
				&Session::onSignedIn, this,
				SignMethod::OnetimePassword, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, quint8, quint32, QString))
				&Session::onSignedIn, this,
				SignMethod::OnetimePassword, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AuthenticationInterface",
					"Cadabra::Ace::Repository::Model::OTPSignIn",
					{
						{
							"frontend_agent",
							QJsonObject
							{
								{"family", 'Q'},
								{"serial", QJsonValue::fromVariant(Core::System::Native::Instance()->getFrameworkSerial())}
							}
						},
						{
							"frontend_engine",
							_skeleton_name
						},

						{"account", account},
						{"code", code},

						{"locale", QLocale::system().name()},
						{"timezone", QString::fromLatin1(QDateTime::currentDateTime().timeZone().id())},
						{"utc_offset", QDateTime::currentDateTime().offsetFromUtc()},

						{"machine_id", Core::System::Native::Instance()->getNativeIdentifier()}
					}
		);
		return true;
	}

	bool Session::signInReceipt()
	{
		onSignedIn(SignMethod::InappReceipt, 0, 0, "Not implemented");
		return false;
	}

	bool Session::signInEmail(const QString& email, const QString& password)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall(true));
		if (!call) {
			return false;
		}

		AuthorizationPhase target_phase(AuthorizationPhase::Authentication);
		if (!_authorization_phase.transit(AuthorizationPhase::Anticipation, target_phase)) {
			return AuthorizationPhase::Authentication == target_phase;
		}

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, QJsonValue))
				&Session::onSignedIn, this,
				SignMethod::AccountPassword, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, quint8, quint32, QString))
				&Session::onSignedIn, this,
				SignMethod::AccountPassword, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AuthenticationInterface",
					"Cadabra::Ace::Repository::Model::EmailSignIn",
					{
						{
							"frontend_agent",
							QJsonObject
							{
								{"family", 'Q'},
								{"serial", QJsonValue::fromVariant(Core::System::Native::Instance()->getFrameworkSerial())}
							}
						},
						{
							"frontend_engine",
							_skeleton_name
						},

						{"email", email.trimmed()},
						{"password", password},
					}
		);

		return true;
	}

	bool Session::signInPhone(const QString& phone, const QString& password)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall(true));
		if (!call) {
			return false;
		}

		AuthorizationPhase target_phase(AuthorizationPhase::Authentication);
		if (!_authorization_phase.transit(AuthorizationPhase::Anticipation, target_phase)) {
			return AuthorizationPhase::Authentication == target_phase;
		}

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, QJsonValue))&Session::onSignedIn,
				this, SignMethod::AccountPassword, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			std::bind
			(
				(void(Session::*)(SignMethod, quint8, quint32, QString))
				&Session::onSignedIn, this,
				SignMethod::AccountPassword, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::AuthenticationInterface",
					"Cadabra::Ace::Repository::Model::PhoneSignIn",
					{
						{
							"frontend_agent",
							QJsonObject
							{
								{"family", 'Q'},
								{"serial", QJsonValue::fromVariant(Core::System::Native::Instance()->getFrameworkSerial())}
							}
						},
						{
							"frontend_engine",
							_skeleton_name
						},

						{"phone", phone.trimmed()},
						{"password", password},
					}
		);

		return true;
	}

	bool Session::signOut(bool oblivion)
	{
		if (oblivion) {
			/// @todo Нужен аналог AccessCall для удаления аккаунта через CRP-вызов Queen::Session -> Ace::AuthorizedSession:
			return false;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		if (!call) {
			invalidate(true);
			return false;
		}

		AuthorizationPhase target_phase(AuthorizationPhase::Authentication);
		if (!_authorization_phase.transit(AuthorizationPhase::Anticipation, target_phase)) {
			return AuthorizationPhase::Authentication == target_phase;
		}

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&Session::onSignedOut)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&Session::onSignedOut)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::ProfileInterface",
					"Cadabra::Ace::Repository::Model::SignOut",
					{
					}
		);

		return true;
	}

	void Session::pickUpProfile()
	{
		ProfileInterface::pickUp();
	}

	QAbstractListModel* Session::sliceHistory()
	{
		QScopedPointer<Data::Session> history_data(new Data::Session());
		history_data->addOrder(history_data->timestamp, Qt::DescendingOrder);

		/// @todo Принимать аргументы и срезать через history_data->addPredicate();
		if (!getRealm()->getDataProvider()->open(history_data.get(), {}, Core::Data::Provider::RowsetType::Regular)) {
			return nullptr;
		}

		QAbstractListModel* retval(history_data.take());
		QQmlEngine::setObjectOwnership(retval, QQmlEngine::JavaScriptOwnership);

		return retval;
	}

	void Session::activate(bool onoff, bool force)
	{
		if (onoff != _active || force) {
			if ((_active = onoff)) {
				resetState("SESSION_INACTIVE");
				/// @??? Any other
			} else {
				setState("SESSION_INACTIVE", tr("Unauthorized Session"), tr("Session is deactivated"), Core::System::StateAlert::Level::Error);
			}
			emit activated(_active);
		}
	}

	void Session::updateToken(const QString& token)
	{
		const QString& actual_token(token.isEmpty() ? _device_token : token);
		if (actual_token.isEmpty()) {
			return ;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&Session::onTokenUpdated)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&Session::onTokenUpdated)
		);
		call->act
		(
			"Cadabra::Ace::Repository::Model::ProfileInterface",
			"Cadabra::Ace::Repository::Model::TokenUpdate",
			{
				{"token", actual_token},
				{"agent", Core::System::Native::Instance()->getBundleIdentifier()}
			}
		);
	}

	const Cadabra::Queen::Repository::Model::Realm* Session::getRealm() const
	{
		return hasDescriptor() ? getDescriptor()->getRealm() : nullptr;
	}

	Cadabra::Queen::Repository::Meta::Session::Id Session::getPoolId() const
	{
		return _descriptor ? _descriptor->getPoolId() : 0;
	}

	Cadabra::Queen::Repository::Meta::Session::Handle Session::getPoolHandle() const
	{
		return _descriptor ? _descriptor->getPoolHandle() : Meta::SessionHandle(0);
	}

	const QString& Session::getSkeletonName() const
	{
		return _skeleton_name;
	}

	Session::Flags Session::getFlagSet() const
	{
		return _flag_set;
	}

	void Session::setSkeletonName(const QString& value)
	{
#ifdef QT_TESTLIB_LIB
		if (_skeleton_name != value) {
			_skeleton_name = value;
			emit skeletonNameChanged();
		}
#endif
	}

	void Session::setFlagSet(Session::Flags value)
	{
		if (value != _flag_set) {
			_flag_set = value;
			emit flagsetChanged();
		}
	}

	void Session::onNotificationRegistered(QString token)
	{
		_device_token = Core::System::NotificationCenter::Instance()->getPrefixedToken();

		if (token != _device_token && !_descriptor.isNull() && _descriptor->hasRealm() && _descriptor->getRealm()->isInitialized()) {
			updateToken(_device_token);
		}
	}

	void Session::onUpstreamed(bool onoff)
	{
		if (onoff) {
			resetState("SESSION_STREAMLESS");
			/// @todo Нужно послать ping на этом стриме, чтобы получить в ответ pong c handle и сравнить его с handle в дескрипторе
		} else if (isUpstream()) { /// < Если upstream отрубился непреднамеренно
			setState("SESSION_STREAMLESS", tr("Offline Mode"), tr("Upstream is lost"), Core::System::StateAlert::Warning);
			verify();
		}
	}

	void Session::onAuthorized()
	{
		upStream(true);
		updateToken();
		activate(true);
	}

	void Session::onValidated(bool real)
	{
		upStream(true);
		updateToken();
		activate(true);
	}

	void Session::onRevalidated(bool real)
	{
		upStream(true);
		updateToken();
		activate(true);
	}

	void Session::onInvalidated(bool silent)
	{
		upStream(false);
		activate(false, true);
	}

	void Session::onSignedUp(Session::SignMethod method, quint8 domain, quint32 code, QString message)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Registration) {
			qWarning() << "Back from non-Registration phase";
		}

		qWarning("Session signup(%c) error(%d, %d): %s", static_cast<char>(method), domain, code, qUtf8Printable(message));
		if (SignMethod::OnetimePassword == method && 2 == domain && 655295053 == code) {
			/**
			 * @brief Вариации domain
			 * 0 == local/queen
			 * 1 == crp
			 * 2 == internal (from the remote point of view)
			 * 3 == external
			 * 4 == unknown
			 */
			/**
			 * @brief Побитовая раскладка code
			 * 655295053 == 0010011100001111 00000010 01001101
			 * incident: 0010011100001111 == 9999 == Repository::Model::PrimeSession::Exception::Error::Verification
			 * range: 00000010 == 2 == Core::Model::Exception::Range::Logic
			 * area: 01001101 == 77 == Core::ApplicationArea::Model
			 */
			emit faulted(AuthorizationPhase::Verification, AuthorizationReject::AuthenticationError, QString("Verification code is incorrect"));
		} else if (0 == domain && code >= 1000) {
			emit faulted(AuthorizationPhase::Registration, AuthorizationReject::NetworkError, QString("SignUp %1 error").arg(code));
		} else {
			emit faulted(AuthorizationPhase::Registration, AuthorizationReject::UnknownError, QString("SignUp %1 error").arg(code));
		}
	}

	void Session::onSignedUp(Session::SignMethod method, QJsonValue payload)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Registration) {
			qWarning() << "Back from non-Registration phase";
		} else if (!payload.isObject()) {
			emit faulted(AuthorizationPhase::Registration, AuthorizationReject::UnknownError, "SignUp data error");
			return ;
		}
		const QJsonObject retval(payload.toObject());
		const Meta::SubjectId id(retval["id"].toString("0").toLongLong());
		const Meta::SessionHandle handle(retval["handle"].toString());
		if (id == 0) { /// < Ok, but need second phase or something
			emit registered(id);
		} else if (!handle) {
			emit registered(id); /// < Registred but not automatically authorized
		} else if (_descriptor->authorize(id, handle, retval["scheme"].toString("0"))) {
			emit authorized();
		} else {
			emit faulted(AuthorizationPhase::Registration, AuthorizationReject::UnknownError, "Unexpected authorization error");
		}
	}

	void Session::onSignedIn(Session::SignMethod method, quint8 domain, quint32 code, QString message)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Authentication) {
			qWarning() << "Back from non-Authentication phase";
		}

		qWarning("Session signin(%c) error(%d, %d): %s", static_cast<char>(method), domain, code, qUtf8Printable(message));
		activate(false);
		/**
		 * @brief Вариации domain
		 * 0 == local/queen
		 * 1 == crp
		 * 2 == internal (from the remote point of view)
		 * 3 == external
		 * 4 == unknown
		 */
		if (SignMethod::OnetimePassword == method && 2 == domain) {
			switch (code) {
				/**
				 * @brief Побитовая раскладка code
				 * 655295053 == 0010011100001111 00000010 01001101
				 * incident: 0010011100001111 == 9999 == Repository::Model::PrimeSession::Exception::Error::Verification
				 * range: 00000010 == 2 == Core::Model::Exception::Range::Logic
				 * area: 01001101 == 77 == Core::ApplicationArea::Model
				 */
				case 655295053:
					emit faulted(AuthorizationPhase::Verification, AuthorizationReject::AuthenticationError, QString("Verification code is incorrect"));
					return ;
				/**
				 * @brief Побитовая раскладка code
				 * 655229517 == 0010011100001110 00000010 01001101
				 * incident: 0010011100001110 == 9998 == Repository::Model::PrimeSession::Exception::Error::AccountMissed
				 * range: 00000010 == 2 == Core::Model::Exception::Range::Logic
				 * area: 01001101 == 77 == Core::ApplicationArea::Model
				 */
				case 655229517:
					emit faulted(AuthorizationPhase::Authentication, AuthorizationReject::AuthenticationError, QString("Account does not exist"));
					return ;
			}
		}

		if (0 == domain && code >= 1000) {
			emit faulted(AuthorizationPhase::Authentication, AuthorizationReject::NetworkError, QString("SignIn %1 error").arg(code));
		} else {
			emit faulted(AuthorizationPhase::Authentication, AuthorizationReject::ReflectionError, QString("SignIn %1 error").arg(code));
		}
	}

	void Session::onSignedIn(Session::SignMethod method, QJsonValue payload)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Authentication) {
			qWarning() << "Back from non-Authentication phase";
		} else if (!payload.isObject()) {
			emit faulted(AuthorizationPhase::Authentication, AuthorizationReject::UnknownError, "SignIn data error");
			return ;
		}
		const QJsonObject retval(payload.toObject());
		const Meta::SubjectId id(retval["id"].toString("0").toLongLong());
		const Meta::SessionHandle handle(retval["handle"].toString());
		if (SignMethod::OnetimePassword == method && id == 0) {
			emit registered(id);
		} else if (id == 0 || !handle) {
			emit declined();
			activate(false);
		} else if (_descriptor->authorize(id, handle, retval["scheme"].toString("0"))) {
			emit authorized();
		} else {
			emit faulted(AuthorizationPhase::Authentication, AuthorizationReject::UnknownError, "Unexpected authentication error");
			activate(false);
		}
	}

	void Session::onResignedIn(quint8 domain, quint32 code, QString message)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Prolongation) {
			qWarning() << "Back from non-Prolongation phase";
		}

		qWarning("Session resignin error(%d, %d): %s", domain, code, qUtf8Printable(message));
		activate(false);
		if (0 == domain && code >= 1000) {
			Core::System::StateAlert* state(setState("SESSION_INACTIVE", tr("Network Error"), message, Core::System::StateAlert::Level::Error));
			if (_flag_set.testFlag(Flag::StubbornProlongation)) {
				state->progressTimer(4000);
			}
			emit faulted(AuthorizationPhase::Prolongation, AuthorizationReject::NetworkError, QString("ResignIn %1 error").arg(code));
		} else if (1 == domain) {
			/// @??? probably need to be invalidated()
			emit faulted(AuthorizationPhase::Prolongation, AuthorizationReject::ReflectionError, QString("ResignIn %1 error").arg(code));
		}
	}

	void Session::onResignedIn(QJsonValue payload)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Prolongation) {
			qWarning() << "Back from non-Prolongation phase";
		}

		const QJsonObject retval(payload.toObject());
		const Meta::SubjectId id(retval["id"].toString("0").toLongLong());
		const Meta::SessionHandle delta(Cadabra::Queen::Repository::Meta::SessionHandle::fromJson(retval["delta"]));
		if (id == 0) { /// < Сессия устарела и не может быть восстановлена
			qDebug("Session is not actual any more");
			emit invalidated();
		} else if (_descriptor->getRealm()->isInitialized()) {  /// < Реавторизация (Горячая замена)
			if (!delta) { /// < Подтверждена актуальность
				emit validated(false);
			} else if ( /// < Ревалидирована, если есть
						_descriptor->reauthorize(
													id,
													_descriptor->getPoolHandle().ment(delta, retval["increment"].toBool(true)),
													_descriptor->getAvatarScheme()
						)
			) {
				emit revalidated(false);
			} else {
				/// @xxx ?
			}
		} else if (!delta) { /// < Первичная авториазция (Холодный старт)
			if (_descriptor->reauthorize(0)) {
				emit validated(true);
			} else {
				emit faulted(AuthorizationPhase::Prolongation, AuthorizationReject::UnknownError, "Reauthorization error");
			}
		} else { /// < Ревалидирована, если есть
			if (_descriptor->reauthorize(delta, retval["increment"].toBool(true))) {
				emit revalidated(true);
			} else {
				emit faulted(AuthorizationPhase::Prolongation, AuthorizationReject::UnknownError, "Reauthorization error");
			}
		}
	}

	void Session::onSignedOut(quint8 domain, quint32 code, QString message)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Authentication) {
			qWarning() << "Back from non-Authentication phase";
		}

		qWarning("SignOut error(%d, %d): %s", domain, code, qUtf8Printable(message));
		_descriptor->deauthorize();
		emit invalidated(true);
	}

	void Session::onSignedOut(QJsonValue payload)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Authentication) {
			qWarning() << "Back from non-Authentication phase";
		}

		_descriptor->deauthorize();
		emit invalidated(true);
	}

	void Session::onVerified(quint8 domain, quint32 code, QString message)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Verification) {
			qWarning() << "Back from non-Verification phase";
		}

		qWarning("Session verification error(%d, %d): %s", domain, code, qUtf8Printable(message));
		if (0 == domain && code >= 1000) {
			emit faulted(AuthorizationPhase::Verification, AuthorizationReject::NetworkError, QString("Session verification %1 error").arg(code));
		} else if (1 == domain && 7472974 == code) {
			/**
			 * @brief Вариации domain
			 * 0 == local/queen
			 * 1 == crp
			 * 2 == internal  (from the remote point of view)
			 * 3 == external
			 * 4 == unknown
			 */
			/**
			 * @brief Побитовая раскладка code
			 * code == 7472974 == 0000000001110 01000000111 01001110:
			 * incident: 0000000001110010 == 114 == Core::Net::ReflectionException::ReflectorMissed
			 * range: 00000111 == 7 == Core::Net::Exception::Range::Reflection
			 * area: 01001110 == 75 == Core::ApplicationArea::Net
			 */
			validate(_descriptor->getPoolId(), _descriptor->getPoolHandle(), _descriptor->getAvatarScheme());
		} else {
			qDebug("Session is not actual any more");
			emit verified(false);
			emit invalidated(false);
		}
	}

	void Session::onVerified(QJsonValue payload)
	{
		if (_authorization_phase.store(AuthorizationPhase::Anticipation) != AuthorizationPhase::Verification) {
			qWarning() << "Back from non-Verification phase";
		}

		if (!payload.toBool(false)) {
			qDebug("Session is not actual any more");
			emit verified(false);
			validate(_descriptor->getPoolId(), _descriptor->getPoolHandle(), _descriptor->getAvatarScheme());
		} else {
			qDebug("Session is verified");
			emit verified(true);
		}
	}

	void Session::onAttached(AbstractApplication* application, quint8 domain, quint32 code, QString message)
	{
		qWarning("Application attach error(%d, %d): %s", domain, code, qUtf8Printable(message));
		application->open(nullptr);
		emit attached(application, AttachFact::RequestError);
	}

	void Session::onAttached(AbstractApplication* application, QJsonValue payload)
	{
		qDebug() << "Attaching to session:" << payload;
		AbstractDescriptor* descriptor
		(
			_descriptor->handle
			(
				Meta::Essence::ToId(payload["id"]),
				Meta::Object::ToHandle(payload["handle"]),
				payload["scheme"].toString("0"),
				payload["name"].toString("Untitled"),
				payload["singleton"].toBool(false),
				payload["option"].toVariant()
			)
		);
		if (!descriptor) {
			qWarning() << "Could not attach unhandled descriptor to the session";
			application->open(nullptr);
			emit attached(application, AttachFact::HandleError);
		} else if (!application) {
			qWarning() << "Could not attach descriptor to the application";
			emit attached(application, AttachFact::NonApplicable);
		} else if (!application->open(descriptor)) {
			qWarning() << "Could not open application with attached descriptor";
			application->open(nullptr);
			emit attached(application, AttachFact::OpenError);
		} else {
			qDebug() << "Application has been succesfully opened with attached descriptor";
			emit attached(application, AttachFact::OpenComplete);
		}
	}

	void Session::onReattached(StatefullDescriptor* descriptor, quint8 domain, quint32 code, QString message)
	{
		qWarning("Could not reattach descriptor to the session because of a error(%d, %d): %s", domain, code, qUtf8Printable(message));
		AceEvent event("Cadabra::Queen::Repository::Model::Handle", Meta::RealmTarget(Meta::Essence::Target::Object, 0));
		event.import(QJsonValue(static_cast<int>(HandleAction::Cause::Invalidation)));
		QCoreApplication::sendEvent(descriptor, &event);
	}

	void Session::onReattached(StatefullDescriptor* descriptor, QJsonValue payload)
	{
		if (descriptor) {
			const Meta::Object::Handle handle(Meta::Object::ToHandle(payload["handle"]));
			if (descriptor->bind(_descriptor.get(), handle) && _descriptor->handle(descriptor, handle)) {
				qDebug() << "Descriptor successfully rettached, rebinded and restacked";
				return;
			}
		}
		qWarning() << "Could not reattach unhandled descriptor";
	}

	void Session::onSubscribed(quint8 domain, quint32 code, QString message)
	{
		qWarning("Session could not be subscribed on the requested event because of a error(%d, %d): %s", domain, code, qUtf8Printable(message));
	}

	void Session::onSubscribed(QJsonValue payload)
	{
		qDebug() << "Session is successfully subscribed on the requested event:" << payload;
	}

	void Session::onLaunched(QSet<QSharedPointer<AbstractJob>>::iterator j, quint8 domain, quint32 code, QString message)
	{
		const QSharedPointer<AbstractJob> job(*j);
		_job_set.erase(j);
		job->fault(message);
	}

	void Session::onLaunched(QSet<QSharedPointer<AbstractJob>>::iterator j, QJsonValue payload)
	{
		const QSharedPointer<AbstractJob> job(*j);
		_job_set.erase(j);
		if (!payload.isBool()) {
			job->fault("Unexpected launch code");
		} else {
			job->plan(payload.toBool(false) ? this : nullptr);
		}
	}

	void Session::onTokenUpdated(quint8 domain, quint32 code, QString message)
	{

	}

	void Session::onTokenUpdated(QJsonValue payload)
	{
		_device_token.clear();
	}

	void Session::onSubjectDumped(quint8 domain, quint32 code, QString message)
	{

	}

	void Session::onSubjectDumped(QJsonValue payload)
	{
		/// @todo
		// getRealm()->getHierarchy()->upsertSubject();
	}

	void Session::onObjectDumped(quint8 domain, quint32 code, QString message)
	{

	}

	void Session::onObjectDumped(QJsonValue payload)
	{
		/// @todo
		// getRealm()->getHierarchy()->upsertObject();
	}

	void Session::onSignal(quint16 signal, QDataStream& stream)
	{

	}

	void Session::onQueueEvent(const QJsonValue& body)
	{
		qDebug() << "Session event in the queue:" << body;
		AceEvent event
		(
			body["name"].toString(),
			Meta::RealmTarget
			(
				static_cast<Meta::Essence::Target>(body["target"]["target"].toInt(0)),
				body["target"]["id"].toVariant().toLongLong()
			)
		);
		if (event.import(body["payload"])) {
			_descriptor->marshal(event);
		}
	}

	bool Session::isActive() const
	{
		return _active;
	}

	bool Session::isRealized() const
	{
		return hasDescriptor();
	}

	bool Session::hasAttached()	const
	{
		return _descriptor ? _descriptor->hasCache() : false;
	}
} } } }
