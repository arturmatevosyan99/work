#include "IntegerRiddle.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	IntegerRiddle::IntegerRiddle(const QString& text, const QString& descriptor, Validator* validator)
	:
		TemplatedRiddle<int>(text, descriptor, validator)
	{

	}

	QString IntegerRiddle::getType() const
	{
		return QString("Integer");
	}
} } } }
