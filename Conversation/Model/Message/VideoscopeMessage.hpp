#ifndef Cadabra_Queen_Conversation_Model_Message_VideoscopeMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_VideoscopeMessage_hpp

#include "AttachmentMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class VideoscopeMessage : public AttachmentMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT		
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		VideoscopeMessage(QObject* parent = nullptr);
		virtual ~VideoscopeMessage() override = default;
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
