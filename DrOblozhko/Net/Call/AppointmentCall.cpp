#include "AppointmentCall.hpp"

#include <QLocale>
#include <QDateTime>
#include <QTimeZone>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Net {
	AppointmentCall::AppointmentCall(Core::Net::Endpoint* endpoint)
	:
		Core::Net::ServiceCall(endpoint, "/droblozhko/reception/appointment")
	{

	}

	void AppointmentCall::request()
	{		
		QUrlQuery query;
		query.addQueryItem("locale", QLocale::system().name());
		query.addQueryItem("timezone", QDateTime::currentDateTime().timeZone().id());
		get(query);
	}

	void AppointmentCall::request(Repository::Meta::Session::Id session_id, Repository::Meta::Session::Handle session_handle)
	{
		QUrlQuery query;
		query.addQueryItem("session_id", QString::number(session_id));
		query.addQueryItem("session_handle", session_handle);
		query.addQueryItem("locale", QLocale::system().name());
		query.addQueryItem("timezone", QDateTime::currentDateTime().timeZone().id());
		get(query);
	}

	void AppointmentCall::request(const EC::Meta::InappReceipt& receipt)
	{
		/// @todo
/*
		QUrlQuery query;
		query.addQueryItem("session_id", QString::number(session_id));
		query.addQueryItem("session_handle", session_handle);
		query.addQueryItem("locale", QLocale::system().name());
		query.addQueryItem("timezone", QDateTime::currentDateTime().timeZone().id());
		post(receipt.getContainer());
*/
	}

	void AppointmentCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		/// @todo Process in onRedirected(reply, url) override
/*
		const QVariant redirection(reply->attribute(QNetworkRequest::RedirectionTargetAttribute));
		const QUrl redirectionURL(redirection.toUrl());
		if (redirectionURL.isValid())	{
			emit redirected(redirectionURL.host(), redirectionURL.port(), redirectionURL.scheme() == "https");
			return ;
		}
*/
		QByteArray data(reply->readAll());
		QJsonParseError error;
		const QJsonDocument document(QJsonDocument::fromJson(data, &error));

		if (error.error != QJsonParseError::NoError || !document.isObject()) {			
			emit failed(error.errorString());
			return ;
		}

		const QJsonObject object(document.object());
		const Repository::Meta::Session::Id session_id(object["session_id"].toString("0").toLongLong());
		const Repository::Meta::Session::Handle session_handle(object["session_handle"].toString("0"));
		const Repository::Meta::Subject::AvatarScheme session_scheme(object["session_scheme"].toString(""));
		const Repository::Meta::Object::Id channel_id(object["channel_id"].toString("0").toLongLong());
		const Repository::Meta::Object::Handle channel_handle(object["channel_handle"].toString("0"));
		const Repository::Meta::Object::ApplicationScheme channel_scheme(object["channel_scheme"].toString(""));

		if (!session_id || session_scheme.isEmpty() || !session_handle || !channel_id || !channel_handle || channel_scheme.isEmpty()) {
			emit failed("Allocation is failed");
			return ;
		}
		emit responded(session_id, session_handle, session_scheme, channel_id, channel_handle, channel_scheme);
	}

	void AppointmentCall::onFault(QSharedPointer<QNetworkReply> reply)
	{		
		emit failed(reply->errorString());
	}
} } } }
