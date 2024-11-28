#include "Oblivion.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Oblivion::Oblivion(QObject* parent)
	:
		Repository::Model::AbstractJob(8660964310169681922, parent)
	{
	}

	Oblivion::~Oblivion()
	{
	}

	QJsonObject Oblivion::makeEnvironment() const
	{
		return {};
	}
} } } }
