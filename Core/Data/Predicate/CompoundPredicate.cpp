/**
 @file CompoundPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "CompoundPredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	const QMap<CompoundPredicate::Logic, QString> CompoundPredicate::Map = {
		{CompoundPredicate::Logic::Or, "OR"},
		{CompoundPredicate::Logic::And, "AND"},
		{CompoundPredicate::Logic::Not, "NOT"}
	};

	CompoundPredicate::CompoundPredicate(CompoundPredicate::Logic logic)
	:
		_logic(logic)
	{

	}
	
	CompoundPredicate::CompoundPredicate(const QSharedPointer<const WherePredicate>& predicate)
	:
		_logic(CompoundPredicate::Logic::And)
	{
		_list.append(predicate);
	}	

	CompoundPredicate::CompoundPredicate(QList<QSharedPointer<const WherePredicate>> list, CompoundPredicate::Logic logic)
	:
		_list(list),
		_logic(logic)
	{

	}

	CompoundPredicate::CompoundPredicate(std::initializer_list<QSharedPointer<const WherePredicate>> list, CompoundPredicate::Logic logic)
	:
		_list(list),
		_logic(logic)
	{

	}
	
	CompoundPredicate::CompoundPredicate()
	:
		_logic(CompoundPredicate::Logic::And)
	{
		
	}
	
	QString CompoundPredicate::makeSQL() const
	{
		QString retval;
		QList<QSharedPointer<const WherePredicate>>::const_iterator last_predicate(--_list.constEnd());
		
		for (QList<QSharedPointer<const WherePredicate>>::const_iterator p = _list.constBegin(); p != _list.constEnd(); p++) {
			retval += "(" + (*p)->makeSQL() + ") " + ((last_predicate == p) ? "" : CompoundPredicate::Map.value(_logic) + " ");
		}
		
		return retval;
	}

	void CompoundPredicate::clear()
	{
		_list.clear();
	}

	void CompoundPredicate::append(const QSharedPointer<const WherePredicate>& predicate)
	{
		_list.push_back(predicate);
	}

	void CompoundPredicate::setLogic(CompoundPredicate::Logic logic)
	{
		_logic = logic;
	}

	bool CompoundPredicate::isEmpty() const
	{
		return _list.empty();
	}
} } } }
