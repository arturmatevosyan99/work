/**
 @file AbstractExpression.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "AbstractExpression.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	const Field& AbstractExpression::retain(const Field& field)
	{
		if (!field.isStandalone()) {
			return field;
		}
		_standalone_list.push_back(field);
		return _standalone_list.back();
	}

	AbstractExpression::operator QString() const
	{
		return makeSQL();
	}
} } } }
