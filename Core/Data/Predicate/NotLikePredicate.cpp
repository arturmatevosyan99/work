/**
 @file NotLikePredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "NotLikePredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	NotLikePredicate::NotLikePredicate(const Field& field, const QString& pattern)
	:
		LikePredicate(retain(field), pattern, true)
	{

	}
} } } }
