#ifndef Cadabra_Queen_Conversation_Data_Entity_ChannelTag_hpp
#define Cadabra_Queen_Conversation_Data_Entity_ChannelTag_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Repository/Data/Entity/Record/Tag.hpp"
#include "ChannelMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class ChannelTag : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ChannelTag(Repository::Meta::ObjectId object_id);
		virtual ~ChannelTag() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field target_object_id;
		Field target_message_id;
		Field owner_subject_id;
		Field tag_id;

		Core::Data::ForeignEntity<Repository::Data::Object> target_object;
		Core::Data::ForeignEntity<ChannelMessage> target_message;
		Core::Data::ForeignEntity<Repository::Data::Tag> tag;
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
