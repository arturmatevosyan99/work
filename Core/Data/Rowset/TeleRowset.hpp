#ifndef Cadabra_Queen_Core_Data_Rowset_TeleRowset_hpp
#define Cadabra_Queen_Core_Data_Rowset_TeleRowset_hpp

#include "../Rowset.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	/**
	 * @brief The TeleRowset class
	 * @note The word television comes from Ancient Greek τῆλε (tele) 'far'
	 * @note Поддерживает только прирост в начало или конец от нулевого километра
	 */
	class TeleRowset : public Rowset
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class End : char
		{
			Tail = '<',
			Head = '>'
		};
	/** @} */

	/** @name Aliasses */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TeleRowset(Provider* parent = nullptr);
		virtual ~TeleRowset() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		int _head_length;
		int _tail_length;
		QSqlRecord _zero_record;
		QStringList _zero_filter;
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

	private:
		void count(QMap<End, qsizetype>& count_map);
	/** @} */

	/** @name Factories */
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
