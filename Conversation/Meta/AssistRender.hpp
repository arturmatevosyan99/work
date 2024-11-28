/**
 @file AssistRender.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_AssistRender_hpp
#define Cadabra_Queen_Repository_Meta_AssistRender_hpp

#include "../../Core/Meta/Scheme/RenderScheme.hpp"

#include "../Model/AbstractAssist.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	class AssistRender : public Core::Meta::RenderScheme
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using _ = Model::AbstractAssist;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		AssistRender();
		AssistRender(const QString& extname, int version_major, int version_minor, const QString& filename);
		AssistRender(const AssistRender& origin);
		AssistRender(AssistRender&& origin);
		virtual ~AssistRender() override = default;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
		AssistRender& operator=(const AssistRender& origin);
		AssistRender& operator=(AssistRender&& origin);
	/** @} */
		
	/** @name Properties */
	/** @{ */
	public:		
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	private:
		Q_INVOKABLE virtual bool load(const Cadabra::Queen::Core::Meta::Qd& qd) override;
	/** @} */
		
	/** @name Converters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
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
Q_DECLARE_METATYPE(Cadabra::Queen::Conversation::Meta::AssistRender)
#endif
