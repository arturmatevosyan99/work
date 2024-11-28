#include "EntityCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	EntityCommand::EntityCommand(QSharedPointer<AbstractEntity> entity)
	:
		AbstractCommand(),

		_entity(entity)
	{

	}

	EntityCommand::EntityCommand(const EntityCommand& origin)
	:
		AbstractCommand(origin),

		_entity(origin._entity)
	{

	}

	EntityCommand::EntityCommand(EntityCommand&& origin)
	:
		AbstractCommand(origin),

		_entity(origin._entity)
	{

	}

	EntityCommand::~EntityCommand()
	{

	}

	const QSharedPointer<AbstractEntity>& EntityCommand::getEntity() const
	{
		return _entity;
	}
} } } }
