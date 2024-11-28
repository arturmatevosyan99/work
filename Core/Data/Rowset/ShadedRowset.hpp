#ifndef Cadabra_Queen_Core_Data_Rowset_ShadedRowset_hpp
#define Cadabra_Queen_Core_Data_Rowset_ShadedRowset_hpp

#include "../Rowset.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class ShadedRowset : public Rowset
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Static */
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
		ShadedRowset(Provider* parent = nullptr);
		virtual ~ShadedRowset() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Rowset _source_rowset;
		QString _source_query;
		int _row_count;
		bool* _ab;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool initialize() override;
		virtual bool reopen(const Rowset::HintTable& hint_table = {}) override;
		virtual bool traverse(bool all = false) override;
		virtual void close() override;

		virtual bool update(const QSqlRecord& record, int row_index) override;
		virtual bool insert(const QSqlRecord& record) override;
		virtual bool remove(int row_index) override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QSqlIndex makeIndex() const override;
		QString makeShadeName(bool ab) const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		virtual void setName(const QString& table_name) override;
		virtual void setFilter(const AbstractPredicate& predicate) override;
		virtual void setLimit(qsizetype limit) override;
		virtual void addSort(const Field& field, Qt::SortOrder order) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
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
	/** @} */
	};
} } } }

#endif
