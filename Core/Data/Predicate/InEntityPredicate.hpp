/**
 @file InEntityPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Ace
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_InEntityPredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_InEntityPredicate_hpp

#include "WherePredicate.hpp"

#include "../AbstractEntity.hpp"
#include "../Field.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class InEntityPredicate : public WherePredicate
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
		InEntityPredicate(const Field& left, const Field& right);
		virtual ~InEntityPredicate() override = default;
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
