/**
 @file OrderPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "OrderPredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	OrderPredicate::OrderPredicate(const Field& field, OrderPredicate::Direction direction)
	:
		_field(retain(field)),
		_direction(direction)
	{
		
		
	}
	
	QString OrderPredicate::makeSQL() const
	{
		return _field.getFullAlias() + " " + (_direction == OrderPredicate::Direction::Ascend ? "ASC" : "DESC");
	}
	
	const Field& OrderPredicate::getField() const
	{
		return _field;
	}
	
	OrderPredicate::Direction OrderPredicate::getDirection() const
	{
		return _direction;
	}
} } } }
