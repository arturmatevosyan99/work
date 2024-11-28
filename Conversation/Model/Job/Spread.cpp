#include "Spread.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	Spread::Spread(QObject* parent)
	:
		Repository::Model::AbstractJob(-8034428181974876159, parent)
	{
	}

	Spread::~Spread()
	{
	}

	void Spread::addChannelId(Repository::Meta::Object::Id channel_id)
	{
		_token_vector.push_back(Repository::Meta::EssenceToken());
		_token_vector.last().type = Repository::Meta::EssenceToken::Type::Object;
		_token_vector.last().value = QString::number(channel_id);
	}

	void Spread::addChannelToken(const Repository::Meta::EssenceToken& token)
	{
		_token_vector.push_back(token);
	}

	void Spread::addChannelTokenVector(const QVector<Repository::Meta::EssenceToken>& token)
	{
		_token_vector.append(token);
	}

	void Spread::setMessageText(const QString& text)
	{
		_message_text = text;
	}

	QJsonObject Spread::makeEnvironment() const
	{
		QJsonArray token_array;
		for (const Repository::Meta::EssenceToken& token : _token_vector) {
			token_array.push_back(token.dump());
		}

		return {
				{"message_text", _message_text},
				{"token_array", token_array}
		};
	}
} } } }
