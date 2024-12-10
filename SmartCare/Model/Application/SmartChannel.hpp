#ifndef Cadabra_Queen_SmartCare_Model_Application_SmartChannel_hpp
#define Cadabra_Queen_SmartCare_Model_Application_SmartChannel_hpp

#include "../../../Conversation/Model/Application/GroupChannel.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class SmartChannel : public Conversation::Model::GroupChannel
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
		SmartChannel(QObject* parent = nullptr);
		virtual ~SmartChannel() override;
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
