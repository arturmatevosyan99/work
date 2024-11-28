#include "ServiceCall.hpp"

#include <QCoreApplication>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	ServiceCall::ServiceCall(const QUrl& endpoint)
	:
		QObject(nullptr),

		_endpoint(Endpoint::Instance(endpoint)),
		_virtual_port(0),
		_resource(endpoint.path()),
		_transfer_timeout(QNetworkRequest::DefaultTransferTimeoutConstant),
		_protected_observer_flag_set(ObserverFlag::Error | ObserverFlag::Finish | ObserverFlag::Redirect | ObserverFlag::SSLError)
	{
		_endpoint->setParent(this);
	}

	ServiceCall::ServiceCall(Core::Net::Endpoint* endpoint)
	:
		QObject(nullptr),

		_endpoint(endpoint),
		_virtual_port(0),
		_transfer_timeout(QNetworkRequest::DefaultTransferTimeoutConstant),
		_protected_observer_flag_set(ObserverFlag::Error | ObserverFlag::Finish | ObserverFlag::Redirect | ObserverFlag::SSLError)
	{

	}

	ServiceCall::ServiceCall(Core::Net::Endpoint* endpoint, const QString& resource)
	:
		QObject(endpoint),

		_endpoint(endpoint),
		_virtual_port(0),
		_resource(resource),
		_transfer_timeout(QNetworkRequest::DefaultTransferTimeoutConstant),
		_protected_observer_flag_set(ObserverFlag::Error | ObserverFlag::Finish | ObserverFlag::Redirect | ObserverFlag::SSLError)
	{

	}

	ServiceCall::~ServiceCall()
	{
		foreach (QNetworkReply* value, _set) {
			qDebug() << "ServiceCall(" << reinterpret_cast<uintptr_t>(this) << ")::QNetworkReply(" << reinterpret_cast<uintptr_t>(value) << ") * Deleting...";
			value->abort();
			QObject::disconnect(value);
			value->deleteLater();
		}
	}

	QString ServiceCall::makeUserAgent() const
	{
		return _user_agent.isEmpty() ? QString("cadabra.%1/%2").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion()) : _user_agent;
	}

	QNetworkRequest ServiceCall::makeRequest(const QUrlQuery& query) const
	{
		QUrl url(_endpoint->makeBaseURL()); /// @??? endpoint === nullptr from VersionCall
		url.setPath(_resource);
		if (!_virtual_scheme.isEmpty()) {
			url.setScheme(_virtual_scheme);
		}
		if (!_virtual_host.isEmpty()) {
			url.setHost(_virtual_host);
		}
		if (_virtual_port > 0) {
			url.setPort(_virtual_port);
		}
		if (!query.isEmpty()) {
			url.setQuery(query);
		}

		QNetworkRequest request;
		for (QMap<QString, QString>::ConstIterator x = _xeader_map.constBegin(); x != _xeader_map.constEnd(); ++x) {
			request.setRawHeader(x.key().toLatin1().prepend("X-"), x.value().toLatin1());
		}
		request.setUrl(url);
		request.setHeader(QNetworkRequest::UserAgentHeader, makeUserAgent());
		request.setRawHeader(QByteArray("Host"), url.host().toUtf8());
		request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::UserVerifiedRedirectPolicy);
		request.setTransferTimeout(_transfer_timeout);

		return request;
	}

	void ServiceCall::observe(QNetworkReply* reply)
	{
		if (!reply) {
			return;
		}

		_set.insert(reply);
		QObject::connect
		(
			reply,
			&QObject::destroyed,

			this,
			&ServiceCall::onDestroyed
		);

		if (_protected_observer_flag_set.testFlag(ObserverFlag::Error) || _public_observer_flag_set.testFlag(ObserverFlag::Error)) {
			QObject::connect
			(
				reply,
				qOverload<QNetworkReply::NetworkError>(&QNetworkReply::errorOccurred),

				this,
				std::bind
				(
					(void(ServiceCall::*)(QNetworkReply* reply_raw, QNetworkReply::NetworkError error))&ServiceCall::onError,
					this,
					reply, std::placeholders::_1
				)
			);
		}
		if (_protected_observer_flag_set.testFlag(ObserverFlag::Finish) || _public_observer_flag_set.testFlag(ObserverFlag::Finish)) {
			QObject::connect
			(
				reply,
				&QNetworkReply::finished,

				this,
				std::bind
				(
					&ServiceCall::onFinished,
					this,
					reply
				)
			);
		}
		if (_protected_observer_flag_set.testFlag(ObserverFlag::Redirect)) {
			QObject::connect
			(
				reply,
				&QNetworkReply::redirected,

				this,
				std::bind
				(
					&ServiceCall::onRedirected,
					this,
					reply, std::placeholders::_1
				)
			);
		}
		if (_protected_observer_flag_set.testFlag(ObserverFlag::Download) || _public_observer_flag_set.testFlag(ObserverFlag::Download)) {
			QObject::connect
			(
				reply,
				&QNetworkReply::downloadProgress,

				this,
				std::bind
				(
					&ServiceCall::onLoaded,
					this,
					reply, true, std::placeholders::_1, std::placeholders::_2
				)
			);
		}
		if (_protected_observer_flag_set.testFlag(ObserverFlag::Upload) || _public_observer_flag_set.testFlag(ObserverFlag::Upload)) {
			QObject::connect
			(
				reply,
				&QNetworkReply::uploadProgress,

				this,
				std::bind
				(
					&ServiceCall::onLoaded,
					this,
					reply, false, std::placeholders::_1, std::placeholders::_2
				)
			);
		}
		if (_protected_observer_flag_set.testFlag(ObserverFlag::SSLError) || _public_observer_flag_set.testFlag(ObserverFlag::SSLError)) {
			QObject::connect
			(
				reply,
				qOverload<const QList<QSslError>&>(&QNetworkReply::sslErrors),

				this,
				std::bind
				(
					(void(ServiceCall::*)(QNetworkReply* reply_raw, const QList<QSslError>& ssl_error_list))&ServiceCall::onError,
					this,
					reply, std::placeholders::_1
				)
			);
		}
	}

	void ServiceCall::get(const QUrl& url)
	{
		if (!_endpoint) {
			return ;
		}

		QNetworkRequest request(url);
		request.setTransferTimeout(_transfer_timeout);
		observe(_endpoint->get(request));
	}

	void ServiceCall::get(const QUrlQuery& query)
	{
		if (!_endpoint) {
			return ;
		}

		QNetworkRequest request(makeRequest(query));
		observe(_endpoint->get(request));
	}

	void ServiceCall::del(const QUrlQuery& query)
	{
		if (!_endpoint) {
			return ;
		}

		QNetworkRequest request(makeRequest(query));
		observe(_endpoint->del(request));
	}

	void ServiceCall::post(QIODevice* content, const QString& type, const QUrlQuery& query)
	{
		if (!_endpoint) {
			return ;
		}

		QNetworkRequest request(makeRequest(query));
		request.setHeader(QNetworkRequest::ContentTypeHeader, type);
		observe(_endpoint->post(request, content));
	}

	void ServiceCall::post(QHttpMultiPart* content, const QUrlQuery& query)
	{
		if (!_endpoint) {
			content->deleteLater();
			return ;
		}
		QNetworkRequest request(makeRequest(query));
		QNetworkReply* reply(_endpoint->post(request, content));
		if (reply) {
			content->setParent(reply);
		} else {
			content->deleteLater();
		}
	}

	void ServiceCall::post(const QByteArray& content, const QString& type, const QUrlQuery& query)
	{
		if (!_endpoint) {
			return ;
		}

		QNetworkRequest request(makeRequest(query));
		request.setHeader(QNetworkRequest::ContentTypeHeader, type);
		observe(_endpoint->post(request, content));
	}

	void ServiceCall::post(const QJsonDocument& document, const QUrlQuery& query)
	{
		post(document.toJson(QJsonDocument::Compact), "application/json", query);
	}

	void ServiceCall::put(QIODevice* content, const QString& type, const QUrlQuery& query)
	{
		if (!_endpoint) {
			return ;
		}
		QNetworkRequest request(makeRequest(query));
		request.setHeader(QNetworkRequest::ContentTypeHeader, type);
		observe(_endpoint->put(request, content));
	}

	void ServiceCall::setEndpoint(Core::Net::Endpoint* endpoint)
	{
		_endpoint = endpoint;
	}

	void ServiceCall::setVirtualServer(const QString& scheme, const QString& host, int port)
	{
		_virtual_scheme = scheme;
		_virtual_host = host;
		_virtual_port = (port >= 0) ? port : (_virtual_scheme == "http" || _virtual_scheme == "ws") ? 80 : (_virtual_scheme == "https" || _virtual_scheme == "wss") ? 443 : port;
	}

	void ServiceCall::setVirtualScheme(const QString& scheme)
	{
		_virtual_scheme = scheme;
	}

	void ServiceCall::setVirtualHost(const QString& host)
	{
		_virtual_host = host;
	}

	void ServiceCall::setVirtualPort(int port)
	{
		_virtual_port = port;
	}

	void ServiceCall::setResource(const QString& resource)
	{
		_resource = resource;
	}

	void ServiceCall::setTransferTimeout(int value)
	{
		_transfer_timeout = value;
	}

	void ServiceCall::setXeader(const QString& key, const QString& value)
	{
		if (value.isEmpty()) {
			_xeader_map.remove(key);
		} else {
			_xeader_map[key] = value;
		}
	}

	void ServiceCall::setObserver(QFlags<ObserverFlag> flag_set)
	{
		_public_observer_flag_set = flag_set;
	}

	Core::Net::Endpoint* ServiceCall::getEndpoint()
	{
		return _endpoint;
	}

	const QString& ServiceCall::getResource() const
	{
		return _resource;
	}

	void ServiceCall::setUserAgent(const QString& agent)
	{
		_user_agent = agent;
	}

	void ServiceCall::onDestroyed(QObject* object_raw)
	{
		QNetworkReply* reply_raw(qobject_cast<QNetworkReply*>(object_raw));
		if (reply_raw) {
			qWarning() << "ServiceCall(" << reinterpret_cast<uintptr_t>(this) << ")::onDestroyed(" << reinterpret_cast<uintptr_t>(reply_raw) << ") > Finished unexpectedly:" << _set.remove(reply_raw);
		}
	}

	void ServiceCall::onFinished(QNetworkReply* reply_raw)
	{
		QObject::disconnect(reply_raw, &QObject::destroyed, nullptr, nullptr);
		QSet<QNetworkReply*>::iterator r(_set.find(reply_raw));
		if (_set.end() == r) {
			reply_raw->close();
			return ;
		}

		QSharedPointer<QNetworkReply> reply_smart(reply_raw, &QObject::deleteLater);
		const bool is_completed(_set.erase(r) == _set.end());
		reply_smart->close();

		if (reply_smart->error() == QNetworkReply::NoError) {
			onSuccess(reply_smart);
			emit finished();
		} else {
			onFault(reply_smart);
			emit faulted(FaultFlag::General);
		}

		if (is_completed) {
			onComplete();
			QCoreApplication::postEvent(this, new QEvent(QEvent::Type::SockClose), Qt::NormalEventPriority);
		} else {
			qDebug("ServiceCall is not empty when response is finished and closed");
		}
	}

	void ServiceCall::onError(QNetworkReply* reply_raw, QNetworkReply::NetworkError error)
	{
		QSet<QNetworkReply*>::iterator r(_set.find(reply_raw));
		if (_set.end() == r) {
			return ;
		}

		QScopedPointer<QNetworkReply> reply_smart(reply_raw);
		onError(reply_smart, error);
		reply_smart.take();
		emit faulted(FaultFlag::Network);
	}

	void ServiceCall::onError(QNetworkReply* reply_raw, const QList<QSslError>& ssl_error_list)
	{
		std::for_each
		(
			ssl_error_list.constBegin(), ssl_error_list.constEnd(),
			[](const QSslError& error)
			{
				qWarning("SSLError(%d): %s", error.error(), qUtf8Printable(error.errorString()));
			}
		);
		emit faulted(FaultFlag::SSL);
	}

	void ServiceCall::onRedirected(QNetworkReply* reply_raw, const QUrl& url)
	{
		QSet<QNetworkReply*>::iterator r(_set.find(reply_raw));
		if (_set.end() == r) {
			return ;
		}

		QScopedPointer<QNetworkReply> reply_smart(reply_raw);
		onRedirect(reply_smart, url);
		reply_smart.take();
		emit redirected(url);
	}

	void ServiceCall::onLoaded(QNetworkReply* reply_raw, bool inout, qint64 current, qint64 total)
	{
		QScopedPointer<QNetworkReply> reply_smart(reply_raw);
		if (inout) {
			onDownload(reply_smart, current, total);
			emit downloaded(current, total);
		} else {
			onUpload(reply_smart, current, total);
			emit uploaded(current, total);
		}
		reply_smart.take();
	}

	void ServiceCall::onComplete()
	{

	}

	void ServiceCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{

	}

	void ServiceCall::onFault(QSharedPointer<QNetworkReply> reply)
	{

	}

	void ServiceCall::onError(const QScopedPointer<QNetworkReply>& reply, QNetworkReply::NetworkError error)
	{

	}

	void ServiceCall::onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url)
	{

	}

	void ServiceCall::onDownload(const QScopedPointer<QNetworkReply>& reply, qint64 received, qint64 total)
	{

	}

	void ServiceCall::onUpload(const QScopedPointer<QNetworkReply>& reply, qint64 received, qint64 total)
	{

	}

	bool ServiceCall::hasEndpoint() const
	{
		return nullptr != _endpoint;
	}
} } } }
