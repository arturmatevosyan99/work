#include "ProgressCircle.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	ProgressCircle::ProgressCircle(QQuickItem* parent)
	:
		QQuickPaintedItem(parent),

		_background_color(Qt::white),
		_border_active_color(Qt::blue),
		_border_inactive_color(Qt::gray),
		_angle(0),
		_time(QTime(0, 0, 0, 0)),
		_timer(new QTimer(this))
	{
		connect(_timer, &QTimer::timeout, this, &ProgressCircle::onTimer);
	}

	ProgressCircle::~ProgressCircle()
	{

	}

	void ProgressCircle::paint(QPainter* painter)
	{
		const QBrush brush(_background_color), active_brush(_border_active_color), inactive_brush(_border_inactive_color);

		painter->setPen(Qt::NoPen);
		painter->setRenderHints(QPainter::Antialiasing, true);

		painter->setBrush(inactive_brush);
		painter->drawEllipse(boundingRect().adjusted(1, 1, -1, -1));

		painter->setBrush(active_brush);
		painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), 90 * 16, _angle * 16);

		painter->setBrush(brush);
		painter->drawEllipse(boundingRect().adjusted(10, 10, -10, -10));
	}

	void ProgressCircle::clear()
	{
		setTime(QTime(0, 0, 0, 0));
		setAngle(0);
		update();
		emit cleared();
	}

	void ProgressCircle::start()
	{
		_timer->start(50);
	}

	void ProgressCircle::stop()
	{
		_timer->stop();
	}

	const QString& ProgressCircle::getName() const
	{
		return _name;
	}

	QColor ProgressCircle::getBackgroundColor() const
	{
		return _background_color;
	}

	QColor ProgressCircle::getBorderActiveColor() const
	{
		return _border_active_color;
	}

	QColor ProgressCircle::getBorderInactiveColor() const
	{
		return _border_inactive_color;
	}

	qreal ProgressCircle::getAngle() const
	{
		return _angle;
	}

	const QTime& ProgressCircle::getTime() const
	{
		return _time;
	}

	void ProgressCircle::setName(const QString& value)
	{
		if (value != _name) {
			_name = value;
			emit nameChanged(value);
		}
	}

	void ProgressCircle::setBackgroundColor(const QColor& value)
	{
		if (value != _background_color) {
			_background_color = value;
			update();
			emit backgroundColorChanged(value);
		}
	}

	void ProgressCircle::setBorderActiveColor(const QColor& value)
	{
		if (value != _border_active_color) {
			_border_active_color = value;
			update();
			emit borderActiveColorChanged(value);
		}
	}

	void ProgressCircle::setBorderInactiveColor(const QColor& value)
	{
		if (value != _border_inactive_color) {
			_border_inactive_color = value;
			update();
			emit borderInactiveColorChanged(value);
		}
	}

	void ProgressCircle::setAngle(const qreal angle)
	{
		if (angle != _angle) {
			_angle = angle <= -360 ? 0 : angle;
			update();
			emit angleChanged(_angle);
		}
	}

	void ProgressCircle::setTime(const QTime& value)
	{
		if (value != _time) {
			_time = value;
			emit circleTimeChanged(value);
		}
	}

	void ProgressCircle::onTimer()
	{
		setAngle(getAngle() - 0.3);
		setTime(getTime().addMSecs(50));
		update();
	}
} } } }
