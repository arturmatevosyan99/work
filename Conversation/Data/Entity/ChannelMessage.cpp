#include "ChannelMessage.hpp"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	ChannelMessage::ChannelMessage(Repository::Meta::ObjectId object_id)
	:
		Core::Data::AbstractEntity("Conversation_ChannelMessage", object_id ? QString::number(object_id) : ""),

		id("id", Field::Type::Integer, false),
		uid("uid", Field::Type::UUID, false),
		reply_id("reply_id", Field::Type::Integer, true),
		post_timestamp("post_timestamp", Field::Type::Timestamp, false),
		modification_timestamp("modification_timestamp", Field::Type::Timestamp, true),
		target_object_id("target_object_id", Field::Type::Integer, false),
		author_subject_id("author_subject_id", Field::Type::Integer, false),
		scheme("scheme", Field::Type::Integer, false),
		body("body", Field::Type::Document, true),

		object(
				{
					{std::ref(target_object_id), &Repository::Data::Object::id}
				},
				this
		),
		member(
				object_id
				? std::initializer_list<QPair<std::reference_wrapper<Field>, Core::Data::ForeignEntity<ChannelMember>::ForeignPointer>>
				{
					{std::ref(author_subject_id), &ChannelMember::subject_id}
				}
				: std::initializer_list<QPair<std::reference_wrapper<Field>, Core::Data::ForeignEntity<ChannelMember>::ForeignPointer>>
				{
					{std::ref(target_object_id), &ChannelMember::object_id},
					{std::ref(author_subject_id), &ChannelMember::subject_id}
				},
				this, object_id
		)
	{
		addField(id);
		addField(uid);
		addField(reply_id);
		addField(post_timestamp);
		addField(modification_timestamp);
		addField(author_subject_id);
		addField(scheme);
		addField(body);

		if (object_id) {
			setPrimaryKey({id});
		} else {
			addField(target_object_id);
			setPrimaryKey({target_object_id, id});
			addForeignKey(object);
		}

		addForeignKey(member);
	}

	void ChannelMessage::onSelect()
	{

	}

	void ChannelMessage::onCreate()
	{
	}

	void ChannelMessage::onForeign()
	{
	}
} } } }
