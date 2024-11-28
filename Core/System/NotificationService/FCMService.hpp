#ifndef Cadabra_Queen_Core_System_NotificationService_FCMService_hpp
#define Cadabra_Queen_Core_System_NotificationService_FCMService_hpp

#include "AndroidNotificationService.hpp"

#include <QString>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class FCMService : public AndroidNotificationService
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
	private:
		FCMService();
		virtual ~FCMService() override = default;
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

	class GMSCompleteListener : public AndroidListener
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
		GMSCompleteListener(QObject* parent);
		virtual ~GMSCompleteListener() = default;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done(QString token);
	/** @} */
	};

	class GMSFailureListener : public AndroidListener
	{
	/** @name Statics */
	/** @{ */
	private:
		static void Back(JNIEnv* environment, jobject self, jlong native, jstring message);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		GMSFailureListener(QObject* parent);
		virtual ~GMSFailureListener() = default;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done(QString error);
	/** @} */
	};
} } } }

#endif
