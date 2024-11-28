#ifndef Cadabra_Queen_Conversation_Model_Message_WithdrawalMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_WithdrawalMessage_hpp

#include "../AbstractMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class WithdrawalMessage : public AbstractMessage
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
		WithdrawalMessage(QObject* parent = nullptr);
		virtual ~WithdrawalMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
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

	/** @name Slots */
	/** @{ */
	public:
		virtual bool isErasable() const override;
		virtual bool hasClipboard() const override;
	/** @} */
	};
} } } }

#endif
