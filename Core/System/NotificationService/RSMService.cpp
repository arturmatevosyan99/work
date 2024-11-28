#include "RSMService.hpp"

#include <QAndroidIntent>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	template<>
	const NotificationCenter::ServiceFactory NotificationCenter::Singleton<NotificationCenter::Family::RSM> = &NotificationService::Instance<RSMService>;

	RSMService::RSMService()
	:
		AndroidNotificationService()
	{
		qInfo() << "Enabling RSM service...";
		QScopedPointer<RSMAvailabilityListener> availability_listener(new RSMAvailabilityListener(this));
		QObject::connect
		(
			availability_listener.data(),
			&RSMAvailabilityListener::done,

			this,
			[](bool success)
			{
				if (!success) {
					qWarning() << "RuStore Messaging Service in unavailable";
					return ;
				}

				QtAndroid::runOnAndroidThread
				(
					[]()
					{
						QAndroidJniObject jni_rustore = QAndroidJniObject::getStaticObjectField
						(
							"ru/rustore/sdk/pushclient/RuStorePushClient",
							"INSTANCE",
							"Lru/rustore/sdk/pushclient/RuStorePushClient;"
						);
						if (!jni_rustore.isValid()) {
							qWarning() << "Unexpected RuStorePushClient instance";
							return;
						}
/*
						QAndroidIntent j_intent
						(
							QtAndroid::androidActivity().object(),
							"systems.cadabra.queen.RSMService"
						);
						j_intent.handle().callObjectMethod
						(
							"putExtra",
							"(Ljava/lang/String;J)Landroid/content/Intent;",
							QAndroidJniObject::fromString("Backend").object(),
							static_cast<jlong>(reinterpret_cast<quintptr>(this))
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
							qWarning() << "Unexpected RSMService component";
							return ;
						}
*/
						QAndroidJniEnvironment jni_environment;
						QAndroidJniObject jni_service_class("systems.cadabra.queen.RSMService");
						jclass j_service_class(jni_environment->GetObjectClass(jni_service_class.object<jobject>()));
						if (!j_service_class) {
							qWarning() << "Unexpected RSMService class";
							return ;
						}
						JNINativeMethod jni_method_array[]
						{
							{
								"Token",
								"(Ljava/lang/String;)V",
								reinterpret_cast<void*>(AndroidNotificationService::JavaNativeInteface<NotificationCenter::Family::RSM>::Token)
							},
							{
								"Message",
								"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
								reinterpret_cast<void*>(AndroidNotificationService::JavaNativeInteface<NotificationCenter::Family::RSM>::Message)
							}
						};
						jni_environment->RegisterNatives(j_service_class, jni_method_array, sizeof(jni_method_array) / sizeof(jni_method_array[0]));
						jni_environment->DeleteLocalRef(j_service_class);

					}
				);
			}
		);

		QtAndroid::runOnAndroidThreadSync
		(
			[&availability_listener]()
			{
				QAndroidJniObject jni_rustore = QAndroidJniObject::getStaticObjectField
				(
					"ru/rustore/sdk/pushclient/RuStorePushClient",
					"INSTANCE",
					"Lru/rustore/sdk/pushclient/RuStorePushClient;"
				);
				if (!jni_rustore.isValid()) {
					qWarning() << "Unexpected RuStorePushClient instance";
					return;
				}

				QAndroidJniObject jni_application
				(
					QtAndroid::androidActivity().callObjectMethod
					(
						"getApplication",
						"()Landroid/app/Application;"
					)
				);
				if (!jni_application.isValid()) {
					qWarning() << "Unexpected Android Application instance";
					return;
				}
#ifdef RSM_SDK
				QAndroidJniObject jni_app_id(QAndroidJniObject::fromString(RSM_SDK));
#else
				QAndroidJniObject jni_app_id(QAndroidJniObject::fromString(""));
#endif
				jni_rustore.callMethod<void>
				(
					"init",
					"(Landroid/app/Application;Ljava/lang/String;)V",
					jni_application.object(),
					jni_app_id.object<jstring>()
				);

				QAndroidJniObject jni_rustore_availability
				(
					jni_rustore.callObjectMethod
					(
						"checkPushAvailability",
						"(Landroid/content/Context;)Lru/rustore/sdk/core/tasks/Task;",
						QtAndroid::androidActivity().object()
					)
				);

				if (jni_rustore_availability.isValid()) {
					jni_rustore_availability.callObjectMethod
					(
						"addOnCompleteListener",
						"(Lru/rustore/sdk/core/tasks/OnCompleteListener;)Lru/rustore/sdk/core/tasks/Task;",
						static_cast<jobject>(*(availability_listener.take()))
					);
				} else {
					qWarning() << "RuStore messaging availability is unknown, but trying to activate...";
					emit availability_listener->done(true);
				}
			}
		);
	}

	void RSMService::requestToken()
	{
		qInfo() << "Requesting the device token from RSM...";
		QScopedPointer<RSMTokenListener> token_listener(new RSMTokenListener(this));
		QObject::connect(token_listener.data(), &RSMTokenListener::done, this, &RSMService::setToken);

		QtAndroid::runOnAndroidThreadSync
		(
			[&token_listener]()
			{
				QAndroidJniObject jni_rustore = QAndroidJniObject::getStaticObjectField
				(
					"ru/rustore/sdk/pushclient/RuStorePushClient",
					"INSTANCE",
					"Lru/rustore/sdk/pushclient/RuStorePushClient;"
				);
				if (!jni_rustore.isValid()) {
					qWarning() << "Unexpected RuStorePushClient instance";
					return;
				}

				QAndroidJniObject token_task = jni_rustore.callObjectMethod("getToken", "()Lru/rustore/sdk/core/tasks/Task;");
				if (!token_task.isValid()) {
					qWarning() << "Unexpected error during RSM token request";
					return;
				}

				token_task.callObjectMethod
				(
					"addOnCompleteListener",
					"(Lru/rustore/sdk/core/tasks/OnCompleteListener;)Lru/rustore/sdk/core/tasks/Task;",
					static_cast<jobject>(*(token_listener.take()))
				);
				qDebug() << "RSM token was request";
			}
		);
		return ;
	}

	QString RSMService::makePrefixedToken() const
	{
		return "R:" + _token;
	}

	QString RSMService::makeCNSToken() const
	{
		return QString("%1@%2.rsm").arg(_token, Native::Instance()->getBundleIdentifier());
	}

	void RSMTokenListener::Back(JNIEnv* environment, jobject self, jlong native, jstring token)
	{
		QScopedPointer<RSMTokenListener, QScopedPointerDeleteLater> listener(reinterpret_cast<RSMTokenListener*>(native));
		emit listener->done(environment->GetStringUTFChars(token, nullptr));
	}

	RSMTokenListener::RSMTokenListener(QObject* parent)
	:
		AndroidListener(parent)
	{
		QAndroidJniObject& jni_object(_jni_object = QAndroidJniObject("systems/cadabra/queen/RSMTokenListener", "(J)V", static_cast<jlong>(reinterpret_cast<quintptr>(this))));
		QtAndroid::runOnAndroidThreadSync
		(
			[&jni_object]()
			{
				QAndroidJniEnvironment jni_environment;
				jclass j_class = jni_environment->GetObjectClass(jni_object.object<jobject>());
				if (j_class != nullptr) {
					JNINativeMethod NativeMethod{"Back", "(JLjava/lang/String;)V", reinterpret_cast<void*>(RSMTokenListener::Back)};
					jni_environment->RegisterNatives(j_class, &NativeMethod, 1);
					jni_environment->DeleteLocalRef(j_class);
				}
			}
		);
	}

	void RSMAvailabilityListener::Back(JNIEnv* environment, jobject self, jlong native, jboolean success)
	{
		QScopedPointer<RSMAvailabilityListener, QScopedPointerDeleteLater> listener(reinterpret_cast<RSMAvailabilityListener*>(native));
		emit listener->done(success);
	}

	RSMAvailabilityListener::RSMAvailabilityListener(QObject* parent)
	:
		AndroidListener(parent)
	{
		QAndroidJniObject& jni_object(_jni_object = QAndroidJniObject("systems/cadabra/queen/RSMAvailabilityListener", "(J)V", static_cast<jlong>(reinterpret_cast<quintptr>(this))));
		QtAndroid::runOnAndroidThreadSync
		(
			[&jni_object]()
			{
				QAndroidJniEnvironment jni_environment;
				jclass j_class = jni_environment->GetObjectClass(jni_object.object<jobject>());
				if (j_class != nullptr) {
					JNINativeMethod NativeMethod{"Back", "(JZ)V", reinterpret_cast<void*>(RSMAvailabilityListener::Back)};
					jni_environment->RegisterNatives(j_class, &NativeMethod, 1);
					jni_environment->DeleteLocalRef(j_class);
				}
			}
		);
	}
} } } }
