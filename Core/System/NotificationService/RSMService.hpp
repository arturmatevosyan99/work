#ifndef Cadabra_Queen_Core_System_NotificationService_RSMService_hpp
#define Cadabra_Queen_Core_System_NotificationService_RSMService_hpp

#include "AndroidNotificationService.hpp"

#include <QString>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	/**
	 * @brief Для работы push-уведомлений необходимо соблюдать несколько условий.
	 * На устройстве пользователя должно быть установлено приложение RuStore.
	 * Приложение RuStore должно поддерживать функциональность push-уведомлений.
	 * Приложению RuStore разрешен доступ к работе в фоновом режиме.
	 * Пользователь должен быть авторизован в приложении RuStore.
	 * Отпечаток подписи приложения должен совпадать с отпечатком, добавленным в Консоль RuStore.
	 */
	class RSMService : public AndroidNotificationService
	{
	friend class NotificationService;

	/** @name Statics */
	/** @{ */
	private:
		static void Token(JNIEnv* environment, jobject self, jstring token);
		static void Message(JNIEnv* environment, jobject self, jstring title, jstring icon, jstring sound, jstring body);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		RSMService();
		virtual ~RSMService() override = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void requestToken() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE virtual QString makePrefixedToken() const override;
		Q_INVOKABLE virtual QString makeCNSToken() const override;
	/** @} */
	};

	class RSMTokenListener : public AndroidListener
	{
	/** @name Statics */
	/** @{ */
	private:
		static void Back(JNIEnv* environment, jobject self, jlong native, jstring token);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		RSMTokenListener(QObject* parent);
		virtual ~RSMTokenListener() = default;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done(QString token);
	/** @} */
	};

	class RSMAvailabilityListener : public AndroidListener
	{
	/** @name Statics */
	/** @{ */
	private:
		static void Back(JNIEnv* environment, jobject self, jlong native, jboolean success);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		RSMAvailabilityListener(QObject* parent);
		virtual ~RSMAvailabilityListener() = default;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done(bool success);
	/** @} */
	};
} } } }

#endif
