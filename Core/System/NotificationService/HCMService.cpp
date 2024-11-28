#include "HCMService.hpp"

#include "../NotificationCenter.hpp"

#include <QAndroidIntent>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	template<>
	const NotificationCenter::ServiceFactory NotificationCenter::Singleton<NotificationCenter::Family::HCM> = &NotificationService::Instance<HCMService>;

	HCMService::HCMService()
	:
		AndroidNotificationService()
	{
		qInfo() << "Enabling HCM service...";
		QtAndroid::runOnAndroidThread
		(
			[]()
			{
				QAndroidIntent j_intent
				(
					QtAndroid::androidActivity().object(),
					"systems.cadabra.queen.HCMService"
				);
				QAndroidJniObject jni_component
				(
					QtAndroid::androidActivity().callObjectMethod
					(
						"startService",
						"(Landroid/content/Intent;)Landroid/content/ComponentName;",
						j_intent.handle().object()
					)
				);
				if (!jni_component.isValid()) {
					return ;
				}

				QAndroidJniEnvironment jni_environment;
				QAndroidJniObject jni_service_class("systems.cadabra.queen.HCMService");
				jclass j_service_class(jni_environment->GetObjectClass(jni_service_class.object<jobject>()));
				if (!j_service_class) {
					return ;
				}
				JNINativeMethod jni_method_array[]
				{
					{
						"Token",
						"(Ljava/lang/String;)V",
						reinterpret_cast<void*>(AndroidNotificationService::JavaNativeInteface<NotificationCenter::Family::HCM>::Token)
					},
					{
						"Message",
						"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
						reinterpret_cast<void*>(AndroidNotificationService::JavaNativeInteface<NotificationCenter::Family::HCM>::Message)
					}
				};
				jni_environment->RegisterNatives(j_service_class, jni_method_array, sizeof(jni_method_array) / sizeof(jni_method_array[0]));
				jni_environment->DeleteLocalRef(j_service_class);
			}
		);
	}

	void HCMService::requestToken()
	{
		qInfo() << "Requesting the device token from HCM service...";
		QAndroidJniObject jni_hms = QAndroidJniObject::callStaticObjectMethod
		(
			"com/huawei/hms/aaid/HmsInstanceId",
			"getInstance",
			"(Landroid/content/Context;)Lcom/huawei/hms/aaid/HmsInstanceId;",
			QtAndroid::androidActivity().object<jobject>()
		);
		if (!jni_hms.isValid()) {
			qWarning() << "Unexpected error during HCM token request!";
			return;
		}

		QAndroidJniObject jni_app_scope(QAndroidJniObject::fromString("HCM"));
#ifdef HCM_SDK
		QAndroidJniObject jni_app_id(QAndroidJniObject::fromString(HCM_SDK));
#else
		QAndroidJniObject jni_app_id(QAndroidJniObject::fromString(""));
#endif

		QAndroidJniEnvironment jni_environment;
		QAndroidJniObject jni_token
		(
			jni_hms.callObjectMethod
			(
				"getToken",
				"(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
				jni_app_id.object<jstring>(),
				jni_app_scope.object<jstring>()
			)
		);
		if (jni_environment->ExceptionCheck()) {
			this->setToken("");
			/// @todo check for ApiException
			qWarning() << "Unexpected error during HCM token request";
			QAndroidJniExceptionCleaner(QAndroidJniExceptionCleaner::OutputMode::Verbose).clean();
			return;
		}

		this->setToken(jni_token.toString());
		return ;
	}

	QString HCMService::makePrefixedToken() const
	{
		return "H:" + _token;
	}

	QString HCMService::makeCNSToken() const
	{
		return QString("%1@%2.hcm").arg(_token, Native::Instance()->getBundleIdentifier());
	}
} } } }
