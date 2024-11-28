#include "HyperlinkMessage.hpp"

#include <QJsonObject>
#include <QDesktopServices>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	HyperlinkMessage::HyperlinkMessage(QObject* parent)
	:
		AbstractMessage(parent)
	{

	}

	bool HyperlinkMessage::browse()
	{
		if (!hasURL()) {
			return false;
		}

		return QDesktopServices::openUrl(_url);
	}

	QJsonObject HyperlinkMessage::exportBody() const
	{
		return {
					{"title", _title},
					{"url", _url.toString()}
		};
	}

	bool HyperlinkMessage::importBody(const QJsonObject& body)
	{
		_title = body["title"].toString("");
		_url.setUrl(body["url"].toString(""));

		return true;
	}

	QString HyperlinkMessage::getPreviewText() const
	{
		return _url.toString();
	}

	const QString& HyperlinkMessage::getTitle() const
	{
		return _title;
	}

	const QUrl& HyperlinkMessage::getURL() const
	{
		return _url;
	}

	bool HyperlinkMessage::hasURL() const
	{
		return (_url.isValid() && !_url.isEmpty());
	}
} } } }
