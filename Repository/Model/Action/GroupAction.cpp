#include "GroupAction.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	GroupAction::GroupAction()
	:
		EssenceAction(),

		_incident(Incident::Internal)
	{

	}

	GroupAction::GroupAction(const GroupAction& origin)
	:
		EssenceAction(origin),

		_incident(origin._incident)
	{

	}

	GroupAction::GroupAction(GroupAction&& origin)
	:
		EssenceAction(std::move(origin)),

		_incident(origin._incident)
	{

	}

	bool GroupAction::import(const QJsonValue& payload)
	{
		if (!EssenceAction::import(payload)) {
			return false;
		}

		const QJsonObject object(payload.toObject());
		_incident = static_cast<Incident>(object.find("incident")->toInt(static_cast<int>(Incident::Flush)));

		return true;
	}

	Meta::Essence::Target GroupAction::getTarget() const
	{
		return Meta::Essence::Target::Group;
	}

	const GroupAction::Incident& GroupAction::getIncident() const
	{
		return _incident;
	}
} } } }
