#include "ITunesGrab.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	Endpoint ITunesGrab::Endpoint = Core::Net::Endpoint("itunes.apple.com", 443, true);

	ITunesGrab::ITunesGrab()
	:
		ServiceCall(&ITunesGrab::Endpoint, "/lookup")
	{
		setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_5) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.1.1 Safari/605.1.15");
	}

	bool ITunesGrab::lookup(const QString& bundle_id)
	{
		if (bundle_id.isEmpty()) {
			return false;
		}
		QUrlQuery query;
		query.addQueryItem("bundleId", bundle_id);
		get(query);
		return true;
	}

	void ITunesGrab::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
			emit faulted(tr("Itunes is not available"));
		}

		const QJsonDocument document(QJsonDocument::fromJson(reply->readAll()));
		if (document.isNull()) {
			emit faulted(tr("Itunes is not available"));
			return;
		}
		const QJsonObject result(document.object());
		if (result.value("resultCount").toInt(0) <= 0) {
			return ;
		}
		const QJsonArray bundle_array(result.value("results").toArray());
		if (bundle_array.count() <= 0) {
			return ;
		}
		const QJsonObject bundle(bundle_array.at(0).toObject());
		emit lookedup(Meta::ITunesItem(bundle.value("trackId").toInt(0), bundle.value("bundleId").toString("noname"), bundle.value("trackCensoredName").toString("noname"), bundle.value("trackName").toString("noname"), bundle.value("description").toString(""), bundle.value("version").toString("0.0.0")));
	}

	void ITunesGrab::onFault(QSharedPointer<QNetworkReply> reply)
	{
		emit faulted(reply->errorString());
	}
} } } }
