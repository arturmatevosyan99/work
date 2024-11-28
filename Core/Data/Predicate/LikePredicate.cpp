/**
 @file LikePredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "LikePredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	LikePredicate::LikePredicate(const Field& field, const QString& pattern, bool negation)
	:
		_field(retain(field)),
		_pattern(pattern),
		_negation(negation)
	{

	}

	QString LikePredicate::makeSQL() const
	{
		return _field.getFullAlias() + " " + (_negation ? "NOT LIKE" : "LIKE") +  " \'" + _pattern + "\'";
	}
} } } }
