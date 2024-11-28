#include "TaggedObject.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	TaggedObject::TaggedObject()
	:
		Object(),

        tag_counter("tag_counter", Field::Type::Integer, false)
	{
//		addField(tag_counter);
	}

	void TaggedObject::onSelect()
	{
		Object::onSelect();		
	}

	void TaggedObject::onCreate()
	{
		Object::onCreate();
	}

	void TaggedObject::onForeign()
	{
		Object::onForeign();
	}
} } } }
