/**
 @file WindowManager.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "WindowManager.hpp"

#include "../System/Native.hpp"

#include <QScreen>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	WindowManager* WindowManager::Instance()
	{
		static WindowManager retval;
		return &retval;
	}

	QObject* WindowManager::QInstance(QQmlEngine*, QJSEngine*)
	{
		WindowManager* retval(Cadabra::Queen::Core::View::WindowManager::Instance());
		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	WindowManager::WindowManager(QObject* parent)
	:
		QObject(parent),

		_screen(qApp->primaryScreen())
	{
		QObject::connect
		(
			this,
			&WindowManager::keyWindowChanged,

			this,
			&WindowManager::safeAreaInsetChanged
		);

		QObject::connect
		(
			this,
			&WindowManager::safeAreaInsetChanged,

			this,
			&WindowManager::safePaddingChanged
		);

		QObject::connect
		(
			_screen,
			&QScreen::orientationChanged,

			this,
			&WindowManager::orientationChanged
		);
	}

	QSize WindowManager::getDefaultSize() const
	{
		const System::Native::Device& device_type(System::Native::Instance()->getDeviceType());
		QSize retval(_screen->size());
		if (System::Native::Device::Desktop == device_type) {
			retval.setHeight(retval.height() * 0.8);
			retval.setWidth(retval.width() * 0.3);
		}
		return retval;
	}

	QSize WindowManager::getScreenSize() const
	{
		return _screen->size();
	}

	Qt::ScreenOrientation WindowManager::getScreenOrientation() const
	{
		return _screen->orientation();
	}

	int WindowManager::getScreenRotationAngle() const
	{
		return _screen->angleBetween(_screen->nativeOrientation(), _screen->orientation());
	}

	int WindowManager::getTopInset() const
	{
		return getSafeAreaRect().top();
	}

	int WindowManager::getBottomInset() const
	{
		return getSafeAreaRect().bottom();
	}
} } } }
