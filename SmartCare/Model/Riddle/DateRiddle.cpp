#include "DateRiddle.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	DateRiddle::DateRiddle(const QString& text, const QString& descriptor, Validator* validator)
	:
		TemplatedRiddle<Qaos::Date>(text, descriptor, validator)
	{

	}

	QString DateRiddle::getType() const
	{
		return QString("Date");
	}
} } } }
