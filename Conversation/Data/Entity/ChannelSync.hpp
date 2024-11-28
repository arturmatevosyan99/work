#ifndef Cadabra_Queen_Conversation_Data_Entity_ChannelSync_hpp
#define Cadabra_Queen_Conversation_Data_Entity_ChannelSync_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Repository/Meta/Essence.hpp"
#include "ChannelMember.hpp"
#include "ChannelMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class ChannelSync : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ChannelSync(Repository::Meta::ObjectId object_id);
		virtual ~ChannelSync() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field object_id;
		Field log_id;
		Field timestamp;

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
