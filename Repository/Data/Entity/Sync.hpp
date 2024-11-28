#ifndef Cadabra_Queen_Repository_Data_Entity_Sync_hpp
#define Cadabra_Queen_Repository_Data_Entity_Sync_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Sync : public Core::Data::AbstractEntity
	{
	/** @name Constructors */
	/** @{ */
	public:
		Sync();
		virtual ~Sync() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:		
		Field target;
		Field timestamp;
		Field acestamp;
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
