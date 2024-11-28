#ifndef Cadabra_Queen_Conversation_Data_Entity_ChannelDraft_hpp
#define Cadabra_Queen_Conversation_Data_Entity_ChannelDraft_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Core/Data/ForeignEntity.hpp"

#include "../../../Repository/Meta/Essence.hpp"
#include "../../../Repository/Data/Entity/Record/Object.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class ChannelDraft : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ChannelDraft(Repository::Meta::ObjectId target_object_id);
		virtual ~ChannelDraft() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field object_id;
		Field uid;
		Field reply_id;
		Field timestamp;
		Field scheme;
		Field body;

		Core::Data::ForeignEntity<Repository::Data::Object> object;
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
