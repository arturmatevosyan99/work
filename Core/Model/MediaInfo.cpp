#include "MediaInfo.hpp"

#include "../../Skeleton.hpp"

#include "../Net/Call/Grab/CubeGrab.hpp"
#include "../Net/Call/Grab/YoutubeGrab.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Model {
	MediaInfo::MediaInfo(QObject* parent)
	:
		QAbstractListModel(parent),

		_service(Service::Unknown),
		_default_alias(Skeleton::ActiveInstance()->getPreferenceOption("core/system/mediaplayer/streamalias", "").toString())
	{

	}

	MediaInfo::~MediaInfo()
	{

	}

	const MediaInfo::Service& MediaInfo::getService() const
	{
		return _service;
	}

	QString MediaInfo::getTitle() const
	{
		return _grabber ? _grabber->getTitle() : QString{};
	}

	QString MediaInfo::getDescription() const
	{
		return _grabber ? _grabber->getDescription() : QString{};
	}

	QUrl MediaInfo::getThumbnail() const
	{
		return _grabber ? _grabber->getThumbnail() : QUrl{};
	}

	const QUrl& MediaInfo::getRawSource() const
	{
		return _raw_source;
	}

	QUrl MediaInfo::getDefaultSource() const
	{
		return !_grabber ? QUrl() : _grabber->getStreamMap().value(_default_alias, _grabber->getSource());
	}

	QString MediaInfo::getDefaultAlias() const
	{
		return _default_alias;
	}

	int MediaInfo::rowCount(const QModelIndex& parent) const
	{
		return !_grabber || parent.isValid() ? 0 : _grabber->getCount();
	}

	QVariant MediaInfo::data(const QModelIndex& index, int role) const
	{
		return !_grabber ? QVariant{} : _grabber->getStreamRoleValue(index.row(), role);
	}

	Qt::ItemFlags MediaInfo::flags(const QModelIndex& index) const
	{
		return {Qt::ItemFlag::ItemIsSelectable};
	}

	QHash<int, QByteArray> MediaInfo::roleNames() const
	{
		return
		{
			{Net::MediaGrab::Role::Alias, "alias"},
			{Net::MediaGrab::Role::Size, "size"},
			{Net::MediaGrab::Role::Duration, "duration"},
			{Net::MediaGrab::Role::URL, "url"},
			{Net::MediaGrab::Role::VideoResolution, "video_resolution"},
			{Net::MediaGrab::Role::VideoCodec, "video_codec"},
			{Net::MediaGrab::Role::AudioQuality, "audio_quality"},
			{Net::MediaGrab::Role::AudioCodec, "audio_codec"}
		};
	}

	void MediaInfo::setRawSource(const QUrl& source)
	{
		beginResetModel();
		_service = Service::Unknown;
		_grabber.reset();
		_raw_source.clear();

		if (source.scheme() == "youtube" || source.host().contains("youtube.com") || source.host().contains("youtu.be")) {
			const QUrlQuery query(source.query());
			const QString id((source.scheme() == "youtube") ? source.fileName() : query.hasQueryItem("v") ? query.queryItemValue("v") : "");
			if (!id.isEmpty()) {
				_service = MediaInfo::Service::Youtube;
				_grabber.reset(new Net::YoutubeGrab(id));
				QObject::connect(_grabber.get(), &Net::MediaGrab::fetched, this, &MediaInfo::onGrabbed);
			}
		} else if (source.scheme() == "cube" || source.host().contains("cube.cadabra.cloud")) {
			QStringList resource_list;
			if (source.scheme() == "cube") { /// < cube://g-u-i-d.video|audio|app|post|picture
				resource_list = source.host().split('.');
				std::reverse(std::begin(resource_list), std::end(resource_list));
			} else { /// < https://cube.cadabra.cloud/video|audio|app|post|picture/g-u-i-d
				resource_list = source.path().split('/');
			}

			if (resource_list.length() < 2) {
				endResetModel();
				return ;
			}

			QUuid uid(resource_list.at(1));
			if (uid.isNull()) {
				endResetModel();
				return ;
			}

			Net::CubeGrab::Plane plane(Net::CubeGrab::Plane::File);
			if (resource_list.front() == "video") {
				plane = Net::CubeGrab::Plane::Video;
			} else if (resource_list.front() == "audio") {
				plane = Net::CubeGrab::Plane::Audio;
			} else if (resource_list.front() == "post") {
				plane = Net::CubeGrab::Plane::Post;
			} else if (resource_list.front() == "file") {
				plane = Net::CubeGrab::Plane::File;
			} else if (resource_list.front() == "application") {
				plane = Net::CubeGrab::Plane::Application;
			} else {
				endResetModel();
				return ;
			}

			const QUrlQuery query(source.query());
			QString api_key;
			if (query.hasQueryItem("capec-objectlicense-serialnumber")) {
				api_key = query.queryItemValue("capec-objectlicense-serialnumber");
			} else if (query.hasQueryItem("X-Api-Key")) { /// < @obsolete
				api_key = query.queryItemValue("X-Api-Key")
							.replace("-", ":")
							.prepend(":")
							.prepend("101.realm.cadabra.cloud")
							.prepend("crn:cc:license:capec:")
				;
			} else { /// < @obsolete
				api_key = "crn:cc:license:capec:"
							"101.realm.cadabra.cloud:"
							"0:0:0:0:0:0"
				;
			}

			_grabber.reset(new Net::CubeGrab(uid, plane, api_key));
			_service = MediaInfo::Service::Cube;
		}/*else if (source.scheme() == "vimeo" || source.host().contains("vimeo.com")) {
			_service = MediaInfo::Service::Vimeo;
		}*/else if (source.scheme() == "http" || source.scheme() == "https") {
			_service = MediaInfo::Service::HTTP;
			_grabber.reset(new DefaultGrab(source));
			onGrabbed();
			emit rawSourceChanged();
			endResetModel();
			return ;
		} else {
			emit rawSourceChanged();
			endResetModel();
			return ;
		}
/*
		if (_original_source.scheme() == "vimeo") {
			_deciphered_source = source;
			emit deciphered(_deciphered_source);
			return ;
		}

		if (_original_source.scheme() == "qrc") {
			_deciphered_source = source;
			emit deciphered(_deciphered_source);
			return ;
		}

		if (_original_source.scheme() == "realm") {
			_deciphered_source = source;
			emit deciphered(_deciphered_source);
			return ;
		}
*/
		QObject::connect(_grabber.get(), &Net::MediaGrab::fetched, this, &MediaInfo::onGrabbed);
		if (_grabber && !_grabber->fetch()) {
			QObject::disconnect(_grabber.get(), &Net::MediaGrab::fetched, this, &MediaInfo::onGrabbed);
			_grabber.reset();
		}
		_raw_source = source;
		emit rawSourceChanged();
		endResetModel();
	}

	void MediaInfo::setDefaultAlias(const QString& alias)
	{
		if (alias != _default_alias && _grabber) {
			QMap<QString, QUrl>::ConstIterator g(_grabber->getStreamMap().find(alias));
			if (_grabber->getStreamMap().constEnd() != g) {
				_default_alias = alias;
				emit defaultAliasChanged();
				emit defaultSourceChanged();
				Skeleton::ActiveInstance()->setPreferenceOption("core/system/mediaplayer/streamalias", _default_alias);
			}
		}
	}

	void MediaInfo::onGrabbed()
	{
		beginResetModel();
		emit grabbed();
		emit defaultSourceChanged();
		endResetModel();
	}

	DefaultGrab::DefaultGrab(const QUrl& url)
	:
		Net::MediaGrab(nullptr)
	{
		addStream("", url);
	}

	bool DefaultGrab::fetch()
	{
		if (getStreamMap().value("").isEmpty()) {
			return false;
		}
		emit fetched();
		return true;
	}
} } } }
