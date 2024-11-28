/**
 @file StateIsland.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "StateIsland.hpp"

#include "WindowManager.hpp"
#include <QPainter>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	StateIsland::StateIsland(QQuickItem* parent)
	:
		QQuickPaintedItem(parent),

		_alert(System::Alert::Instance()),
		_interval(0),
		_timer_id(0),
		_queue_index(-1)
	{
		if (_alert) {
			QObject::connect(_alert, &System::Alert::headlessChanged, this, &StateIsland::restartTimer);
			QObject::connect(_alert, &System::Alert::stated, this, &StateIsland::onAlerted);
		}

		const QSize screen_size(Core::View::WindowManager::Instance()->getScreenSize());
		const int size(qMin(screen_size.height(), screen_size.width()));

		setWidth(size / 4);
		setHeight(width() / 2);
		setX((size / 2) - (width() / 2));
		setY(0/*Core::View::WindowManager::Instance()->getTopInset() + (width() / 2)*/);
		setZ(std::numeric_limits<qreal>::max());
		setVisible(false);
		setParentItem(parent);
	}

	void StateIsland::paint(QPainter* painter)
	{
		if (
			!_alert
			||
			_alert->isHeadless()
			||
			_queue_index < 0
			||
			_alert->getStateQueue().count() < _queue_index
		) {
			setVisible(false);
			return ;
		}

		/// @todo Depends on a device frame color and system theme
		QBrush brush(Qt::black);
		painter->setBrush(brush);

		QPen pen(Qt::NoPen);
		painter->setPen(pen);

		painter->setRenderHints(QPainter::Antialiasing, true);

		const int top_left_radius(0), top_right_radius(0), bottom_left_radius(10), bottom_right_radius(10);
		const qreal rectangle_width(width());
		const qreal rectangle_height(height());
		QRectF rectangle;
		/// @note top-left
		rectangle = QRectF(0, 0, 2 * top_left_radius, 2 * top_left_radius);
		painter->drawPie(rectangle, /*::Left*/180 * 16, 90 * -1 * 16);
		/// @note top-right
		rectangle = QRectF(rectangle_width - 2 * top_right_radius, 0, 2 * top_right_radius, 2 * top_right_radius);
		painter->drawPie(rectangle, /*::Top*/90 * 16, 90 * -1 * 16);
		/// @note bottom-right
		rectangle = QRectF(rectangle_width - 2 * bottom_right_radius, rectangle_height - 2 * bottom_right_radius, 2 * bottom_right_radius, 2 * bottom_right_radius);
		painter->drawPie(rectangle, /*::Right*/0 * 16, 90 * -1 * 16);
		/// @note bottom-left
		rectangle = QRectF(0, rectangle_height - 2 * bottom_left_radius, 2 * bottom_left_radius, 2 * bottom_left_radius);
		painter->drawPie(rectangle, /*::Bottom*/270 * 16, 90 * -1 * 16);
		QPointF points[12] = {
								QPointF(top_left_radius, 0),
								QPointF(top_left_radius, top_left_radius),
								QPointF(0, top_left_radius),
								QPointF(0, rectangle_height - bottom_left_radius),
								QPointF(bottom_left_radius, rectangle_height - bottom_left_radius),
								QPointF(bottom_left_radius, rectangle_height),
								QPointF(rectangle_width - bottom_right_radius, rectangle_height),
								QPointF(rectangle_width - bottom_right_radius, rectangle_height - bottom_right_radius),
								QPointF(rectangle_width, rectangle_height - bottom_right_radius),
								QPointF(rectangle_width, top_right_radius),
								QPointF(rectangle_width - top_right_radius, top_right_radius),
								QPointF(rectangle_width - top_right_radius, 0)
		};
		painter->drawPolygon(points, 12);

		/// @todo Opposite for
		pen.setColor(Qt::white);
		pen.setStyle(Qt::SolidLine);
		pen.setCapStyle(Qt::RoundCap);
		painter->setPen(pen);
		painter->setFont(QFont(painter->font().family(), height() / 4));
		painter->drawText(
							QRectF
							{
								4,
								4,
								width() - 8,
								height() - 8
							},
							QString(_alert->getStateQueue().count(), QChar(' ')).prepend(_alert->getStateQueue().at(_queue_index)->getTitle()).prepend(QChar::SpecialCharacter::LineFeed), ///•
							QTextOption
							{
								Qt::AlignCenter
							}
		);
		setVisible(true);
	}

	void StateIsland::restartTimer()
	{
		if (_timer_id > 0) {
			killTimer(_timer_id);
			_timer_id = 0;
			update(QRect(0, 0, width(), height()));
		}
		if (
			_alert
			&&
			!_alert->isHeadless()
			&&
			_interval > 0
			&&
			!_alert->getStateQueue().isEmpty()
			&&
			_alert->getStateQueue().last() != _alert->getStateQueue().head()
		) {
			_timer_id = startTimer(_interval, Qt::CoarseTimer);
		}
	}

	System::Alert* StateIsland::getAlert() const
	{
		return _alert;
	}

	int StateIsland::getInterval() const
	{
		return _interval;
	}

	void StateIsland::setAlert(System::Alert* value)
	{
		if (value != _alert) {
			if (_alert) {
				QObject::disconnect(_alert, nullptr, this, nullptr);
			}
			if ((_alert = value) != nullptr) {
				QObject::connect(_alert, &System::Alert::headlessChanged, this, &StateIsland::restartTimer);
				QObject::connect(_alert, &System::Alert::stated, this, &StateIsland::onAlerted);
			}
			emit alertChanged();
		}
	}

	void StateIsland::setInterval(int value)
	{
		if (value != _interval) {
			_interval = value;
			emit intervalChanged();
			restartTimer();
		}
	}

	void StateIsland::onAlerted(System::StateAlert* state, bool onoff)
	{
		if (_alert) {
			const int queue_length(_alert->getStateQueue().count());
			if (onoff) {
				if ((_queue_index = _alert->getStateQueue().lastIndexOf(state)) < 0) {
					_queue_index = queue_length - 1;
				}
			} else if (queue_length < 1) {
				_queue_index = -1;
			} else if (_queue_index >= queue_length) {
				_queue_index = queue_length - 1;
			}
		} else {
			/// @todo need a fake constEnd
		}
		update(QRect(0, 0, width(), height()));
		restartTimer();
	}

	void StateIsland::timerEvent(QTimerEvent *event)
	{
		const int queue_length(_alert->getStateQueue().count());
		if (queue_length < 1) {
			_queue_index = -1;
		} else if (++_queue_index >= queue_length) {
			_queue_index = 0;
		}

		update(QRect(0, 0, width(), height()));
	}
} } } }
