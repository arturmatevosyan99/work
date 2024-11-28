#include "Widgetboard.hpp"

#include <QtGlobal>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	Widgetboard::Widgetboard(QQuickItem* parent)
	:
		QQuickItem(parent),

		_domain(nullptr),
		_spacing(4)
	{
	}

	Widgetboard::~Widgetboard()
	{
	}

	void Widgetboard::dash(Cadabra::Queen::Repository::View::Widget* widget, bool capture)
	{
		widget->setParentItem(this);
		if (capture) {
			widget->setParent(this);
		}
	}
/*
	void Widgetboard::dash(Cadabra::Queen::Repository::Meta::Dashboard::WidgetScheme)
	{

	}
*/
	void Widgetboard::arrange(Widget* widget)
	{
		QList<QQuickItem*> item_list(childItems());
		QMap<Widget*, int> widget_map;
		QMap<Widget*, int>::Iterator w(widget_map.end());
		for (QSet<QQuickItem*>::value_type item : item_list) {
			Widget* widget_item(qobject_cast<Widget*>(item));
			if (widget_item) {
				if (widget_item == widget) {
					w = widget_map.insert(widget_item, widget_item->getPriority());
				} else {
					widget_map.insert(widget_item, widget_item->getPriority());
				}
			}
		}

		/// @note Если начинаем с самого первого(и есть с чего начинать)
		if (!widget && !widget_map.empty()) {
			w = widget_map.begin();
		}

		qreal w_x(_spacing), w_y(_spacing);
		for (; w != widget_map.end(); ++w) {
			if (!w.key()->isVisible() || w.key()->width() <= 0 || w.key()->height() <= 0) {
				continue;
			}
			/// @brief Считаем x, y в зависимости от priority, visibility и размеров соседей
			qreal w_height(w.key()->height());
			qreal w_width(w.key()->width());
			if (w_x + w_width > width() - _spacing) {
				/// @note new line
				w_x = _spacing;
				w_y += _spacing + w_height;
			}

			w.key()->setWidth(w_width);
			w.key()->setHeight(w_height);
			w.key()->setX(w_x);
			w.key()->setY(w_y);

			w_x += w.key()->width() + _spacing;
		}
	}

	Cadabra::Queen::Repository::Model::Domain* Widgetboard::getDomain() const
	{		
		return _domain;
	}

	QRectF Widgetboard::getRect() const
	{
		return {x(), y(), width() - (_spacing * 2), height() - (_spacing * 2)};
	}

	qreal Widgetboard::getSpacing() const
	{
		return _spacing;
	}

	void Widgetboard::setDomain(Cadabra::Queen::Repository::Model::Domain* domain)
	{
		if (_domain != domain) {
			_domain = domain;
			emit domainChanged();
		}
	}

	void Widgetboard::setSpacing(qreal value)
	{
		if (_spacing != value) {
			_spacing = value;
			emit spacingChanged();
			arrange();
		}
	}

	void Widgetboard::onLayoutChanged(Widget* widget)
	{
		arrange(widget);
	}

	void Widgetboard::onVisibilityChanged(Widget* widget)
	{
		arrange(widget);
	}

	void Widgetboard::onPositionChanged(Widget* widget)
	{
		arrange(widget);
	}

	bool Widgetboard::hasDomain() const
	{
		return _domain;
	}

	void Widgetboard::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData& value)
	{
		if (QQuickItem::ItemChange::ItemChildAddedChange == change) {
			Widget* widget(qobject_cast<Widget*>(value.item));
			if (widget) {
				QObject::connect(widget, &Widget::layoutChanged, std::bind(&Widgetboard::onLayoutChanged, this, widget));
				QObject::connect(widget, &Widget::visibleChanged, std::bind(&Widgetboard::onVisibilityChanged, this, widget));
				QObject::connect(widget, &Widget::priorityChanged, std::bind(&Widgetboard::onPositionChanged, this, widget));
				widget->setBoard(this);
				arrange(widget);
			}
		} else if (QQuickItem::ItemChange::ItemChildRemovedChange == change) {
			Widget* widget(qobject_cast<Widget*>(value.item));
			if (widget) {
				QObject::disconnect(widget, nullptr, this, nullptr);
				widget->setBoard(nullptr);
				/// @todo rellocation
			}
		}
		QQuickItem::itemChange(change, value);
	}

	void Widgetboard::geometryChanged(const QRectF& new_geometry, const QRectF& old_geometry)
	{
		QQuickItem::geometryChanged(new_geometry, old_geometry);
		arrange();
		emit rectChanged(getRect());
	}
} } } }
