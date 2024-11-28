/**
 @file ConditionPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "ConditionPredicate.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	ConditionPredicate::ConditionPredicate(const Field& left, const Field& right, ConditionPredicate::Condition condition)
	:
		_left(retain(left)),
		_right(retain(right)),
		_condition(condition)
	{
		
		
	}
	
	QString ConditionPredicate::makeSQL() const
	{
		return _left.express() + " " + Field::ConditionMap.value(_condition) + " " + _right.express();
	}
} } } }
