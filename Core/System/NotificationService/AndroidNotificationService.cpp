#include "AndroidNotificationService.hpp"

#include <QString>
#include <QtAndroid>
#include <QAndroidJniObject>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	AndroidNotificationService::AndroidNotificationService()
	:
		NotificationService()
	{

	}

	void AndroidNotificationService::requestPermission()
	{
		QAndroidJniObject j_manifest_permission
		(
			QAndroidJniObject::getStaticObjectField
			(
				"android/Manifest$permission",
				"POST_NOTIFICATIONS",
				"Ljava/lang/String;"
			)
		);
		if (QtAndroid::androidSdkVersion() < static_cast<std::uint8_t>(System::Android::Version::Tiramisu)) {
			return onPermissionGranted({{j_manifest_permission.toString(), QtAndroid::PermissionResult::Granted}});
		}

		bool retval(false);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval, &j_manifest_permission]()
			{
				qWarning() << "Checking a permission for notifications before request...";
				retval = QtAndroid::checkPermission(j_manifest_permission.toString()) == QtAndroid::PermissionResult::Granted;

				if (QtAndroid::shouldShowRequestPermissionRationale(j_manifest_permission.toString())) {
					qWarning() << "...rationale request is needed";
					/**
					 * @todo: display an educational UI explaining to the user the features that will be enabled
					 * by them granting the POST_NOTIFICATION permission. This UI should provide the user
					 * "OK" and "No thanks" buttons. If the user selects "OK," directly request the permission.
					 * If the user selects "No thanks," allow the user to continue without notifications.
					 */
				}
			}
		);

		if (retval) {
			return onPermissionGranted({{j_manifest_permission.toString(), QtAndroid::PermissionResult::Granted}});
		}

		QtAndroid::requestPermissions
		(
			{j_manifest_permission.toString()},
			std::bind(&AndroidNotificationService::onPermissionGranted, this, std::placeholders::_1)
		);
		qWarning() << "...permission was requested";
	}

	void AndroidNotificationService::requestToken()
	{
		return ;
	}

	void AndroidNotificationService::onPermissionGranted(const QtAndroid::PermissionResultMap& permission_map)
	{
		QAndroidJniObject j_manifest_permission
		(
			QAndroidJniObject::getStaticObjectField
			(
				"android/Manifest$permission",
				"POST_NOTIFICATIONS",
				"Ljava/lang/String;"
			)
		);
		if (permission_map.value(j_manifest_permission.toString(), QtAndroid::PermissionResult::Denied) == QtAndroid::PermissionResult::Granted) {
			emit permissionGranted();
			requestToken();
		}
	}

	bool AndroidNotificationService::hasPermission() const
	{
		if (QtAndroid::androidSdkVersion() < static_cast<std::uint8_t>(System::Android::Version::Tiramisu)) {
			return true;
		}

		bool retval(false);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval]()
			{
				qInfo() << "Checking for a grant for notifications...";

				QAndroidJniObject j_manifest_permission
				(
					QAndroidJniObject::getStaticObjectField
					(
						"android/Manifest$permission",
						"POST_NOTIFICATIONS",
						"Ljava/lang/String;"
					)
				);

				if (QtAndroid::checkPermission(j_manifest_permission.toString()) == QtAndroid::PermissionResult::Granted) {
					retval = true;
				}
			}
		);
		return retval;
	}
} } } }
