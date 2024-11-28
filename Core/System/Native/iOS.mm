#include "../Native.hpp"

#include "iOS.h"

#include "../NotificationCenter.hpp"
#include "../../View/WindowManager.hpp"
#include "../../Net/Call/ITunesGrab.hpp"

#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <QGuiApplication>
#include <QQuickView>
#include <QQuickItem>
#include <QInputMethod>
#include <QScreen>
#include <QRectF>
#include <QWindow>
#include <QMetaObject>
#include <QDesktopServices>
#include <QtGlobal>
#include <QDebug>

#import <UIKit/UIKit.h>
#import <UIKit/UIFont.h>
#import <UIKit/UIFontMetrics.h>
#import <UIKit/UIDocumentInteractionController.h>
#import <UIKit/UIImagePickerController.h>
#import <UIKit/UIGestureRecognizerSubclass.h>
#import <StoreKit/StoreKit.h>
#import <AudioToolbox/AudioServices.h>
#import <Foundation/Foundation.h>

#include "../NotificationService.hpp"

static NSAutoreleasePool* AutoreleasePool = nil;

static dispatch_once_t QueenKeyWindowGuard;
UIWindow* QueenKeyWindow()
{
	static UIWindow* Instance = nil;
	dispatch_once
	(
		&QueenKeyWindowGuard,
		^{
			if (@available(iOS 13.0, *)) {
				UIWindowScene* window_scene = (UIWindowScene*)[UIApplication sharedApplication].connectedScenes.allObjects.firstObject;
				if ([window_scene isKindOfClass:[UIWindowScene class]]) {
					NSArray<UIWindow*>* windows = window_scene.windows;
					for (UIWindow* window in windows) {
						if (window.isKeyWindow) {
							Instance = window;
							break;
						}
					}
				}
			}/* else if () {
				NSArray* window_array = UIApplication.sharedApplication.windows;
				for (UIWindow* window in window_array) {
					if (window.isKeyWindow) {
						Instance = window;
						break;
					}
				}
			}*/ else {
				Instance = UIApplication.sharedApplication.keyWindow;
			}
		}
	);
	return Instance;
}

static NSDictionary* QueenLaunchDictionaryInstance;
NSDictionary* QueenLaunchDictionary()
{
	return QueenLaunchDictionaryInstance;
}

@implementation QIOSApplicationDelegate(Queen)
-(BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
	QueenLaunchDictionaryInstance = [launchOptions copy];

	/// @brief Refresh token if had one
	Cadabra::Queen::Core::System::NotificationService* apn_service(Cadabra::Queen::Core::System::NotificationCenter::Singleton<Cadabra::Queen::Core::System::NotificationCenter::Family::APN>());
	if (apn_service && apn_service->hasToken()) {
		apn_service->requestToken();
	}

@try {
	[
		[NSNotificationCenter defaultCenter]
											addObserver: self
											selector: @selector(keyWindowDidChanged:)
											name: UIWindowDidBecomeKeyNotification
											object: nil
	];
} @catch (NSException* exception) {
	qWarning() << QString::fromNSString(exception.reason);
}

	return YES;
}

-(void) keyWindowDidChanged:(NSNotification*)notification
{
	QueenKeyWindowGuard = 0;
	emit Cadabra::Queen::Core::View::WindowManager::Instance()->keyWindowChanged();
}
/*
-(BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
	return [[FBSDKApplicationDelegate sharedInstance] application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
}
*/
@end

@interface QIOSViewController : UIViewController
@end

@interface QIOSViewController(Queen)
@end

@implementation QIOSViewController(Queen)
-(void) didReceiveMemoryWarning
{
	emit Cadabra::Queen::Core::System::Native::Instance()->memoryExhausted();
	[super didReceiveMemoryWarning];
}

-(void) motionEnded:(UIEventSubtype) motion withEvent:(UIEvent *) event
{
	emit Cadabra::Queen::Core::System::Native::Instance()->motionEnded();
	[super motionEnded:motion withEvent:(UIEvent *) event];
}
@end

@interface iOSViewerController : UIViewController<UIDocumentInteractionControllerDelegate>
@end

@interface iOSImageController : UIViewController<UIImagePickerControllerDelegate, UINavigationControllerDelegate>
@end

@interface iOSAppStoreProductController : UIViewController<SKStoreProductViewControllerDelegate>
@end

@interface iOSViewerController()
@end

@interface iOSImageController()
@end

@implementation iOSViewerController
-(void) viewDidLoad
{
	[super viewDidLoad];
}

-(UIViewController *) documentInteractionControllerViewControllerForPreview:(UIDocumentInteractionController*) controller
{
	return self;
}

-(void) documentInteractionControllerDidEndPreview:(UIDocumentInteractionController*) controller
{
	[self removeFromParentViewController];
}
@end

@implementation iOSImageController
-(void) take
{
	UIImagePickerController* picker = [[UIImagePickerController alloc] init];
	picker.delegate = self;
	picker.allowsEditing = NO;
	picker.sourceType = UIImagePickerControllerSourceTypeCamera;

	[self presentViewController: picker animated: YES completion: NULL];
}

