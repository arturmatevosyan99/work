#include "Event.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	Event::Event(const QString& name)
	:
		Core::Model::Event<AbstractAccident>(name)
	{

	}

	Event::Event(const Event& origin)
	:
		Core::Model::Event<AbstractAccident>(origin)
	{

	}

	Event::~Event()
	{
/*
		QMetaType::destroy(_id, _action);
		_action = nullptr;
*/
	}
} } } }
