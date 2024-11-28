#ifndef Cadabra_Queen_Conversation_Model_Message_ProxyMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_ProxyMessage_hpp

#include "../AbstractMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class ProxyMessage : public AbstractMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		ProxyMessage(QObject* parent = nullptr);
		virtual ~ProxyMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QSharedPointer<AbstractMessage> _message;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, const Cadabra::Queen::Conversation::Meta::Message& message_meta) override;

	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		virtual const AssistList& getAssistList() const override;
		AbstractMessage* getTarget();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:		
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void loaded();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
