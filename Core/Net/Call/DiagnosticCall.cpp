#include "DiagnosticCall.hpp"

#include "../../System/Native.hpp"

#include <QString>
#include <QScopedPointer>
#include <QCoreApplication>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	Endpoint DiagnosticCall::SOSEndpoint = Endpoint("sos.cadabra.cloud", 443, true);

	DiagnosticCall::DiagnosticCall()
	:
		Net::ServiceCall(&DiagnosticCall::SOSEndpoint)
	{
		_protected_observer_flag_set.setFlag(ServiceCall::ObserverFlag::Upload);
	}

	DiagnosticCall::~DiagnosticCall()
	{
	}

	bool DiagnosticCall::upload(QIODevice* io_device)
	{
/*
		if (!file->isOpen() && !file->open(QFile::ReadOnly)) {
			return false;
		}
*/
		_input = io_device;
		if (_input.isNull()) {
			return false;
		} else if (!_input->isOpen() && !_input->open(QIODevice::ReadOnly)) {
			return false;
		}

		setVirtualServer();
		setResource(QString("/upload/cap/diagnostic/%1/%2/%3/cdb").arg(
																		QCoreApplication::applicationName(),
																		QCoreApplication::applicationVersion(),
																		System::Native::Instance()->getNativeIdentifier()
																	)
		);
		get();
		return true;
	}

	void DiagnosticCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
			emit faulted(tr("Could not upload diagnostic bundle, please try again later"));
		} else {
			emit loaded(1);
		}
		_input.clear();
		setEndpoint(&DiagnosticCall::SOSEndpoint);
	}

	void DiagnosticCall::onFault(QSharedPointer<QNetworkReply> reply)
	{
		if (QNetworkReply::NetworkError::OperationCanceledError == reply->error()) {
			return ;
		}
		_input.clear();
		setEndpoint(&DiagnosticCall::SOSEndpoint);
		emit faulted(QString("%1: %2").arg(tr("Upload error"), reply->errorString()));
	}

	void DiagnosticCall::onUpload(const QScopedPointer<QNetworkReply>& reply, qint64 out, qint64 total)
	{
		emit loaded(0 == out ? 0 : static_cast<double>(out) / static_cast<double>(total));
	}

	void DiagnosticCall::onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url)
	{
		reply->close();
		setVirtualServer(url.scheme(), url.host(), url.port());
		setResource(url.path());
		put(_input, "application/x-cap-diag-bundle", QUrlQuery(url));
	}
} } } }
