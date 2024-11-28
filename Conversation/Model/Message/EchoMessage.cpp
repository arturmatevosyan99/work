#include "EchoMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	EchoMessage::EchoMessage(QObject* parent)
	:
		CommandMessage(parent)
	{

	}

	EchoMessage::~EchoMessage()
	{

	}
	const QString& EchoMessage::getText() const
	{
		return getOutput();
	}
} } } }
