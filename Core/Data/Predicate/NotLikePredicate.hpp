/**
 @file NotLikePredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_NotLikePredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_NotLikePredicate_hpp

#include "LikePredicate.hpp"

#include "../AbstractEntity.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class NotLikePredicate : public LikePredicate
	{
	/** @name Constructors */
	/** @{ */
	public:
		NotLikePredicate(const Field& field, const QString& pattern);
		virtual ~NotLikePredicate() override = default;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
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
