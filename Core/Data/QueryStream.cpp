#include "QueryStream.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	QueryStream::QueryStream()
	:
		QTextStream(&_query_string)
	{
	}

	QueryStream::QueryStream(const QString& query)
	:
		QTextStream(&_query_string)
	{
		operator<<(query);
	}

	QueryStream::operator const QString&() const
	{
		return _query_string;
	}

	void QueryStream::clear()
	{
		_query_string.clear();
		QTextStream::reset();
	}

	bool QueryStream::append(const QString& text)
	{
		QTextStream::operator<<(text);
		return status() != Status::WriteFailed;
	}

	bool QueryStream::isEmpty() const
	{
		return _query_string.isEmpty();
	}
} } } }
