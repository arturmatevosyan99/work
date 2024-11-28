#include "FolderInterface.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Core::Net::CRPDelay FolderInterface::index()
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::FolderInterface",
			"Cadabra::Ace::Repository::Model::Index",
			{
			}
		).detach(this, &FolderInterface::onIndex);
	}

	void FolderInterface::onIndexed(const Map& map)
	{

	}

	void FolderInterface::onIndex(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			/// @log
			reaction.log("FolderInterface::onIndex");
			return ;
		} else if (!(*reaction).isArray()) {
			qWarning("Unknown data structure, could not list");
			return ;
		}
		onIndexed(Meta::Object::Parse(*reaction));
	}
} } } }
