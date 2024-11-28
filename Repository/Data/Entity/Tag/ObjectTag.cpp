#include "ObjectTag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	ObjectTag::ObjectTag()
	:
		AbstractTag("Object"),

		target({{std::ref(target_id), &Object::id}}, this)
	{
		addForeignKey(target);
	}

	void ObjectTag::onSelect()
	{
		AbstractTag::onSelect();
	}

	void ObjectTag::onCreate()
	{
		AbstractTag::onCreate();
	}

	void ObjectTag::onForeign()
	{
		AbstractTag::onForeign();
	}
} } } }
