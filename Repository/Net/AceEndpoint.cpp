/**
 @file AceEndpoint.cpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#include "AceEndpoint.hpp"

#include <QDomDocument>

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	AceEndpoint::AceEndpoint()
	:
		Core::Net::Endpoint(AceEndpoint::DefaultHost, AceEndpoint::DefaultPort, AceEndpoint::DefaultSecure),

		_version_call(this)
	{
	}

	AceEndpoint::AceEndpoint(const QUrl& url)
	:
		Core::Net::Endpoint(url),

		_version_call(this)
	{

	}

	AceEndpoint::AceEndpoint(const QString& url)
	:
		Core::Net::Endpoint(QUrl(url)),

		_version_call(this)
	{

	}

	AceEndpoint::AceEndpoint(const QString& host, int port, bool secure)
	:
		Core::Net::Endpoint(host, port, secure),

		_version_call(this)
	{

	}

	AceEndpoint::~AceEndpoint()
	{

	}

	void AceEndpoint::reach()
	{
		_version_call.get(QUrlQuery{});
	}

	AceEndpoint::VersionCall::VersionCall(AceEndpoint* endpoint)
	:
		Core::Net::ServiceCall(endpoint, "/")
	{

	}

	void AceEndpoint::VersionCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		const int http_code(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
		if (http_code == 422) {
			emit getEndpoint()->outdated(Side::Client);
		} else if (http_code != 200) {
			emit getEndpoint()->maintenanced();
		}

		QDomDocument document;
		if(!document.setContent(reply->readAll())) {
			qWarning() << "Failed to parse version document";
			return ;
		}

		const QDomElement status_element(document.documentElement().firstChildElement("status"));
		if (!status_element.isNull()) {
			const QString status(status_element.text().trimmed());
			if (status.compare("OK", Qt::CaseInsensitive) != 0) {
				emit getEndpoint()->maintenanced();
			} else {
				emit getEndpoint()->reached();
			}
		}
	}

	void AceEndpoint::VersionCall::onFault(QSharedPointer<QNetworkReply> reply)
	{
		switch (reply->error()) {
			case QNetworkReply::SslHandshakeFailedError:
			case QNetworkReply::ContentAccessDenied:
			case QNetworkReply::ContentNotFoundError:
			case QNetworkReply::AuthenticationRequiredError:
			case QNetworkReply::ContentGoneError:
			case QNetworkReply::InternalServerError:
			case QNetworkReply::ConnectionRefusedError:
				emit getEndpoint()->maintenanced();
				break;

			case QNetworkReply::UnknownContentError:
				emit getEndpoint()->outdated(Side::Client);
				break;

/*
			case QNetworkReply::RemoteHostClosedError:
			case QNetworkReply::HostNotFoundError:
			case QNetworkReply::TimeoutError:
			case QNetworkReply::OperationCanceledError:
			case QNetworkReply::TemporaryNetworkFailureError:
			case QNetworkReply::NetworkSessionFailedError:
			case QNetworkReply::BackgroundRequestNotAllowedError:
			case QNetworkReply::TooManyRedirectsError:
			case QNetworkReply::InsecureRedirectError:
			case QNetworkReply::ProxyConnectionRefusedError:
			case QNetworkReply::ProxyConnectionClosedError:
			case QNetworkReply::ProxyNotFoundError:
			case QNetworkReply::ProxyTimeoutError:
			case QNetworkReply::ProxyAuthenticationRequiredError:
			case QNetworkReply::ContentOperationNotPermittedError:
			case QNetworkReply::ContentReSendError:
			case QNetworkReply::ContentConflictError:
			case QNetworkReply::OperationNotImplementedError:
			case QNetworkReply::ServiceUnavailableError:
			case QNetworkReply::ProtocolUnknownError:
			case QNetworkReply::ProtocolInvalidOperationError:
			case QNetworkReply::UnknownNetworkError:
			case QNetworkReply::UnknownProxyError:
			case QNetworkReply::UnknownContentError:
			case QNetworkReply::ProtocolFailure:
			case QNetworkReply::UnknownServerError:
*/
			default:
				emit getEndpoint()->maintenanced();
				break;
		}
	}

#ifdef DEFAULT_ACE_HOST
	const QString AceEndpoint::DefaultHost = DEFAULT_ACE_HOST;
#else
	const QString Endpoint::DefaultAceHost = "ace0.cadabra.systems";
#endif

#ifdef DEFAULT_ACE_PORT
	const int AceEndpoint::DefaultPort = DEFAULT_ACE_PORT;
#else
	const int Endpoint::DefaultAcePort = 80;
#endif

#ifdef DEFAULT_ACE_SECURE
	const bool AceEndpoint::DefaultSecure = DEFAULT_ACE_SECURE;
#else
	const bool Endpoint::DefaultAceSecure = false;
#endif
} } } }
