/**
 @file Endpoint.cpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#include "Endpoint.hpp"

#include "../System/Alert/StateAlert.hpp"

#include <QNetworkProxy>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	Endpoint* Endpoint::Instance(const QUrl& url)
	{
		QString scheme(url.scheme());
		const bool secure(scheme.isEmpty() ? false : scheme.back() == 's');
		return new Endpoint(url.host(), url.port(secure ? 443 : 80), secure);
	}

	Endpoint::Endpoint(QObject* parent)
	:
		QObject(parent)
	{
		QObject::connect(this, &Endpoint::reached, this, &Endpoint::onReached, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::outdated, this, &Endpoint::onOutdated, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::maintenanced, this, &Endpoint::onMaintenanced, Qt::ConnectionType::DirectConnection);
	}

	Endpoint::Endpoint(const QString& host, int port, bool secure)
	:
		QObject(nullptr),

		_host(host),
		_port(port),
		_secure(secure)
	{
		QObject::connect(this, &Endpoint::reached, this, &Endpoint::onReached, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::outdated, this, &Endpoint::onOutdated, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::maintenanced, this, &Endpoint::onMaintenanced, Qt::ConnectionType::DirectConnection);
	}

	Endpoint::Endpoint(const QString& url)
	:
		Endpoint(QUrl(url))
	{
		QObject::connect(this, &Endpoint::reached, this, &Endpoint::onReached, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::outdated, this, &Endpoint::onOutdated, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::maintenanced, this, &Endpoint::onMaintenanced, Qt::ConnectionType::DirectConnection);
	}

	Endpoint::Endpoint(const QUrl& url)
	:
		QObject(nullptr),

		_host(url.host()),
		_port(url.port()),
		_secure(url.scheme() == "https")
	{
		QObject::connect(this, &Endpoint::reached, this, &Endpoint::onReached, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::outdated, this, &Endpoint::onOutdated, Qt::ConnectionType::DirectConnection);
		QObject::connect(this, &Endpoint::maintenanced, this, &Endpoint::onMaintenanced, Qt::ConnectionType::DirectConnection);
	}

	Endpoint::~Endpoint()
	{

	}

	void Endpoint::reach()
	{
		emit reached();
	}

	QNetworkReply* Endpoint::get(const QNetworkRequest& request)
	{
		return _access_manager.get(request);
	}

	QNetworkReply* Endpoint::del(const QNetworkRequest& request)
	{
		return _access_manager.deleteResource(request);
	}

	QNetworkReply* Endpoint::post(const QNetworkRequest& request, QIODevice* content)
	{
		return _access_manager.post(request, content);
	}

	QNetworkReply* Endpoint::post(const QNetworkRequest& request, QHttpMultiPart* content)
	{
		return _access_manager.post(request, content);
	}

	QNetworkReply* Endpoint::post(const QNetworkRequest& request, const QByteArray& content)
	{
		return _access_manager.post(request, content);
	}

	QNetworkReply* Endpoint::put(const QNetworkRequest& request, QIODevice* content)
	{
		return _access_manager.put(request, content);
	}

	QUrl Endpoint::makeBaseURL() const
	{
		QUrl retval;
		retval.setScheme(_secure ? "https" : "http");
		retval.setHost(_host);
		retval.setPort(_port);

		return retval;
	}

	const QString& Endpoint::getHost() const
	{
		return _host;
	}

	int Endpoint::getPort() const
	{
		return _port;
	}

	QNetworkProxy Endpoint::getProxy() const
	{
		return _access_manager.proxy();
	}

	bool Endpoint::isSecured() const
	{
		return _secure;
	}

	bool Endpoint::isProxied() const
	{
		return _access_manager.proxy().type() != QNetworkProxy::DefaultProxy;
	}

	void Endpoint::setURL(const QUrl& value)
	{
		_secure = value.scheme().contains("https") || value.scheme().contains("aces");
		emit secureChanged();
		_host = value.host();
		emit hostChanged();
		_port = value.port(_secure ? 443 : 80);
		emit portChanged();
	}

	void Endpoint::setURL(const QString& value)
	{
		setURL(QUrl(value));
	}

	void Endpoint::setPort(int port)
	{
		if (_port != port) {
			_port = port;
			emit portChanged();
		}
	}

	void Endpoint::setHost(const QString& host)
	{
		if (_host != host) {
			_host = host;
			emit hostChanged();
		}
	}

	void Endpoint::setSecure(bool onoff)
	{
		if (_secure != onoff) {
			_secure = onoff;
			emit secureChanged();
		}
	}

	void Endpoint::setProxy(const QNetworkProxy& proxy)
	{
		_access_manager.setProxy(proxy);
		emit proxyChanged();
	}

	void Endpoint::onReached()
	{
		Core::System::StateAlert* state(findChild<Core::System::StateAlert*>("ENDPOINT_UNAVAILABLE"));
		if (state->discard()) {
			state->deleteLater();
		}
	}

	void Endpoint::onOutdated(Side side)
	{
		Core::System::StateAlert* state(findChild<Core::System::StateAlert*>("ENDPOINT_UNAVAILABLE"));
		if (!state) {
			Core::System::Alert::Instance()->enqueue
			(
				new Core::System::StateAlert
				(
					this,
					"ENDPOINT_UNAVAILABLE",
					tr("Connection unavailable"),
					Side::Server == side ? tr("Server is outdated") : tr("Client is outdated"),
					this,
					Core::System::StateAlert::Level::Warning
				)
			);
		}
	}

	void Endpoint::onMaintenanced()
	{
		Core::System::StateAlert* state(findChild<Core::System::StateAlert*>("ENDPOINT_UNAVAILABLE"));
		if (!state) {
			Core::System::Alert::Instance()->enqueue
			(
				new Core::System::StateAlert
				(
					this,
					"ENDPOINT_UNAVAILABLE",
					tr("Offline Mode"),
					tr("Server is temporarily out of service due to maintenance downtime or capacity problems"),
					this,
					Core::System::StateAlert::Level::Warning
				)
			);
		}
	}

	QDataStream& operator<<(QDataStream& out, const Endpoint& endpoint)
	{
		out << endpoint._host << endpoint._port << endpoint._secure;

		return out;
	}

	QDataStream& operator>>(QDataStream& in, Endpoint& endpoint)
	{
		in >> endpoint._host;
		in >> endpoint._port;
		in >> endpoint._secure;

		return in;
	}
} } } }
