#include "ChannelTag.hpp"

#include <QSqlRecord>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	ChannelTag::ChannelTag(Repository::Meta::ObjectId object_id)
	:
		Core::Data::AbstractEntity("Conversation_ChannelTag", object_id ? QString::number(object_id) : ""),

		target_object_id("target_object_id", Field::Type::Integer, false),
		target_message_id("target_message_id", Field::Type::Integer, false),
		owner_subject_id("owner_subject_id", Field::Type::Integer, false),
		tag_id("tag_id", Field::Type::Integer, false),

		target_object(
						{
							{std::ref(target_object_id), &Repository::Data::Object::id}
						},
						this
		),
		target_message(
						object_id
						? std::initializer_list<QPair<std::reference_wrapper<Field>, Core::Data::ForeignEntity<ChannelMessage>::ForeignPointer>>
						{
							{std::ref(target_message_id), &ChannelMessage::id}
						}
						: std::initializer_list<QPair<std::reference_wrapper<Field>, Core::Data::ForeignEntity<ChannelMessage>::ForeignPointer>>
						{
							{std::ref(target_object_id), &ChannelMessage::target_object_id},
							{std::ref(target_message_id), &ChannelMessage::id}
						},
						this, object_id
		),
		tag(
			{
				{std::ref(tag_id), &Repository::Data::Tag::id}
			},
			this
		)
	{
		addField(owner_subject_id);
		addField(target_message_id);
		addField(tag_id);

		if (object_id) {
			setPrimaryKey({target_message_id, owner_subject_id, tag_id});
		} else {
			addField(target_object_id);
			addForeignKey(target_object);
			setPrimaryKey({target_object_id, target_message_id, owner_subject_id, tag_id});
		}

		addForeignKey(target_message);
		addForeignKey(tag);
	}

	void ChannelTag::onSelect()
	{
	}

	void ChannelTag::onCreate()
	{
	}

	void ChannelTag::onForeign()
	{
	}
} } } }
