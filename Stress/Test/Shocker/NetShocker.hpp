#ifndef Cadabra_Queen_Stress_Test_Shocker_NetShocker_hpp
#define Cadabra_Queen_Stress_Test_Shocker_NetShocker_hpp

#include "../../../Core/Net/Endpoint.hpp"

#include "../../../Core/Test/AbstractShocker.hpp"

#include <QNetworkProxy>

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	class NetShocker : public Core::Test::AbstractShocker
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Net::Endpoint* endpoint MEMBER _endpoint)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		NetShocker(QObject* parent = nullptr);
		virtual ~NetShocker() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		Core::Net::Endpoint* _endpoint;
		QNetworkProxy _original_proxy;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual bool turnOn() override;
		virtual bool turnOff() override;
	/** @} */
	};
} } } }

#endif
