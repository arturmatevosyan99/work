#ifndef Cadabra_Queen_Stress_Net_Call_SignalCall_hpp
#define Cadabra_Queen_Stress_Net_Call_SignalCall_hpp

#include "../../../Core/Net/ServiceCall.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Net {
	/**
	 * @brief The SignalCall class
	 *
	 * @todo Move to AceEndpoint like VersionCall has been moved
	 */
	class SignalCall : public Core::Net::ServiceCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SignalCall(Core::Net::Endpoint* endpoint);
		virtual ~SignalCall() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QAtomicInt _counter;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool call(int no);
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
		void completed(int no);
		void faulted(const QString& error);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isIdle() const;
	/** @} */
	};
} } } }

#endif
