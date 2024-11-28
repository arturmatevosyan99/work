#include "ProxyMessage.hpp"

#include "../../Meta/MessageRender.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	ProxyMessage::ProxyMessage(QObject* parent)
	:
		AbstractMessage(parent)
	{
	}

	bool ProxyMessage::import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, const Cadabra::Queen::Conversation::Meta::Message& message_meta)
	{
		if (message_meta.getSchemeIdent() == 0) {
			_message.reset();
			emit imported();
			return true;
		} else if (!(_message = Core::Meta::Scheme<Conversation::Meta::MessageRender>::Produce(message_meta.getSchemeIdent()))) {
			return false;
		} else if (_message->import(channel_application, message_meta)) {
			emit imported();
			return true;
		}
		return false;
	}

	QJsonObject ProxyMessage::exportBody() const
	{
		return {};
	}

	bool ProxyMessage::importBody(const QJsonObject& body)
	{		
		return true;
	}

	QString ProxyMessage::getPreviewText() const
	{
		return !_message ? tr("...") : _message->getPreviewText();
	}

	const AbstractMessage::AssistList& ProxyMessage::getAssistList() const
	{
		return !_message ? AbstractMessage::getAssistList() : _message->getAssistList();
	}

	AbstractMessage* ProxyMessage::getTarget()
	{
		return !_message ? this : _message.get();
	}
} } } }
