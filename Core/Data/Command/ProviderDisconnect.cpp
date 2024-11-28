#include "ProviderDisconnect.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	ProviderDisconnect::ProviderDisconnect()
	:
		AbstractCommand()
	{

	}

	ProviderDisconnect::ProviderDisconnect(const ProviderDisconnect& origin)
	:
		AbstractCommand(origin)
	{

	}

	ProviderDisconnect::ProviderDisconnect(ProviderDisconnect&& origin)
	:
		AbstractCommand(origin)
	{

	}

	ProviderDisconnect::~ProviderDisconnect()
	{

	}
} } } }
