/**
 @file ConditionPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_ConditionPredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_ConditionPredicate_hpp

#include "WherePredicate.hpp"

#include "../AbstractEntity.hpp"

#include <QList>
#include <QString>
#include <QMap>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class ConditionPredicate : public WherePredicate
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		using Condition = Field::Condition;
	/** @} */

	/** @name Contstants */
	/** @{ */
	public:
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		ConditionPredicate(const Field& left, const Field& right, ConditionPredicate::Condition condition = ConditionPredicate::Condition::Equal);
		virtual ~ConditionPredicate() override = default;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
		virtual QString makeSQL() const override;
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
		const Field& _left;
		const Field& _right;
		const ConditionPredicate::Condition _condition;
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */
		
	/** @name Converters */
	/** @{ */
	public:
	/** @} */
	
	/** @name Setters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Formatters */
	/** @{ */
	public:
	/** @} */
		
	/** @name States */
	/** @{ */
	public:
	/** @} */
		
	/** @name Hooks */
	/** @{ */
	public:
	/** @} */
		
	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
