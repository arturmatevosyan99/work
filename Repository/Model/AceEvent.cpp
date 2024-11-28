#include "AceEvent.hpp"

#include "AbstractAction.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AceEvent::AceEvent(const QString& name, Meta::RealmTarget target)
	:
		Core::Model::Event<AbstractAction>(name),

		_target(target)
	{

	}

	AceEvent::AceEvent(const AceEvent& origin)
	:
		Core::Model::Event<AbstractAction>(origin),

		_target(origin._target)
	{

	}

	AceEvent::~AceEvent()
	{
/*
		QMetaType::destroy(_id, _action);
		_action = nullptr;
*/
	}

	bool AceEvent::import(const QJsonValue& payload)
	{		
		return (_event) ? _event->import(payload) : false;
	}

	const Meta::RealmTarget& AceEvent::getTarget() const
	{
		return _target;
	}
} } } }
