#ifndef Cadabra_Queen_Repository_Data_Entity_AbstractTag_hpp
#define Cadabra_Queen_Repository_Data_Entity_AbstractTag_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Core/Data/ForeignEntity.hpp"
#include "Record/Tag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class AbstractTag : public Core::Data::AbstractEntity
	{
	/** @name Constructors */
	/** @{ */
	public:
		AbstractTag(const QString& name);
		virtual ~AbstractTag() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field target_id;
		Field tag_id;

		Core::Data::ForeignEntity<Tag> tag;
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
