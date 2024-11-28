/**
 @file LikePredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_LikePredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_LikePredicate_hpp

#include "WherePredicate.hpp"

#include "../AbstractEntity.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class LikePredicate : public WherePredicate
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
		LikePredicate(const Field& field, const QString& pattern, bool negation = false);
		virtual ~LikePredicate() override = default;
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
		const QString _pattern;
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
