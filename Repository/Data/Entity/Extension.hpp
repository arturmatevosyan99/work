#ifndef Cadabra_Queen_Repository_Data_Entity_Extension_hpp
#define Cadabra_Queen_Repository_Data_Entity_Extension_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Extension : public Core::Data::AbstractEntity
	{
	/** @name Constructors */
	/** @{ */
	public:
		Extension();
		virtual ~Extension() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field name;
		Field revision;
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
