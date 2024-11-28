#include "Extension.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Extension::Extension()
	:
		Core::Data::AbstractEntity("Repository_Extension", ""),

		name("name", Field::Type::Text, false),
		revision("revision", Field::Type::Integer, false)
	{
		addField(name);
		addField(revision);

		setPrimaryKey({name});
	}

	void Extension::onSelect()
	{

	}

	void Extension::onCreate()
	{

	}

	void Extension::onForeign()
	{

	}
} } } }
