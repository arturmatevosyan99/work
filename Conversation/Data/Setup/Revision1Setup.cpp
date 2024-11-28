#include "Revision1Setup.hpp"

#include "../Entity/ChannelMember.hpp"
#include "../Entity/ChannelMessage.hpp"
#include "../Entity/ChannelDraft.hpp"
#include "../Entity/ChannelTag.hpp"
#include "../Entity/ChannelSync.hpp"

#include "../../../Core/Data/Factory.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	Revision1Setup::Revision1Setup()
	:
		Repository::Data::AbstractSetup()
	{

	}

	bool Revision1Setup::go(Core::Data::Provider& data_provider) const
	{
		Data::ChannelMember channel_member_data(0);
		Data::ChannelMessage channel_message_data(0);
		Data::ChannelDraft channel_draft_data(0);
		Data::ChannelTag channel_tag_data(0);
		Data::ChannelSync channel_sync_data(0);

		Core::Data::Factory data_factory(data_provider.makeFactory());
		return (
				data_factory.create(&channel_member_data, false)
				&&
				data_factory.create(&channel_message_data, false)
				&&
				data_factory.create(&channel_draft_data, false)
				&&
				data_factory.create(&channel_tag_data, false)
				&&
				data_factory.create(&channel_sync_data, false)
		);
	}
} } } }
