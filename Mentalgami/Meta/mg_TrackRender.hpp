/**
 @file TrackRender.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Mentalgami_Meta_TrackRender_hpp
#define Cadabra_Queen_Mentalgami_Meta_TrackRender_hpp

#include "../../Core/Meta/Scheme/RenderScheme.hpp"

#include "../Mentalgami.hpp"
#include "../Model/mg_AbstractTrack.hpp"

#include <QFlags>
#include <QString>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Meta {
	class TrackRender : public Core::Meta::RenderScheme
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			LayoutSet = Core::Meta::RenderScheme::Role::CustomRole,
			ArtefactName
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using _ = Model::AbstractTrack;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		TrackRender();
		TrackRender(const QString& extname, int version_major, int version_minor, const QString& filename, const QFlags<TrackRenderLayout>& layout_set = {TrackRenderLayout::PlaylistLayout}, const QString& artefactname = "");
		TrackRender(const TrackRender& origin);
		TrackRender(TrackRender&& origin);
		virtual ~TrackRender() override = default;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
		TrackRender& operator=(const TrackRender& origin);
		TrackRender& operator=(TrackRender&& origin);
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
		QFlags<TrackRenderLayout> _layout_set;
		QString _artefactname;
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	private:
		Q_INVOKABLE virtual bool load(const Cadabra::Queen::Core::Meta::Qd& qd) override;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
		using RenderScheme::makeURL;
		Q_INVOKABLE QUrl makeURL(Cadabra::Queen::Mentalgami::TrackRenderLayout layout) const;
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
		virtual QHash<int, QByteArray> roleNames() const override;
		virtual QVariant data(int role) const override;

		const QFlags<TrackRenderLayout>& getLayoutSet() const;
		const QString& getArtefactName() const;
	/** @} */
		
	/** @name Formatters */
	/** @{ */
	public:
	/** @} */
		
	/** @name States */
	/** @{ */
	public:
	/** @} */
		
	/** @name Hooks */
	/** @{ */
	public:
	/** @} */
		
	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Mentalgami::Meta::TrackRender)

#endif
