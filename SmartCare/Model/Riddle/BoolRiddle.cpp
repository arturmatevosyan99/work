#include "BoolRiddle.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	BoolRiddle::BoolRiddle(const QString& text, const QString& descriptor, Validator* validator)
	:
		TemplatedRiddle<bool>(text, descriptor, validator)
	{

	}

	QString BoolRiddle::getType() const
	{
		return QString("Bool");
	}
} } } }
