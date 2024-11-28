/**
 @file macOS.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "macOS.h"

#import <AppKit/AppKit.h>

#include <QGuiApplication>
#include <QScreen>

static NSAutoreleasePool* AutoreleasePool = nil;

static dispatch_once_t QueenKeyWindowGuard;
NSWindow* QueenKeyWindow()
{
	static NSWindow* Instance = nil;
	dispatch_once
	(
		&QueenKeyWindowGuard,
		^{
			Instance = [[NSApplication sharedApplication] mainWindow];
		}
	);
	return Instance;
}

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	const Native::FactoryMethod Native::Factory = &(Native::Instance<macOS>);

	macOS::macOS()
	:
		Native(
					Platform::macOS,
					QString::fromNSString([[NSBundle mainBundle] bundleIdentifier]),
					QUuid::fromNSUUID([NSUUID UUID]).toString(QUuid::WithoutBraces) /// < QUuid::fromNSUUID([[NSHost currentHost] identifierForVendor]).toString(QUuid::WithoutBraces)
		)
	{
		/// @??? @autoreleasepool есть в qt_main_wrapper
		AutoreleasePool = [[NSAutoreleasePool alloc] init];
	}

	macOS::~macOS()
	{
		/// @??? @autoreleasepool есть в qt_main_wrapper
//		[AutoreleasePool drain];
	}

	bool macOS::filterEvent(QObject* receiver, QEvent* event)
	{
		if (event->type() == QEvent::Close) {
			emit exitRequested();
			return true;
		}
		return false;
	}

	bool macOS::shareFile(QUrl url)
	{
		return false;
	}

	void macOS::openAlbum()
	{
	}

	void macOS::openCamera()
	{
	}

	bool macOS::openFile(QUrl url)
	{
		bool retval(false);

		return retval;
	}

	void macOS::checkApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void macOS::runApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void macOS::requestApplicationRate()
	{
	}

	void macOS::vibrate()
	{

	}

	void macOS::alertInput(const QString& title, const QString& placeholder, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	void macOS::alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	qreal macOS::getScreenKeyboardScreenHeight() const
	{
		return 0;
	}

	int macOS::getScreenDensity() const
	{
		return QGuiApplication::primaryScreen()->physicalDotsPerInch();
	}

	qreal macOS::getDefaultFontScale() const
	{
		return 1; // return QGuiApplication::font().pointSize();
	}
} } } }
