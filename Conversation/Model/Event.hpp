#ifndef Cadabra_Queen_Conversation_Model_Event_hpp
#define Cadabra_Queen_Conversation_Model_Event_hpp

#include "../../Core/Model/Event.hpp"
#include "AbstractAccident.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class Event : public Core::Model::Event<AbstractAccident>
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Event(const QString& name);
		using Core::Model::Event<AbstractAccident>::Event;
		Event(const Event& origin);
		virtual ~Event() override;
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
	};
} } } }

#endif
