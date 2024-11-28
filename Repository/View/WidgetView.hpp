#ifndef Cadabra_Queen_Repository_View_WidgetView_hpp
#define Cadabra_Queen_Repository_View_WidgetView_hpp

#include <QQuickPaintedItem>

#include <QFlags>
#include <QPainter>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	class WidgetLayout;

	class WidgetView : public QQuickPaintedItem
	{
	/** @name Classes */
	/** @{ */
	public:
		enum HorizontalSizeType
		{
			CustomHorizontal,
			MaxHorizontal,
			HalfHorizontal
		};
		enum VerticalSizeType
		{
			CustomVertical,
			MaxVertical,
			HalfVertical
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(HorizontalSizeType)
		Q_ENUM(VerticalSizeType)
		Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(int priority READ getPriority WRITE setPriority NOTIFY priorityChanged)
		Q_PROPERTY(bool available READ isAvailable WRITE setAvailable NOTIFY availableChanged)
		Q_PROPERTY(HorizontalSizeType horizontalSizeType READ getHorizontalSizeType WRITE setHorizontalSizeType NOTIFY horizontalSizeTypeChanged)
		Q_PROPERTY(VerticalSizeType verticalSizeType READ getVerticalSizeType WRITE setVerticalSizeType NOTIFY verticalSizeTypeChanged)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const int Padding;
		static const int Radius;
		static const int Right;
		static const int Left;
		static const int Top;
		static const int Bottom;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit WidgetView(QQuickItem* parent = nullptr);
		virtual ~WidgetView() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _title;
		int _priority;
		bool _available;
		HorizontalSizeType _horizontal_size_type;
		VerticalSizeType _vertical_size_type;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void paint(QPainter* painter) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getTitle() const;
		int getPriority() const;
		HorizontalSizeType getHorizontalSizeType() const;
		VerticalSizeType getVerticalSizeType() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTitle(const QString& title);
		void setPriority(int priority);
		void setAvailable(bool onoff);
		void setHorizontalSizeType(HorizontalSizeType size_type);
		void setVerticalSizeType(VerticalSizeType size_type);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void titleChanged();
		void priorityChanged();
		void availableChanged();
		void horizontalSizeTypeChanged();
		void verticalSizeTypeChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isAvailable() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void childEvent(QChildEvent* event)	override;
	/** @} */
	};
} } } }

template <>
struct std::less<Cadabra::Queen::Repository::View::WidgetView*>
{
	bool operator()(const Cadabra::Queen::Repository::View::WidgetView*& lhs, const Cadabra::Queen::Repository::View::WidgetView*& rhs) const
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
