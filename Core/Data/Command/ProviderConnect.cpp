#include "ProviderConnect.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	ProviderConnect::ProviderConnect()
	:
		AbstractCommand()
	{

	}

	ProviderConnect::ProviderConnect(const QString& file_path, const QString& connection_name)
	:
		AbstractCommand(),

		_file_path(file_path),
		_connection_name(connection_name)
	{

	}

	ProviderConnect::ProviderConnect(const ProviderConnect& origin)
	:
		AbstractCommand(origin)
	{

	}

	ProviderConnect::ProviderConnect(ProviderConnect&& origin)
	:
		AbstractCommand(origin)
	{

	}

	ProviderConnect::~ProviderConnect()
	{

	}

	const QString& ProviderConnect::getFilePath() const
	{
		return _file_path;
	}

	const QString& ProviderConnect::getConnectionName() const
	{
		return _connection_name;
	}
} } } }
