/**
 @file BitflagPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_BitflagPredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_BitflagPredicate_hpp

#include "WherePredicate.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class BitflagPredicate : public WherePredicate
	{
	/** @name Constructors */
	/** @{ */
	public:
	/** @} */

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
		BitflagPredicate(const Field& field, qint64 mask, bool negation = false);
		virtual ~BitflagPredicate() override = default;
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
		const qint64 _mask;
		const bool _negation;
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
