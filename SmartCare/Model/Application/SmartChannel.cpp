#include "SmartChannel.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	SmartChannel::SmartChannel(QObject* parent)
	:
		Conversation::Model::GroupChannel(parent)
	{
	}

	SmartChannel::~SmartChannel()
	{
	}
} } } }
