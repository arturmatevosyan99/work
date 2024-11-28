#include "Widget.hpp"

#include "Widgetboard.hpp"

#include <QPen>
#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	const int Widget::Padding = 24;
	const int Widget::Radius = 10;
	const int Widget::Right = 0;
	const int Widget::Left = 180;
	const int Widget::Top = 90;
	const int Widget::Bottom = 270;
	const QColor Widget::BackgroundColor = QColor(243, 244, 246);
	const QColor Widget::FontColor = QColor(0, 0, 0);

	Widget::Widget(QQuickItem *parent)
	:
		QQuickPaintedItem(parent),

		_board(nullptr),
		_priority(0),
		_available(true)
	{
		setClip(true);
		QObject::connect(this, &Widget::widthChanged, this, &Widget::onBoardChanged);
		QObject::connect(this, &Widget::heightChanged, this, &Widget::onBoardChanged);
		QObject::connect(this, &Widget::boardChanged, this, &Widget::onBoardChanged);
	}

	void Widget::paint(QPainter* painter)
	{
		QPen pen(Qt::NoPen);
		painter->setPen(pen);

		const qreal rectangle_width(width());
		const qreal rectangle_height(height());

		QBrush brush(Widget::BackgroundColor);
		painter->setBrush(brush);
		painter->setRenderHints(QPainter::Antialiasing, true);

		const int top_left_radius(Widget::Radius), top_right_radius(Widget::Radius), bottom_left_radius(Widget::Radius), bottom_right_radius(Widget::Radius);

		QRectF rectangle;
		/// @note top-left
		rectangle = QRectF(0, 0, 2 * top_left_radius, 2 * top_left_radius);
		painter->drawPie(rectangle, Widget::Left * 16, 90 * -1 * 16);

		/// @note top-right
		rectangle = QRectF(rectangle_width - 2 * top_right_radius, 0, 2 * top_right_radius, 2 * top_right_radius);
		painter->drawPie(rectangle, Widget::Top * 16, 90 * -1 * 16);

		/// @note bottom-right
		rectangle = QRectF(rectangle_width - 2 * bottom_right_radius, rectangle_height - 2 * bottom_right_radius, 2 * bottom_right_radius, 2 * bottom_right_radius);
		painter->drawPie(rectangle, Widget::Right * 16, 90 * -1 * 16);

		/// @note bottom-left
		rectangle = QRectF(0, rectangle_height - 2 * bottom_left_radius, 2 * bottom_left_radius, 2 * bottom_left_radius);
		painter->drawPie(rectangle, Widget::Bottom * 16, 90 * -1 * 16);

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
		if (!_available) {
			painter->drawText(QRectF(0, 0, rectangle_width, rectangle_height), Qt::AlignHCenter | Qt::AlignVCenter, "Widget is not available");
		}
	}

	void Widget::setTitle(const QString& title)
	{
		if (_title != title) {
			_title = title;
			emit titleChanged();
		}
	}

	void Widget::setBoard(Cadabra::Queen::Repository::View::Widgetboard* widgetboard)
	{
		if (_board != widgetboard) {
			if (_board) {
				QObject::disconnect(_board, &Widgetboard::rectChanged, this, &Widget::boardRectChanged);
			}
			_board = widgetboard;
			QObject::connect(widgetboard, &Widgetboard::rectChanged, this, &Widget::boardRectChanged);
			emit boardChanged();
		}
	}

	void Widget::setPriority(int priority)
	{
		if (_priority != priority) {
			_priority = priority;
			emit priorityChanged();
		}
	}

	void Widget::setAvailable(bool onoff)
	{
		if (onoff != _available) {
			_available = onoff;
			emit availableChanged();
			onBoardChanged();
		}
	}

	Cadabra::Queen::Repository::View::Widgetboard* Widget::getBoard() const
	{
		return _board;
	}

	Cadabra::Queen::Repository::Model::Domain* Widget::getDomain() const
	{
		return _board ? _board->getDomain() : nullptr;
	}

	const QString& Widget::getTitle() const
	{
		return _title;
	}

	QRectF Widget::getBoardRect() const
	{
		return _board ? _board->getRect() : QRectF{0, 0, 0, 0};
	}

	int Widget::getPriority() const
	{
		return _priority;
	}

	bool Widget::onBoard() const
	{
		return nullptr != _board;
	}

	bool Widget::hasDomain() const
	{
		return !_board ? false : _board->hasDomain();
	}

	bool Widget::isAvailable() const
	{
		return _available;
	}

	void Widget::onBoardChanged()
	{
		emit layoutChanged();
		update(QRect(0, 0, width(), height()));
	}

	void Widget::childEvent(QChildEvent* event)
	{
		if (!_available && event->type() == QEvent::Paint) {
			return ;
		}
		QQuickItem::childEvent(event);
	}
} } } }
