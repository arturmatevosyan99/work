/**
 @file InPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_InPredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_InPredicate_hpp

#include "WherePredicate.hpp"

#include "../AbstractEntity.hpp"

#include <QList>
#include <QVariant>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class InPredicate : public WherePredicate
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
		InPredicate(const Field& source_field);
		InPredicate(const Field& source_field, const std::initializer_list<QVariant>& value_list);
		InPredicate(const Field& source_field, const QVariantList& value_list);
		virtual ~InPredicate() override = default;
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
		const Field& _source_field;
		QList<const Field*> _field_list;
		QList<QVariant> _value_list;
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	public:
		void emplace(const Field& field);
		void emplace(QVariant&& value);
		void emplace(const QVariant& value);
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
