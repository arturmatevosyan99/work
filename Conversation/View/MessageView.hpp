#ifndef Cadabra_Queen_Conversation_View_MessageView_hpp
#define Cadabra_Queen_Conversation_View_MessageView_hpp

#include "../Model/AbstractMessage.hpp"

#include <QQuickPaintedItem>
#include <QPainter>
#include <QElapsedTimer>
#include <QTimer>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	class MessageLayout;

	class MessageView : public QQuickPaintedItem
	{
	/** @name Statics */
	/** @{ */
	public:
		static const qint64 HoldThreshold;
		static const int Gap;
		static const int Radius;
		static const int Right;
		static const int Left;
		static const int Top;
		static const int Bottom;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Conversation::View::MessageLayout* layout READ getLayout WRITE setLayout NOTIFY layoutChanged)
		Q_PROPERTY(Cadabra::Queen::Conversation::Model::AbstractMessage* model READ getModel WRITE setModel NOTIFY modelChanged)
		Q_PROPERTY(qreal contentWidth READ width WRITE setContentWidth NOTIFY widthChanged)
		Q_PROPERTY(qreal contentHeight READ height WRITE setContentHeight NOTIFY heightChanged)
		Q_PROPERTY(qreal minimumWidth READ getMinimumWidth NOTIFY layoutMinimized)
		Q_PROPERTY(qreal minimumHeight READ getMinimumHeight NOTIFY layoutMinimized)
		Q_PROPERTY(qreal gap READ getGap WRITE setGap NOTIFY gapChanged)
		Q_PROPERTY(QColor fontColor READ getFontColor NOTIFY layoutChanged)
		Q_PROPERTY(bool laydown READ hasLayout NOTIFY layoutChanged)
		Q_PROPERTY(bool bubble READ isBubble WRITE setBubble NOTIFY bubbleChanged)
		Q_PROPERTY(bool darkBackground READ hasDarkBackground NOTIFY layoutChanged)
		Q_PROPERTY(QSize backgroundImageSize READ getBackgroundImageSize NOTIFY backgroundChanged)
		Q_PROPERTY(qint64 holdThreshold READ getHoldThreshold WRITE setHoldThreshold NOTIFY holdThresholdChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit MessageView(QQuickItem* parent = nullptr);
		virtual ~MessageView() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cadabra::Queen::Conversation::View::MessageLayout* _layout;
		Cadabra::Queen::Conversation::Model::AbstractMessage* _model;
		QScopedPointer<QImage> _background;
		qreal _gap;
		bool _bubble_mode;
		qint64 _hold_threshold;

		QQuickItem* _holder_item;
		QPoint _holder_position;
		QTimer _holder_timer;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void paint(QPainter *painter) override;

		Q_INVOKABLE void loadBackgroundURLImage(QUrl url);
		Q_INVOKABLE void loadBackgroundBase64Image(QString base64);

		Q_INVOKABLE void loadBackgroundURLImage(QUrl url, QSize size);
		Q_INVOKABLE void loadBackgroundBase64Image(QString base64, QSize size);

		Q_INVOKABLE void loadBackgroundURLImage(QUrl url, qreal scale);
		Q_INVOKABLE void loadBackgroundBase64Image(QString base64, qreal scale);

	private:
		bool resetHolder(QEvent::Type event_type);
		bool touchHolder(QTouchEvent* event);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Conversation::View::MessageLayout* getLayout() const;
		Cadabra::Queen::Conversation::Model::AbstractMessage* getModel() const;
		QColor getFontColor() const;
		QSize getBackgroundImageSize() const;
		qreal getMinimumHeight() const;
		qreal getMinimumWidth() const;
		qreal getGap() const;
		qint64 getHoldThreshold() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setContentWidth(qreal value);
		void setContentHeight(qreal value);
		void setLayout(Cadabra::Queen::Conversation::View::MessageLayout* layout);
		void setModel(Cadabra::Queen::Conversation::Model::AbstractMessage* model);
		void setGap(qreal value);
		void setBubble(bool onoff);
		void setHoldThreshold(qint64 value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void completed();
		void held(QPoint position, bool canceled);
		void layoutChanged();
		void layoutMinimized();
		void modelChanged();
		void gapChanged();
		void bubbleChanged();
		void holdThresholdChanged();
		void backgroundChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasLayout() const;
		bool hasModel() const;
		bool isBubble() const;
		bool hasDarkBackground() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onAppearanceChanged();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void classBegin() override;
		virtual void componentComplete() override;
		virtual bool event(QEvent* event) override;
		void holder();
	/** @} */
	};
} } } }

#endif
