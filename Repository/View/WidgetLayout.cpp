#include "WidgetLayout.hpp"

#include <QHash>
#include <QtGlobal>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	WidgetView* WidgetLayout::Item(QQmlListProperty<WidgetView>* list, int index)
	{
		WidgetLayout* layout(reinterpret_cast<WidgetLayout*>(list->object));
		return layout && layout->_view_list.length() > index ? layout->_view_list.at(index) : nullptr;
	}

	int WidgetLayout::Size(QQmlListProperty<WidgetView>* list)
	{
		WidgetLayout* layout(reinterpret_cast<WidgetLayout*>(list->object));
		return layout ? layout->_view_list.length() : 0;
	}

	void WidgetLayout::Clear(QQmlListProperty<WidgetView>* list)
	{
		WidgetLayout* layout(reinterpret_cast<WidgetLayout*>(list->object));
		if (layout) {
			qDeleteAll(layout->_view_list);
			layout->_view_list.clear();
			/// @todo rellocation
		}
	}

	void WidgetLayout::Append(QQmlListProperty<WidgetView>* list, WidgetView* view)
	{
		WidgetLayout* layout(reinterpret_cast<WidgetLayout*>(list->object));
		if (layout && view) {
			view->setParentItem(layout);
			layout->_view_list.append(view);
			QObject::connect(view, &WidgetView::visibleChanged, std::bind(&WidgetLayout::onVisibilityChanged, layout, view));
			QObject::connect(view, &WidgetView::priorityChanged, std::bind(&WidgetLayout::onPositionChanged, layout, view));
			layout->arrange(view);
		}
	}

	WidgetLayoutAttachment* WidgetLayout::qmlAttachedProperties(QObject* object)
	{
		return new WidgetLayoutAttachment(object);
	}

	WidgetLayout::WidgetLayout(QQuickItem* parent)
	:
		QQuickItem(parent),

		_spacing(4),
		_user_avatar(nullptr)
	{
	}

	WidgetLayout::~WidgetLayout()
	{
	}

	void WidgetLayout::arrange(WidgetView* widget_view)
	{
		QHash<WidgetView*, int> widget_hashtable;
		QHash<WidgetView*, int>::Iterator w(widget_hashtable.end());
		for (const QList<WidgetView*>::value_type& view : qAsConst(_view_list)) {
			if (view == widget_view) {
				w = widget_hashtable.insert(view, view->getPriority());
			} else {
				widget_hashtable.insert(view, view->getPriority());
			}
		}

		/// @note Если начинаем с самого первого(и есть с чего начинать)
		if (!widget_view && !widget_hashtable.empty()) {
			w = widget_hashtable.begin();
		}

		qreal w_x(_spacing), w_y(_spacing);
		for (; w != widget_hashtable.end(); ++w) {
			/// @brief Считаем x, y в зависимости от priority, visibility и размеров соседей

			qreal w_width(w.key()->width());
			if (w.key()->getHorizontalSizeType() == WidgetView::HorizontalSizeType::MaxHorizontal) {
				w_width = width() - _spacing - _spacing;
			} else if (w.key()->getHorizontalSizeType() == WidgetView::HorizontalSizeType::HalfHorizontal) {
				w_width = (width() / 2 ) - _spacing - _spacing;
			} else {
				w_width = qMin((width() - _spacing - _spacing), w_width);
			}

			qreal w_height(w.key()->height());
			if (w.key()->getVerticalSizeType() == WidgetView::VerticalSizeType::MaxVertical) {
				w_height = height() - _spacing - _spacing;
			} else if (w.key()->getVerticalSizeType() == WidgetView::VerticalSizeType::HalfVertical) {
				w_height = (height() / 2 ) - _spacing - _spacing;
			} else {
				w_height = qMin((height() - _spacing - _spacing), w_height);
			}

			if (!w.key()->isVisible() || w_height <= 0 || w_width <= 0) {
				continue;
			} else if ((w_x + w_width) > (width() - _spacing)) {
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

	QQmlListProperty<WidgetView> WidgetLayout::makeViewPropertyList()
	{
		return QQmlListProperty<WidgetView>
		(
			this,
			nullptr,
			&WidgetLayout::Append,
			&WidgetLayout::Size,
			&WidgetLayout::Item,
			&WidgetLayout::Clear
		);
	}

	Cadabra::Queen::Repository::Model::Avatar* WidgetLayout::getUserAvatar() const
	{
		return _user_avatar;
	}

	QRectF WidgetLayout::getRect() const
	{
		return {x(), y(), width() - (_spacing * 2), height() - (_spacing * 2)};
	}

	qreal WidgetLayout::getSpacing() const
	{
		return _spacing;
	}

	void WidgetLayout::setUserAvatar(Cadabra::Queen::Repository::Model::Avatar* avatar)
	{
		if (_user_avatar != avatar) {
			_user_avatar = avatar;
			emit userAvatarChanged();
		}
	}

	void WidgetLayout::setSpacing(qreal value)
	{
		if (_spacing != value) {
			_spacing = value;
			emit spacingChanged();
			arrange();
		}
	}

	void WidgetLayout::onLayoutChanged(WidgetView* widget)
	{
		arrange(widget);
	}

	void WidgetLayout::onVisibilityChanged(WidgetView* widget)
	{
		arrange(widget);
	}

	void WidgetLayout::onPositionChanged(WidgetView* widget)
	{
		arrange(widget);
	}

	void WidgetLayout::geometryChanged(const QRectF& new_geometry, const QRectF& old_geometry)
	{
		QQuickItem::geometryChanged(new_geometry, old_geometry);
		arrange();
		emit rectChanged(getRect());
	}

	WidgetLayoutAttachment::WidgetLayoutAttachment(QObject* widget_view)
	:
		QObject(widget_view),

		_layout(nullptr),
		_view(nullptr)
	{
		QStack<View::WidgetView*> view_stack;

		QObject* parent_object(widget_view);
		QQuickItem* parent_item(nullptr);
		while (parent_object) {
			parent_item = qobject_cast<QQuickItem*>(parent_object);
			if (parent_item) {
				break;
			}
			parent_object = parent_object->parent();
		}
		while (parent_item) {
			view_stack.push(qobject_cast<View::WidgetView*>(parent_item));
			if (!view_stack.top()) {
				view_stack.pop();
			}
			parent_item = parent_item->parentItem();
		}
		_view = view_stack.isEmpty() ? nullptr : view_stack.top();
		if (!_view) {
			return ;
		}
		_layout  = qobject_cast<WidgetLayout*>(_view->parent());
	}

	WidgetLayout* WidgetLayoutAttachment::getLayout()
	{
		return _layout;
	}

	Cadabra::Queen::Repository::Model::Avatar* WidgetLayoutAttachment::getUserAvatar() const
	{
		return _layout ? _layout->getUserAvatar() : nullptr;
	}
} } } }
