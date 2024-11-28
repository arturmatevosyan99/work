#ifndef Cadabra_Queen_Conversation_Model_Message_EchoMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_EchoMessage_hpp

#include "CommandMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class EchoMessage : public CommandMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString text READ getText NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		EchoMessage(QObject* parent = nullptr);
		virtual ~EchoMessage() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getText() const;
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
