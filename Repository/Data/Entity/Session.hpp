#ifndef Cadabra_Queen_Repository_Data_Entity_Session_hpp
#define Cadabra_Queen_Repository_Data_Entity_Session_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Session : public Core::Data::AbstractEntity
	{
	/** @name Constructors */
	/** @{ */
	public:
		Session();
		virtual ~Session() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field handle;
		Field timestamp;
		Field object_id;
		Field application_scheme;
		Field application_handle;
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
