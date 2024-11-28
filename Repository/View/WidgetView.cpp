#include "WidgetView.hpp"

#include "WidgetLayout.hpp"

#include "../../Core/View/Theme.hpp"

#include <QPen>
#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	const int WidgetView::Padding = 24;
	const int WidgetView::Radius = 10;
	const int WidgetView::Right = 0;
	const int WidgetView::Left = 180;
	const int WidgetView::Top = 90;
	const int WidgetView::Bottom = 270;

	WidgetView::WidgetView(QQuickItem *parent)
	:
		QQuickPaintedItem(parent),

		_priority(0),
		_available(true),
		_horizontal_size_type(HorizontalSizeType::CustomHorizontal),
		_vertical_size_type(VerticalSizeType::CustomVertical)
	{
		setClip(true);
	}

	void WidgetView::paint(QPainter* painter)
	{
		QPen pen(Qt::NoPen);
		painter->setPen(pen);

		const qreal rectangle_width(width());
		const qreal rectangle_height(height());

		QBrush brush(Core::View::Theme::Instance()->getBackgroundColor());
		painter->setBrush(brush);
		painter->setRenderHints(QPainter::Antialiasing, true);

		const int top_left_radius(WidgetView::Radius), top_right_radius(WidgetView::Radius), bottom_left_radius(WidgetView::Radius), bottom_right_radius(WidgetView::Radius);

		QRectF rectangle;
		/// @note top-left
		rectangle = QRectF(0, 0, 2 * top_left_radius, 2 * top_left_radius);
		painter->drawPie(rectangle, WidgetView::Left * 16, 90 * -1 * 16);

		/// @note top-right
		rectangle = QRectF(rectangle_width - 2 * top_right_radius, 0, 2 * top_right_radius, 2 * top_right_radius);
		painter->drawPie(rectangle, WidgetView::Top * 16, 90 * -1 * 16);

		/// @note bottom-right
		rectangle = QRectF(rectangle_width - 2 * bottom_right_radius, rectangle_height - 2 * bottom_right_radius, 2 * bottom_right_radius, 2 * bottom_right_radius);
		painter->drawPie(rectangle, WidgetView::Right * 16, 90 * -1 * 16);

		/// @note bottom-left
		rectangle = QRectF(0, rectangle_height - 2 * bottom_left_radius, 2 * bottom_left_radius, 2 * bottom_left_radius);
		painter->drawPie(rectangle, WidgetView::Bottom * 16, 90 * -1 * 16);

		QPointF points[12] =
		{
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
			painter->drawText(QRectF(0, 0, rectangle_width, rectangle_height), Qt::AlignHCenter | Qt::AlignVCenter, "WidgetView is not available");
		}
	}

	void WidgetView::setTitle(const QString& title)
	{
		if (_title != title) {
			_title = title;
			emit titleChanged();
		}
	}

	void WidgetView::setPriority(int priority)
	{
		if (_priority != priority) {
			_priority = priority;
			emit priorityChanged();
		}
	}

	void WidgetView::setAvailable(bool onoff)
	{
		if (onoff != _available) {
			_available = onoff;
			emit availableChanged();
		}
	}

	void WidgetView::setHorizontalSizeType(WidgetView::HorizontalSizeType size_type)
	{
		if (size_type != _horizontal_size_type) {
			_horizontal_size_type = size_type;
			emit horizontalSizeTypeChanged();
			/// @todo resize
		}
	}

	void WidgetView::setVerticalSizeType(WidgetView::VerticalSizeType size_type)
	{
		if (size_type != _vertical_size_type) {
			_vertical_size_type = size_type;
			emit verticalSizeTypeChanged();
			/// @todo resize
		}
	}

	const QString& WidgetView::getTitle() const
	{
		return _title;
	}

	int WidgetView::getPriority() const
	{
		return _priority;
	}

	WidgetView::HorizontalSizeType WidgetView::getHorizontalSizeType() const
	{
		return _horizontal_size_type;
	}

	WidgetView::VerticalSizeType WidgetView::getVerticalSizeType() const
	{
		return _vertical_size_type;
	}

	bool WidgetView::isAvailable() const
	{
		return _available;
	}

	void WidgetView::childEvent(QChildEvent* event)
	{
		if (!_available && event->type() == QEvent::Paint) {
			return ;
		}
		QQuickItem::childEvent(event);
	}
} } } }
