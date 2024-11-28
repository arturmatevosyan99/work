#include "ObjectFile.hpp"

#include <QFile>

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	const QMimeDatabase ObjectFile::MimeDatabase = QMimeDatabase();

	ObjectFile::ObjectFile(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectId object_id)
	:
		Core::Net::ServiceCall(endpoint, "/repository/object/file")
	{
		_query.addQueryItem("session_id", QString::number(session_id));
		_query.addQueryItem("session_handle", session_handle);
		_query.addQueryItem("object_id", QString::number(object_id));
	}

	ObjectFile::ObjectFile(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectHandle application_handle)
	:
		Core::Net::ServiceCall(endpoint, "/repository/object/file")
	{
		_query.addQueryItem("session_id", QString::number(session_id));
		_query.addQueryItem("session_handle", session_handle);
		_query.addQueryItem("application_handle", QString::number(application_handle));
	}

	ObjectFile::ObjectFile(Core::Net::Endpoint* endpoint, QUrlQuery&& moniker)
	:
		Core::Net::ServiceCall(endpoint, "/repository/object/file"),

		_query(std::move(moniker))
	{

	}

	void ObjectFile::download(const QString& file_name)
	{
		QUrlQuery query(_query);
		if (!file_name.isEmpty()) {
			query.addQueryItem("file_name", file_name);
		}
		setVirtualServer();
		get(query);
	}

	void ObjectFile::upload(QIODevice* input, const QString& file_name)
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
		const QString content_type(ObjectFile::MimeDatabase.mimeTypeForData(input).name());
		post(input, content_type, query);
	}

	void ObjectFile::upload(QBuffer* input, const QString& file_name)
	{
		if (!input) {
			return ;
		}
		setVirtualServer();
		QUrlQuery query(_query);
		if (!file_name.isEmpty()) {
			query.addQueryItem("file_name", file_name);
		}
		const QString content_type(ObjectFile::MimeDatabase.mimeTypeForData(input->data()).name());
		post(input, content_type, query);
	}

	void ObjectFile::load(QFile* input)
	{
		if (!input) {
			return ;
		} else if (!input->isOpen() && !input->open(QIODevice::ReadOnly)) {
			_data.clear();
			emit faulted(QNetworkReply::ContentNotFoundError, "Could not load local file");
			return;
		} else {
			input->seek(0);
		}
		_data = input->readAll();
		emit downloaded(input->fileName());
	}

	bool ObjectFile::save(const QString& file_name)
	{
		if (_data.size() <= 0) {
			return false;
		}
		QFile file(file_name);
		if (file.open(QIODevice::WriteOnly)) {
			qDebug("Saving object file at: %s", qPrintable(file_name));
			file.write(_data);
		}
		file.close();
		emit saved(file_name, _data.size());
		return true;
	}

	const QByteArray& ObjectFile::getData() const
	{
		return _data;
	}

	void ObjectFile::onComplete()
	{
		emit finished(!_data.isEmpty());
	}

	void ObjectFile::onSuccess(QSharedPointer<QNetworkReply> reply)
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

	void ObjectFile::onFault(QSharedPointer<QNetworkReply> reply)
	{
		if (QNetworkReply::NetworkError::OperationCanceledError == reply->error()) {
			return ;
		}
		_data.clear();
		emit faulted(reply->error(), "Could not load object's file");
	}

	void ObjectFile::onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url)
	{
		reply->close();
		setVirtualServer(url.scheme(), url.host(), url.port());
		setResource(url.path());
		get(QUrlQuery(url));
	}
} } } }
