#ifndef Cadabra_Queen_Repository_Data_Entity_Tag_ObjectTag_hpp
#define Cadabra_Queen_Repository_Data_Entity_Tag_ObjectTag_hpp

#include "../AbstractTag.hpp"

#include "../Record/Object.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class ObjectTag : public AbstractTag
	{
	/** @name Constructors */
	/** @{ */
	public:
		ObjectTag();
		virtual ~ObjectTag() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Core::Data::ForeignEntity<Object> target;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual void onSelect() override;
		virtual void onCreate() override;
		virtual void onForeign() override;
	/** @} */
	};
} } } }

#endif
