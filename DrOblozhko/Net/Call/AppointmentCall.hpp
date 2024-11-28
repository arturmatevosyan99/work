#ifndef Cadabra_Queen_DrOblozhko_Net_Call_AppointmentCall_hpp
#define Cadabra_Queen_DrOblozhko_Net_Call_AppointmentCall_hpp

#include "../../../Core/Net/ServiceCall.hpp"
#include "../../../Repository/Meta/Subject.hpp"
#include "../../../Repository/Meta/Object.hpp"
#include "../../../Repository/Meta/Session.hpp"
#include "../../../EC/Meta/InappReceipt.hpp"

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Net {
	class AppointmentCall : public Core::Net::ServiceCall
	{
	Q_OBJECT
	/** @name Constructors */
	/** @{ */
	public:
		AppointmentCall(Core::Net::Endpoint* endpoint = nullptr);
		virtual ~AppointmentCall() override = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void request();
		void request(Repository::Meta::Session::Id session_id, Repository::Meta::Session::Handle session_handle);
		void request(const EC::Meta::InappReceipt& receipt);
	/** @} */

	/** @name Getters */
	/** @{ */
	private slots:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void responded(Cadabra::Queen::Repository::Meta::SubjectId session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme session_scheme, Cadabra::Queen::Repository::Meta::ObjectId channel_id, Cadabra::Queen::Repository::Meta::Object::Handle channel_hash, Cadabra::Queen::Repository::Meta::Object::ApplicationScheme channel_scheme);
		void redirected(QString host, int port, bool secure);
		void failed(QString error);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
	/** @} */
	};
} } } }

#endif
