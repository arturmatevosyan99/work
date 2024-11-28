#ifndef Cadabra_Queen_Core_System_NotificationService_HCMService_hpp
#define Cadabra_Queen_Core_System_NotificationService_HCMService_hpp

#include "AndroidNotificationService.hpp"

#include <QString>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class HCMService : public AndroidNotificationService
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
		HCMService();
		virtual ~HCMService() override = default;
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
} } } }

#endif
