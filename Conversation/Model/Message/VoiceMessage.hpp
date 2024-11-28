#ifndef Cadabra_Queen_Conversation_Model_Message_VoiceMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_VoiceMessage_hpp

#include "AttachmentMessage.hpp"

#include <QScopedPointer>
#include <QAudioOutput>
#include <QTemporaryFile>
#include <QFile>
#include <QUrl>
#include <QString>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class VoiceMessage : public AttachmentMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		VoiceMessage(QObject* parent = nullptr);
		virtual ~VoiceMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:				
		QScopedPointer<QAudioOutput> _output_audio;				
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool play();
		Q_INVOKABLE bool pause();
		Q_INVOKABLE bool stop();
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
