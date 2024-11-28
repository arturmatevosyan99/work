#include "ChannelSync.hpp"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	ChannelSync::ChannelSync(Repository::Meta::ObjectId target_object_id)
	:
		Core::Data::AbstractEntity("Conversation_ChannelSync", target_object_id ? QString::number(target_object_id) : ""),

		object_id("object_id", Field::Type::Integer, false),
		log_id("log_id", Field::Type::Integer, false),
		timestamp("timestamp", Field::Type::Timestamp, false),

		object(
				{
					{std::ref(object_id), &Repository::Data::Object::id}
				},
				this
		)
	{
		addField(log_id);
		addField(timestamp);

		if (!target_object_id) {
			setPrimaryKey({log_id});
		} else {
			addField(object_id);
			addForeignKey(object);
			setPrimaryKey({object_id, log_id});
		}
	}

	void ChannelSync::onSelect()
	{

	}

	void ChannelSync::onCreate()
	{
	}

	void ChannelSync::onForeign()
	{		
	}
} } } }
