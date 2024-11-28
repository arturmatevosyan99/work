#ifndef Cadabra_Queen_Repository_View_Widgetboard_hpp
#define Cadabra_Queen_Repository_View_Widgetboard_hpp

#include "Widget.hpp"

#include "../Model/Application/Domain.hpp"

#include "../../Core/Meta/Qd.hpp"

#include <QQuickItem>
#include <QQmlComponent>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	/**
	 * @brief The Widgetboard class
	 * @obsolete Use WidgetLayout
	 */
	class Widgetboard : public QQuickItem
	{
	/** @name Enum */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Domain* domain READ getDomain WRITE setDomain NOTIFY domainChanged)
		Q_PROPERTY(QRectF rect READ getRect NOTIFY rectChanged)
		Q_PROPERTY(qreal spacing READ getSpacing WRITE setSpacing NOTIFY spacingChanged)
		Q_PROPERTY(bool hasDomain READ hasDomain NOTIFY domainChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Widgetboard(QQuickItem* parent = nullptr);
		virtual ~Widgetboard() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cadabra::Queen::Repository::Model::Domain* _domain;
		qreal _spacing;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void dash(Cadabra::Queen::Repository::View::Widget* widget, bool capture = true);
//		Q_INVOKABLE void dash(Cadabra::Queen::Repository::Meta::Dashboard::WidgetScheme);

	private:
		void arrange(Widget* widget = nullptr);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Repository::Model::Domain* getDomain() const;
		QRectF getRect() const;
		qreal getSpacing() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setDomain(Cadabra::Queen::Repository::Model::Domain* domain);
		void setSpacing(qreal value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void rectChanged(const QRectF& rect);
		void domainChanged();
		void spacingChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onLayoutChanged(Cadabra::Queen::Repository::View::Widget* widget);
		void onVisibilityChanged(Cadabra::Queen::Repository::View::Widget* widget);
		void onPositionChanged(Cadabra::Queen::Repository::View::Widget* widget);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasDomain() const;
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value) override;
		virtual void geometryChanged(const QRectF& new_geometry, const QRectF& old_geometry) override;
	/** @} */
	};
} } } }

#endif
