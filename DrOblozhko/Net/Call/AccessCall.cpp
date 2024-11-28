#include "AccessCall.hpp"

#include "../../../Core/System/NotificationCenter.hpp"

#include <QLocale>
#include <QDateTime>
#include <QTimeZone>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Net {
	AccessCall::AccessCall(Core::Net::Endpoint* endpoint)
	:
		Core::Net::ServiceCall(endpoint, "/droblozhko/reception/access")
	{

	}

	void AccessCall::recognize(const QString& account)
	{
		_account = account;
		if (!_account.contains('@')) {
			_account.remove(QRegularExpression("[\\+( )\\.\\-]+"));
		}

		QUrlQuery query;
		query.addQueryItem("account", _account);
		query.addQueryItem("code", "R");
		get(query);
	}

	void AccessCall::recover(const QString& account)
	{
		_account = account;
		if (!_account.contains('@')) {
			_account.remove(QRegularExpression("[\\+( )\\.\\-]+"));
		}

		QUrlQuery query;
		query.addQueryItem("account", _account);
		/// @note Если не указан явно, то считаем, что делаем recover
		query.addQueryItem("code", "r");
		get(query);
	}

	void AccessCall::restore(const QString& code)
	{
		if (_account.isEmpty()) {
			emit failed("Account does not exist");
			return ;
		}
		QUrlQuery query;
		query.addQueryItem("account", _account);
		query.addQueryItem("code", code);
		get(query);
	}

	void AccessCall::ratify(const QString& code)
	{
		restore(code);
	}

	void AccessCall::reassign(const QString& name)
	{
		if (_account.isEmpty()) {
			emit failed("Account does not exist");
			return ;
		}
		QUrlQuery query;
		query.addQueryItem("account", _account);
		post(
				QJsonDocument
				{
								QJsonObject
								{
									{"name", name},
									{"locale", QLocale::system().name()},
									{"timezone", QString::fromLatin1(QDateTime::currentDateTime().timeZone().id())},
									{"device_token", Core::System::NotificationCenter::Instance()->getCNSToken()}
								}
				},
				query
		);
	}

	void AccessCall::resign(Cadabra::Queen::Repository::Model::Session* session)
	{
		if (!session) {
			return ;
		}

		QUrlQuery query;
		query.addQueryItem("account", QString::number(session->getPoolId()));
		query.addQueryItem("code", session->getPoolHandle().getValue());
		del(query);
	}

	void AccessCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		QByteArray data(reply->readAll());
		QJsonParseError error;
		const QJsonDocument document(QJsonDocument::fromJson(data, &error));
		if (error.error != QJsonParseError::NoError || !document.isObject()) {
			emit failed(error.errorString());
			return ;
		}

		const QJsonObject object(document.object());
		const QString status(object["status"].toString("error"));

		if (status.compare("error") == 0) {
			emit failed("Unexpected error has been occured");
		} else if (status.compare("ratified") == 0) {
			emit ratified(true);
		} else if (status.compare("recovered") == 0) {
			emit recovered(true);
		} else if (status.compare("unrecovered") == 0) {
			emit recovered(false);
		} else if (status.compare("prepared") == 0) {
			emit prepared(true);
		} else if (status.compare("unprepared") == 0) {
			emit prepared(false);
		} else if (status.compare("restored") == 0 || status.compare("reassigned") == 0) {
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
			if (status.compare("restored") == 0) {
				emit restored(session_id, session_handle, session_scheme, channel_id, channel_handle, channel_scheme);
//				Core::System::NotificationCenter::Instance()->requestToken();
			} else if (status.compare("reassigned") == 0) {
				emit reassigned(session_id, session_handle, session_scheme, channel_id, channel_handle, channel_scheme);
			}
		} else if (status.compare("unrestored") == 0) {
			emit restored(0, 0, "", 0, 0, "");
		} else if (status.compare("unassigned") == 0) {
			emit reassigned(0, 0, "", 0, 0, "");
		} else if (status.compare("resigned") == 0) {
			emit resigned();
		} else {
			emit failed(status);
		}
	}

	void AccessCall::onFault(QSharedPointer<QNetworkReply> reply)
	{		
		emit failed(reply->errorString());
	}

	bool AccessCall::hasAccount() const
	{
		return !_account.isEmpty();
	}
} } } }
