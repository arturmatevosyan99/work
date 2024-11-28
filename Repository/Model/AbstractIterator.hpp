#ifndef Cadabra_Queen_Repository_Model_AbstractIterator_hpp
#define Cadabra_Queen_Repository_Model_AbstractIterator_hpp

#include "AbstractMap.hpp"

#include "../Meta/Essence.hpp"
#include "../Meta/EssenceToken.hpp"
#include "../Meta/EssenceExpression.hpp"
#include "../Meta/FilterDelegate.hpp"

#include <QObject>
#include <QSharedPointer>
#include <QPointer>
#include <QList>
#include <QMap>
#include <QPair>
#include <QByteArray>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {	
	/**
	 * @brief The AbstractIterator class
	 * Filter Iterator for Identity Map(AbstractMap)
	 */
	class AbstractIterator : public QSortFilterProxyModel
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using FilterCondition = QPair<Meta::EssenceExpression::OperatorType, QVariant>;
		using FilterRoleMap = QMap<int, QList<FilterCondition>>;
		using FilterConditionList = QList<FilterCondition>;
		using SortRoleList = QList<QPair<int, Qt::SortOrder>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractIterator(AbstractMap* source_model_map);
		virtual ~AbstractIterator();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		AbstractMap* _source_model_map;

		FilterRoleMap _filter_role_map;
		FilterConditionList _filter_tag_list;
		FilterConditionList _filter_any_list;
		SortRoleList _sort_list;

	protected:
		Meta::FilterDelegate _filter_delegate;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool filter(const Meta::EssenceExpression& expression);
		virtual bool filter(const QVector<Meta::EssenceToken>& token_vector);
		virtual bool order(int role, Qt::SortOrder order);
		virtual bool order(const QVector<int>& sort_vector, bool reverse);
		void clear();
		bool validate();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
		virtual void setSourceModel(QAbstractItemModel* source_model) override;
		bool addRoleFilter(int role, const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);

	public:
		bool addIdFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
		bool addTagFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
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
		virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
		virtual bool filterAcceptsRow(const FilterCondition& filter, int source_row, const QModelIndex& source_parent) const;
		virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;
	/** @} */
	};
} } } }

#endif
