#include "mg_AbstractTrack.hpp"

#include "../Meta/mg_TrackRender.hpp"

#include <cmath>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	AbstractTrack::AbstractTrack(QObject* parent)
	:
		QObject(parent),

		_course_descriptor(nullptr),

		_revision_code(0),
		_language_code(0),
		_weight(0),
		_content_size(0),
		_content_length(0),
		_remote_progress(0),
		_local_progress(0)
	{

	}

	AbstractTrack::~AbstractTrack()
	{
		if (!_course_descriptor.isNull()) {
			_course_descriptor->removeEventFilter(this);
		}
	}

	bool AbstractTrack::import(SmartCourse* course_application, const Cadabra::Queen::Mentalgami::Meta::Track& track_meta)
	{
		return import(
						course_application,
						track_meta.getKey(), track_meta.getRevisionCode(), track_meta.getLanguageCode(),
						track_meta.getTitle(), track_meta.getDescription(), track_meta.getArtwork(),
						track_meta.getFlag(), track_meta.getWeight(),
						track_meta.getContentType(), track_meta.getContentSize(), track_meta.getContentLength(),
						track_meta.getLifeDateTime(), track_meta.getFile(), track_meta.getFileDateTime(),
						track_meta.getMeta(), track_meta.getMetaDateTime(),
						track_meta.getRemoteProgress(), track_meta.getRemoteProgressDateTime(),
						track_meta.getLocalProgress(), track_meta.getLocalProgressDateTime()
		);
	}

	bool AbstractTrack::import(Cadabra::Queen::Mentalgami::Model::SmartCourse* course_application, const QString& key, qint16 revision_code, qint16 language_code, const QString& title, const QString& description, const QString& artwork, const QString& flag, qint32 weight, const QString& content_type, qint32 content_size, qint32 content_length, const QDateTime& life_time, const QString& file, const QDateTime& file_time, const QJsonObject& meta, const QDateTime& meta_time, double remote_progress, const QDateTime& remote_progress_time, double local_progress, const QDateTime& local_progress_time)
	{
		if (!course_application || !course_application->hasDescriptor()) {
			return false;
		}
		_course_descriptor = course_application->getDescriptor();
		_key = key;
		_revision_code = revision_code;
		_language_code = language_code;
		_title = title;
		_description = description;
		_artwork = artwork;
		_flag = flag;
		_weight = weight;
		_content_type = content_type;
		_content_size = content_size;
		_content_length = content_length;
		_life_time = life_time;
		_file = file;
		_file_time = file_time;
		_meta = meta;
		_meta_time = meta_time;
		_remote_progress = remote_progress;
		_remote_progress_time = remote_progress_time;
		_local_progress = local_progress;
		_local_progress_time = local_progress_time;

		emit imported();
		emit progressed();
		emit filed();

		return true;
	}

	bool AbstractTrack::sign(const QString& file, const QDateTime& life_time)
	{
		if (file.isEmpty() || life_time < QDateTime::currentDateTimeUtc() || !hasCourseDescriptor()) {
			return false;
		}

		_file = file;
		_life_time = life_time;

		emit filed();
		return true;
	}

	bool AbstractTrack::update(double value, const QDateTime& time)
	{
		if (time <= _remote_progress_time || std::islessequal(value, _remote_progress)) {
			return false;
		}

		_remote_progress = value;
		_remote_progress_time = time;

		emit progressed();
		return true;
	}

	bool AbstractTrack::progress(double value)
	{
		if (std::isgreater(_local_progress, value) || std::isless(value, _remote_progress)) {
			return false;
		}

		_local_progress = value;

		emit progressed();
		return true;
	}

	QString AbstractTrack::makePreviewText() const
	{
		return "...";
	}

	Cadabra::Queen::Repository::Model::AbstractDescriptor* AbstractTrack::getCourseDescriptor() const
	{
		return _course_descriptor;
	}

	Meta::Track::SchemeIdent AbstractTrack::getSchemeIdent() const
	{
		const int type_id(QMetaType::type(QString("*").prepend(this->metaObject()->className()).toLatin1().data()));
		return QMetaType::UnknownType == type_id ? 0 : Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Ident(QMetaType(type_id));
	}

	Cadabra::Queen::Core::Meta::Qd AbstractTrack::getSchemeQd() const
	{
		return getSchemeIdent();
	}

	Meta::Track::Scheme AbstractTrack::getScheme() const
	{
		return QString::number(getSchemeIdent());
	}

	const QString& AbstractTrack::getKey() const
	{
		return _key;
	}

	const qint16& AbstractTrack::getRevisionCode() const
	{
		return _revision_code;
	}

	const qint16& AbstractTrack::getLanguageCode() const
	{
		return _language_code;
	}

	const QString& AbstractTrack::getTitle() const
	{
		return _title;
	}

	const QString& AbstractTrack::getDescription() const
	{
		return _description;
	}

	const QString& AbstractTrack::getArtwork() const
	{
		return _artwork;
	}

	const QString& AbstractTrack::getFlag() const
	{
		return _flag;
	}

	const qint32& AbstractTrack::getWeight() const
	{
		return _weight;
	}

	const QString& AbstractTrack::getContentType() const
	{
		return _content_type;
	}

	const qint32& AbstractTrack::getContentSize() const
	{
		return _content_size;
	}

	const qint32& AbstractTrack::getContentLength() const
	{
		return _content_length;
	}

	const QDateTime& AbstractTrack::getLifeDateTime() const
	{
		return _life_time;
	}

	const QString& AbstractTrack::getFile() const
	{
		return _file;
	}

	const QDateTime& AbstractTrack::getFileDateTime() const
	{
		return _file_time;
	}

	const QJsonObject& AbstractTrack::getMeta() const
	{
		return _meta;
	}

	const QDateTime& AbstractTrack::getMetaDateTime() const
	{
		return _meta_time;
	}

	const double& AbstractTrack::getRemoteProgress() const
	{
		return _remote_progress;
	}

	const QDateTime& AbstractTrack::getRemoteProgressDateTime() const
	{
		return _remote_progress_time;
	}

	const double& AbstractTrack::getLocalProgress() const
	{
		return _local_progress;
	}

	const QDateTime& AbstractTrack::getLocalProgressDateTime() const
	{
		return _local_progress_time;
	}

	const double& AbstractTrack::getActualProgress() const
	{
		return _local_progress > _remote_progress ? _local_progress : _remote_progress;
	}

	bool AbstractTrack::hasCourseDescriptor() const
	{
		return nullptr != _course_descriptor;
	}

	bool AbstractTrack::isImported() const
	{
		return nullptr != _course_descriptor;
	}

	bool AbstractTrack::isSynchronized() const
	{
		return std::isgreaterequal(_remote_progress, _local_progress) || _local_progress - _remote_progress <= 0.01;
	}
} } } }
