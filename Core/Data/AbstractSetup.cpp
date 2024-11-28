#include "AbstractSetup.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	AbstractSetup::AbstractSetup(const QString& extension_name, qulonglong extension_revision, const QMap<QString, qulonglong>& dependency_map)
	:
		Queen::Setup(extension_name, extension_revision, dependency_map)
	{

	}

	AbstractSetup::~AbstractSetup()
	{

	}

	bool AbstractSetup::go(Core::Data::Provider& data_provider) const
	{
		return Setup::go();
	}
} } } }
