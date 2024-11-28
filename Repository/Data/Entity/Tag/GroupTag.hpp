#ifndef Cadabra_Queen_Repository_Data_Entity_Tag_GroupTag_hpp
#define Cadabra_Queen_Repository_Data_Entity_Tag_GroupTag_hpp

#include "../AbstractTag.hpp"

#include "../Record/Group.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class GroupTag : public AbstractTag
	{
	/** @name Constructors */
	/** @{ */
	public:
		GroupTag();
		virtual ~GroupTag() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Core::Data::ForeignEntity<Group> target;
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
