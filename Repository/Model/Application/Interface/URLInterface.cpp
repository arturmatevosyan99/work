#include "URLInterface.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	URLInterface::Type URLInterface::SchemeType(const QUrl& url)
	{
		if (url.scheme().startsWith("youtube://")) {
			return Type::YouTube;
		} else if (url.scheme().startsWith("cube://")) {
			return Type::CadabraTube;
		} else if (url.scheme().compare("http") == 0 || url.scheme().compare("https") == 0) {
			if (url.path().endsWith(".mp4") || url.path().endsWith(".avi") || url.path().endsWith(".mpeg") || url.path().endsWith(".m3u8")) {
				return Type::File;
			} else {
				return Type::HyperLink;
			}
		} else if (url.scheme().startsWith("rtsp://")) {
			return Type::RTSP;
		}

		return Type::Unknown;
	}

	Core::Net::CRPDelay URLInterface::fetch(const QString& name)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::URLInterface",
			"Cadabra::Ace::Repository::Model::Fetch",
			{
				{"name", name}
			}
		).detach(this, &URLInterface::onFetch);
	}

	Core::Net::CRPDelay URLInterface::sign(const QString& name)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::URLInterface",
			"Cadabra::Ace::Repository::Model::Sign",
			{
				{"name", name}
			}

		).detach(this, &URLInterface::onSign);
	}

	void URLInterface::onFetched(Type type, const QUrl& url)
	{

	}

	void URLInterface::onSigned(Type type, const QUrl& url, const QDateTime& life_time, const QVariantMap& token_map, const QString& view_preset)
	{

	}

	void URLInterface::onFetch(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return ;
		} else if (!(*reaction).isString()) {
			return onFetched(Type::Unaccessible, {});
		}

		const QUrl url((*reaction).toString("file:///dev/null"));
		onFetched(SchemeType(url), url);
	}

	void URLInterface::onSign(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return ;
		} else if (!(*reaction).isObject()) {
			return onSigned(Type::Unaccessible, {}, {}, {}, "");
		}

		const QJsonObject object((*reaction).toObject());
		const QUrl url(object.value("resource_locator").toString(""));
		onSigned
		(
			SchemeType(url),
			url,
			QDateTime::fromSecsSinceEpoch(object.value("life_time").toInt(0), Qt::UTC),
			object.value("token_map").toObject({}).toVariantMap(),
			object.value("view_preset").toString("")
		);
	}
} } } }
