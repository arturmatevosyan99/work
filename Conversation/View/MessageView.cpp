#include "MessageView.hpp"
#include "MessageLayout.hpp"

#include "../../Core/View/Theme.hpp"

#include <QPen>
#include <QtGlobal>
#include <QApplication>
#include <QImageReader>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	const qint64 MessageView::HoldThreshold = 1700;
	const int MessageView::Gap = 24;
	const int MessageView::Radius = 10;
	const int MessageView::Right = 0;
	const int MessageView::Left = 180;
	const int MessageView::Top = 90;
	const int MessageView::Bottom = 270;

	MessageView::MessageView(QQuickItem *parent)
	:
		QQuickPaintedItem(parent),

		_layout(nullptr),
		_model(nullptr),
		_background(nullptr),
		_gap(MessageView::Gap),
		_bubble_mode(true),
		_hold_threshold(MessageView::HoldThreshold),
		_holder_item(nullptr),
		_holder_position(0,0)
	{
		_holder_timer.setSingleShot(true);
		QObject::connect(&_holder_timer, &QTimer::timeout, this, &MessageView::holder);

		setAcceptedMouseButtons(Qt::RightButton);
		setAcceptTouchEvents(true);

		QObject::connect(this, &MessageView::widthChanged, this, &MessageView::onAppearanceChanged);
		QObject::connect(this, &MessageView::heightChanged, this, &MessageView::onAppearanceChanged);
		QObject::connect(this, &MessageView::bubbleChanged, this, &MessageView::onAppearanceChanged);
		QObject::connect(this, &MessageView::layoutChanged, this, &MessageView::onAppearanceChanged);
		QObject::connect(this, &MessageView::backgroundChanged, this, &MessageView::onAppearanceChanged);
		QObject::connect(this, &MessageView::modelChanged, this, &MessageView::onAppearanceChanged);
	}

	void MessageView::paint(QPainter *painter)
	{
		const MessageLayout::Side side(_layout ? _layout->getSide() : MessageLayout::Side::Center);
		if (!_bubble_mode) {
			return ;
		}

		QPen pen(Qt::NoPen);
		painter->setPen(pen);

		const qreal rectangle_width(width());
		const qreal rectangle_height(height());

		QBrush brush
		(
			MessageLayout::Side::Center == side
			? QColor()
			: MessageLayout::Side::Left == side
				? Core::View::Theme::Instance()->getBackgroundColor()
				: Core::View::Theme::Instance()->getPrimaryColor()
		);
		if (_background) {
			brush.setTextureImage(*_background);
		}
		painter->setBrush(brush);
		painter->setRenderHints(QPainter::Antialiasing, true);

		/// @note MessageView::Radius
		const int top_left_radius(MessageLayout::Side::Left == side ? 0 : MessageView::Radius);
		const int top_right_radius(MessageLayout::Side::Right == side ? 0 : MessageView::Radius);
		const int bottom_left_radius(MessageView::Radius);
		const int bottom_right_radius(MessageView::Radius);

		QRectF rectangle;
		/// @note top-left
		rectangle = QRectF(0, 0, 2 * top_left_radius, 2 * top_left_radius);
		painter->drawPie(rectangle, MessageView::Left * 16, 90 * -1 * 16);

		/// @note top-right
		rectangle = QRectF(rectangle_width - 2 * top_right_radius, 0, 2 * top_right_radius, 2 * top_right_radius);
		painter->drawPie(rectangle, MessageView::Top * 16, 90 * -1 * 16);

		/// @note bottom-right
		rectangle = QRectF(rectangle_width - 2 * bottom_right_radius, rectangle_height - 2 * bottom_right_radius, 2 * bottom_right_radius, 2 * bottom_right_radius);
		painter->drawPie(rectangle, MessageView::Right * 16, 90 * -1 * 16);

		/// @note bottom-left
		rectangle = QRectF(0, rectangle_height - 2 * bottom_left_radius, 2 * bottom_left_radius, 2 * bottom_left_radius);
		painter->drawPie(rectangle, MessageView::Bottom * 16, 90 * -1 * 16);

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
	}

	void MessageView::loadBackgroundURLImage(QUrl url)
	{
		loadBackgroundURLImage(url, {0, 0});
	}

	void MessageView::loadBackgroundBase64Image(QString base64)
	{
		loadBackgroundBase64Image(base64, {0, 0});
	}

	void MessageView::loadBackgroundURLImage(QUrl url, QSize size)
	{
		QImageReader reader(url.toLocalFile());
		reader.setAutoTransform(true);
		reader.setAutoDetectImageFormat(true);
		if (size.isNull()) {
			size = reader.size();
		}
		if (!(width() > 0) && !(height() > 0) & hasLayout()) {
			setWidth(_layout->getViewMinimumWidth());
			setHeight(_layout->getViewMinimumHeight());
		}
		bool is_landscape(width() > height());
		if (reader.transformation().testFlag(QImageIOHandler::TransformationRotate90)) {
			is_landscape = !is_landscape;
		}
		size *= is_landscape ? width() / size.width() : height() / size.height();

		reader.setScaledSize(size);
		_background.reset(new QImage());
		if (!reader.read(_background.get())) {
			_background.reset();
			return ;
		}
		setSize(reader.transformation().testFlag(QImageIOHandler::TransformationRotate90) ? size.transposed() : size);
		emit backgroundChanged();
	}

	void MessageView::loadBackgroundBase64Image(QString base64, QSize size)
	{
		if (base64.isEmpty()) {
			return ;
		}
		QImage background(QImage::fromData(QByteArray::fromBase64(base64.toLatin1())));
		if (background.isNull()) {
			return ;
		}
		if (size.isNull()) {
			size.setHeight(background.height());
			size.setWidth(background.width());
		}
		if (!(width() > 0) && !(height() > 0) & hasLayout()) {
			setWidth(_layout->getViewMinimumWidth());
			setHeight(_layout->getViewMinimumHeight());
		}
		bool is_landscape(width() > height());
		size *= is_landscape ? width() / size.width() : height() / size.height();

		_background.reset(new QImage(background.scaled(size, Qt::KeepAspectRatio)));
		setSize(size);
		emit backgroundChanged();
	}

	void MessageView::loadBackgroundURLImage(QUrl url, qreal scale)
	{
		QImageReader reader(url.toLocalFile());
		reader.setAutoTransform(true);
		reader.setAutoDetectImageFormat(true);
		QSize size(reader.size());

		bool is_landscape(size.width() > size.height());
/*
		if (reader.transformation().testFlag(QImageIOHandler::TransformationRotate90)) {
			is_landscape = !is_landscape;
		}
*/
		if (size.width() > _layout->getViewMinimumWidth()) {
			if (!is_landscape) {
				size.scale(_layout->getViewMinimumWidth() * scale, size.height(), Qt::KeepAspectRatio);
			} else {
				size.scale(_layout->getViewMinimumWidth(), size.height(), Qt::KeepAspectRatio);
			}
		}
		reader.setScaledSize(size);
		_background.reset(new QImage());
		if (!reader.read(_background.get())) {
			_background.reset();
			return ;
		}
//		setSize(reader.transformation().testFlag(QImageIOHandler::TransformationRotate90) ? size.transposed() : size);
		setSize(size);
		emit backgroundChanged();
	}

	void MessageView::loadBackgroundBase64Image(QString base64, qreal scale)
	{
		if (base64.isEmpty()) {
			return ;
		}
		QImage background(QImage::fromData(QByteArray::fromBase64(base64.toLatin1())));
		if (background.isNull()) {
			return ;
		}
		bool is_landscape(background.width() > background.height());
		if (background.width() > _layout->getViewMinimumWidth()) {
			if (!is_landscape) {
				_background.reset(new QImage(background.scaledToWidth(_layout->getViewMinimumWidth() * scale)));
			} else {
				_background.reset(new QImage(background.scaledToWidth(_layout->getViewMinimumWidth())));
			}
		} else {
			_background.reset(new QImage(std::move(background)));
		}
		setSize(_background->size());
		emit backgroundChanged();
	}

	bool MessageView::resetHolder(QEvent::Type event_type)
	{
//		qDebug() << "Holder reset(" << QTime::currentTime() << "):" << _holder_item;
		if (QEvent::Type::ContextMenu == event_type) {
			emit held(_holder_position, false);
		} else if (!_holder_timer.isActive()) {
			return false;
		} else if (QEvent::Type::TouchUpdate != event_type) {
			emit held(_holder_position, true);
		}

		_holder_timer.stop();
		_holder_item = nullptr;
		_holder_position = {0, 0};

		return true;
	}

	bool MessageView::touchHolder(QTouchEvent* event)
	{
		if (event->touchPointStates().testFlag(Qt::TouchPointReleased)) {
			return false;
		} else if (event->touchPointStates().testFlag(Qt::TouchPointStationary)) {
			return true;
		} else if (!event->touchPointStates().testFlag(Qt::TouchPointMoved)) {
			return false;
		}

		const QTouchEvent::TouchPoint& touch_point(event->touchPoints().front());
		if (touch_point.state() != Qt::TouchPointStationary) {
			if ((touch_point.pos() - touch_point.lastPos()).manhattanLength() > 4) {
				return false;
			}
		}
		return true;
	}

	void MessageView::setContentWidth(qreal value)
	{
		setWidth(qMin(value + _gap, hasLayout() ? _layout->getViewMinimumWidth() : 0));
	}

	void MessageView::setContentHeight(qreal value)
	{
		setHeight(value + _gap);
	}

	Cadabra::Queen::Conversation::View::MessageLayout* MessageView::getLayout() const
	{
		return _layout;
	}

	Cadabra::Queen::Conversation::Model::AbstractMessage* MessageView::getModel() const
	{
		return _model;
	}

	QColor MessageView::getFontColor() const
	{
		const MessageLayout::Side side(_layout ? _layout->getSide() : MessageLayout::Side::Center);
		return
				MessageLayout::Side::Center == side
				? QColor(0, 0, 0)
				: MessageLayout::Side::Left == side
					? Core::View::Theme::Instance()->getForegroundColor()
					: Core::View::Theme::Instance()->getBackgroundColor()
		;
	}

	QSize MessageView::getBackgroundImageSize() const
	{
		return _background ? QSize(_background->width(), _background->height()) : QSize();
	}

	qreal MessageView::getMinimumHeight() const
	{
		return _layout ? _layout->getViewMinimumHeight() - _gap : height();
	}

	qreal MessageView::getMinimumWidth() const
	{
		return _layout ? _layout->getViewMinimumWidth() - _gap : width();
	}

	qreal MessageView::getGap() const
	{
		return _gap;
	}

	qint64 MessageView::getHoldThreshold() const
	{
		return _hold_threshold;
	}

	void MessageView::setLayout(Cadabra::Queen::Conversation::View::MessageLayout* layout)
	{
		if (_layout == layout) {
			return;
		} else if (!layout) {
			QObject::disconnect(_layout, &MessageLayout::viewWidthMinimized, this, &MessageView::layoutMinimized);
		} else {
			QObject::connect(layout, &MessageLayout::viewWidthMinimized, this, &MessageView::layoutMinimized);
		}
		_layout = layout;
		emit layoutMinimized();
		emit layoutChanged();
	}

	void MessageView::setModel(Cadabra::Queen::Conversation::Model::AbstractMessage* model)
	{
		if (_model != model) {
			_model = model;
			emit modelChanged();
		}
	}

	void MessageView::setGap(qreal value)
	{
		if (_gap < value || _gap > value) {
			_gap = value;
			emit gapChanged();
		}
	}

	void MessageView::setBubble(bool onoff)
	{
		if (_bubble_mode != onoff) {
			_bubble_mode = onoff;
			emit bubbleChanged();
		}
	}

	void  MessageView::setHoldThreshold(qint64 value)
	{
		if (_hold_threshold != value && value > 0) {
			_hold_threshold = value;
			emit holdThresholdChanged();
		}
	}

	bool MessageView::hasLayout() const
	{
		return nullptr != _layout;
	}

	bool MessageView::hasModel() const
	{
		return nullptr != _model;
	}

	bool MessageView::isBubble() const
	{
		return _bubble_mode;
	}

	bool MessageView::hasDarkBackground() const
	{
		return _layout ? (MessageLayout::Side::Right == _layout->getSide()) : false;
	}

	void MessageView::onAppearanceChanged()
	{
		update(QRect(0, 0, width(), height()));
	}

	void MessageView::classBegin()
	{
		QQuickPaintedItem::classBegin();
	}

	void MessageView::componentComplete()
	{
		QQuickPaintedItem::componentComplete();
		emit completed();
	}

	bool MessageView::event(QEvent* event)
	{
		event->accept();

		switch (event->type()) {
			case QEvent::MouseButtonPress:
				_holder_position = static_cast<QMouseEvent*>(event)->globalPos();
				_holder_item = this;
				_holder_timer.start(_hold_threshold);
				break;

			case QEvent::TouchBegin:
				_holder_position = static_cast<QTouchEvent*>(event)->touchPoints().last().screenPos().toPoint();
				_holder_item = this;
				_holder_timer.start(_hold_threshold);
				break;

//			case QEvent::MouseMove:
			case QEvent::TouchUpdate:
				if (!_holder_timer.isActive()) {
					event->ignore();
				} else if (!touchHolder(static_cast<QTouchEvent*>(event)) && _holder_timer.remainingTime() > 0) {
					resetHolder(event->type());
				}
				break;

			case QEvent::TouchEnd:
			case QEvent::MouseButtonRelease:
				if (!_holder_timer.isActive()) {
					event->ignore();
				} else if (_holder_timer.remainingTime() > 0) {
					resetHolder(event->type());
				}
				break;

			default:
				event->ignore();
		}
		if (event->isAccepted() && _holder_timer.remainingTime() == 0) {
			resetHolder(QEvent::Type::ContextMenu);
			return true;
		}
		return event->isAccepted() ? true : QQuickItem::event(event);
	}

	void MessageView::holder()
	{
		qDebug() << "Holder timeout(" << QTime::currentTime() << "):" << _holder_item;
		if (nullptr != _holder_item) {
			resetHolder(QEvent::Type::ContextMenu);
		}
	}
} } } }
