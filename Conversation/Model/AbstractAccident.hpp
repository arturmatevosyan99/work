#ifndef Cadabra_Queen_Conversation_Model_AbstractAccident_hpp
#define Cadabra_Queen_Conversation_Model_AbstractAccident_hpp

#include "../../Core/Model/AbstractEvent.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class AbstractAccident : public Core::Model::AbstractEvent
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit AbstractAccident();
		AbstractAccident(const AbstractAccident& action);
		AbstractAccident(AbstractAccident&& action);
		virtual ~AbstractAccident();
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

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
