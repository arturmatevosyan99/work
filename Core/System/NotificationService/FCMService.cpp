#include "FCMService.hpp"

#include <QAndroidIntent>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	template<>
	const NotificationCenter::ServiceFactory NotificationCenter::Singleton<NotificationCenter::Family::FCM> = &NotificationService::Instance<FCMService>;

	FCMService::FCMService()
	:
		AndroidNotificationService()
	{
		qInfo() << "Enabling FCM service...";
		QtAndroid::runOnAndroidThread
		(
			[]()
			{
				QAndroidIntent j_intent
				(
					QtAndroid::androidActivity().object(),
					"systems.cadabra.queen.FCMService"
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
				QAndroidJniObject jni_service_class("systems.cadabra.queen.FCMService");
				jclass j_service_class(jni_environment->GetObjectClass(jni_service_class.object<jobject>()));
				if (!j_service_class) {
					return ;
				}
				JNINativeMethod jni_method_array[]
				{
					{
						"Token",
						"(Ljava/lang/String;)V",
						reinterpret_cast<void*>(AndroidNotificationService::JavaNativeInteface<NotificationCenter::Family::FCM>::Token)
					},
					{
						"Message",
						"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
						reinterpret_cast<void*>(AndroidNotificationService::JavaNativeInteface<NotificationCenter::Family::FCM>::Message)
					}
				};
				jni_environment->RegisterNatives(j_service_class, jni_method_array, sizeof(jni_method_array) / sizeof(jni_method_array[0]));
				jni_environment->DeleteLocalRef(j_service_class);

				QAndroidJniObject firebase = QAndroidJniObject::callStaticObjectMethod
				(
					"com/google/firebase/messaging/FirebaseMessaging", "getInstance", "()Lcom/google/firebase/messaging/FirebaseMessaging;"
				);
				firebase.callMethod<void>("setAutoInitEnabled", "(Z)V", true);
			}
		);
	}

	void FCMService::requestToken()
	{
		/**
		 * @brief Android automatically asks the user for permission the first time your app creates a notification channel,
		 * as long as the app is in the foreground. However, there are important caveats regarding the timing of channel creation
		 *  and permission requests: https://firebase.google.com/docs/cloud-messaging/android/client#java_1
		 */
		qInfo() << "Requesting the device token from FCM service...";

		QScopedPointer<GMSCompleteListener> complete_listener(new GMSCompleteListener(this));
		QObject::connect
		(
			complete_listener.data(),
			&GMSCompleteListener::done,

			this,
			&FCMService::setToken
		);
		QScopedPointer<GMSFailureListener> failure_listener(new GMSFailureListener(this));
		QObject::connect
		(
			failure_listener.data(),
			&GMSFailureListener::done,

			this,
			std::bind(&FCMService::setToken, this, QString())
		);

		QtAndroid::runOnAndroidThreadSync
		(
			[&complete_listener, &failure_listener]()
			{
				QAndroidJniObject firebase = QAndroidJniObject::callStaticObjectMethod
				(
					"com/google/firebase/messaging/FirebaseMessaging", "getInstance", "()Lcom/google/firebase/messaging/FirebaseMessaging;"
				);
				if (!firebase.isValid()) {
					qWarning() << "Unexpected error during FCM token request!";
					return;
				}

				QAndroidJniObject token_task = firebase.callObjectMethod("getToken", "()Lcom/google/android/gms/tasks/Task;");
				if (!token_task.isValid()) {
					qWarning() << "Unexpected error during FCM token request!";
					return;
				}

				token_task.callObjectMethod
				(
					"addOnCompleteListener",
					"(Lcom/google/android/gms/tasks/OnCompleteListener;)Lcom/google/android/gms/tasks/Task;",
					static_cast<jobject>(*(complete_listener.take()))
				);
				token_task.callObjectMethod
				(
					"addOnFailureListener",
					"(Lcom/google/android/gms/tasks/OnFailureListener;)Lcom/google/android/gms/tasks/Task;",
					static_cast<jobject>(*(failure_listener.take()))
				);
				qDebug() << "FCM token was request";
			}
		);
		return ;
	}

	QString FCMService::makePrefixedToken() const
	{
		return "F:" + _token;
	}

	QString FCMService::makeCNSToken() const
	{
		return QString("%1@%2.fcm").arg(_token, Native::Instance()->getBundleIdentifier());
	}

	void GMSCompleteListener::Back(JNIEnv* environment, jobject self, jlong native, jstring token)
	{
		QScopedPointer<GMSCompleteListener, QScopedPointerDeleteLater> listener(reinterpret_cast<GMSCompleteListener*>(native));
		emit listener->done(environment->GetStringUTFChars(token, nullptr));
	}

	GMSCompleteListener::GMSCompleteListener(QObject* parent)
	:
		AndroidListener(parent)
	{
		QAndroidJniObject& jni_object(_jni_object = QAndroidJniObject("systems/cadabra/queen/GMSCompleteListener", "(J)V", static_cast<jlong>(reinterpret_cast<quintptr>(this))));
		QtAndroid::runOnAndroidThreadSync
		(
			[&jni_object]()
			{
				QAndroidJniEnvironment jni_environment;
				jclass j_class = jni_environment->GetObjectClass(jni_object.object<jobject>());
				if (j_class != nullptr) {
					JNINativeMethod NativeMethod{"Back", "(JLjava/lang/String;)V", reinterpret_cast<void*>(GMSCompleteListener::Back)};
					jni_environment->RegisterNatives(j_class, &NativeMethod, 1);
					jni_environment->DeleteLocalRef(j_class);
				}
			}
		);
	}

	void GMSFailureListener::Back(JNIEnv* environment, jobject self, jlong native, jstring message)
	{
		QScopedPointer<GMSFailureListener, QScopedPointerDeleteLater> listener(reinterpret_cast<GMSFailureListener*>(native));
		emit listener->done(environment->GetStringUTFChars(message, nullptr));
	}

	GMSFailureListener::GMSFailureListener(QObject* parent)
	:
		AndroidListener(parent)
	{
		QAndroidJniObject& jni_object(_jni_object = QAndroidJniObject("systems/cadabra/queen/GMSFailureListener", "(J)V", static_cast<jlong>(reinterpret_cast<quintptr>(this))));
		QtAndroid::runOnAndroidThreadSync
		(
			[&jni_object]()
			{
				QAndroidJniEnvironment jni_environment;
				jclass j_class = jni_environment->GetObjectClass(jni_object.object<jobject>());
				if (j_class != nullptr) {
					JNINativeMethod NativeMethod{"Back", "(JLjava/lang/String;)V", reinterpret_cast<void*>(GMSFailureListener::Back)};
					jni_environment->RegisterNatives(j_class, &NativeMethod, 1);
					jni_environment->DeleteLocalRef(j_class);
				}
			}
		);
	}
} } } }
