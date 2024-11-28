/**
 @file BetweenPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_BetweenPredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_BetweenPredicate_hpp

#include "WherePredicate.hpp"

#include "../AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class BetweenPredicate : public WherePredicate
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Contstants */
	/** @{ */
	public:
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		BetweenPredicate(const Field& field, const Field& min, const Field& max);
		virtual ~BetweenPredicate() override = default;
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
		const Field& _field;
		const Field& _min;
		const Field& _max;
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
