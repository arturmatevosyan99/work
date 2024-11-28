#include "Sync.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Sync::Sync()
	:
		Core::Data::AbstractEntity("Repository_Sync", ""),

		target("target", Field::Type::Integer, false),
		timestamp("timestamp", Field::Type::Text, false),
		acestamp("acestamp", Field::Type::Text, false)
	{		
		addField(target);
		addField(timestamp);
		addField(acestamp);

		setPrimaryKey({target, timestamp, acestamp});
	}

	void Sync::onSelect()
	{

	}

	void Sync::onCreate()
	{

	}

	void Sync::onForeign()
	{

	}
} } } }
