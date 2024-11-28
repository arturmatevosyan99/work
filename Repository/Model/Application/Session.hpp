#ifndef Cadabra_Queen_Repository_Model_Application_Session_hpp
#define Cadabra_Queen_Repository_Model_Application_Session_hpp

#include "../AbstractApplication.hpp"

#include "Interface/ProfileInterface.hpp"

#include "../AbstractJob.hpp"

#include "../../Meta/Essence.hpp"
#include "../../Meta/Session.hpp"
#include "../../Meta/Subject.hpp"

#include "../../Data/Entity/Session.hpp"

#include "../../../Core/System/NotificationCenter.hpp"

#include "../../../Qaos/AtomicEnum.hpp"

#include <QSet>
#include <QSharedPointer>
#include <QAtomicInteger>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Realm;
	class SessionDescriptor;
	class StatefullDescriptor;

	/**
	 * @brief Session representation at the back-end pool
	 * @author Daniil A Megrabyan (danil@megrabyan.pro)
	 *
	 * Each application model is allocated at the appropriate pool. Pool is relative to a subject(user in the common sense)
	 * and can be considered as an isolated RAM where applications keep states in a runtime as long as a front-end application model needs.
	 * To get an instance of a session authorization must be obtained in the particular pool. Like, if you need a session as `danilabagroff`
	 * you need to pass through login&password check(or any other authentication method) in the pool with id = 7, because `pool id` equals to `subject id`
	 * and 7 exactly what `danilabagroff`s id is.
	 */
	class Session : public AbstractApplication, public Cadabra::Queen::Repository::Model::ProfileInterface
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Flag
		{
			Default = 0,
			AutoRettach = 0b1,
			StubbornProlongation = 0b10
		};
		Q_DECLARE_FLAGS(Flags, Flag)

		/**
		 * @brief Session authorization error
		 */
		enum class AuthorizationReject
		{
			UnknownError = 0, ///< Everything is unclear
			NetworkError = 1, ///< Network or just backend in the net is unreachable
			AuthenticationError = 2, ///< Access denied or credentials are incorrect
			ReflectionError = 3, ///< CRP(protocol) error
			ExpirationError = 4 ///< [not in use] Token is expired
		};

		/**
		 * @brief Session authorization phase
		 */
		enum class AuthorizationPhase
		{
			Anticipation = 0,
			Registration = 1,
			Verification = 2,
			Authentication = 3,
			Prolongation = 4,
		};

		/**
		 * @brief Application attach state
		 */
		enum class AttachFact
		{
			OpenComplete = 0,
			NonApplicable = 1,
			HandleError = 2,
			OpenError = 3,
			RequestError = 4
		};

		/**
		 * @brief Authorization method
		 */
		enum class SignMethod
		{
			OnetimePassword = 'O',
			AccountPassword = 'P',
			InappReceipt = 'R',/*,
			Facebook = 'F',
			VK = 'V',
			Cadabra = 'C'*/
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(Cadabra::Queen::Repository::Model::ProfileInterface)
		Q_FLAG(Flags)
		Q_ENUM(AuthorizationPhase)
		Q_ENUM(AuthorizationReject)
		Q_ENUM(AttachFact)
		Q_ENUM(SignMethod)
		/**
		 * @brief Pool Id (or Subject Id)
		 * @todo Need a proper notify
		 */
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Session::Id poolId READ getPoolId)
		/**
		 * @brief Session handle in the subject's pool (or Session Handle)
		 * @todo Need a proper notify
		 */
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Session::Handle poolHandle READ getPoolHandle)
		/**
		 * @brief Skeketon Name for signing in and up purposes
		 */
		Q_PROPERTY(QString skeletonName READ getSkeletonName WRITE setSkeletonName NOTIFY skeletonNameChanged)
		/**
		 * @brief ...
		 */
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Session::Flags flags READ getFlagSet WRITE setFlagSet NOTIFY flagsetChanged)
		/**
		 * @brief Authorized on the endpoint (ready to attach local applets with remote servlets)
		 */
		Q_PROPERTY(bool isActive READ isActive NOTIFY activated)
		/**
		 * @brief Initialized and linked to Realm? (ready to basic use in other words)
		 */
		Q_PROPERTY(bool isRealized READ isRealized NOTIFY realized)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Session(QObject* parent = nullptr);
		virtual ~Session() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Qaos::AtomicEnum<AuthorizationPhase> _authorization_phase;
		bool _active;
		QSharedPointer<SessionDescriptor> _descriptor;
		QString _device_token;
		QString _skeleton_name;
		Flags _flag_set;
		QSet<QSharedPointer<AbstractJob>> _job_set;
	/** @} */

	/** @name Qrocedures */
	/** @{ */
	public:
		/**
		 * @brief Session cannot be opened in the same manner as a regular application can, use ::signIn-, ::validate, ::revalidate and ::authroize methods.
		 * @param descriptor
		 * @return Success
		 */
		virtual Q_INVOKABLE bool open(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor) override;

		/**
		 * @brief Session cannot be closed in the same manner as a regular application can, use ::derealize, ::signOut instead to achieve this.
		 * @return False
		 */
		virtual Q_INVOKABLE bool close() override;

		/**
		 * @brief Link frontend-Realm(local on a queen instance) with backend-Realm(remote on an ace instance)
		 * This is very basic initialization and strictly required before further use
		 * @param realm
		 * @return Success
		 */
		Q_INVOKABLE bool realize(Cadabra::Queen::Repository::Model::Realm* realm);

		/**
		 * @brief Use credentials somehow obtained elsewhere (eg via Appointment CREST API)
		 * @param session_id
		 * @param session_handle
		 * @param avatar_scheme
		 * @return Success
		 */
		Q_INVOKABLE bool authorize(Cadabra::Queen::Repository::Meta::Session::Id session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme avatar_scheme);

		/**
		 * @brief Reset local Realm to defaults and shutdown synchronization channel(river)
		 */
		Q_INVOKABLE void derealize();

		/**
		 * @brief Request verification with auto validation if the current session is not valid anymore
		 * @return Success
		 */
		Q_INVOKABLE bool verify();

		/**
		 * @brief Request a validation to check and renew if session is obsolete but authorization which was granted for this handle is still valid
		 * @param id
		 * @param handle
		 * @param scheme
		 * @return Success
		 */
		Q_INVOKABLE bool validate(Cadabra::Queen::Repository::Meta::Session::Id session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme avatar_scheme);

		/**
		 * @brief Force to change current session
		 * All local applications' descriptors will be revalidated(models will be reopened/reattached) if pool still the same,
		 * otherwise session is in the pretty different pool and all local applications will be closed
		 * @param id
		 * @param handle
		 * @param scheme
		 * @return Success
		 */
		Q_INVOKABLE bool revalidate(Cadabra::Queen::Repository::Meta::Session::Id id, Cadabra::Queen::Repository::Meta::Session::Handle handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme scheme);

		/**
		 * @brief Brute reset to defaults
		 * Remote session will be lost if was allocated in the pool before, but hopefully collected and destroyed later on as unclaimed garbage
		 * @param silent Give a shout about it to all listeners (via further `::invalidated` signal)
		 * @return Success
		 */
		Q_INVOKABLE bool invalidate(bool silent = false);

		/**
		 * @brief Try to validate session by restored Realm
		 * @param realm Can come from cache
		 * @return Success
		 */
		Q_INVOKABLE bool prolongate(Cadabra::Queen::Repository::Model::Realm* realm);

		/**
		 * @brief Attach locally application somehow remotely opened model before
		 * @param object_id Identifier in the Realm repository
		 * @param object_handle Memory address in the remote Session Pool
		 * @param application_scheme Sometimes different models can be used to open the same Realm-object
		 * @param singleton If application can be opened just once along session
		 * @return Application descriptor
		 */
		Q_INVOKABLE Cadabra::Queen::Repository::Model::AbstractDescriptor* handle(const Cadabra::Queen::Repository::Meta::ObjectId& object_id, const Cadabra::Queen::Repository::Meta::Object::Handle& object_handle, const Cadabra::Queen::Repository::Meta::Object::ApplicationScheme& application_scheme, const QString& name, bool singleton = false);

		/**
		 * @brief Open remote application model and attach if succeed locally
		 * @param application Local application model wich is attaching to a remote one on the backend via authorized session pool
		 * @param object_id Identifier in the Realm repository
		 * @param singleton_mode If application can be opened just once along session the common handle will be used and return instead actual opening and model allocation
		 * @param option Custom attributes
		 * @return Success
		 */
		Q_INVOKABLE bool attach(Cadabra::Queen::Repository::Model::AbstractApplication* application, Cadabra::Queen::Repository::Meta::Object::Id object_id, bool singleton_mode, QVariant option = {});

		/**
		 * @brief Reopen same application model remotely(eg. because session was renewed) and attach locally if succeed
		 * @param descriptor
		 * @return Success
		 */
		Q_INVOKABLE bool reattach(Cadabra::Queen::Repository::Model::StatefullDescriptor* descriptor);

		/**
		 * @brief Gracefully close the application model and remove from both ends localy and remotely
		 * @param application
		 * @todo Not implemented yet
		 * @return Success
		 */
		Q_INVOKABLE bool detach(Cadabra::Queen::Repository::Model::AbstractApplication* application);

		/**
		 * @brief Request verification code or validate received one (registration)
		 * @param account
		 * @param code
		 */
		Q_INVOKABLE bool signUpOTP(const QString& account, const QString& code, const QString& scheme_alias);

		/**
		 * @brief Request verification code or validate received one (authorization)
		 * @param account
		 * @param code
		 */
		Q_INVOKABLE bool signInOTP(const QString& account, const QString& code);

		/**
		 * @brief Sign with receipt of InApp purchase (if any)
		 */
		Q_INVOKABLE bool signInReceipt();

		/**
		 * @brief Request authorization by email address and password
		 * @param email
		 * @param password
		 */
		Q_INVOKABLE bool signInEmail(const QString& email, const QString& password);

		/**
		 * @brief Request authorization by phone number and password
		 * @param phone
		 * @param password
		 */
		Q_INVOKABLE bool signInPhone(const QString& phone, const QString& password);

		/**
		 * @brief Gracefully close the remote session
		 * Queen need to default local session when backend will respond us back (in the `::onSignedOut` slot)
		 */
		Q_INVOKABLE bool signOut(bool oblivion = false);

		/**
		 * @brief Load profile data of a session owner
		 */
		Q_INVOKABLE void pickUpProfile();
		/** @} */

		/**
		 * @brief Return the history of a session
		 * @return List Model
		 */
		Q_INVOKABLE QAbstractListModel* sliceHistory();

	private:
		/**
		 * @brief Retry prolongation if any
		 * @return Success
		 */
		bool prolongate();

	/** @name Procedures */
	/** @{ */
	public:
		bool subscribe(const QString& key, bool onoff);
		bool subscribe(Meta::Session::SubscriptionKey target, Meta::Essence::Id target_id, bool onoff);
		bool launch(QSharedPointer<AbstractJob> job);

	private:
		void activate(bool onoff, bool force = false);
		void updateToken(const QString& token = "");
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Cadabra::Queen::Repository::Model::Realm* getRealm() const;
		Cadabra::Queen::Repository::Meta::Session::Id getPoolId() const;
		Cadabra::Queen::Repository::Meta::Session::Handle getPoolHandle() const;
		const QString& getSkeletonName() const;
		Flags getFlagSet() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setSkeletonName(const QString& value);
		void setFlagSet(Flags value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void activated(bool onoff);
		/**
		 * @brief Signal "Local Realm is initialized"
		 * @??? Synchronization with remote model is live
		 */
		void realized();
		/**
		 * @brief Signal "Session was not authorized"
		 * Authorization denied
		 */
		void declined();
		/**
		 * @brief Signal "Session was authorized"
		 * Authorization succeeded
		 */
		void authorized();
		/**
		 * @brief Signal "Account was registered or half-registered when need an OTP code"
		 * @param id An identifier of a DB record
		 */
		void registered(int id);
		/**
		 * @brief Signal "Session was validated"
		 * @param cold = true or false == hot Is realization needed?
		 */
		void validated(bool cold);
		/**
		 * @brief Signal "Session was automatically revalidated and prolongated"
		 * Session still can be used but handle was changed and all local applications are invalid and need to be reopened/reattached
		 * @param cold = true or false == hot Is realization needed?
		 */
		void revalidated(bool cold);
		/**
		 * @brief Signal "Session was not validated and inapplicable"
		 * @param silent Just a sign of how unexpected it was and need to be(or not) proclaimed somehow
		 */
		void invalidated(bool silent = false);
		/**
		 * @brief Signal "Session was verified(or not)"
		 * @param success
		 */
		void verified(bool success);
		/**
		 * @brief Signal "Authorization failed"
		 * @param phase
		 * @param reject
		 * @param message
		 */
		void faulted(Cadabra::Queen::Repository::Model::Session::AuthorizationPhase phase, Cadabra::Queen::Repository::Model::Session::AuthorizationReject reject, QString message);
		/**
		 * @brief Signal "Application an attaching process has been completed"
		 * @param application
		 * @param fact
		 */
		void attached(Cadabra::Queen::Repository::Model::AbstractApplication* application, Cadabra::Queen::Repository::Model::Session::AttachFact fact);
		/**
		 * @brief Signal "Skeleton name is customized" (available just in test environments like Ladybug)
		 */
		void skeletonNameChanged();
		/**
		 * @brief Signal "Flags are changed"
		 */
		void flagsetChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onNotificationRegistered(QString token);

		void onAuthorized();
		void onValidated(bool real);
		void onRevalidated(bool real);
		void onInvalidated(bool silent);

		void onUpstreamed(bool onoff);

		void onSignedUp(SignMethod method, quint8 domain, quint32 code, QString message);
		void onSignedUp(SignMethod method, QJsonValue payload);

		void onSignedIn(SignMethod method, quint8 domain, quint32 code, QString message);
		void onSignedIn(SignMethod method, QJsonValue payload);

		void onResignedIn(quint8 domain, quint32 code, QString message);
		void onResignedIn(QJsonValue payload);

		void onSignedOut(quint8 domain, quint32 code, QString message);
		void onSignedOut(QJsonValue payload);

		void onVerified(quint8 domain, quint32 code, QString message);
		void onVerified(QJsonValue payload);

		void onAttached(AbstractApplication* application, quint8 domain, quint32 code, QString message);
		void onAttached(AbstractApplication* application, QJsonValue payload);

		void onReattached(StatefullDescriptor* descriptor, quint8 domain, quint32 code, QString message);
		void onReattached(StatefullDescriptor* descriptor, QJsonValue payload);

		void onSubscribed(quint8 domain, quint32 code, QString message);
		void onSubscribed(QJsonValue payload);

		void onLaunched(QSet<QSharedPointer<AbstractJob>>::iterator j, quint8 domain, quint32 code, QString message);
		void onLaunched(QSet<QSharedPointer<AbstractJob>>::iterator j, QJsonValue payload);

		void onTokenUpdated(quint8 domain, quint32 code, QString message);
		void onTokenUpdated(QJsonValue payload);

		void onSubjectDumped(quint8 domain, quint32 code, QString message);
		void onSubjectDumped(QJsonValue payload);

		void onObjectDumped(quint8 domain, quint32 code, QString message);
		void onObjectDumped(QJsonValue payload);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSignal(quint16 signal, QDataStream& stream) override;
		virtual void onQueueEvent(const QJsonValue& action) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isActive() const;
		bool isRealized() const;
		Q_INVOKABLE bool hasAttached() const;
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Repository::Model::Session::Flags)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Model::Session::Flags)

#endif
