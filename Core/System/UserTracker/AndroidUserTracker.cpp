#include "../UserTracker.hpp"

#include <QtAndroid>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void UserTracker::activate()
	{
		QtAndroid::runOnAndroidThreadSync
		(
			[&]()
			{
				QtAndroid::androidActivity().callMethod<void>("trackUser", "()V");
			}
		);
	}

	void UserTracker::deactivate()
	{
		return ;
	}

	bool UserTracker::logPurchaseEvent(const QString& payment_system, const QString& sku, double quantity, long price, const QString& currency)
	{
		bool retval(false);
		QAndroidJniObject j_payment_system = QAndroidJniObject::fromString(payment_system);
		QAndroidJniObject j_sku = QAndroidJniObject::fromString(sku);
		QAndroidJniObject j_currency = QAndroidJniObject::fromString(currency);
		QtAndroid::runOnAndroidThreadSync
		(
			[&]()
			{
				retval = QtAndroid::androidActivity().callMethod<jint>
				(
					"trackUserPurchase", "(Ljava/lang/String;Ljava/lang/String;DJLjava/lang/String;)Z",
					j_payment_system.object<jstring>(),
					j_sku.object<jstring>(),
					quantity,
					static_cast<jlong>(price),
					j_currency.object<jstring>()
				);
			}
		);
		return retval;
	}

	bool UserTracker::logCustomEvent(const QString& message)
	{
		bool retval(false);
		QAndroidJniObject j_message = QAndroidJniObject::fromString(message);
		QtAndroid::runOnAndroidThreadSync
		(
			[&]()
			{
				retval = QtAndroid::androidActivity().callMethod<jint>
				(
					"trackUserEvent", "(Ljava/lang/String;)Z",
					j_message.object<jstring>()
				);
			}
		);
		return retval;
	}
} } } }
