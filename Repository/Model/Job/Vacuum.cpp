#include "Vacuum.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Vacuum::Vacuum(QObject* parent)
	:
		Repository::Model::AbstractJob(8660964310169681921, parent)
	{
	}

	Vacuum::~Vacuum()
	{
	}

	QJsonObject Vacuum::makeEnvironment() const
	{
		return {};
	}
} } } }
