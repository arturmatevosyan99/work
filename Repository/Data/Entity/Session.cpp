#include "Session.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Session::Session()
	:
		Core::Data::AbstractEntity("Repository_Session", ""),

		handle("handle", Field::Type::Text, false),
		timestamp("timestamp", Field::Type::Timestamp, false),
		object_id("object_id", Field::Type::Integer, false),
		application_scheme("application_scheme", Field::Type::Integer, false),
		application_handle("application_handle", Field::Type::Text, false)
	{
		addField(timestamp);
		addField(handle);
		addField(object_id);
		addField(application_scheme);
		addField(application_handle);

//		setPrimaryKey({});
		setPrimaryIndex({handle, object_id, timestamp});
	}

	void Session::onSelect()
	{

	}

	void Session::onCreate()
	{

	}

	void Session::onForeign()
	{

	}
} } } }
