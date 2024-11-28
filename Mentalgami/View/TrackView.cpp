#include "TrackView.hpp"

#include "TrackLayout.hpp"

#include <QtGlobal>
#include <QApplication>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace View {
	TrackView::TrackView(QQuickItem* parent)
	:
		QQuickItem(parent),

		_layout(nullptr),
		_model(nullptr)
	{
		QObject::connect(this, &TrackView::widthChanged, this, &TrackView::onAppearanceChanged);
		QObject::connect(this, &TrackView::heightChanged, this, &TrackView::onAppearanceChanged);
		QObject::connect(this, &TrackView::layoutChanged, this, &TrackView::onAppearanceChanged);
		QObject::connect(this, &TrackView::modelChanged, this, &TrackView::onAppearanceChanged);
	}

	Cadabra::Queen::Mentalgami::View::TrackLayout* TrackView::getLayout() const
	{
		return _layout;
	}

	Cadabra::Queen::Mentalgami::Model::AbstractTrack* TrackView::getModel() const
	{
		return _model;
	}

	qreal TrackView::getLayoutHeight() const
	{
		return _layout ? _layout->height() : 0;
	}

	qreal TrackView::getLayoutWidth() const
	{
		return _layout ? _layout->width() : 0;
	}

	void TrackView::setLayout(Cadabra::Queen::Mentalgami::View::TrackLayout* layout)
	{
		if (_layout == layout) {
			return;
		}

		if (layout) {
			QObject::connect(layout, &TrackLayout::heightChanged, this, &TrackView::layoutHeightChanged);
			QObject::connect(layout, &TrackLayout::widthChanged, this, &TrackView::layoutWidthChanged);
			setParentItem(layout);
		} else {
			QObject::disconnect(layout, &TrackLayout::heightChanged, this, &TrackView::layoutHeightChanged);
			QObject::disconnect(layout, &TrackLayout::widthChanged, this, &TrackView::layoutWidthChanged);
		}

		_layout = layout;
		setParent(_layout);

		emit layoutChanged();
		emit layoutWidthChanged();
		emit layoutHeightChanged();
	}

	void TrackView::setModel(Cadabra::Queen::Mentalgami::Model::AbstractTrack* model)
	{
		if (_model == model) {
			return;
		}

		_model = model;
		emit modelChanged();
	}

	void TrackView::onAppearanceChanged()
	{
//		update();
	}

	bool TrackView::hasLayout() const
	{
		return nullptr != _layout;
	}

	bool TrackView::hasModel() const
	{
		return nullptr != _model;
	}
} } } }
