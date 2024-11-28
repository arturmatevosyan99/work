#include "ObjectCursor.hpp"

#include <QFile>

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
/*
	ObjectCursor::ObjectCursor(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectId object_id)
	:
		Core::Net::ServiceCall(endpoint, "/repository/object/file")
	{
		_query.addQueryItem("session_id", QString::number(session_id));
		_query.addQueryItem("session_handle", session_handle);
		_query.addQueryItem("object_id", QString::number(object_id));
	}

	ObjectCursor::ObjectCursor(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectHandle application_handle)
	:
		Core::Net::ServiceCall(endpoint, "/repository/object/file")
	{
		_query.addQueryItem("session_id", QString::number(session_id));
		_query.addQueryItem("session_handle", session_handle);
		_query.addQueryItem("application_handle", QString::number(application_handle));
	}

	ObjectCursor::ObjectCursor(Core::Net::Endpoint* endpoint, QUrlQuery&& moniker)
	:
		Core::Net::ServiceCall(endpoint, "/repository/object/file"),

		_query(std::move(moniker))
	{

	}

	void ObjectCursor::slice(const QString& file_name)
	{
		QUrlQuery query(_query);
		if (!file_name.isEmpty()) {
			query.addQueryItem("file_name", file_name);
		}
		setVirtualServer();
		get(query);
	}

	void ObjectCursor::page(QIODevice* input, const QString& file_name)
	{
		if (!input || !input->isReadable()) {
			return ;
		}
		setVirtualServer();
		QUrlQuery query(_query);
		if (!file_name.isEmpty()) {
			query.addQueryItem("file_name", file_name);
		}
		input->seek(0);
		const QString content_type(ObjectCursor::MimeDatabase.mimeTypeForData(input).name());
		post(input, content_type, query);
	}

	void ObjectCursor::sort(QBuffer* input, const QString& file_name)
	{
		if (!input) {
			return ;
		}
		setVirtualServer();
		QUrlQuery query(_query);
		if (!file_name.isEmpty()) {
			query.addQueryItem("file_name", file_name);
		}
		const QString content_type(ObjectCursor::MimeDatabase.mimeTypeForData(input->data()).name());
		post(input, content_type, query);
	}

	void ObjectCursor::onComplete()
	{
		emit finished(!_data.isEmpty());
	}

	void ObjectCursor::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		_data = reply->readAll();

		const QUrlQuery query(reply->url().query());
		const QString file_name(query.hasQueryItem("file_name") ? query.queryItemValue("file_name") : "");
		if (reply->operation() == QNetworkAccessManager::Operation::PostOperation) { /// < upload
			emit uploaded(file_name);
		} else if (reply->operation() == QNetworkAccessManager::Operation::GetOperation) { /// < download
			emit downloaded(file_name);
		}
	}

	void ObjectCursor::onFault(QSharedPointer<QNetworkReply> reply)
	{
		if (QNetworkReply::NetworkError::OperationCanceledError == reply->error()) {
			return ;
		}
		_data.clear();
		emit faulted(reply->error(), "Could not load object's file");
	}

	void ObjectCursor::onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url)
	{
		reply->close();
		setVirtualServer(url.scheme(), url.host(), url.port());
		setResource(url.path());
		get(QUrlQuery(url));
	}
*/
} } } }
