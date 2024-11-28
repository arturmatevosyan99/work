/**
 @file FieldExpression.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Expression_FieldExpression_hpp
#define Cadabra_Queen_Core_Data_Expression_FieldExpression_hpp

#include "../AbstractExpression.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class FieldExpression : public AbstractExpression
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		FieldExpression();
		virtual ~FieldExpression() = default;
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
		Field* _target;
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
		void setTarget(Field* field);
	/** @} */
		
	/** @name Getters */
	/** @{ */
	protected:
		Field& getTarget() const;
	/** @} */
		
	/** @name Formatters */
	/** @{ */
	public:
	/** @} */
		
	/** @name States */
	/** @{ */
	public:
		bool hasTarget() const;
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
