#include "EntityClose.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	EntityClose::EntityClose()
	:
		EntityCommand(nullptr)
	{

	}

	EntityClose::EntityClose(QSharedPointer<AbstractEntity> entity)
	:
		EntityCommand(entity)
	{

	}

	EntityClose::EntityClose(const EntityClose& origin)
	:
		EntityCommand(origin)
	{

	}

	EntityClose::EntityClose(EntityClose&& origin)
	:
		EntityCommand(std::move(origin))
	{

	}

	EntityClose::~EntityClose()
	{

	}
} } } }
