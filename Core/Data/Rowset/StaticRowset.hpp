#ifndef Cadabra_Queen_Core_Data_Rowset_StaticRowset_hpp
#define Cadabra_Queen_Core_Data_Rowset_StaticRowset_hpp

#include "../Rowset.hpp"

#include <QVector>
#include <QSqlRecord>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class StaticRowset : public Rowset
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliasses */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		StaticRowset(const QSqlIndex& index, const QSqlRecord& record, int cache_size = 0);
		virtual ~StaticRowset() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QVector<QSqlRecord> _vector;
		QVector<QSqlRecord>::ConstIterator _v;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool initialize() override;

		virtual void close() override;

		virtual bool traverse(bool all = false) override;
		virtual void traverseMore() override;
		virtual void traverseAll() override;
		virtual void traverseOffset(int value = Rowset::FetchSize) override;

		virtual bool seek(int row_index) override;
		virtual QSqlRecord seekRecord(int row_index) override;
		virtual QSqlField seekField(int row_index, int column_index) override;
		virtual QVariant seekData(int row_index, int column_index) override;

		virtual bool update(const QSqlRecord& record, int row_index) override;
		virtual bool insert(const QSqlRecord& record) override;
		virtual bool remove(int row_index) override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		virtual void setFilter(const AbstractPredicate& predicate) override;
		virtual void setLimit(qsizetype limit) override;
		virtual void addSort(const Field& field, Qt::SortOrder order) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual int getOffset() const override;
		virtual int getLength() const override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool isEmpty() const override;
		virtual bool isEOF() const override;
		virtual bool hasIndex(const QList<QSqlIndex>& index_list, bool all = false) const override;
	/** @} */
	};
} } } }

#endif
