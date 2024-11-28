#include "LogClient.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace System {
	LogClient::LogClient(LogFormat format, QObject* parent)
	:
		QObject(parent),

		_format(format)
	{

	}

	LogClient::~LogClient()
	{

	}

	bool LogClient::send(const QString& message)
	{
		if (LogFormat::Text == _format) {
			return write(message.toUtf8().append('\n'));
		} else if (LogFormat::JSON == _format) {
			return false;
		} else if (LogFormat::XML == _format) {
			return false;
		}
		return false;
	}
} } } }
