#include "YoutubeGrab.hpp"

#include <QJsonDocument>
#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	YoutubeGrab::YoutubeGrab(const QString& video_id)
	:
		MediaGrab(QUrl("https://www.youtube.com/get_video_info")),

		_video_id(video_id)
	{

	}

	bool YoutubeGrab::fetch()
	{
		QUrlQuery query;
		query.addQueryItem("video_id", _video_id);
		query.addQueryItem("asv", "3");
		query.addQueryItem("el", "detailpage");
		query.addQueryItem("hl", "en_US");

		get(query);
		return true;
	}

	void YoutubeGrab::onVideoInfo(QSharedPointer<QNetworkReply> reply)
	{
		QByteArray data(reply->readAll());
		QJsonObject video_info;

		const QUrlQuery query(QUrl::fromPercentEncoding(data));
		if (query.hasQueryItem("player_response")) {
			video_info = QJsonDocument::fromJson(query.queryItemValue("player_response").toUtf8()).object();

			QJsonObject::ConstIterator d(video_info.find("videoDetails"));
			if (d != video_info.constEnd()) {
				const QJsonObject details(d->toObject());
				QJsonObject::ConstIterator t(details.find("title"));
				if (t != details.constEnd()) {
					addTitle("ru", t->toString("Untitled"));
				}

				QJsonObject::ConstIterator n(details.find("thumbnail"));
				if (n != details.constEnd()) {
					QJsonObject::ConstIterator na(n->toObject().find("thumbnails"));
					if (na->isArray())  {
						const QJsonArray thumbnail_array(na->toArray());
						if (!thumbnail_array.isEmpty()) {
							/// @todo add others
							const QJsonObject thumbnail(thumbnail_array[0].toObject());
							QJsonObject::ConstIterator u(thumbnail.find("url"));
							if (thumbnail.constEnd() != u) {
								addThumbnail("", u->toString());
							}
						}
					}
				}
			}
		}

		bool need_deciphering(false);
		/// @brief Берем то, что после url_encoded_fmt_stream_map в data
		const int stream_offset(data.indexOf("url_encoded_fmt_stream_map="));
		if (stream_offset <= 0) {
			emit faulted("Could not find encoded stream map");
			return ;
		}

		/// Полученное сплитим & и работаем с первым[0] куском
		QList<QByteArray> stream_data_list(data.mid(stream_offset + 27).split('&'));
		if (!stream_data_list.isEmpty()) {
			const QStringList stream_raw_list(QUrl::fromPercentEncoding(stream_data_list.first()).split(','));
			for (const QString& stream_raw : stream_raw_list) {
				const QUrlQuery stream_query(stream_raw);
				if (!stream_query.hasQueryItem("quality") || !stream_query.hasQueryItem("url")) {
					continue;
				}
				addStream(stream_query.queryItemValue("quality", QUrl::FullyDecoded), stream_query.queryItemValue("url", QUrl::FullyDecoded));
/*
				if (stream_query.hasQueryItem("expire")) {
					stream.expiration_date = QDateTime::fromSecsSinceEpoch(stream_query.queryItemValue("expire", QUrl::FullyDecoded).toLongLong(), Qt::UTC);
				} else {
					stream.expiration_date = QDateTime::currentDateTime();
				}
				if (stream_query.hasQueryItem("ipbits")) {
					stream.ipbits = stream_query.queryItemValue("ipbits", QUrl::FullyDecoded);
				}
				if (stream_query.hasQueryItem("ip")) {
					stream.ip_address = stream_query.queryItemValue("ip", QUrl::FullyDecoded);
				}
				if (stream_query.hasQueryItem("type")) {
					const QStringList type_list(stream_query.queryItemValue("type", QUrl::FullyDecoded).split(";"));
					if (!type_list.isEmpty()) {
						stream.type = type_list.first();
					}
				}
*/
				if (stream_query.hasQueryItem("s") && !need_deciphering) {
					need_deciphering = true;
				}
			}
		}

		if (need_deciphering) {
			emit faulted("Deciphered is needed, but have not been implemented yet");
			return;
			setResource("/watch");
			QUrlQuery query;
			query.addQueryItem("v", _video_id);
			get(query);
		} else {
			emit fetched();
		}
	}

	void YoutubeGrab::onPlayerInfo(QSharedPointer<QNetworkReply> reply)
	{
		QByteArray data(reply->readAll());
		const int player_offset(data.indexOf("/yts/jsbin/player"));
		if (player_offset >= 0) {
			QList<QByteArray> player_data_list(data.mid(player_offset + 17).split('"'));
			if (!player_data_list.isEmpty()) {
				QString::fromUtf8(player_data_list.first());
				setResource("/yts/jsbin/player");
			}
		}
		emit fetched();
	}

	void YoutubeGrab::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		if ("/get_video_info" == getResource()) {
			onVideoInfo(reply);
			return;
		}
		if ("/watch" == getResource()) {
			onPlayerInfo(reply);
			return ;
		}
	}

	void YoutubeGrab::onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url)
	{
		emit reply->redirectAllowed();
	}
} } } }
