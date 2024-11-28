#include "GroupTag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	GroupTag::GroupTag()
	:
		AbstractTag("Group"),

		target({{std::ref(target_id), &Group::id}}, this)
	{
		addForeignKey(target);
	}

	void GroupTag::onSelect()
	{
		AbstractTag::onSelect();
	}

	void GroupTag::onCreate()
	{
		AbstractTag::onCreate();
	}

	void GroupTag::onForeign()
	{
		AbstractTag::onForeign();
	}
} } } }
