#ifndef Cadabra_Queen_Core_View_ProgressCircle_hpp
#define Cadabra_Queen_Core_View_ProgressCircle_hpp

#include <QtQuick/QQuickPaintedItem>

#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QTime>
#include <QTimer>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class ProgressCircle : public QQuickPaintedItem
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
		Q_PROPERTY(QColor borderActiveColor READ getBorderActiveColor WRITE setBorderActiveColor NOTIFY borderActiveColorChanged)
		Q_PROPERTY(QColor borderInactiveColor READ getBorderInactiveColor WRITE setBorderInactiveColor NOTIFY borderInactiveColorChanged)
		Q_PROPERTY(qreal angle READ getAngle WRITE setAngle NOTIFY angleChanged)
		Q_PROPERTY(QTime time READ getTime WRITE setTime NOTIFY circleTimeChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ProgressCircle(QQuickItem* parent = nullptr);
		virtual ~ProgressCircle() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		/**
		 * @brief The name of the object
		 */
		QString	_name;

		/**
		 * @brief The main background color
		 */
		QColor _background_color;

		/**
		 * @brief The color of the border, filling with the progress bezel timer
		 */
		QColor _border_active_color;

		/**
		 * @brief The color of the background of the border
		 */
		QColor _border_inactive_color;

		/**
		 * @brief The rotation angle of the pie chart type, will generate progress on the rim
		 */
		qreal _angle;

		/**
		 * @brief Current time of the timer
		 */
		QTime _time;

		/**
		 * @brief The timer, which will vary according to the time
		 */
		QTimer* const _timer;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void paint(QPainter* painter) override;

		Q_INVOKABLE void clear();
		Q_INVOKABLE void start();
		Q_INVOKABLE void stop();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getName() const;
		QColor getBackgroundColor() const;
		QColor getBorderActiveColor() const;
		QColor getBorderInactiveColor() const;
		qreal getAngle() const;
		const QTime& getTime() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setName(const QString& name);
		void setBackgroundColor(const QColor& value);
		void setBorderActiveColor(const QColor& value);
		void setBorderInactiveColor(const QColor& value);
		void setAngle(qreal value);
		void setTime(const QTime& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void cleared();
		void nameChanged(QString name);
		void backgroundColorChanged(QColor value);
		void borderActiveColorChanged(QColor value);
		void borderInactiveColorChanged(QColor value);
		void angleChanged(qreal value);
		void circleTimeChanged(QTime value);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onTimer();
	/** @} */
	};
} } } }

#endif
