#include "YoutubeMessage.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	YoutubeMessage::YoutubeMessage(QObject* parent)
	:
		AbstractMessage(parent)
	{

	}

	QJsonObject YoutubeMessage::exportBody() const
	{
		return {
					{"id", _video_id},
					{"cover_note", _custom_note},
					{"note_load", _note_load},
					{"cover_load", _cover_load},
					{"hyperlink", _hyperlink}
		};
	}

	bool YoutubeMessage::importBody(const QJsonObject& body)
	{
		_video_id = body["id"].toString();
		_custom_note = body["custom_note"].toString();
		_cover_load = body["cover_load"].toBool();
		_note_load = body["note_load "].toBool();
		_hyperlink = body["hyperlink"].toBool();

		return true;
	}

	QString YoutubeMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Youtube Video"));
	}

	QString YoutubeMessage::getVideoId() const
	{
		return _video_id;
	}

	QUrl YoutubeMessage::getHyperlink() const
	{
		return (_hyperlink && !_video_id.isEmpty()) ? QUrl("http://youtube.com/watch?v=" + _video_id) : QUrl();
	}

	QUrl YoutubeMessage::getThumbnailHQ() const
	{
		return QUrl("https://img.youtube.com/vi/" + _video_id + "/hqdefault.jpg");
	}

	QUrl YoutubeMessage::getThumbnailMQ() const
	{
		return QUrl("https://img.youtube.com/vi/" + _video_id + "/mqdefault.jpg");
	}

	QUrl YoutubeMessage::getThumbnailSQ() const
	{
		return QUrl("https://img.youtube.com/vi/" + _video_id + "/sqdefault.jpg");
	}

	QString YoutubeMessage::getNote() const
	{
		return _custom_note;
	}

	bool YoutubeMessage::hasHyperlink() const
	{
		return _hyperlink;
	}

	bool YoutubeMessage::needCover() const
	{
		return _cover_load;
	}

	bool YoutubeMessage::needNote() const
	{
		return _note_load;
	}
} } } }
