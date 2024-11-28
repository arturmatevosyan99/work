#include "SyncAction.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	SyncAction::SyncAction()
	{

	}

	SyncAction::SyncAction(const SyncAction& action)
	{

	}

	SyncAction::SyncAction(SyncAction&& action)
	{

	}

	bool SyncAction::import(const QJsonValue& payload)
	{
/*
		if (payload.isArray()) {
			return import(payload.toArray());
		} else if (payload.isObject()) {
			return import(payload.toObject());
		}
*/
		return false;
	}
} } } }