-(void) pick
{
	UIImagePickerController* picker = [[UIImagePickerController alloc] init];
	picker.delegate = self;
	picker.allowsEditing = NO;
	picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;

	[self presentViewController: picker animated: YES completion: NULL];
}

-(void) imagePickerController:(UIImagePickerController*) picker didFinishPickingMediaWithInfo:(NSDictionary*) info
{
	NSURL* filename = [info objectForKey:@"UIImagePickerControllerReferenceURL"];
	QUrl url(QUrl::fromNSURL(filename));
	[picker dismissViewControllerAnimated: YES completion: NULL];
	QMetaObject::invokeMethod(Cadabra::Queen::Core::System::Native::Instance(), "albumItemPicked", Qt::DirectConnection, Q_ARG(QUrl, url));
}

-(void) imagePickerControllerDidCancel:(UIImagePickerController*) picker
{
	[picker dismissViewControllerAnimated: YES completion: NULL];
	QMetaObject::invokeMethod(Cadabra::Queen::Core::System::Native::Instance(), "albumDiscarded", Qt::DirectConnection);
}
@end

@implementation iOSAppStoreProductController
-(id) init
{
	self = [super init];
	return self;
}

-(void) presentViewController:(UIViewController*) vc applicationIdentifier:(NSNumber*) id
{
	SKStoreProductViewController* svc = [[SKStoreProductViewController alloc] init];
	[svc setDelegate: self];
	[svc
		loadProductWithParameters: @{SKStoreProductParameterITunesItemIdentifier: id}
		completionBlock:^(BOOL result, NSError *error) {
			if (!result) {
				NSLog(@"SKStoreProductViewController: %@", error);
			} else {
				[vc presentViewController: svc animated: YES completion: nil];
			}
		}
	];
//	QDesktopServices::openUrl("itms-apps://itunes.apple.com/app/apple-store/id375380948?mt=8");
}

