#include "SubjectTag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	SubjectTag::SubjectTag()
	:
		AbstractTag("Subject"),

		target({{std::ref(target_id), &Subject::id}}, this)
	{
		addForeignKey(target);
	}

	void SubjectTag::onSelect()
	{
		AbstractTag::onSelect();
	}

	void SubjectTag::onCreate()
	{
		AbstractTag::onCreate();
	}

	void SubjectTag::onForeign()
	{
		AbstractTag::onForeign();
	}
} } } }
