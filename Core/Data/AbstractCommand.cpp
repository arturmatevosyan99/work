#include "AbstractCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	AbstractCommand::AbstractCommand()
	:
		Core::Model::AbstractEvent()
	{

	}

	AbstractCommand::AbstractCommand(const AbstractCommand& action)
	:
		Core::Model::AbstractEvent(action)
	{

	}

	AbstractCommand::AbstractCommand(AbstractCommand&& action)
	:
		Core::Model::AbstractEvent(action)
	{

	}

	AbstractCommand::~AbstractCommand()
	{

	}
} } } }
