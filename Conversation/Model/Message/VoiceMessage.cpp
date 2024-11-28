#include "VoiceMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	VoiceMessage::VoiceMessage(QObject* parent)
	:
		AttachmentMessage(parent, -8034428181974876152)
	{

	}

	bool VoiceMessage::play()
	{

		return false;
	}

	bool VoiceMessage::pause()
	{

		return false;
	}

	bool VoiceMessage::stop()
	{

		return false;
	}

	QString VoiceMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Voice Message"));
	}
} } } }
