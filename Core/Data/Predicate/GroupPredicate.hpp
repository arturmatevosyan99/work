/**
 @file GroupPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_GroupPredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_GroupPredicate_hpp

#include "../AbstractPredicate.hpp"

#include "../AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class GroupPredicate : public AbstractPredicate
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Direction
		{
			Ascend,
			Descend
		};
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		GroupPredicate(const Field& field);
		virtual ~GroupPredicate() override = default;
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
		const Field& getField() const;
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
