#include "PDFMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	PDFMessage::PDFMessage(QObject* parent)
	:
		AttachmentMessage(parent, -8034428181974876059)
	{

	}

	QString PDFMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Document"));
	}
} } } }
