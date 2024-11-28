/**
 @file AbstractPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "AbstractPredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	const Field& AbstractPredicate::retain(const Field& field)
	{
		if (!field.isStandalone()) {
			return field;
		}
		_standalone_list.push_back(field);
		return _standalone_list.back();
	}

	AbstractPredicate::operator QString() const
	{
		return makeSQL();
	}
} } } }
