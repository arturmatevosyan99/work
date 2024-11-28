#include "../StatusBar.hpp"

#include "../../System/Native/macOS.h"

#include <AppKit/AppKit.h>

#include <QGuiApplication>

#include <QTimer>
#include <QScreen>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class macOSStatusBar : public StatusBar
	{
	/** @name Constructors */
	/** @{ */
	public:
		macOSStatusBar(QObject* parent = nullptr)
		:
			StatusBar(parent)
		{
			QObject::connect(this, &StatusBar::colorChanged, this, &macOSStatusBar::onColorChanged);
			QObject::connect(this, &StatusBar::themeChanged, this, &macOSStatusBar::onThemeChanged);
			QObject::connect(qApp, &QGuiApplication::applicationStateChanged, this, &macOSStatusBar::onApplicationActivated);
			QObject::connect(qApp->primaryScreen(), &QScreen::orientationChanged, this, &macOSStatusBar::onScreenRotated);

			// screen->setOrientationUpdateMask(Qt::PortraitOrientation | Qt::LandscapeOrientation | Qt::InvertedPortraitOrientation | Qt::InvertedLandscapeOrientation);
		}
		virtual ~macOSStatusBar() override = default;
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

		}
	/** @} */

	/** @name Getters */
	/** @{ */
	private:
		CGSize getSize() const
		{
			return CGSize{0, 0};
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
			return false;
		}
	/** @} */
	};

	StatusBar* StatusBar::Instance()
	{
		static Cadabra::Queen::Core::View::macOSStatusBar retval;
		return &retval;
	}
} } } }
