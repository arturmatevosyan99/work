/**
 @file JoinPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Ace
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Ace_Core_Data_Predicate_JoinPredicate_hpp
#define Cadabra_Ace_Core_Data_Predicate_JoinPredicate_hpp

#include "WherePredicate.hpp"

#include "CompoundPredicate.hpp"
#include "../AbstractEntity.hpp"

#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <initializer_list>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class JoinPredicate : public WherePredicate
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Intersection
		{
			Inner,
			Left,
			Right,
			Full
		};
	/** @} */

	/** @name Contstants */
	/** @{ */
	public:
		static const QMap<JoinPredicate::Intersection, QString> Map;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		JoinPredicate(QSharedPointer<AbstractEntity> entity, CompoundPredicate compound, JoinPredicate::Intersection intersection = JoinPredicate::Intersection::Inner);
		virtual ~JoinPredicate() override = default;
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
		const QSharedPointer<AbstractEntity> _entity;
		const CompoundPredicate _compound;
		const JoinPredicate::Intersection _intersection;
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
		const QSharedPointer<AbstractEntity>& getEntity() const;
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
