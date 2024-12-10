#ifndef Cadabra_Queen_SmartCare_View_TrackView_hpp
#define Cadabra_Queen_SmartCare_View_TrackView_hpp

#include "../Model/AbstractTrack.hpp"

#include "../SmartCare.hpp"

#include <QQuickItem>
#include <QPainter>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace View {
	class TrackLayout;

	class TrackView : public QQuickItem
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::SmartCare::View::TrackLayout* layout READ getLayout WRITE setLayout NOTIFY layoutChanged)
		Q_PROPERTY(Cadabra::Queen::SmartCare::Model::AbstractTrack* model READ getModel WRITE setModel NOTIFY modelChanged)
		Q_PROPERTY(qreal layoutHeight READ getLayoutHeight NOTIFY layoutHeightChanged)
		Q_PROPERTY(qreal layoutWidth READ getLayoutWidth NOTIFY layoutWidthChanged)
		Q_PROPERTY(bool laydown READ hasLayout NOTIFY layoutChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit TrackView(QQuickItem* parent = nullptr);
		virtual ~TrackView() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cadabra::Queen::SmartCare::View::TrackLayout* _layout;
		Cadabra::Queen::SmartCare::Model::AbstractTrack* _model;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::SmartCare::View::TrackLayout* getLayout() const;
		Cadabra::Queen::SmartCare::Model::AbstractTrack* getModel() const;
		qreal getLayoutHeight() const;
		qreal getLayoutWidth() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setLayout(Cadabra::Queen::SmartCare::View::TrackLayout* layout);
		void setModel(Cadabra::Queen::SmartCare::Model::AbstractTrack* model);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void layoutChanged();
		void layoutHeightChanged();
		void layoutWidthChanged();
		void modelChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasLayout() const;
		bool hasModel() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onAppearanceChanged();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
