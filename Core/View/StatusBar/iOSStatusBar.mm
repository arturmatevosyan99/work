#include "../StatusBar.hpp"

#include "../../System/Native/iOS.h"

#include <UIKit/UIKit.h>
#include <QGuiApplication>

#include <QScreen>
#include <QTimer>

@interface QIOSViewController : UIViewController
@end

@interface QIOSViewController(QueenStatusBar)
@end

@implementation QIOSViewController(QueenStatusBar)
-(UIStatusBarStyle) preferredStatusBarStyle
{
	if (Cadabra::Queen::Core::View::StatusBar::Instance()->getTheme() == Cadabra::Queen::Core::View::StatusBar::Theme::Light) {
		return UIStatusBarStyleLightContent;
	} else if (@available(iOS 13.0, *)) {
		return UIStatusBarStyleDarkContent;
	}
	return UIStatusBarStyleDefault;
}
@end

@interface UIStatusBarManager(QueenStatusBar)
@end

@implementation UIStatusBarManager(QueenStatusBar)
-(void) handleTapAction:(id)arg1
{
	emit Cadabra::Queen::Core::View::StatusBar::Instance()->touched();
}
@end

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class iOSStatusBar : public StatusBar {
	/** @name Constructors */
	/** @{ */
	public:
		iOSStatusBar(QObject* parent = nullptr)
		:
			StatusBar(parent)
		{
			[UITraitCollection traitCollectionWithUserInterfaceStyle: UIUserInterfaceStyleLight];

			QObject::connect(this, &StatusBar::colorChanged, this, &iOSStatusBar::onColorChanged);
			QObject::connect(this, &StatusBar::themeChanged, this, &iOSStatusBar::onThemeChanged);
			QObject::connect(qApp, &QGuiApplication::applicationStateChanged, this, &iOSStatusBar::onApplicationActivated);
			QObject::connect(qApp->primaryScreen(), &QScreen::orientationChanged, this, &iOSStatusBar::onScreenRotated);

			// screen->setOrientationUpdateMask(Qt::PortraitOrientation | Qt::LandscapeOrientation | Qt::InvertedPortraitOrientation | Qt::InvertedLandscapeOrientation);
		}
		virtual ~iOSStatusBar() override = default;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onColorChanged()
		{
		}

		void onThemeChanged()
		{
			updatePreferredStatusBarStyle();
		}

		void onApplicationActivated(Qt::ApplicationState state)
		{
			if (state == Qt::ApplicationActive) {
				updatePreferredStatusBarStyle();
			}
		}

		void onScreenRotated(Qt::ScreenOrientation orienation)
		{
			updatePreferredStatusBarStyle();
		}
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		void updatePreferredStatusBarStyle()
		{
			if (QueenKeyWindow()) {
				QIOSViewController* view_controller(static_cast<QIOSViewController *>(QueenKeyWindow().rootViewController));
				if (view_controller) {
					[view_controller setNeedsStatusBarAppearanceUpdate];
				}
			}
		}
	/** @} */

	/** @name Getters */
	/** @{ */
	private:
		CGSize getSize() const
		{
			if (@available(iOS 13.0, *)) {
				return QueenKeyWindow() ? QueenKeyWindow().windowScene.statusBarManager.statusBarFrame.size : CGSize{0, 0};
			}
			return UIApplication.sharedApplication.statusBarFrame.size;
		}

	public:
		virtual int getHeight() override
		{
			const CGSize status_bar_size(getSize());
			return static_cast<int>(floor(MIN(status_bar_size.width, status_bar_size.height)));
		}
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool hasCutOut() override
		{
			if (@available(iOS 11.0, *)) {
				return QueenKeyWindow() && QueenKeyWindow().safeAreaInsets.top > 20.0;
			}
			return false;
		}
	/** @} */
	};

	StatusBar* StatusBar::Instance()
	{
		static Cadabra::Queen::Core::View::iOSStatusBar retval;
		return &retval;
	}
} } } }
