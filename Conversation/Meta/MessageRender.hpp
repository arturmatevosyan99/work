/**
 @file MessageRender.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_MessageRender_hpp
#define Cadabra_Queen_Repository_Meta_MessageRender_hpp

#include "../../Core/Meta/Scheme/RenderScheme.hpp"

#include "../Conversation.hpp"
#include "../Model/AbstractMessage.hpp"

#include <QFlags>
#include <QString>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	class MessageRender : public Core::Meta::RenderScheme
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
		using _ = Model::AbstractMessage;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		MessageRender();
		MessageRender(const QString& extname, int version_major, int version_minor, const QString& filename, const QFlags<RenderLayout>& layout_set = {RenderLayout::FeedLayout}, const QString& artefactname = "");
		MessageRender(const MessageRender& origin);
		MessageRender(MessageRender&& origin);
		virtual ~MessageRender() override = default;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
		MessageRender& operator=(const MessageRender& origin);
		MessageRender& operator=(MessageRender&& origin);
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
		QFlags<RenderLayout> _layout_set;
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
		Q_INVOKABLE QUrl makeURL(Cadabra::Queen::Conversation::RenderLayout layout) const;
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
		virtual QHash<int, QByteArray> roleNames() const override;
		virtual QVariant data(int role) const override;

		const QFlags<RenderLayout>& getLayoutSet() const;
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

Q_DECLARE_METATYPE(Cadabra::Queen::Conversation::Meta::MessageRender)

#endif
