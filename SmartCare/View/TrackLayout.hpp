#ifndef Cadabra_Queen_SmartCare_View_TrackLayout_hpp
#define Cadabra_Queen_SmartCare_View_TrackLayout_hpp

#include "TrackView.hpp"
#include "../SmartCare.hpp"
#include "../Model/AbstractTrack.hpp"
#include "../../Core/Meta/Qd.hpp"

#include <QQuickItem>
#include <QQmlComponent>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace View {
	class TrackLayout : public QQuickItem
	{
	/** @name Enum */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::SmartCare::Model::AbstractTrack* model READ getModel NOTIFY rendered)
		Q_PROPERTY(Cadabra::Queen::SmartCare::View::TrackView* view READ getView NOTIFY viewChanged)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd scheme READ getScheme NOTIFY rendered)
		Q_PROPERTY(bool hasModel READ hasModel NOTIFY rendered)
		Q_PROPERTY(bool hasView READ hasView NOTIFY viewChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit TrackLayout(QQuickItem* parent = nullptr);
		virtual ~TrackLayout() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Core::Meta::Qd _scheme;
		TrackView* _view;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool represent(const QString& content_type, Cadabra::Queen::SmartCare::TrackRenderLayout type);
		Q_INVOKABLE void render(Cadabra::Queen::Core::Meta::Qd scheme, Cadabra::Queen::SmartCare::TrackRenderLayout type);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::SmartCare::Model::AbstractTrack* getModel() const;
		Cadabra::Queen::SmartCare::View::TrackView* getView() const;
		Cadabra::Queen::Core::Meta::Qd getScheme() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void rendered();
		void modelImported();
		void viewChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onViewWidthChanged();
		void onViewHeightChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasModel() const;
		bool hasView() const;
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
