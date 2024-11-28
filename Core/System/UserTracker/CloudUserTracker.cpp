#include "../UserTracker.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	bool UserTracker::activate()
	{
		return false;
	}

	bool UserTracker::deactivate()
	{
		return false;
	}

	bool UserTracker::logPurchaseEvent(const QString& payment_system, const QString& sku, double quantity, long price, const QString& currency)
	{
		return false;
	}

	bool UserTracker::logCustomEvent(const QString& message)
	{
		return false;
	}
} } } }
