#include "ProfileInterface.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Core::Net::CRPDelay ProfileInterface::pickUp()
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::ProfileInterface",
			"Cadabra::Ace::Repository::Model::PickUp",
			{
			}
		).detach(this, &ProfileInterface::onPickUp);
	}

	void ProfileInterface::onPickedUp(const Meta::Profile& profile)
	{

	}

	void ProfileInterface::onPickUp(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			/// @log
			reaction.log("ProfileInterface::onPickUp");
			return ;
		} else if (!(*reaction).isObject()) {
			qWarning("Unknown data structure, could not parse");
			return ;
		}

		const Meta::ObjectId home_object_id(Meta::Essence::ToId((*reaction).toObject().value("home_object_id"), true));
		onPickedUp(Meta::Profile(home_object_id));
	}
} } } }
