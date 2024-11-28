/**
 @file JoinPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "JoinPredicate.hpp"

#include "../AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	const QMap<JoinPredicate::Intersection, QString> JoinPredicate::Map = {
		{JoinPredicate::Intersection::Inner, "INNER"},
		{JoinPredicate::Intersection::Left, "LEFT OUTER"},
		{JoinPredicate::Intersection::Right, "RIGHT OUTER"},
		{JoinPredicate::Intersection::Full, "FULL OUTER"}
	};
	
	JoinPredicate::JoinPredicate(const QSharedPointer<AbstractEntity> entity, CompoundPredicate compound, JoinPredicate::Intersection intersection)
	:
		_entity(entity),
		_compound(compound),
		_intersection(intersection)
	{
		
		
	}
	
	QString JoinPredicate::makeSQL() const
	{
		return (" " + JoinPredicate::Map.value(_intersection) + " JOIN " + _entity->getAlias() + (_entity->hasUserAlias() ? " " + _entity->getUserAlias() : "") + " ON " + _compound);
	}
	
	const QSharedPointer<AbstractEntity>& JoinPredicate::getEntity() const
	{
		return _entity;
	}
} } } }
