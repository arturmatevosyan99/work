/**
 @file RoundExpression.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Expression_RoundExpression_hpp
#define Cadabra_Queen_Core_Data_Expression_RoundExpression_hpp

#include "FieldExpression.hpp"

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class RoundExpression : public FieldExpression
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		RoundExpression(qsizetype precision);
		virtual ~RoundExpression() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		qsizetype _precision;
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QString makeSQL() const override;
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
