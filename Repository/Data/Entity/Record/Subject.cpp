#include "Subject.hpp"

#include "../Tag/SubjectTag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Subject::Subject()
	:
		AbstractRecord("Subject"),

		name("name", Field::Type::Text, false),
		avatar_scheme("avatar_scheme", Field::Type::Integer, true),
		description("description", Field::Type::Text, true),
		picture("picture", Field::Type::Text, true),
		locale("locale", Field::Type::Text, false),
		timezone("timezone", Field::Type::Text, false),
		creation_timestamp("creation_timestamp", Field::Type::DateTime, true),
		modification_timestamp("modification_timestamp", Field::Type::DateTime, true),
		meta("meta", Field::Type::Document, true)
	{
		addField(name);
		addField(avatar_scheme);
		addField(description);
		addField(picture);
		addField(locale);
		addField(timezone);
		addField(creation_timestamp);
		addField(modification_timestamp);
		addField(meta);
	}

	QSharedPointer<AbstractTag> Subject::makeTag() const
	{
		return QSharedPointer<SubjectTag>::create();
	}

	void Subject::onSelect()
	{
		AbstractRecord::onSelect();
	}

	void Subject::onCreate()
	{
		AbstractRecord::onCreate();
	}

	void Subject::onForeign()
	{
		AbstractRecord::onForeign();
	}
} } } }
