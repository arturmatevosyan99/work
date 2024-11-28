/**
 @file RoundExpression.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "RoundExpression.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	RoundExpression::RoundExpression(qsizetype precision)
	:
		_precision(precision)
	{

	}

	QString RoundExpression::makeSQL() const
	{
		return QString("ROUND(%1, %2)").arg(hasTarget() ? getTarget().getFullAlias() : "0.0000000").arg(_precision);
	}
} } } }
