#ifndef Cadabra_Queen_Core_Data_Rowset_VersionedRowset_hpp
#define Cadabra_Queen_Core_Data_Rowset_VersionedRowset_hpp

#include "../Rowset.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	/**
	 * @brief The VersionedRowset class
	 * Замудка в создании версионного набора данных(слепков)
	 * с одного и того же источника
	 * с учетом одних и те же ограничений
	 * и одинаковых правилах сортировки
	 * @status Не законеченно из-за невозможности
	 * удалять временные таблицы при наличии открытых запросов даже
	 * к любым другим таблицам
	 * @todo Был включен WAL, поэтому нужна актулизация @status
	 * @todo Поддержка переключения между версиями
	 */
	class VersionedRowset : public Rowset
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
		VersionedRowset(Provider* parent = nullptr);
		virtual ~VersionedRowset() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Rowset _source_rowset;
		int _row_count;
		quint64 _revision;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool initialize() override;
		virtual bool reopen(const HintTable& hint_table = {}) override;
		virtual bool reopen(quint64 revision = 0);
		virtual bool traverse(bool all = false) override;
		virtual void close() override;

		virtual bool update(const QSqlRecord& record, int row_index) override;
		virtual bool insert(const QSqlRecord& record) override;
		virtual bool remove(int row_index) override;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
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
