/**
 @file AbstractExpression.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_AbstractExpression_hpp
#define Cadabra_Queen_Core_Data_AbstractExpression_hpp

#include "Field.hpp"

#include <QString>
#include <QList>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class AbstractExpression
	{
	/** @name Constants */
	/** @{ */
	public:
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		AbstractExpression() = default;
		virtual ~AbstractExpression() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QList<Field> _standalone_list;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	protected:
		const Field& retain(const Field& field);
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QString makeSQL() const = 0;
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
		operator QString() const;
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
