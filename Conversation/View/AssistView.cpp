#include "AssistView.hpp"
#include "AssistLayout.hpp"

#include "../../Core/View/Theme.hpp"

#include <QPen>
#include <QtGlobal>
#include <QApplication>
#include <QImageReader>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	AssistView::AssistView(QQuickItem *parent)
	:
		QQuickPaintedItem(parent),

		_layout(nullptr),
		_model(nullptr)
	{
		QObject::connect(this, &AssistView::widthChanged, this, &AssistView::onAppearanceChanged);
		QObject::connect(this, &AssistView::heightChanged, this, &AssistView::onAppearanceChanged);
		QObject::connect(this, &AssistView::layoutChanged, this, &AssistView::onAppearanceChanged);
		QObject::connect(this, &AssistView::modelChanged, this, &AssistView::onAppearanceChanged);
	}

	void AssistView::paint(QPainter* painter)
	{

	}

	Cadabra::Queen::Conversation::View::AssistLayout* AssistView::getLayout() const
	{
		return _layout;
	}

	Cadabra::Queen::Conversation::Model::AbstractAssist* AssistView::getModel() const
	{
		return _model;
	}

	qreal AssistView::getLayoutHeight() const
	{
		return _layout ? _layout->height() : 0;
	}

	qreal AssistView::getLayoutWidth() const
	{
		return _layout ? _layout->width() : 0;
	}

	void AssistView::setLayout(Cadabra::Queen::Conversation::View::AssistLayout* layout)
	{
		if (_layout == layout) {
			return;
		}

		if (layout) {
			QObject::connect(layout, &AssistLayout::heightChanged, this, &AssistView::layoutHeightChanged);
			QObject::connect(layout, &AssistLayout::widthChanged, this, &AssistView::layoutWidthChanged);
			setParentItem(layout);
		} else {
			QObject::disconnect(layout, &AssistLayout::heightChanged, this, &AssistView::layoutHeightChanged);
			QObject::disconnect(layout, &AssistLayout::widthChanged, this, &AssistView::layoutWidthChanged);
		}

		_layout = layout;
		setParent(_layout);

		emit layoutChanged();
		emit layoutWidthChanged();
		emit layoutHeightChanged();
	}

	void AssistView::setModel(Cadabra::Queen::Conversation::Model::AbstractAssist* model)
	{
		if (_model == model) {
			return;
		}

		_model = model;
		emit modelChanged();
	}

	void AssistView::onAppearanceChanged()
	{
		update(QRect(0, 0, width(), height()));
	}

	bool AssistView::hasLayout() const
	{
		return nullptr != _layout;
	}

	bool AssistView::hasModel() const
	{
		return nullptr != _model;
	}
} } } }
