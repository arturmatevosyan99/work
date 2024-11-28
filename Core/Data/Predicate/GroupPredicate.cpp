/**
 @file GroupPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "GroupPredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	GroupPredicate::GroupPredicate(const Field& field)
	:
		_field(retain(field))
	{
		
		
	}

	QString GroupPredicate::makeSQL() const
	{
		return _field.getFullAlias();
	}
	
	const Field& GroupPredicate::getField() const
	{
		return _field;
	}
} } } }
