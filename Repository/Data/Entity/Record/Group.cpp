#include "Group.hpp"

#include "../Tag/GroupTag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Group::Group()
	:
		AbstractRecord("Group"),

		name("name", Field::Type::Text, false),
		description("description", Field::Type::Text, true),
		picture("picture", Field::Type::Text, true),
		creation_timestamp("creation_timestamp", Field::Type::DateTime, true),
		modification_timestamp("modification_timestamp", Field::Type::DateTime, true),
		meta("meta", Field::Type::Document, true)
	{
		addField(name);
		addField(description);
		addField(picture);
		addField(creation_timestamp);
		addField(modification_timestamp);
		addField(meta);
	}

	QSharedPointer<AbstractTag> Group::makeTag() const
	{
		return QSharedPointer<GroupTag>::create();
	}

	void Group::onSelect()
	{
		AbstractRecord::onSelect();
	}

	void Group::onCreate()
	{
		AbstractRecord::onCreate();
	}

	void Group::onForeign()
	{
		AbstractRecord::onForeign();
	}
} } } }
