#include "TrackStackview.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace View {
	TrackStackview::TrackStackview(QQuickItem* parent)
	:
		TrackView(parent),

		_header(nullptr),

		_footer(nullptr)
	{

	}

	void TrackStackview::arrange()
	{
		if (_header) {
			_header->setY(0);
			_header->setX(0);
			_header->setWidth(width());
		} else {

		}

		if (_footer) {
			_footer->setY(height() - _footer->height());

			_footer->setX(0);
			_footer->setWidth(width());
		} else {

		}

		emit arranged();
	}

	QQuickItem* TrackStackview::getHeader()
	{
		return _header;
	}

	QQuickItem* TrackStackview::getFooter()
	{
		return _footer;
	}

	qreal TrackStackview::getImplicitHeaderHeight() const
	{
		return _header ? _header->implicitHeight() : 0;
	}

	qreal TrackStackview::getImplicitHeaderWidth() const
	{
		return _header ? _header->implicitWidth() : 0;
	}

	qreal TrackStackview::getImplicitFooterHeight() const
	{
		return _footer ? _footer->implicitHeight() : 0;
	}

	qreal TrackStackview::getImplicitFooterWidth() const
	{
		return _footer ? _footer->implicitWidth() : 0;
	}

	void TrackStackview::setHeader(QQuickItem* item)
	{
		if (item != _header) {
			if (_header) {
				_header->setParentItem(nullptr);
			}
			if (item) {
				item->setParentItem(this);
			}
			_header = item;
			emit headerChanged();
		}
	}

	void TrackStackview::setFooter(QQuickItem* item)
	{
		if (item != _footer) {
			if (_footer) {
				_footer->setParentItem(nullptr);
			}
			if (item) {
				item->setParentItem(this);
			}
			_footer = item;
			emit footerChanged();
		}
	}

	void TrackStackview::geometryChanged(const QRectF& new_geometry, const QRectF& old_geometry)
	{
		QQuickItem::geometryChanged(new_geometry, old_geometry);
		arrange();
	}
} } } }
