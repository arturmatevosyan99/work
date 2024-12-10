#include "mg_BoolRiddle.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
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
