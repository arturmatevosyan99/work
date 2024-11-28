#include "VideoscopeMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	VideoscopeMessage::VideoscopeMessage(QObject* parent)
	:
		AttachmentMessage(parent, -8034428181974876155)
	{

	}

	QString VideoscopeMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Video Message"));
	}
} } } }
