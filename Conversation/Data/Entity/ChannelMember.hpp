#ifndef Cadabra_Queen_Conversation_Data_Entity_ChannelMember_hpp
#define Cadabra_Queen_Conversation_Data_Entity_ChannelMember_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Core/Data/ForeignEntity.hpp"

#include "../../../Repository/Meta/Essence.hpp"
#include "../../../Repository/Data/Entity/Record/Object.hpp"
#include "../../../Repository/Data/Entity/Record/Subject.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class ChannelMember : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ChannelMember(Repository::Meta::ObjectId target_object_id);
		virtual ~ChannelMember() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field object_id;
		Field subject_id;
		Field subject_role;
		Field silent_mode;
		Field last_receive_id;
		Field last_read_id;

		Core::Data::ForeignEntity<Repository::Data::Object> object;
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
