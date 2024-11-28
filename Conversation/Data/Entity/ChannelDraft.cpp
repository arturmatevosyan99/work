#include "ChannelDraft.hpp"

#include <QSqlRecord>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	ChannelDraft::ChannelDraft(Repository::Meta::ObjectId target_object_id)
	:
		Core::Data::AbstractEntity("Conversation_ChannelDraft", target_object_id ? QString::number(target_object_id) : ""),

		object_id("object_id", Field::Type::Integer, false),
		uid("uid", Field::Type::UUID, false),
		reply_id("reply_id", Field::Type::Integer, true),
		timestamp("timestamp", Field::Type::Timestamp, false),
		scheme("scheme", Field::Type::Integer, false),
		body("body", Field::Type::Document, true),

		object(
				{
					{std::ref(object_id), &Repository::Data::Object::id}
				},
				this
		)
	{
		addField(uid);
		addField(reply_id);
		addField(timestamp);
		addField(scheme);
		addField(body);

		if (target_object_id) {
			setPrimaryKey({uid});
		} else {
			addField(object_id);
			addForeignKey(object);
			setPrimaryKey({object_id, uid});
		}
	}

	void ChannelDraft::onSelect()
	{

	}

	void ChannelDraft::onCreate()
	{
	}

	void ChannelDraft::onForeign()
	{
	}
} } } }
