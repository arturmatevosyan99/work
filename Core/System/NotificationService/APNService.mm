#include "APNService.h"

#include "../Native.hpp"

#include <QEvent>
#include <QDebug>
#include <QCoreApplication>

#import <UIKit/UIKit.h>
#import <UserNotifications/UserNotifications.h>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	template<>
	const NotificationCenter::ServiceFactory NotificationCenter::Singleton<NotificationCenter::Family::APN> = &NotificationService::Instance<APNService>;

	APNService::APNService()
	:
		NotificationService()
	{
		[
			[UNUserNotificationCenter currentNotificationCenter]
			getNotificationSettingsWithCompletionHandler: ^(UNNotificationSettings* settings)
			{
				if(
					settings.authorizationStatus != UNAuthorizationStatusDenied
					&&
					settings.authorizationStatus != UNAuthorizationStatusNotDetermined
				) {
					_active = true;
					emit permissionGranted();
				}
			}
		];
	}

	APNService::~APNService()
	{

	}

	void APNService::requestPermission()
	{
		if (_active) {
			return requestToken();
		}
		[
			[UNUserNotificationCenter currentNotificationCenter]
			requestAuthorizationWithOptions:
			(
				UNAuthorizationOptionBadge
				|
				UNAuthorizationOptionSound
				|
				UNAuthorizationOptionAlert
			)
			completionHandler: ^(BOOL granted, NSError* _Nullable error)
			{
				if (error) {
					qCritical() << "APNService UN Error: " << QString::fromNSString(error.localizedDescription);
				} else {
					_active = true;
					emit permissionGranted();
					[[UIApplication sharedApplication] registerForRemoteNotifications];
				}
			}
		];
	}

	void APNService::requestToken()
	{
		if (_active) {
			[[UIApplication sharedApplication] registerForRemoteNotifications];
			return ;
		}
		[
			[UNUserNotificationCenter currentNotificationCenter]
			requestAuthorizationWithOptions:
			(
				UNAuthorizationOptionBadge
				|
				UNAuthorizationOptionSound
				|
				UNAuthorizationOptionAlert
			)
			completionHandler: ^(BOOL granted, NSError* _Nullable error)
			{
				if (error) {
					qCritical() << "APNService UN Error: " << QString::fromNSString(error.localizedDescription);
				} else if (granted) {
					qWarning() << "APNService authorized for notifications";
					_active = true;
					emit permissionGranted();
					[[UIApplication sharedApplication] registerForRemoteNotifications];
				} else {
					[
						[UNUserNotificationCenter currentNotificationCenter]
						getNotificationSettingsWithCompletionHandler: ^(UNNotificationSettings* settings)
						{
							if (
								UNAuthorizationStatusDenied != settings.authorizationStatus
								&&
								UNAuthorizationStatusNotDetermined != settings.authorizationStatus
							) {
								_active = true;
								emit permissionGranted();
								[[UIApplication sharedApplication] registerForRemoteNotifications];
							}
						}
					];
				}
			}
		];
	}

	QString APNService::makePrefixedToken() const
	{
		return "A:" + _token;
	}

	QString APNService::makeCNSToken() const
	{
		return QString("%1@%2.apn").arg(_token, Native::Instance()->getBundleIdentifier());
	}

	bool APNService::hasPermission() const
	{
		return _active;
	}
} } } }

@interface NotificationDelegate : NSObject <UNUserNotificationCenterDelegate>
@end

@implementation NotificationDelegate
-(void) userNotificationCenter:(UNUserNotificationCenter*)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler
{
	completionHandler(UNNotificationPresentationOptionList | UNNotificationPresentationOptionBanner | UNNotificationPresentationOptionSound);
}

-(void) userNotificationCenter:(UNUserNotificationCenter*)center didReceiveNotificationResponse:(UNNotificationResponse*)response withCompletionHandler:(void (^)(void))completionHandler
{
/*
	NSString* identifier = [[[response notification] request] identifier];
*/
	completionHandler();
}
@end

@interface QIOSApplicationDelegate : UIResponder <UIApplicationDelegate>
@end

@interface QIOSApplicationDelegate(QueenAPNService)
@end

@implementation QIOSApplicationDelegate(QueenAPNService)
-(BOOL) application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
	/// @note You must assign your delegate object to the UNUserNotificationCenter object before your app finishes launching
	[[UNUserNotificationCenter currentNotificationCenter] setDelegate: [[NotificationDelegate alloc] init]];
	return YES;
}

-(void) application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
	const unsigned *token_bytes = (const unsigned*)[deviceToken bytes];
	NSString *ns_token = [NSString
									stringWithFormat:@"%08x%08x%08x%08x%08x%08x%08x%08x",
									ntohl(token_bytes[0]), ntohl(token_bytes[1]), ntohl(token_bytes[2]),
									ntohl(token_bytes[3]), ntohl(token_bytes[4]), ntohl(token_bytes[5]),
									ntohl(token_bytes[6]), ntohl(token_bytes[7])
	];
	Cadabra::Queen::Core::System::NotificationCenter::Singleton<Cadabra::Queen::Core::System::NotificationCenter::Family::APN>()->setToken(QString::fromNSString(ns_token));
}

-(void) application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	if (error.code == 3010) {
		Cadabra::Queen::Core::System::NotificationCenter::Singleton<Cadabra::Queen::Core::System::NotificationCenter::Family::APN>()->setToken("ios_simulator_token");
	} else {
		qCritical() << "Failed to register device in APN service: " << QString::fromNSString(error.localizedDescription);
	}
}
@end

