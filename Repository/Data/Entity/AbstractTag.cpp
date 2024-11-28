#include "AbstractTag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	AbstractTag::AbstractTag(const QString& name)
	:
		Core::Data::AbstractEntity("Repository_" + name + "Tag", ""),

		target_id("target_id", Field::Type::Integer, false),
		tag_id("tag_id", Field::Type::Integer, false),

		tag({{std::ref(tag_id), &Tag::id}}, this)
	{
		addField(target_id);
		addField(tag_id);

		setPrimaryKey({target_id, tag_id});

		addForeignKey(tag);
	}

	void AbstractTag::onSelect()
	{

	}

	void AbstractTag::onCreate()
	{

	}

	void AbstractTag::onForeign()
	{

	}
} } } }
