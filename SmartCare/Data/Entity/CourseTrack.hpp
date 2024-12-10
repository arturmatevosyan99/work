#ifndef Cadabra_Queen_SmartCare_Data_Entity_CourseTrack_hpp
#define Cadabra_Queen_SmartCare_Data_Entity_CourseTrack_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Core/Data/ForeignEntity.hpp"

#include "../../../Repository/Data/Entity/Record/Subject.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Data {
	class CourseTrack : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CourseTrack(Repository::Meta::ObjectId object_id);
		virtual ~CourseTrack() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field subject_id;
		Field key;
		Field revision_code;
		Field language_code;
		Field flag;
		Field title;
		Field description;
		Field artwork;
		Field weight;
		Field content_type;
		Field content_length;
		Field content_size;
		Field file;
		Field file_timestamp;
		Field life_timestamp;
		Field meta;
		Field meta_timestamp;
		Field local_progress;
		Field local_progress_timestamp;
		Field remote_progress;
		Field remote_progress_timestamp;

		Core::Data::ForeignEntity<Repository::Data::Subject> subject;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
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
