#include "AbstractAction.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AbstractAction::AbstractAction()
	:
		Core::Model::AbstractEvent()
	{

	}

	AbstractAction::AbstractAction(const AbstractAction& action)
	:
		Core::Model::AbstractEvent(action)
	{

	}

	AbstractAction::AbstractAction(AbstractAction&& action)
	:
		Core::Model::AbstractEvent(action)
	{

	}

	AbstractAction::~AbstractAction()
	{

	}

	bool AbstractAction::import(const QJsonValue& payload)
	{
		return false;
	}
} } } }
