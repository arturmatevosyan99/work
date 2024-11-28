#include "Teleport.hpp"

#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Teleport::Teleport(const QString& description)
	:
		Action(description)
	{

	}

	Teleport::~Teleport()
	{

	}

	void Teleport::run()
	{
		qDebug() << "Teleport!";
	}
} } } }
