/**
 @file BitflagPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "BitflagPredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	BitflagPredicate::BitflagPredicate(const Field& field, qint64 mask, bool negation)
	:
		_field(retain(field)),
		_mask(mask),
		_negation(negation)
	{

	}

	QString BitflagPredicate::makeSQL() const
	{
		return _field.getFullAlias() + " & " + QString::number(_mask) + (!_negation ? " <> " : " = ") + QString::number(_mask);
	}
} } } }
