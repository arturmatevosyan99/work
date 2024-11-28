#include "../UserTracker.hpp"

#include "../Native/iOS.h"

#include <array>
#include <functional>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void UserTracker::activate()
	{

	}

	void UserTracker::deactivate()
	{

	}

	bool UserTracker::logPurchaseEvent(const QString& payment_system, const QString& sku, double quantity, long price, const QString& currency_code)
	{
		return false;
	}

	bool UserTracker::logCustomEvent(const QString& message)
	{
		return true;
	}
} } } }
