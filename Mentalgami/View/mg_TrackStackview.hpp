#ifndef Cadabra_Queen_Mentalgami_View_TrackStackview_hpp
#define Cadabra_Queen_Mentalgami_View_TrackStackview_hpp

#include "mg_TrackView.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace View {
	class TrackLayout;

	class TrackStackview : public TrackView
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QQuickItem* header READ getHeader WRITE setHeader NOTIFY headerChanged)
		Q_PROPERTY(QQuickItem* footer READ getFooter WRITE setFooter NOTIFY footerChanged)
		Q_PROPERTY(qreal implicitHeaderHeight READ getImplicitHeaderHeight NOTIFY arranged)
		Q_PROPERTY(qreal implicitHeaderWidth READ getImplicitHeaderWidth NOTIFY arranged)
		Q_PROPERTY(qreal implicitFooterHeight READ getImplicitFooterHeight NOTIFY arranged)
		Q_PROPERTY(qreal implicitFooterWidth READ getImplicitFooterWidth NOTIFY arranged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit TrackStackview(QQuickItem* parent = nullptr);
		virtual ~TrackStackview() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QQuickItem* _header;
		QQuickItem* _footer;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void arrange();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QQuickItem* getHeader();
		QQuickItem* getFooter();
		qreal getImplicitHeaderHeight() const;
		qreal getImplicitHeaderWidth() const;
		qreal getImplicitFooterHeight() const;
		qreal getImplicitFooterWidth() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setHeader(QQuickItem* item);
		void setFooter(QQuickItem* item);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void headerChanged();
		void footerChanged();
		void arranged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void geometryChanged(const QRectF& new_geometry, const QRectF& old_geometry) override;
	/** @} */
	};
} } } }

#endif
