#include "ProviderClone.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	ProviderClone::ProviderClone()
	:
		AbstractCommand()
	{

	}

	ProviderClone::ProviderClone(const QString& origin_name, const QString& destination_name)
	:
		AbstractCommand(),

		_origin_name(origin_name),
		_destination_name(destination_name)
	{

	}

	ProviderClone::ProviderClone(const ProviderClone& origin)
	:
		AbstractCommand(origin)
	{

	}

	ProviderClone::ProviderClone(ProviderClone&& origin)
	:
		AbstractCommand(origin)
	{

	}

	ProviderClone::~ProviderClone()
	{

	}

	const QString& ProviderClone::getOriginName() const
	{
		return _origin_name;
	}

	const QString& ProviderClone::getDestinationName() const
	{
		return _destination_name;
	}
} } } }
