#ifndef Cadabra_Queen_Repository_Model_Iterator_ObjectIterator_hpp
#define Cadabra_Queen_Repository_Model_Iterator_ObjectIterator_hpp

#include "../AbstractIterator.hpp"

#include "../Map/ObjectMap.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class ObjectIterator : public AbstractIterator
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectIterator(ObjectMap* parent = nullptr);
		virtual ~ObjectIterator() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		bool addApplicationFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type);
		bool addObjectFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type);
		bool addPathFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type);
		bool addParentIdFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool filterAcceptsRow(const FilterCondition& filter, int source_row, const QModelIndex& source_parent) const override;
	/** @} */
	};
} } } }

#endif