-(void) productViewControllerDidFinish:(SKStoreProductViewController*) svc
{
	[svc dismissViewControllerAnimated: YES completion: nil];
	[svc autorelease];
}
@end

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	const Native::FactoryMethod Native::Factory = &(Native::Instance<iOS>);

	iOS::iOS(QObject* parent)
	:
		Native(
				Platform::iOS,
				QString::fromNSString([[NSBundle mainBundle] bundleIdentifier]),
				QUuid::fromNSUUID([[UIDevice currentDevice] identifierForVendor]).toString(QUuid::WithoutBraces)
		)
	{

	}

	iOS::~iOS()
	{

	}

	bool iOS::filterEvent(QObject* receiver, QEvent* event)
	{
		return false;
	}

	bool iOS::openFile(QUrl url)
	{
		static iOSViewerController* vc = nil;
		if (vc == nil) {
			/// @xxx when do we really need to release this?
			vc = [[iOSViewerController alloc] init];
		} else {
			[vc removeFromParentViewController];
		}

		NSURL* ns_url = [NSURL fileURLWithPath:url.toLocalFile().toNSString()];
		UIDocumentInteractionController* dic = [UIDocumentInteractionController interactionControllerWithURL: ns_url];
		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc != nil) {
			[rvc addChildViewController: vc];
			[dic setDelegate: vc];
			[dic presentPreviewAnimated: YES];
//			[dic presentOpenInMenuFromRect:[button frame] inView:self.view animated: YES];
			return true;
		}
		return false;
	}

	bool iOS::shareFile(QUrl url)
	{
		NSURL* ns_url = [NSURL fileURLWithPath:url.toLocalFile().toNSString()];
		UIDocumentInteractionController* documentInteractionController = [UIDocumentInteractionController interactionControllerWithURL: ns_url];
		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc != nil) {
			return ([documentInteractionController presentOpenInMenuFromRect: CGRectZero inView: rvc.view animated: true] == YES);
		}
		return false;
	}

	void iOS::openAlbum()
	{
		static iOSImageController* vc = nil;
		if (vc == nil) {
			/// @xxx when do we really need to release this?
			vc = [[iOSImageController alloc] init];
		} else {
			[vc removeFromParentViewController];
		}

		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc != nil) {

			[rvc addChildViewController: vc];
			[vc pick];
		}
	}

	void iOS::openCamera()
	{
		static iOSImageController* vc = nil;
		if (vc == nil) {
			/// @xxx when do we really need to release this?
			vc = [[iOSImageController alloc] init];
		} else {
			[vc removeFromParentViewController];
		}

		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc != nil) {
			[rvc addChildViewController: vc];
			[vc take];
		}
	}

	void iOS::requestApplicationRate()
	{
/*
		"_OBJC_CLASS_$_SKStoreReviewController", referenced from:
		if (@available(iOS 10.3, *)) {
			[SKStoreReviewController requestReview];
		}
*/
	}

	void iOS::updateUpplication(bool auto_offer)
	{
		QString ident(Native::Instance()->getBundleIdentifier());
		QString version(Native::Instance()->getFrameworkVersion());

		QScopedPointer<Net::ITunesGrab> grabber(new Net::ITunesGrab());
		Net::ITunesGrab* self(grabber.get());
		QObject::connect
		(
			grabber.get(),
			&Net::ITunesGrab::faulted,

			grabber.get(),
			[self]() -> void
			{
				self->deleteLater();
				emit Native::Instance()->applicationUpdated(false);
			}
		);

		QObject::connect
		(
			grabber.get(),
			&Net::ITunesGrab::lookedup,

			grabber.get(),
			[self, ident, version, auto_offer](Meta::ITunesItem bundle) -> void
			{
				self->deleteLater();
				if (!auto_offer) {
					emit Native::Instance()->applicationUpdated(bundle.getIdent() == ident && bundle.isHigher(version));
				} else if (bundle.getIdent() == ident && bundle.isHigher(version)) {
					UIViewController* rvc = [QueenKeyWindow() rootViewController];
					if (rvc == nil) {
						return ;
					}
					static iOSAppStoreProductController* apc = nil;
					if (apc == nil) {
						/// @xxx when do we really need to release this?
						apc = [[iOSAppStoreProductController alloc] init];
					}
					[apc presentViewController: rvc applicationIdentifier: [NSNumber numberWithInteger: bundle.getId()]];
				}
			}
		);
		grabber.take()->lookup(ident);
	}

	void iOS::checkApplicationUpdate()
	{
		return updateUpplication(false);
	}

	void iOS::runApplicationUpdate()
	{
		return updateUpplication(true);
	}

	void iOS::vibrate()
	{
		AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
	}

	void iOS::alertInput(const QString& title, const QString& placeholder, AlertFlags flag_set, const QString& ident)
	{
		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc == nil) {
			return ;
		}

		UIAlertController* alert = [UIAlertController
														alertControllerWithTitle: @""
														message: title.toNSString()
														preferredStyle: UIAlertControllerStyleAlert
		];

		NSString* ns_ident(ident.toNSString());
		UIAlertAction* action_ok = [UIAlertAction
													actionWithTitle: tr("Ok").toNSString()
													style: UIAlertActionStyleDefault
													handler:^(UIAlertAction * _Nonnull action) {
														emit inputAlertDone(QString::fromNSString(alert.textFields[0].text), true, QString::fromNSString(ns_ident));
													}
		];
		[alert addAction: action_ok];

		UIAlertAction* action_cancel = [UIAlertAction
														actionWithTitle: tr("Cancel").toNSString()
														style: UIAlertActionStyleCancel
														handler:^(UIAlertAction * _Nonnull action) {
															emit inputAlertDone(QString::fromNSString(alert.textFields[0].text), false, QString::fromNSString(ns_ident));
														}
		];
		[alert addAction:action_cancel];

		[alert
				addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull text_field) {
					text_field.placeholder = placeholder.toNSString();
				}
		];

		[rvc presentViewController: alert animated: YES completion: nil];
	}

	void iOS::alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set, const QString& ident)
	{
		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc == nil) {
			return ;
		}

		UIAlertController * alert = [UIAlertController
														alertControllerWithTitle: title.toNSString()
														message: message.toNSString()
														preferredStyle: flag_set.testFlag(AlertProperty::ActionSheet) ? UIAlertControllerStyleActionSheet : UIAlertControllerStyleAlert
		];
		NSString* ns_ident(ident.toNSString());
		for (int a = 0; a < action_list.count(); ++a) {
			QString text(action_list.at(a));
			if (text.isEmpty()) {
				continue;
			}
			UIAlertActionStyle style = UIAlertActionStyleDefault;
			switch (text.front().toLatin1()) {
				case '!':
					style = UIAlertActionStyleDestructive;
					text.remove(0, 1);
					break;

				case '^':
					style = UIAlertActionStyleCancel;
					text.remove(0, 1);
					break;
			}

			[alert addAction:
							[UIAlertAction
											actionWithTitle: text.toNSString()
											style: style
											handler: ^(UIAlertAction* action)
											{
												emit actionAlertDone(a, QString::fromNSString(ns_ident));
											}
							]
			];
		}

		if (flag_set.testFlag(AlertProperty::ActionCancel)) {
			[alert addAction:
							[UIAlertAction
											actionWithTitle: tr("Cancel").toNSString()
											style: UIAlertActionStyleCancel
											handler: ^(UIAlertAction* action)
											{
												emit actionAlertDone(-1, QString::fromNSString(ns_ident));
											}
							]
			];
		}

		[rvc presentViewController: alert animated: YES completion: nil];
	}

	qreal iOS::getScreenKeyboardScreenHeight() const
	{
		return QGuiApplication::inputMethod()->keyboardRectangle().height();
	}

	qreal iOS::getDefaultFontScale() const
	{
		CGFloat system_size = [UIFont systemFontSize];
		return [[UIFontMetrics defaultMetrics] scaledValueForValue: system_size] / system_size;
	}
} } } }
