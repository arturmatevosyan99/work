#include "MessageDelivery.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
    MessageDelivery::MessageDelivery()
	:
        AbstractAccident()
	{

	}

    MessageDelivery::MessageDelivery(const MessageDelivery::Map& map)
	:
		AbstractAccident(),

        _map(map)
	{

	}

    MessageDelivery::MessageDelivery(const MessageDelivery& origin)
	:
		AbstractAccident(origin),

        _map(origin._map)
	{

	}

    MessageDelivery::MessageDelivery(MessageDelivery&& origin)
	:
		AbstractAccident(std::move(origin)),

        _map(std::move(origin._map))
	{
	}

    MessageDelivery::~MessageDelivery()
	{

	}

    const MessageDelivery::Map& MessageDelivery::getMap() const
	{
        return _map;
    }
} } } }
