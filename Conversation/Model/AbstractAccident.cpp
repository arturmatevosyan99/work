#include "AbstractAccident.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	AbstractAccident::AbstractAccident()
	:
		Core::Model::AbstractEvent()
	{

	}

	AbstractAccident::AbstractAccident(const AbstractAccident& action)
	:
		Core::Model::AbstractEvent(action)
	{

	}

	AbstractAccident::AbstractAccident(AbstractAccident&& action)
	:
		Core::Model::AbstractEvent(action)
	{

	}

	AbstractAccident::~AbstractAccident()
	{

	}
} } } }
