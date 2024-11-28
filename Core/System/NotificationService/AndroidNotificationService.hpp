#ifndef Cadabra_Queen_Core_System_NotificationService_AndroidNotificationService_hpp
#define Cadabra_Queen_Core_System_NotificationService_AndroidNotificationService_hpp

#include "../NotificationService.hpp"

#include "../Native/Android.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class AndroidNotificationService : public NotificationService
	{
	/** @name Statics */
	/** @{ */
	protected:
		template <NotificationCenter::Family F>
		struct JavaNativeInteface
		{
		/** @name Statics */
		/** @{ */
		public:
			static void Token(JNIEnv* environment, jobject self, jstring token)
			{
				NotificationCenter::Singleton<F>()->setToken(environment->GetStringUTFChars(token, nullptr));
			}

			static void Message(JNIEnv* environment, jobject self, jstring title, jstring icon, jstring sound, jstring body)
			{
				QMetaObject::invokeMethod
				(
					NotificationCenter::Singleton<F>(),
					"messageReceived",
					Qt::DirectConnection,
					Q_ARG(QString, environment->GetStringUTFChars(title, nullptr)),
					Q_ARG(QString, environment->GetStringUTFChars(icon, nullptr)),
					Q_ARG(QString, environment->GetStringUTFChars(sound, nullptr)),
					Q_ARG(QString, environment->GetStringUTFChars(body, nullptr))
				);
			}
		/** @} */
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AndroidNotificationService();
		virtual ~AndroidNotificationService() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void requestPermission() override final;
		virtual void requestToken() override;
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onPermissionGranted(const QtAndroid::PermissionResultMap& permission_map);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual bool hasPermission() const override final;
	/** @} */
	};
} } } }

#endif
