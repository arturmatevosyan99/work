#ifndef Cadabra_Queen_Mentalgami_Data_Entity_CourseBrief_hpp
#define Cadabra_Queen_Mentalgami_Data_Entity_CourseBrief_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Core/Data/ForeignEntity.hpp"

#include "../../../Repository/Data/Entity/Record/Subject.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Data {
	class CourseBrief : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CourseBrief(Repository::Meta::ObjectId object_id);
		virtual ~CourseBrief() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field title;
		Field description;
		Field author;
		Field copyright;
		Field artwork;
		Field revision_code;
		Field language_code;
		Field release_datetime;
		Field key;
		Field count;
		Field meta;
		Field subject_id;

		Core::Data::ForeignEntity<Repository::Data::Subject> subject;
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
