/**
 @file FieldExpression.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "FieldExpression.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	FieldExpression::FieldExpression()
	:
		_target(nullptr)
	{
	}

	void FieldExpression::setTarget(Field* target)
	{
		_target = target;
	}

	Field& FieldExpression::getTarget() const
	{
		return *_target;
	}

	bool FieldExpression::hasTarget() const
	{
		return nullptr != _target;
	}
} } } }
