#include "../WindowManager.hpp"

#include "../../System/Native/iOS.h"

#import <UIKit/UIKit.h>

@interface QIOSViewController : UIViewController
@end

@interface QIOSViewController(QueenWindowManager)
+(NSUInteger) ConvertScreenOrientation:(Qt::ScreenOrientations) set;
+(NSUInteger) ScreenOrientationFlagSet:(Qt::ScreenOrientations) set;
+(NSUInteger) ScreenOrientationMask;
@end

@implementation QIOSViewController(QueenWindowManager)
static NSUInteger QueenScreenOrientation = [QIOSViewController ConvertScreenOrientation: Qt::ScreenOrientation::PrimaryOrientation];

+(NSUInteger) ConvertScreenOrientation:(Qt::ScreenOrientations) set
{
	NSUInteger mask(0);
	if (Qt::ScreenOrientation::PrimaryOrientation == set) {
		if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
			mask = UIInterfaceOrientationMaskAllButUpsideDown;
		} else {
			mask = UIInterfaceOrientationMaskAll; /// < UIUserInterfaceIdiomPad
		}
	} else {
		if (set.testFlag(Qt::ScreenOrientation::LandscapeOrientation)) {
			mask |= UIInterfaceOrientationMaskLandscapeLeft;
		}
		if (set.testFlag(Qt::ScreenOrientation::PortraitOrientation)) {
			mask |= UIInterfaceOrientationMaskPortrait;
		}
		if (set.testFlag(Qt::ScreenOrientation::InvertedLandscapeOrientation)) {
			mask |= UIInterfaceOrientationMaskLandscapeRight;
		}
		if (set.testFlag(Qt::ScreenOrientation::InvertedPortraitOrientation)) {
			mask |= UIInterfaceOrientationMaskPortraitUpsideDown;
		}
	}

	return mask;
}

+(NSUInteger) ScreenOrientationFlagSet:(Qt::ScreenOrientations) set
{
	NSUInteger mask([QIOSViewController ConvertScreenOrientation: set]);
	@synchronized (self) {
		QueenScreenOrientation = mask;
	}
	return mask;
}

+(NSUInteger) ScreenOrientationMask
{
	@synchronized (self) {
		return QueenScreenOrientation;
	}
}

-(NSUInteger) supportedInterfaceOrientations
{
	@try {
		return [QIOSViewController ScreenOrientationMask];
	} @catch (NSException* e) {
		qCritical() << QString("Window manager exception(%1): %2").arg(QString::fromNSString(e.name), QString::fromNSString(e.reason));
	}
	return 0;
}

-(BOOL) shouldAutorotate
{
	return YES;
}
@end

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	void WindowManager::keepScreen(bool onoff)
	{
		/// @todo
	}

	void WindowManager::orientScreen(Qt::ScreenOrientations mask)
	{
		[QIOSViewController ScreenOrientationFlagSet: mask];
	}

	QRectF WindowManager::getSafeAreaRect() const
	{
		QRectF retval(0, 0, 0, 0);
		if (@available(iOS 11.0, *)) {
			if (QueenKeyWindow()) {
				retval.setBottom(QueenKeyWindow().safeAreaInsets.bottom);
				retval.setTop(QueenKeyWindow().safeAreaInsets.top);
				retval.setLeft(QueenKeyWindow().safeAreaInsets.left);
				retval.setRight(QueenKeyWindow().safeAreaInsets.right);
			}
		}
		return retval;
	}

	int WindowManager::getTopPadding() const
	{
		/// @todo Landscape/Portrait
		return getSafeAreaRect().top();
	}

	int WindowManager::getBottomPadding() const
	{
		/// @todo Landscape/Portrait
		return getSafeAreaRect().bottom();
	}
} } } }
