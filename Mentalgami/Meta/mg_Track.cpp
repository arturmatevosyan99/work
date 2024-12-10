/**
 @file Track.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "mg_Track.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Meta {
	Track::Track()
	:
		_revision_code(0),
		_language_code(0),
		_weight(0),
		_content_size(0),
		_content_length(0),
		_remote_progress(0),
		_local_progress(0)
	{

	}

	Track::Track(const QString& key, qint16 revision_code, qint16 language_code, const QString& title, const QString& description, const QString& artwork, const QString& flag, qint32 weight, const QString& content_type, qint32 content_size, qint32 content_length, const QDateTime& life_time, const QString& file, const QDateTime& file_time, const QJsonObject& meta, const QDateTime& meta_time, double remote_progress, const QDateTime& remote_progress_time, double local_progress, const QDateTime& local_progress_time)
	:
		_key(key),
		_revision_code(revision_code),
		_language_code(language_code),
		_title(title),
		_description(description),
		_artwork(artwork),
		_flag(flag),
		_weight(weight),
		_content_type(content_type),
		_content_size(content_size),
		_content_length(content_length),
		_life_time(life_time),
		_file(file),
		_file_time(file_time),
		_meta(meta),
		_meta_time(meta_time),
		_remote_progress(remote_progress),
		_remote_progress_time(remote_progress_time),
		_local_progress(local_progress),
		_local_progress_time(local_progress_time)
	{

	}

	Track::Track(const Track& origin)
	:
		_key(origin._key),
		_revision_code(origin._revision_code),
		_language_code(origin._language_code),
		_title(origin._title),
		_description(origin._description),
		_artwork(origin._artwork),
		_flag(origin._flag),
		_weight(origin._weight),
		_content_type(origin._content_type),
		_content_size(origin._content_size),
		_content_length(origin._content_length),
		_life_time(origin._life_time),
		_file(origin._file),
		_file_time(origin._file_time),
		_meta(origin._meta),
		_meta_time(origin._meta_time),
		_remote_progress(origin._remote_progress),
		_remote_progress_time(origin._remote_progress_time),
		_local_progress(origin._local_progress),
		_local_progress_time(origin._local_progress_time)
	{

	}

	Track::Track(Track&& origin)
	:
		_key(std::move(origin._key)),
		_revision_code(std::move(origin._revision_code)),
		_language_code(std::move(origin._language_code)),
		_title(std::move(origin._title)),
		_description(std::move(origin._description)),
		_artwork(std::move(origin._artwork)),
		_flag(std::move(origin._flag)),
		_weight(std::move(origin._weight)),
		_content_type(std::move(origin._content_type)),
		_content_size(std::move(origin._content_size)),
		_content_length(std::move(origin._content_length)),
		_life_time(std::move(origin._life_time)),
		_file(std::move(origin._file)),
		_file_time(std::move(origin._file_time)),
		_meta(std::move(origin._meta)),
		_meta_time(std::move(origin._meta_time)),
		_remote_progress(std::move(origin._remote_progress)),
		_remote_progress_time(std::move(origin._remote_progress_time)),
		_local_progress(std::move(origin._local_progress)),
		_local_progress_time(std::move(origin._local_progress_time))
	{

	}

	Track::~Track()
	{

	}

	Track::operator QJsonObject() const
	{
		return QJsonObject{
							{"key", _key},
							{"revision_code", _revision_code},
							{"language_code", _language_code},
							{"title", _title},
							{"description", _description},
							{"artwork", _artwork},
							{"flag", _flag},
							{"weight", _weight},
							{"content_type", _content_type},
							{"content_size", _content_size},
							{"content_length", _content_length},
							{"life_timestamp", _life_time.toSecsSinceEpoch()},
							{"file", _file},
							{"file_timestamp", _file_time.toSecsSinceEpoch()},
							{"meta", _meta},
							{"meta_timestamp", _meta_time.toSecsSinceEpoch()},
							{"remote_progress", _remote_progress},
							{"remoteprogress_timestamp", _remote_progress_time.toSecsSinceEpoch()},
							{"local_progress", _local_progress},
							{"localprogress_timestamp", _local_progress_time.toSecsSinceEpoch()}
		};
	}

	Track& Track::operator=(const Track& rhs)
	{
		_key = rhs._key;
		_revision_code = rhs._revision_code;
		_language_code = rhs._language_code;
		_title = rhs._title;
		_description = rhs._description;
		_artwork = rhs._artwork;
		_flag = rhs._flag;
		_weight = rhs._weight;
		_content_type = rhs._content_type;
		_content_size = rhs._content_size;
		_content_length = rhs._content_length;
		_life_time = rhs._life_time;
		_file = rhs._file;
		_file_time = rhs._file_time;
		_meta = rhs._meta;
		_meta_time = rhs._meta_time;
		_remote_progress = rhs._remote_progress;
		_remote_progress_time = rhs._remote_progress_time;
		_local_progress = rhs._local_progress;
		_local_progress_time = rhs._local_progress_time;

		return *this;
	}

	Track& Track::operator=(Track&& rhs)
	{
		_key = std::move(rhs._key);
		_revision_code = std::move(rhs._revision_code);
		_language_code = std::move(rhs._language_code);
		_title = std::move(rhs._title);
		_description = std::move(rhs._description);
		_artwork = std::move(rhs._artwork);
		_flag = std::move(rhs._flag);
		_weight = std::move(rhs._weight);
		_content_type = std::move(rhs._content_type);
		_content_size = std::move(rhs._content_size);
		_content_length = std::move(rhs._content_length);
		_life_time = std::move(rhs._life_time);
		_file = std::move(rhs._file);
		_file_time = std::move(rhs._file_time);
		_meta = std::move(rhs._meta);
		_meta_time = std::move(rhs._meta_time);
		_remote_progress = std::move(rhs._remote_progress);
		_remote_progress_time = std::move(rhs._remote_progress_time);
		_local_progress = std::move(rhs._local_progress);
		_local_progress_time = std::move(rhs._local_progress_time);

		return *this;
	}

	QVariant Track::get(int role) const
	{
		return get(static_cast<Role>(role));
	}

	QVariant Track::get(Role role) const
	{
		switch (role) {
			case Role::Key: return _key;
			case Role::RevisionCode: return _revision_code;
			case Role::LanguageCode: return _language_code;
			case Role::Title: return _title;
			case Role::Description: return _description;
			case Role::Artwork: return _artwork;
			case Role::Flag: return _flag;
			case Role::Weight: return _weight;
			case Role::ContentType: return _content_type;
			case Role::ContentSize: return _content_size;
			case Role::ContentLength: return _content_length;
			case Role::LifeTimestamp: return _life_time;
			case Role::File: return _file;
			case Role::FileTimestamp: return _file_time;
			case Role::Meta: return _meta;
			case Role::MetaTimestamp: return _meta_time;
			case Role::RemoteProgressTimestamp: return _remote_progress_time;
			case Role::RemoteProgress: return _remote_progress;
			case Role::LocalProgressTimestamp: return _local_progress_time;
			case Role::LocalProgress: return _local_progress;
			default: return QVariant();
		}
	}

	const QString& Track::getKey() const
	{
		return _key;
	}

	const qint16& Track::getRevisionCode() const
	{
		return _revision_code;
	}

	const qint16& Track::getLanguageCode() const
	{
		return _language_code;
	}

	const QString& Track::getTitle() const
	{
		return _title;
	}

	const QString& Track::getDescription() const
	{
		return _description;
	}

	const QString& Track::getArtwork() const
	{
		return _artwork;
	}

	const QString& Track::getFlag() const
	{
		return _flag;
	}

	const qint32& Track::getWeight() const
	{
		return _weight;
	}

	const QString& Track::getContentType() const
	{
		return _content_type;
	}

	const qint32& Track::getContentSize() const
	{
		return _content_size;
	}

	const qint32& Track::getContentLength() const
	{
		return _content_length;
	}

	const QDateTime& Track::getLifeDateTime() const
	{
		return _life_time;
	}

	const QString& Track::getFile() const
	{
		return _file;
	}

	const QDateTime& Track::getFileDateTime() const
	{
		return _file_time;
	}

	const QJsonObject& Track::getMeta() const
	{
		return _meta;
	}

	const QDateTime& Track::getMetaDateTime() const
	{
		return _meta_time;
	}

	const double& Track::getRemoteProgress() const
	{
		return _remote_progress;
	}

	const QDateTime& Track::getRemoteProgressDateTime() const
	{
		return _remote_progress_time;
	}

	const double& Track::getLocalProgress() const
	{
		return _local_progress;
	}

	const QDateTime& Track::getLocalProgressDateTime() const
	{
		return _local_progress_time;
	}
} } } }
