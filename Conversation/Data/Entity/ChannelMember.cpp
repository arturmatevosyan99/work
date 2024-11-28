#include "ChannelMember.hpp"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	ChannelMember::ChannelMember(Repository::Meta::ObjectId target_object_id)
	:
		Core::Data::AbstractEntity("Conversation_ChannelMember", target_object_id ? QString::number(target_object_id) : ""),

		object_id("object_id", Field::Type::Integer, false),
		subject_id("subject_id", Field::Type::Integer, false),
		subject_role("subject_role", Field::Type::Integer, false),
		silent_mode("silent_mode", Field::Type::Boolean, true),
		last_receive_id("last_receive_id", Field::Type::Integer, true),
		last_read_id("last_read_id", Field::Type::Integer, true),

		object(
				{
					{std::ref(object_id), &Repository::Data::Object::id}
				},
				this
		),
		subject(
				{
					{std::ref(subject_id), &Repository::Data::Subject::id}
				},
				this
		)
	{
		addField(subject_id);
		addField(subject_role);
		addField(silent_mode);
		addField(last_receive_id);
		addField(last_read_id);

		addForeignKey(subject);

		if (target_object_id) {
			setPrimaryKey({subject_id});
		} else {
			addField(object_id);
			addForeignKey(object);
			setPrimaryKey({object_id, subject_id});
		}
	}

	void ChannelMember::onSelect()
	{
	}

	void ChannelMember::onCreate()
	{
	}

	void ChannelMember::onForeign()
	{
	}
} } } }
