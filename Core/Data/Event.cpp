#include "Event.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	Event::Event(const QString& name)
	:
		Core::Model::Event<AbstractCommand>(name)
	{

	}

	Event::Event(const Event& origin)
	:
		Core::Model::Event<AbstractCommand>(origin)
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
