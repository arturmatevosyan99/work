#include "EssenceAction.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	EssenceAction::EssenceAction()
	:
		_acestamp("0000x0000000000000000"),
		_timestamp("0000-00-00 00:00:00.000000"),
		_target_id(0)
	{

	}

	EssenceAction::EssenceAction(const EssenceAction& origin)
	:
		_acestamp(origin._acestamp),
		_timestamp(origin._timestamp),
		_target_id(origin._target_id)
	{

	}

	EssenceAction::EssenceAction(EssenceAction&& origin)
	:
		_acestamp(std::move(origin._acestamp)),
		_timestamp(std::move(origin._timestamp)),
		_target_id(std::move(origin._target_id))
	{

	}

	bool EssenceAction::import(const QJsonValue& payload)
	{
		const QJsonObject object(payload.toObject());

		_acestamp = object.find("acestamp")->toString("0000x0000000000000000");
		_timestamp = object.find("timestamp")->toString("0000-00-00 00:00:00.000000");
		_target_id = object.find("target_id")->toString("0").toLongLong();

		return true;
	}

	const Meta::Essence::Id& EssenceAction::getTargetId() const
	{
		return _target_id;
	}

	const QString& EssenceAction::getAceStamp() const
	{
		return _acestamp;
	}

	const QString& EssenceAction::getTimeStamp() const
	{
		return _timestamp;
	}
} } } }
