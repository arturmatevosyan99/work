#include "CubeGrab.hpp"

#include "../../../System/Native.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <array>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CubeGrab::CubeGrab(const QUuid& uid, Plane plane, const QString& license_key)
	:
		MediaGrab(QUrl(QString("https://cts.cadabra.cloud/meta/%1/%2").arg(static_cast<char>(plane)).arg(uid.toString(QUuid::WithoutBraces)))),

		_cache_path(QString("%1/%2.cctube").arg(Core::System::Native::Instance()->getDataDirectory().absolutePath(), QString::number(qHash(license_key), 16))),
		_uid(uid),
		_plane(plane)
	{
		setXeader(
					"Cadabra-Cloud-License",
					license_key
		);
		setXeader(
					"Cadabra-Cloud-Agent",
					QString("crn:cc:agent:%1.%2").arg
					(
						Core::System::Native::Instance()->getBundleIdentifier(),
						Core::System::Native::Instance()->getNativeIdentifier()
					)
		);
		if (!_cache_path.mkpath(".")) {
			qWarning("Failed to create writable directory for Cube cache at %s", qPrintable(_cache_path.absolutePath()));
		}
	}

	bool CubeGrab::fetch()
	{
		QFile file(makeCacheFile());
		if (
			file.exists()
			&&
			file.open(QIODevice::ReadOnly)
			&&
			add(QJsonDocument::fromJson(file.readAll()))
		) {
			emit fetched();
			return true;
		}
		get();
		return true;
	}

	bool CubeGrab::add(const QJsonDocument& document)
	{
		if (!document.isObject()) {
			return false;
		}

		const QJsonObject object(document.object());
		const qint64 delta_time(object.value("sign_expiration").toInt(0) - QDateTime::currentSecsSinceEpoch());
		if (delta_time < 3600) {
			return false;
		}
		const QString host_name(object.value("host_name").toString(""));
		if (host_name.isEmpty()) {
			return false;
		}

		const QString sign_policy(object.value("sign_policy").toString(""));
		const QString sign_ature(object.value("sign_ature").toString(""));
		const QString sign_key_pair_id(object.value("sign_key_pair_id").toString(""));

		/// @todo index on the first scan
		for (
				QMap<QString, QSize>::ConstIterator r = MediaGrab::VideoResolutionMap.constBegin();
				r != MediaGrab::VideoResolutionMap.constEnd();
				++r
			) {
			for (const QString& resolution : std::array<QString, 2>{QString("%1x%2").arg(r->width()).arg(r->height()), QString("%1x%2").arg(r->height()).arg(r->width())}) {
				const QJsonValue list_value(object.value(resolution));
				if (list_value.isArray()) {
					const QJsonArray value_list(list_value.toArray());
					for (QJsonArray::ConstIterator v = value_list.constBegin(); v != value_list.constEnd(); ++v) {
						const QString file_name(v->isString() ? v->toString("") : "");
						if (file_name.endsWith(".m3u8")) {
							addStream(
										r.key(),
										QString("https://cts.cadabra.cloud/download/%1/%2.m3u8?Filename=%3&Policy=%4&Signature=%5&Key-Pair-Id=%6")
										.arg(static_cast<char>(_plane)).arg(_uid.toString(QUuid::WithoutBraces), file_name, sign_policy, sign_ature, sign_key_pair_id));
						}
					}
					break;
				}
			}
		}
		return true;
	}

	QFile CubeGrab::makeCacheFile() const
	{
		switch (_plane) {
			case Plane::Video:
				return QFile(_cache_path.filePath(_uid.toString(QUuid::WithoutBraces)) + ".video");

			default:
				return QFile(_cache_path.filePath(_uid.toString(QUuid::WithoutBraces)));
		}
	}

	void CubeGrab::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		const QJsonDocument document(QJsonDocument::fromJson(reply->readAll()));
		if (!add(document)) {
			qWarning() << "Could not scan cube meta file:" << _uid.toString(QUuid::WithoutBraces).append(".json");
			return ;
		}
		emit fetched();
		QFile file(makeCacheFile());
		if (
			!file.open(QIODevice::Truncate | QIODevice::WriteOnly)
			||
			!file.write(document.toJson(QJsonDocument::JsonFormat::Indented))
		) {
			qWarning() << "Could not save cube meta file to:" << file.fileName() << "because of a error:" << file.errorString();
			return ;
		}
	}

	void CubeGrab::onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url)
	{
		qWarning() << "Unexpected redirection to:" << url;
		emit reply->redirectAllowed();
	}
} } } }
