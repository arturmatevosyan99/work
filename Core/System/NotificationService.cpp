#include "NotificationService.hpp"

#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	NotificationService::NotificationService()
	:
		QObject(nullptr)
	{

	}

	const QString& NotificationService::getToken() const
	{
		return _token;
	}

	void NotificationService::setToken(QString token)
	{
		if (token == _token) {
			qWarning() << "Token" << _token << "was validated";
		} else if (token.isEmpty()) {
			qWarning() << "Token" << _token << "was invalidated";
		} else if (_token.isEmpty()) {
			qWarning() << "Token" << token << "was registered";
		} else {
			qWarning() << "Token" << _token << "was revalidated";
		}
		_token.swap(token);
		emit tokenAcquired(token);
	}

	bool NotificationService::hasToken() const
	{
		return !_token.isEmpty();
	}
} } } }
