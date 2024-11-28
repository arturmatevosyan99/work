#include "MediaGrab.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	const QMap<QString, QSize> MediaGrab::VideoResolutionMap =
	{
		{"", {0, 0}},
//		{"SD", {426, 240}}, /// < 260P
		{"SD", {640, 360}}, /// < 360P
//		{"SD", {854, 480}}, /// < 480P
		{"HD", {1280, 720}}, /// < 720P
		{"FHD", {1920, 1080}}, /// < 1080P / FullHD
		{"2K", {2560, 1440}}, /// < QuadHD
		{"4K", {3840, 2160}}, /// < Ultra4K
		{"8K", {7680, 4320}} /// < Ultra8K
	};

	MediaGrab::MediaGrab(Core::Net::Endpoint* endpoint, QString resource)
	:
		ServiceCall(endpoint, resource)
	{
//		setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_5) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.1.1 Safari/605.1.15");
	}

	MediaGrab::MediaGrab(Core::Net::Endpoint* endpoint)
	:
		MediaGrab(endpoint, "")
	{

	}

	MediaGrab::MediaGrab(const QUrl& url)
	:
		ServiceCall(url)
	{
//		setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_5) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.1.1 Safari/605.1.15");
	}

	void MediaGrab::addTitle(const QString& language_code, const QString& title)
	{
		_title_map[language_code] = title;
	}

	void MediaGrab::addDescription(const QString& language_code, const QString& description)
	{
		_description_map[language_code] = description;
	}

	void MediaGrab::addThumbnail(const QString& language_code, const QUrl& url)
	{
		_thumbnail_map[language_code] = url;
	}

	void MediaGrab::addStream(const QString& key, const QUrl& url)
	{
		_stream_map[key] = url;
	}

	void MediaGrab::addVariant(const QString& key, const QVariant& value)
	{
		_variant_map[key] = value;
	}

	bool MediaGrab::fetch(bool allow_cache)
	{
		if (!allow_cache) {
			return fetch();
		} else if (1 == 2) {
			/// @todo look for a cache
		}
		return fetch();
	}

	QVariant MediaGrab::getStreamRoleValue(qsizetype index, int role) const
	{
		if (getCount() <= index) {
			return QVariant{};
		}
		const QString key(getStreamKey(index));
		switch (role) {
			case Role::Alias: return getStreamAlias(key);
			case Role::Duration: return getStreamDuration(key);
			case Role::Size: return getStreamSize(key);
			case Role::URL: return getStreamURL(key);
			case Role::VideoResolution: return getStreamVideoResolution(key);
			case Role::VideoCodec: return getStreamVideoCodec(key);
			case Role::AudioQuality: return getStreamAudioQuality(key);
			case Role::AudioCodec: return getStreamAudioCodec(key);
		}
		return QVariant();
	}

	QString MediaGrab::getStreamKey(qsizetype index) const
	{
		return (_stream_map.constBegin() + index).key();
	}

	QString MediaGrab::getStreamAlias(const QString& key) const
	{
		return _stream_map.contains(key) ? key : QString{};
	}

	qsizetype MediaGrab::getStreamSize(const QString& key) const
	{
		return 0;
	}

	qsizetype MediaGrab::getStreamDuration(const QString& key) const
	{
		return 0;
	}

	QUrl MediaGrab::getStreamURL(const QString& key) const
	{
		return _stream_map.value(key, QUrl{});
	}

	QString MediaGrab::getStreamVideoResolution(const QString& key) const
	{
		return _stream_map.contains(key) ? key : QString{};
	}

	QString MediaGrab::getStreamVideoCodec(const QString& key) const
	{
		return QString{};
	}

	QString MediaGrab::getStreamAudioQuality(const QString& key) const
	{
		return QString{};
	}

	QString MediaGrab::getStreamAudioCodec(const QString& key) const
	{
		return QString{};
	}

	QUrl MediaGrab::getSource() const
	{
		return _stream_map.isEmpty() ? QUrl() : _stream_map.value("", _stream_map.first());
	}

	QString MediaGrab::getTitle() const
	{
		return _title_map.isEmpty() ? QString() : _title_map.value("", _title_map.first());
	}

	QString MediaGrab::getDescription() const
	{
		return _description_map.isEmpty() ? QString() : _description_map.value("", _description_map.first());
	}

	QUrl MediaGrab::getThumbnail() const
	{
		return _thumbnail_map.isEmpty() ? QUrl() : _thumbnail_map.value("", _thumbnail_map.first());
	}

	qsizetype MediaGrab::getCount() const
	{
		return _stream_map.size();
	}

	const QMap<QString, QString>& MediaGrab::getTitleMap() const
	{
		return _title_map;
	}

	const QMap<QString, QString>& MediaGrab::getDescriptionMap() const
	{
		return _description_map;
	}

	const QMap<QString, QUrl>& MediaGrab::getThumbnailMap() const
	{
		return _thumbnail_map;
	}

	const QMap<QString, QUrl>& MediaGrab::getStreamMap() const
	{
		return _stream_map;
	}

	const QMap<QString, QVariant>& MediaGrab::getVariantMap() const
	{
		return _variant_map;
	}

	void MediaGrab::onFault(QSharedPointer<QNetworkReply> reply)
	{
		qDebug() << "Network fault:" << reply->errorString();
		emit faulted(reply->errorString());
	}
} } } }
