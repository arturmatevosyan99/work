#include "Tag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Tag::Tag()
	:
		AbstractRecord("Tag"),

		timestamp("timestamp", Field::Type::Text, false),
		alias("alias", Field::Type::Text, false)
	{
		addField(timestamp);
		addField(alias);
	}

	QSharedPointer<AbstractTag> Tag::makeTag() const
	{
		return nullptr;
	}

	void Tag::onSelect()
	{
		AbstractRecord::onSelect();
	}

	void Tag::onCreate()
	{
		AbstractRecord::onCreate();
	}

	void Tag::onForeign()
	{
		AbstractRecord::onForeign();
	}
} } } }
