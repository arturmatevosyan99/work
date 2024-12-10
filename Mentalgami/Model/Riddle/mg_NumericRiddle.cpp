#include "mg_NumericRiddle.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	NumericRiddle::NumericRiddle(const QString& text, const QString& descriptor, Validator* validator)
	:
		TemplatedRiddle<double>(text, descriptor, validator)
	{

	}

	QString NumericRiddle::getType() const
	{
		return QString("Numeric");
	}
} } } }
