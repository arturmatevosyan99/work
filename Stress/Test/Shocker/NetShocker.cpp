#include "NetShocker.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	NetShocker::NetShocker(QObject* parent)
	:
		Core::Test::AbstractShocker(parent),

		_endpoint(nullptr)
	{

	}

	bool NetShocker::turnOn()
	{
		if (!_endpoint || _endpoint->isProxied()) {
			return false;
		}

		_original_proxy = _endpoint->getProxy();
		qWarning() << "Going offline...";
		_endpoint->setProxy(QNetworkProxy::Socks5Proxy);
		return true;
	}

	bool NetShocker::turnOff()
	{
		if (!_endpoint || !_endpoint->isProxied()) {
			return false;
		}

		_endpoint->setProxy(_original_proxy);
		qWarning() << "Going online...";
		_original_proxy = QNetworkProxy();
		return true;
	}
} } } }
