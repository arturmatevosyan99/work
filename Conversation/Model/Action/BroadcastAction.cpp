#include "BroadcastAction.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	BroadcastAction::BroadcastAction()
	{

	}

	BroadcastAction::BroadcastAction(const BroadcastAction& action)
	{

	}

	BroadcastAction::BroadcastAction(BroadcastAction&& action)
	{

	}

	bool BroadcastAction::import(const QJsonValue& payload)
	{
		_log_id = payload["log_id"].toVariant().toLongLong();
		_message_id = payload["message_id"].toVariant().toLongLong();
		_broadcaster_subject_id = payload["broadcaster_subject_id"].toVariant().toLongLong();
		_timestamp = payload["timestamp"].toInt();
		_incident = static_cast<Incident>(payload["incident"].toInt());

		return _message_id > 0;
	}

	const Meta::Message::Id& BroadcastAction::getLogId() const
	{
		return _log_id;
	}

	const Meta::Message::Id& BroadcastAction::getMessageId() const
	{
		return _message_id;
	}

	const Repository::Meta::Subject::Id& BroadcastAction::getBroadcasterSubjectId() const
	{
		return _broadcaster_subject_id;
	}

	const BroadcastAction::Incident& BroadcastAction::getIncident() const
	{
		return _incident;
	}
} } } }
