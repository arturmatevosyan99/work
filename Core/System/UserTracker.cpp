/**
 @file UserTracker.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "UserTracker.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	bool UserTracker::Lock(bool onoff)
	{
		static bool retval(onoff);
		return retval;
	}

	UserTracker* UserTracker::Instance()
	{
		static UserTracker retval(!UserTracker::Lock());
		return &retval;
	}

	QObject* UserTracker::QInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
	{
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		UserTracker* retval(UserTracker::Instance());

		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	bool UserTracker::PurchaseEvent(const QString& payment_system, const QString& sku, double quantity, long price, const QString& currency)
	{
		return UserTracker::Instance()->logPurchaseEvent(payment_system, sku, quantity, price, currency);
	}

	bool UserTracker::CustomEvent(const QString& message)
	{
		return UserTracker::Instance()->logCustomEvent(message);
	}

	UserTracker::UserTracker(bool auto_activation)
	:
		QObject(nullptr)
	{
		if (auto_activation) {
			activate();
		}
	}

	UserTracker::~UserTracker()
	{
		deactivate();
	}
#ifndef USERTRACKER_SDK
	void UserTracker::activate()
	{
		return ;
	}

	void UserTracker::deactivate()
	{
		return ;
	}

	bool UserTracker::logPurchaseEvent(const QString& payment_system, const QString& sku, double quantity, long price, const QString& currency)
	{
		return false;
	}

	bool UserTracker::logCustomEvent(const QString& message)
	{
		return false;
	}
#endif
} } } }
