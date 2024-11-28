#ifndef Cadabra_Queen_Conversation_Data_Entity_ChannelMessage_hpp
#define Cadabra_Queen_Conversation_Data_Entity_ChannelMessage_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Repository/Meta/Essence.hpp"
#include "../../../Repository/Data/Entity/Record/Object.hpp"

#include "ChannelMember.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class ChannelMessage : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ChannelMessage(Repository::Meta::ObjectId object_id);
		virtual ~ChannelMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field id;
		Field uid;
		Field reply_id;
		Field post_timestamp;
		Field modification_timestamp;
		Field target_object_id;
		Field author_subject_id;
		Field scheme;
		Field body;

		Core::Data::ForeignEntity<Repository::Data::Object> object;
		Core::Data::ForeignEntity<ChannelMember> member;
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
