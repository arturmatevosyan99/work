#include "mg_SmartCourseMessage.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	SmartCourseMessage::SmartCourseMessage(QObject* parent)
	:
		Conversation::Model::AppletMessage(parent, 588829656820482051)
	{

	}

	SmartCourseMessage::~SmartCourseMessage()
	{

	}

	QJsonObject SmartCourseMessage::exportBody() const
	{
		QJsonObject retval(AppletMessage::exportBody());
		retval["subject_id"] = QString::number(_subject_id);
		retval["default_track_key"] = _default_track_key;
		retval["artwork"] = _artwork;

		return retval;
	}

	bool SmartCourseMessage::importBody(const QJsonObject& body)
	{
		if (!AppletMessage::importBody(body)) {
			return false;
		}
		_subject_id = body["subject_id"].toString("0").toLongLong();
		_default_track_key = body["default_track_key"].toString("");
		_artwork  = body["artwork"].toString("");

		return true;
	}

	Repository::Meta::Subject::Id SmartCourseMessage::getSubjectId() const
	{
		return _subject_id;
	}

	Core::Meta::Qd SmartCourseMessage::getSubjectQd() const
	{
		return _subject_id;
	}

	const QString& SmartCourseMessage::getDefaultTrackKey() const
	{
		return _default_track_key;
	}

	const QString& SmartCourseMessage::getArtwork() const
	{
		return _artwork;
	}

	QUrl SmartCourseMessage::getArtworkURL() const
	{
		return _artwork;
	}
} } } }
