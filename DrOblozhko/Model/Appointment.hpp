#ifndef Cadabra_Queen_DrOblozhko_Model_Appointment_hpp
#define Cadabra_Queen_DrOblozhko_Model_Appointment_hpp

#include "../../Repository/Meta/Object.hpp"
#include "../../Repository/Meta/Subject.hpp"
#include "../../Repository/Meta/Session.hpp"
#include "../../Repository/Model/Application/Session.hpp"
#include "../Net/Call/AppointmentCall.hpp"
#include "../Net/Call/AccessCall.hpp"

#include <QObject>
#include <QScopedPointer>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class Appointment : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::SubjectId sessionId READ getSessionId NOTIFY issued)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Session::Handle sessionHandle READ getSessionHandle NOTIFY issued)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Subject::AvatarScheme sessionScheme READ getSessionScheme NOTIFY issued)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::ObjectId channelId READ getChannelId NOTIFY issued)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Object::Handle channelHandle READ getChannelHandle NOTIFY issued)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Object::ApplicationScheme channelScheme READ getChannelScheme NOTIFY issued)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Appointment(QObject* parent = nullptr);
		virtual ~Appointment() = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Net::AppointmentCall _appointment_call;
		Net::AccessCall _access_call;

		Repository::Meta::Session::Id _session_id;
		Repository::Meta::Session::Handle _session_handle;
		Repository::Meta::Subject::AvatarScheme _session_scheme;
		Repository::Meta::Object::Id _channel_id;
		Repository::Meta::Object::Handle _channel_handle;
		Repository::Meta::Object::ApplicationScheme _channel_scheme;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/**
		 * @brief Complete signup process with username
		 * @param name
		 * @return False if account was not semi-registered before
		 */
		Q_INVOKABLE bool enroll(QString name);

		/**
		 * @brief Request a new session based on an outdated one
		 * @param session_id Session/Subject Pool Id (Subject Id)
		 * @param session_handle Pointer on a session in the pool
		 * @return Always true
		 */
		Q_INVOKABLE bool reroll(Cadabra::Queen::Repository::Meta::Session::Id session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle);

		/**
		 * @brief Recover(like strict ::recover) or semi-register(needs ::enroll to complete)
		 * @param account
		 * @return Always true
		 */
		Q_INVOKABLE bool recognize(QString account);

		/**
		 * @brief Try to lock an account for the futher restore
		 * @param account
		 * @return Always true
		 */
		Q_INVOKABLE bool recover(QString account);

		/**
		 * @brief Verify previously locked or semi-registered account
		 * @param code Verification code
		 * @return Was sent or not
		 */
		Q_INVOKABLE bool restore(QString code);

		/**
		 * @brief Alias of ::restore
		 * @param code Verification code
		 * @return Was sent or not
		 */
		Q_INVOKABLE bool ratify(QString code);

		/**
		 * @brief Setup endpoint of remote Realm(Repoistory) Ace Aaemon
		 * @param endpoint
		 * @param url (aces://host[:port], ace://host[:port], http://host[:port], https://host[:port]
		 */
		Q_INVOKABLE void point(Cadabra::Queen::Core::Net::Endpoint* endpoint, QString url = "");

		/**
		 * @brief Remove account and according data
		 * @param session
		 */
		Q_INVOKABLE void resign(Cadabra::Queen::Repository::Model::Session* session);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Cadabra::Queen::Repository::Meta::Session::Id& getSessionId() const;
		const Cadabra::Queen::Repository::Meta::Session::Handle& getSessionHandle() const;
		const Cadabra::Queen::Repository::Meta::Subject::AvatarScheme& getSessionScheme() const;
		const Cadabra::Queen::Repository::Meta::ObjectId& getChannelId() const;
		const Cadabra::Queen::Repository::Meta::Object::Handle& getChannelHandle() const;
		const Cadabra::Queen::Repository::Meta::Object::ApplicationScheme& getChannelScheme() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:		
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		/**
		 * @brief Some operation was requested, next signal is upcoming...
		 */
		void requested();

		/**
		 * @brief Appointment was successfully issued
		 * All received credentials are obligatory to use during interacting with remote rad
		 *
		 * @param session_id SessionPool Id (Subject Id)
		 * @param session_handle Session handle in the pool of the remote application instance model
		 * @param session_scheme Session application default scheme
		 * @param channel_id Object Id of the general appointment channel(chat)
		 * @param channel_handle Channel handle of the remote application instance model
		 * @param channel_scheme Channel application default scheme
		 */
		void issued(Cadabra::Queen::Repository::Meta::Session::Id session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme session_scheme, Cadabra::Queen::Repository::Meta::ObjectId channel_id, Cadabra::Queen::Repository::Meta::Object::Handle channel_handle, Cadabra::Queen::Repository::Meta::Object::ApplicationScheme channel_scheme);

		/**
		 * @brief Account was semi-registred and needs to be verified via ::ratify
		 * @param success ...or false if not
		 */
		void arranged(bool success);

		/**
		 * @brief Was retified or restored
		 * @param ... or false if verification code is wrong
		 */
		void verified(bool sucess);

		/**
		 * @brief Enrollment is blocked and appointment cannot be issued
		 * Manually disabled now on a backend or temporarily out of service
		 */
		void blocked();

		/**
		 * @brief Locked for restoration
		 * @param success False if account does not exist or cannot be locked
		 */
		void locked(bool success);

		/**
		 * @brief Requested operation was declined or canceled because of an error
		 * @param error Reason failure
		 */
		void rejected(QString error);

		/**
		 * @brief Account was reset
		 */
		void reset();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onEnrollSuccess(Cadabra::Queen::Repository::Meta::Session::Id session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme session_scheme, Cadabra::Queen::Repository::Meta::ObjectId channel_id, Cadabra::Queen::Repository::Meta::Object::Handle channel_handle, Cadabra::Queen::Repository::Meta::Object::ApplicationScheme channel_scheme);
		void onEnrollRedirect(QString host, int port, bool secure);
		void onEnrollFailure(QString error);
		void onAccessRecovery(bool success);
		void onAccessPrepared(bool success);
		void onAccessRatified(bool success);
		void onAccessResign();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		/**
		 * @brief Was authorized
		 * @return ... or not
		 */
		Q_INVOKABLE bool isEnrolled() const;
	/** @} */
	};
} } } }

#endif
