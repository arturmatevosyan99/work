#ifndef Cadabra_Queen_Core_View_WindowManager_hpp
#define Cadabra_Queen_Core_View_WindowManager_hpp

#include <QObject>
#include <QRectF>
#include <QQmlEngine>
#include <QGuiApplication>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class WindowManager : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Orientation {
			None = 0b000000,
			Mix = 0b000001,
			Stream = 0b000010,
			Background = 0b000100
		};
		Q_DECLARE_FLAGS(OrientationFlags, Orientation)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static WindowManager* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* script_engine);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAGS(OrientationFlags)
		Q_PROPERTY(QSize defaultSize READ getDefaultSize NOTIFY orientationChanged)
		Q_PROPERTY(QSize screenSize READ getScreenSize NOTIFY orientationChanged)
		Q_PROPERTY(Qt::ScreenOrientation screenOrientation READ getScreenOrientation NOTIFY orientationChanged)
		Q_PROPERTY(int screenRotationAngle READ getScreenRotationAngle CONSTANT)
		Q_PROPERTY(QRectF safeAreaInset READ getSafeAreaRect NOTIFY safeAreaInsetChanged)
		Q_PROPERTY(int topSafeInset READ getTopInset NOTIFY safeAreaInsetChanged)
		Q_PROPERTY(int bottomSafeInset READ getBottomInset NOTIFY safeAreaInsetChanged)
		Q_PROPERTY(int topSafePadding READ getTopPadding NOTIFY safePaddingChanged)
		Q_PROPERTY(int bottomSafePadding READ getBottomPadding NOTIFY safePaddingChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit WindowManager(QObject* parent = nullptr);
		virtual ~WindowManager() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QScreen* _screen;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void keepScreen(bool onoff);
		Q_INVOKABLE void orientScreen(Qt::ScreenOrientations mask);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QSize getDefaultSize() const;
		QSize getScreenSize() const;
		Qt::ScreenOrientation getScreenOrientation() const;
		int getScreenRotationAngle() const;

		QRectF getSafeAreaRect() const;
		int getTopInset() const;
		int getBottomInset() const;

		int getTopPadding() const;
		int getBottomPadding() const;
//		int getLeftPadding() const;
//		int getRightPadding() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void keyWindowChanged();
		void orientationChanged();
		void safeAreaInsetChanged();
		void safePaddingChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
