#include "ObjectAction.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	ObjectAction::ObjectAction()
	:
		EssenceAction(),

		_incident(Incident::Internal)
	{

	}

	ObjectAction::ObjectAction(const ObjectAction& origin)
	:
		EssenceAction(origin),

		_incident(origin._incident)
	{

	}

	ObjectAction::ObjectAction(ObjectAction&& origin)
	:
		EssenceAction(std::move(origin)),

		_incident(origin._incident)
	{

	}

	bool ObjectAction::import(const QJsonValue& payload)
	{
		if (!EssenceAction::import(payload)) {
			return false;
		}

		const QJsonObject object(payload.toObject());
		_incident = static_cast<Incident>(object.find("incident")->toInt(static_cast<int>(Incident::Flush)));

		return true;
	}

	Meta::Essence::Target ObjectAction::getTarget() const
	{
		return Meta::Essence::Target::Object;
	}

	const ObjectAction::Incident& ObjectAction::getIncident() const
	{
		return _incident;
	}
} } } }
