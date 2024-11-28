#include "../StatusBar.hpp"

#include "../../System/Native/Android.hpp"

#include <QtAndroid>
#include <QtGlobal>

#define FLAG_TRANSLUCENT_STATUS 0x04000000
#define FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS 0x80000000

#define SYSTEM_UI_FLAG_LIGHT_STATUS_BAR 0x00002000
#define SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN 0x00000400

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class AndroidStatusBar : public StatusBar
	{
	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AndroidStatusBar(QObject* parent = nullptr)
		:
			StatusBar(parent)
		{
			QtAndroid::runOnAndroidThreadSync
			(
				[]
				()
				{
					QAndroidJniObject window = QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;");
					window.callMethod<void>("addFlags", "(I)V", FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
					window.callMethod<void>("clearFlags", "(I)V", FLAG_TRANSLUCENT_STATUS);
				}
			);
			if (QtAndroid::androidSdkVersion() < 21) {
				return ;
			}

			_color = "transparent";

			QObject::connect(this, &StatusBar::colorChanged, this, &AndroidStatusBar::onColorChanged);
			QObject::connect(this, &StatusBar::themeChanged, this, &AndroidStatusBar::onThemeChanged);

			onColorChanged();
			onThemeChanged();
		}
		virtual ~AndroidStatusBar() override = default;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onColorChanged()
		{
			QColor color(getColor());
			QtAndroid::runOnAndroidThreadSync
			(
				[&color]
				()
				{
					QAndroidJniObject window = QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;");
					window.callMethod<void>("setStatusBarColor", "(I)V", color.rgba());
				}
			);
		}

		void onThemeChanged()
		{
			Theme theme(getTheme());
			QtAndroid::runOnAndroidThreadSync
			(
				[&theme]
				()
				{
					QAndroidJniObject window = QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;");
					QAndroidJniObject view = window.callObjectMethod("getDecorView", "()Landroid/view/View;");
					int visibility = view.callMethod<int>("getSystemUiVisibility", "()I");
					if (theme == StatusBar::Theme::Light) {
						visibility &= ~SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
					} else {
						visibility |= SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
					}
					visibility |= SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
					view.callMethod<void>("setSystemUiVisibility", "(I)V", visibility);
				}
			);
		}
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual int getHeight() override
		{
			if (QtAndroid::androidSdkVersion() < static_cast<quint8>(System::Android::Version::L)) {
				return 0;
			}

			int retval(0);
			QtAndroid::runOnAndroidThreadSync
			(
				[&retval]
				{
					QAndroidJniObject arg1 = QAndroidJniObject::fromString("status_bar_height");
					QAndroidJniObject arg2 = QAndroidJniObject::fromString("dimen");
					QAndroidJniObject arg3 = QAndroidJniObject::fromString("android");

					QAndroidJniObject resources = QtAndroid::androidActivity().callObjectMethod("getResources", "()Landroid/content/res/Resources;");
					jint identifier = resources.callMethod<jint>
					(
						"getIdentifier", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",
						arg1.object<jstring>(),
						arg2.object<jstring>(),
						arg3.object<jstring>()
					);
					retval = (identifier > 0) ? resources.callMethod<jint>("getDimensionPixelSize", "(I)I", identifier) : 0;
				}
			);
			/// @note Greater or equal to WindowManager::topSafeInset
			return retval / System::Native::Instance()->getScreenDensity();
		}
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool hasCutOut() override
		{
			/// @brief As per material design guidelines the status bar height for Android devices is 24dp
			return getHeight() > 24;
		}
	/** @} */
	};

	StatusBar* StatusBar::Instance()
	{
		static Cadabra::Queen::Core::View::AndroidStatusBar retval;
		return &retval;
	}
} } } }
