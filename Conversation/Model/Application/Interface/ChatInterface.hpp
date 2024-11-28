#ifndef Cadabra_Queen_Conversation_Model_Application_Interface_ChatInterface_hpp
#define Cadabra_Queen_Conversation_Model_Application_Interface_ChatInterface_hpp

#include <QtPlugin>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class ChatInterface
	{
	/** @name Qroperties */
	/** @{ */
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ChatInterface() = default;
		virtual ~ChatInterface() = default;
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

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::Conversation::Model::ChatInterface, "cadabra.queen.conversation.model.chat/1.0");

#endif
