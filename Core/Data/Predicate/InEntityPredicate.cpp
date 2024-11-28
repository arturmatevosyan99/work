/**
 @file InEntityPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Ace
 @author Daniil A Megrabyan
*/

#include "InEntityPredicate.hpp"

#include "../QueryStream.hpp"

#include <QTextStream>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	InEntityPredicate::InEntityPredicate(const Field& left, const Field& right)
	:
		_left(retain(left)),
		_right(retain(right))
	{


	}

	QString InEntityPredicate::makeSQL() const
	{
		QString string;
		QTextStream stream(&string);
		stream << _left.getFullAlias() << " IN (SELECT " << _right.getFullAlias();

		if (!_right.isStandalone() && !_right.getEntity()->getCompoundPredicate().isEmpty()) {
			stream << " FROM " << _right.getEntityAlias() << " WHERE " << _right.getEntity()->getCompoundPredicate();
		}

		stream << ")";
		return stream.readAll();
	}
} } } }
