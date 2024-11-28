/**
 @file AceEndpoint.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Repository_Net_AceEndpoint_hpp
#define Cadabra_Queen_Repository_Net_AceEndpoint_hpp

#include "../../Core/Net/Endpoint.hpp"

#include "../../Core/Net/ServiceCall.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	class AceEndpoint : public Core::Net::Endpoint
	{
	/** @name Classes */
	/** @{ */
	private:
		class VersionCall : public Core::Net::ServiceCall
		{
		/** @name Constructors */
		/** @{ */
		public:
			VersionCall(AceEndpoint* endpoint = nullptr);
			virtual ~VersionCall() override = default;
		/** @} */

		/** @name Hooks */
		/** @{ */
		protected:
			virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
			virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
		/** @} */
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QString DefaultHost;
		static const int DefaultPort;
		static const bool DefaultSecure;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AceEndpoint();
		AceEndpoint(const QUrl& url);
		AceEndpoint(const QString& url);
		AceEndpoint(const QString& host, int port, bool secure);
		virtual ~AceEndpoint() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		VersionCall _version_call;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE virtual void reach() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
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
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Friends */
	/** @{ */
	/** @} */
	};
} } } }

#endif
