#ifndef Cadabra_Queen_Mentalgami_Meta_RiddleRender_hpp
#define Cadabra_Queen_Mentalgami_Meta_RiddleRender_hpp

#include "../Model/AbstractRiddle.hpp"

#include "../../Core/Meta/Scheme/RenderScheme.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Meta {
	class RiddleRender : public Core::Meta::RenderScheme
	{
	/** @name Aliases */
	/** @{ */
	public:
		using _ = Model::AbstractRiddle;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		RiddleRender();
		RiddleRender(const QString& extname, int version_major, int version_minor, const QString& filename);
		RiddleRender(const RiddleRender& origin);
		RiddleRender(RiddleRender&& origin);

		virtual ~RiddleRender() override = default;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		RiddleRender& operator=(const RiddleRender& origin);
		RiddleRender& operator=(RiddleRender&& origin);
	/** @} */

	/** @name Procedures  */
	/** @{ */
	private:
		Q_INVOKABLE virtual bool load(const Cadabra::Queen::Core::Meta::Qd& qd) override;
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Mentalgami::Meta::RiddleRender)

#endif
