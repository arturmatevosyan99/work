#include "../WindowManager.hpp"

#include "../../System/Native/Android.hpp"
#include "../../View/StatusBar.hpp"

#include <QtAndroid>
#include <QtGlobal>

/// @brief ActivityInfo

#define SCREEN_FLAG_ORIENTATION_UNSPECIFIED 0xFFFFFFFF
#define SCREEN_FLAG_ORIENTATION_LANDSCAPE 0x00000000
#define SCREEN_FLAG_ORIENTATION_PORTRAIT 0x00000001
#define SCREEN_FLAG_ORIENTATION_REVERSE_LANDSCAPE 0x00000008
#define SCREEN_FLAG_ORIENTATION_REVERSE_PORTRAIT 0x00000009

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	void WindowManager::keepScreen(bool onoff)
	{
		QtAndroid::runOnAndroidThread
		(
			[=]()
			{
				QtAndroid::androidActivity().callMethod<void>("keepScreen", "(Z)V", onoff);
			}
		);
	}

	void WindowManager::orientScreen(Qt::ScreenOrientations mask)
	{
		QtAndroid::runOnAndroidThread
		(
			[mask]()
			{
				int jmask(0);
				if (mask.testFlag(Qt::ScreenOrientation::PrimaryOrientation)) {
					jmask |= SCREEN_FLAG_ORIENTATION_UNSPECIFIED;
				} else if (mask.testFlag(Qt::ScreenOrientation::LandscapeOrientation)) {
					jmask |= SCREEN_FLAG_ORIENTATION_LANDSCAPE;
				} else if (mask.testFlag(Qt::ScreenOrientation::PortraitOrientation)) {
					jmask |= SCREEN_FLAG_ORIENTATION_PORTRAIT;
				} else if (mask.testFlag(Qt::ScreenOrientation::InvertedLandscapeOrientation)) {
					jmask |= SCREEN_FLAG_ORIENTATION_REVERSE_LANDSCAPE;
				} else if (mask.testFlag(Qt::ScreenOrientation::InvertedPortraitOrientation)) {
					jmask |= SCREEN_FLAG_ORIENTATION_REVERSE_PORTRAIT;
				}
				QtAndroid::androidActivity().callMethod<void>("orientScreen", "(I)V", jmask);
			}
		);
	}

	QRectF WindowManager::getSafeAreaRect() const
	{
		const int density(System::Native::Instance()->getScreenDensity());

		QRectF retval(0, 0, 0, 0);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval, &density]
			()
			{
				QAndroidJniObject window(QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;"));
				QAndroidJniObject decor_view(window.callObjectMethod("getDecorView", "()Landroid/view/View;"));
				if (
					decor_view.isValid()
					&&
					decor_view != nullptr
					&&
					QtAndroid::androidSdkVersion() >= static_cast<std::uint8_t>(System::Android::Version::P)
				) {
					QAndroidJniObject root_view(decor_view.callObjectMethod("getRootView", "()Landroid/view/View;"));
					QAndroidJniObject window_inset(root_view.callObjectMethod("getRootWindowInsets", "()Landroid/view/WindowInsets;"));
					QAndroidJniObject display_cutout(window_inset.callObjectMethod("getDisplayCutout", "()Landroid/view/DisplayCutout;"));
					if (display_cutout.isValid()) {
						retval.setBottom(display_cutout.callMethod<jint>("getSafeInsetBottom") / density);
						retval.setLeft(display_cutout.callMethod<jint>("getSafeInsetLeft") / density);
						retval.setRight(display_cutout.callMethod<jint>("getSafeInsetRight") / density);
						retval.setTop(display_cutout.callMethod<jint>("getSafeInsetTop") / density);
					}
				}
			}
		);
		return retval;
	}

	int WindowManager::getTopPadding() const
	{
		/// @note Overkill
/*
		int retval(0);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval]
			{
				QAndroidJniObject window(QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;"));
				QAndroidJniObject rectangle("android/graphics/Rect");
				QAndroidJniObject decor_view(window.callObjectMethod("getDecorView", "()Landroid/view/View;"));
				decor_view.callMethod<void>("getWindowVisibleDisplayFrame", "(Landroid/graphics/Rect;)V", rectangle.object<jobject>());
				retval = rectangle.getField<jint>("top");
			}
		);
		return reval / System::Native::Instance()->getScreenDensity();
*/
		/// @todo Landscape/Portrait
		return View::StatusBar::Instance()->getHeight();
	}

	int WindowManager::getBottomPadding() const
	{
		/// @todo
		return 0;
	}
} } } }
