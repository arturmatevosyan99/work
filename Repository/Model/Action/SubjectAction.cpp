#include "SubjectAction.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	SubjectAction::SubjectAction()
	:
		EssenceAction(),

		_incident(Incident::Internal)
	{

	}

	SubjectAction::SubjectAction(const SubjectAction& origin)
	:
		EssenceAction(origin),

		_incident(origin._incident)
	{

	}

	SubjectAction::SubjectAction(SubjectAction&& origin)
	:
		EssenceAction(std::move(origin)),

		_incident(origin._incident)
	{

	}

	bool SubjectAction::import(const QJsonValue& payload)
	{
		if (!EssenceAction::import(payload)) {
			return false;
		}

		const QJsonObject object(payload.toObject());
		_incident = static_cast<Incident>(object.find("incident")->toInt(static_cast<int>(Incident::Flush)));

		return true;
	}

	Meta::Essence::Target SubjectAction::getTarget() const
	{
		return Meta::Essence::Target::Subject;
	}

	const SubjectAction::Incident& SubjectAction::getIncident() const
	{
		return _incident;
	}
} } } }
