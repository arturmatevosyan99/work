#include "HandleAction.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	HandleAction::HandleAction()
	:
		_cause(Cause::Validation)
	{

	}

	HandleAction::HandleAction(const HandleAction& origin)
	:
		_cause(origin._cause)
	{

	}

	HandleAction::HandleAction(HandleAction&& origin)
	:
		_cause(origin._cause)
	{

	}

	bool HandleAction::import(const QJsonValue& payload)
	{
		if (!payload.isDouble()) {
			return false;
		}

		_cause = static_cast<Cause>(payload.toInt(0));
		return true;
	}

	const HandleAction::Cause& HandleAction::getCause() const
	{
		return _cause;
	}
} } } }
