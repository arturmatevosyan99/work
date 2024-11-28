#include "FileCall.hpp"

#include "../../../Qaos/String.hpp"

#include <QDir>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	Endpoint FileCall::GeneralEndpoint = Endpoint("", 0, false);

	FileCall::FileCall(const QString& filename_template)
	:
		Core::Net::ServiceCall(&FileCall::GeneralEndpoint),

		_file_path(QDir::tempPath()),
		_filename_template(filename_template)
	{
		_file.setAutoRemove(true);
		setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_5) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.1.1 Safari/605.1.15");
	}

	FileCall::State FileCall::load(QByteArray&& data)
	{
		if (data.isEmpty()) {
			emit faulted("Byte array is empty");
			return State::Fault;
		}

		_data.swap(data);
		if (_file.isOpen()) {
			_file.close();
		}
		_file.setFileTemplate(QString("%1/%2").arg(_file_path, _filename_template));
		if (_file.open() && _file.write(_data) > 0 && _file.flush()) {
			emit loaded();
			return State::Cached;
		}
		emit faulted(QString("File save error(%1): %2").arg(_file.error()).arg(_file.errorString()));
		return State::Fault;
	}

	bool FileCall::remove()
	{
		_file.close();
		if (!_file.remove()) {
			return false;
		}
		_file.setAutoRemove(false);
		return true;
	}

	FileCall::State FileCall::load(const QUrl& url)
	{
		get(url);
		return State::Deffer;
	}

	QUrl FileCall::makeURL() const
	{
		return QUrl::fromLocalFile(_file.fileName());
	}

	FileCall::CheckSum FileCall::makeCheckSum() const
	{
		return {"crc32", Qaos::String::CRC32(_data.constBegin(), _data.constEnd())};
	}

	void FileCall::setAutoRemove(bool onoff)
	{
		_file.setAutoRemove(onoff);
	}

	void FileCall::setFilenameTemplate(const QString& value)
	{
		_filename_template = value.isEmpty() ? QString("XXXXXX.tmp") : value;
	}

	void FileCall::setCachePath(const QString& path, bool auto_create)
	{
		_file_path = path.isEmpty() ? QDir::tempPath() : path;
		if (auto_create && !QDir(_file_path).mkpath(".")) {
			qWarning("Cache path is unreachable");
		}
	}

	const QString& FileCall::getFilenameTemplate() const
	{
		return _filename_template;
	}

	const QString& FileCall::getCachePath() const
	{
		return _file_path;
	}

	const QByteArray& FileCall::getData() const
	{
		return _data;
	}

	const QFile& FileCall::getFile() const
	{
		return _file;
	}

	void FileCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		if (!reply || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
			emit faulted(tr("We are currently undergoing scheduled maintenance. Please come back later."));
		} else {
			load(reply->readAll());
		}
	}

	void FileCall::onFault(QSharedPointer<QNetworkReply> reply)
	{
		qDebug() << "Network fault:" << reply->errorString();
		emit faulted(reply->errorString());
/*
		switch (reply->error()) {
			case QNetworkReply::SslHandshakeFailedError:
			case QNetworkReply::ContentAccessDenied:
			case QNetworkReply::ContentNotFoundError:
			case QNetworkReply::AuthenticationRequiredError:
			case QNetworkReply::ContentGoneError:
			case QNetworkReply::InternalServerError:
			case QNetworkReply::ConnectionRefusedError:
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
		}
*/
	}
} } } }
