#ifndef Cadabra_Queen_DrOblozhko_Model_Application_AppointmentChannel_hpp
#define Cadabra_Queen_DrOblozhko_Model_Application_AppointmentChannel_hpp

#include "../../../SmartCare/Model/Application/SmartChannel.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class AppointmentChannel : public SmartCare::Model::SmartChannel
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AppointmentChannel(QObject* parent = nullptr);
		virtual ~AppointmentChannel() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
