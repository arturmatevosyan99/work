/**
 @file BetweenPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "BetweenPredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	BetweenPredicate::BetweenPredicate(const Field& field, const Field& min, const Field& max)
	:
		_field(retain(field)),
		_min(retain(min)),
		_max(retain(max))
	{
		
		
	}

	QString BetweenPredicate::makeSQL() const
	{
		return _field.getFullAlias() + " BETWEEN " + (_min.isQuoted() ? ("\"" + _min.getPrint() + "\"") : _min.getPrint()) +  " AND " + (_max.isQuoted() ? ("\"" + _max.getPrint() + "\"") : _max.getPrint());
	}
} } } }
