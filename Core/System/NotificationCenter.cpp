#include "NotificationCenter.hpp"

#include "NotificationService.hpp"

#ifdef APS_KIT
#include "NotificationService/APNService.h"
#endif
#ifdef FCM_SDK
#include "NotificationService/FCMService.hpp"
#endif
#ifdef HCM_SDK
#include "NotificationService/HCMService.hpp"
#endif
#ifdef RSM_SDK
#include "NotificationService/RSMService.hpp"
#endif

#include "Native.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	NotificationCenter* NotificationCenter::Instance()
	{
		static NotificationCenter retval;
		return &retval;
	}

	QObject* NotificationCenter::QInstance(QQmlEngine*, QJSEngine*)
	{
		NotificationCenter* retval(NotificationCenter::Instance());
		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	NotificationCenter::NotificationCenter()
	:
		_family(Family::Unknown),
		_service(nullptr)
	{

	}

	NotificationCenter::~NotificationCenter()
	{

	}

	bool NotificationCenter::setUp(NotificationCenter::Family family, NotificationService* service)
	{
		_family = family;
		if (_service) {
			QObject::disconnect(_service);
		}
		emit initialized(_family);
		if ((_service = service)) {
			QObject::connect( _service, &NotificationService::permissionGranted, this, &NotificationCenter::activated);
			QObject::connect(_service, &NotificationService::tokenAcquired, this, &NotificationCenter::registered);
			QObject::connect(_service, &NotificationService::messageReceived, this, &NotificationCenter::received);
			emit registered(_service->getToken());
		}
		return NotificationCenter::Family::Unknown != family && nullptr != _service;
	}

	bool NotificationCenter::setUp(NotificationCenter::Family family)
	{
		return setUp(family, getService(family));
	}

	bool NotificationCenter::setUp(const QString& token)
	{
		const int at_offset(token.indexOf('@'));
		if (at_offset < 0) {
			return setUp();
		} else if (token.endsWith(".apn")) {
			return setUp(Family::APN, token.left(at_offset));
		} else if (token.endsWith(".fcm") || token.endsWith(".gcm")) {
			return setUp(Family::FCM, token.left(at_offset));
		} else if (token.endsWith(".hcm")) {
			return setUp(Family::HCM, token.left(at_offset));
		} else if (token.endsWith(".rsm")) {
			return setUp(Family::RSM, token.left(at_offset));
		}
		return false;
	}

	bool NotificationCenter::setUp(NotificationCenter::Family family, const QString& token)
	{
		if (!setUp(family)) {
			return false;
		}
		_service->setToken(token);
		return true;
	}

	void NotificationCenter::requestToken() const
	{
		if (_service) {
			_service->requestPermission();
		}
	}

	void NotificationCenter::updateToken() const
	{
		if (_service && !_service->hasToken()) {
			_service->requestPermission();
		}
	}

	void NotificationCenter::propagateMessage(const QString& title, const QString& icon, const QString& sound, const QString& body)
	{
		/// @todo via default service
	}

	NotificationService* NotificationCenter::getService(NotificationCenter::Family family) const
	{
		switch (family) {
			case Family::APN: return Singleton<NotificationCenter::Family::APN>();
			case Family::FCM: return Singleton<NotificationCenter::Family::FCM>();
			case Family::HCM: return Singleton<NotificationCenter::Family::HCM>();
			case Family::RSM: return Singleton<NotificationCenter::Family::RSM>();
			default: return nullptr;
		}
	}

	NotificationService* NotificationCenter::getService() const
	{
		return _service;
	}

	NotificationCenter::Family NotificationCenter::getFamily() const
	{
		return _family;
	}

	QString NotificationCenter::getToken() const
	{
		return _service ? _service->getToken() : "";
	}

	QString NotificationCenter::getPrefixedToken() const
	{
		return _service ? _service->makePrefixedToken() : "";
	}

	QString NotificationCenter::getCNSToken() const
	{
		return _service ? _service->makeCNSToken() : "";
	}

	bool NotificationCenter::hasService() const
	{
		return nullptr != _service;
	}

	bool NotificationCenter::hasToken() const
	{
		return _service && _service->getToken().isEmpty();
	}

	bool NotificationCenter::hasPermission() const
	{
		return _service && _service->hasPermission();
	}
} } } }
