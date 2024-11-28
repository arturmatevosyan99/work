#ifndef Cadabra_Queen_Repository_View_Widget_hpp
#define Cadabra_Queen_Repository_View_Widget_hpp

#include "../Model/Application/Domain.hpp"

#include <QQuickPaintedItem>
#include <QPainter>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	class Widgetboard;

	/**
	 * @brief The Widget class
	 * @obsolete Use WidgetView
	 */
	class Widget : public QQuickPaintedItem
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::View::Widgetboard* board READ getBoard WRITE setBoard NOTIFY boardChanged)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Domain* domain READ getDomain NOTIFY boardChanged)
		Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(int priority READ getPriority WRITE setPriority NOTIFY priorityChanged)
//		Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
//		Q_PROPERTY(qreal height READ width WRITE setHeight NOTIFY heightChanged)
//		Q_PROPERTY(QRectF childrenRect READ childrenRect NOTIFY childrenRectChanged)
		Q_PROPERTY(QRectF boardRect READ getBoardRect NOTIFY boardRectChanged)
//		Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
//		Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
		Q_PROPERTY(bool available READ isAvailable WRITE setAvailable NOTIFY availableChanged)
		Q_PROPERTY(bool onboard READ onBoard NOTIFY boardChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		static const int Padding;
		static const int Radius;
		static const int Right;
		static const int Left;
		static const int Top;
		static const int Bottom;
		static const QColor BackgroundColor;
		static const QColor FontColor;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Widget(QQuickItem* parent = nullptr);
		virtual ~Widget() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cadabra::Queen::Repository::View::Widgetboard* _board;
		QString _title;
		int _priority;
		bool _available;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void paint(QPainter *painter) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Repository::View::Widgetboard* getBoard() const;
		Cadabra::Queen::Repository::Model::Domain* getDomain() const;
		const QString& getTitle() const;
		int getPriority() const;
		QRectF getBoardRect() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setBoard(Cadabra::Queen::Repository::View::Widgetboard* widgetboard);
		void setTitle(const QString& title);
		void setPriority(int priority);
		void setAvailable(bool onoff);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void boardChanged();
		void boardRectChanged(const QRectF& rect);
		void titleChanged();
		void priorityChanged();
		void availableChanged();
		void layoutChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool onBoard() const;
		bool hasDomain() const;
		bool isAvailable() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onBoardChanged();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void childEvent(QChildEvent* event)	override;
	/** @} */
	};
} } } }

template <>
struct std::less<Cadabra::Queen::Repository::View::Widget*>
{
	bool operator()(const Cadabra::Queen::Repository::View::Widget*& lhs, const Cadabra::Queen::Repository::View::Widget*& rhs) const
	{
		if (!lhs || !rhs) {
			return lhs < rhs;
		} else if (lhs->getPriority() < rhs->getPriority()) {
			return true;
		} else if (lhs->getPriority() > rhs->getPriority()) {
			return false;
		}
		return lhs < rhs;
	}
};

#endif
