#ifndef Cadabra_Queen_Conversation_Model_Message_PDFMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_PDFMessage_hpp

#include "AttachmentMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class PDFMessage : public AttachmentMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		PDFMessage(QObject* parent = nullptr);
		virtual ~PDFMessage() override = default;
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
	};
} } } }

#endif
