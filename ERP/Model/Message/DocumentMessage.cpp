#include "DocumentMessage.hpp"

namespace Cadabra { namespace Queen { namespace ERP { namespace Model {
	DocumentMessage::DocumentMessage(QObject* parent)
	:
		AbstractMessage(parent)
	{
	}

	void DocumentMessage::view()
	{
		/// @todo open new View
	}

	QJsonObject DocumentMessage::exportBody() const
	{
		return QJsonObject{};
	}

	bool DocumentMessage::importBody(const QJsonObject& body)
	{
		ERP::Model::AbstractDocument* document(getDocument());
		return !document ? false : document->parse(body);
	}

	QString DocumentMessage::getPreviewText() const
	{
		return getDescription();
	}

	QString DocumentMessage::getDescription() const
	{
		return hasDocument() ? getDocument()->getDescription() : tr("Unknown document");
	}

	ERP::Model::AbstractDocument* DocumentMessage::getDocument() const
	{
		return nullptr;
	}

	bool DocumentMessage::hasDocument() const
	{
		return getDocument() != nullptr;
	}
} } } }
