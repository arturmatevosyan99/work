#ifndef Cadabra_Queen_Core_View_StatusBar_hpp
#define Cadabra_Queen_Core_View_StatusBar_hpp

#include <QObject>
#include <QColor>
#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class StatusBar : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Theme {
			Light,
			Dark
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static StatusBar* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Theme)
		Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
		Q_PROPERTY(Theme theme READ getTheme WRITE setTheme NOTIFY themeChanged)
		Q_PROPERTY(int height READ getHeight CONSTANT)
		Q_PROPERTY(bool hasCutOut READ hasCutOut CONSTANT)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit StatusBar(QObject* parent = nullptr);
		virtual ~StatusBar() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
		QColor _color;
		StatusBar::Theme _theme;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QColor getColor();
		virtual StatusBar::Theme getTheme();
		virtual int getHeight() = 0;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		virtual void setColor(const QColor &color);
		virtual void setTheme(StatusBar::Theme theme);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void colorChanged();
		void themeChanged();
		void touched();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool hasCutOut() = 0;
	/** @} */
	};
} } } }

#endif
