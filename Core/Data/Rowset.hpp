#ifndef Cadabra_Queen_Core_Data_Rowset_hpp
#define Cadabra_Queen_Core_Data_Rowset_hpp

#include "Provider.hpp"
#include "Field.hpp"
#include "QueryStream.hpp"
#include "AbstractPredicate.hpp"

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QSqlError>
#include <QList>
#include <QSet>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class Rowset : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const int FetchSize;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Mutation : char
		{
			Insert = 'I',
			Remove = 'R',
			Update = 'U',
			Append = 'a',
			Prepend = 'p',
			Suspend = 's',
			Patch = 'u',
			Ignore = 'r'
		};
	/** @} */

	/** @name Aliasses */
	/** @{ */
	public:
		using RoleTable = QHash<int, QByteArray>;
		using NameTable = QHash<QString, int>;
		using HintTable = QMultiMap<Mutation, QSqlRecord>;
		using MutationList = QList<QPair<Mutation, QSqlRecord>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Rowset(Provider* parent = nullptr);
		virtual ~Rowset() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QSqlDatabase _database;

		QString _name;
		QString _user_filter;

		QStringList _sort_list;
		QSqlIndex _custom_index;
		bool _reverse;
		qsizetype _limit;
		QSqlQuery _query;
		int _offset;
		bool _eof;

		QVector<QSqlRecord> _appendix_vector;
		QSet<QSqlRecord> _patch_set;
		QSet<QSqlRecord> _ignore_set;

	protected:
		mutable QSqlError _error;
		QString _system_filter;
		QSqlRecord _record;
		QSqlIndex _index;
		RoleTable _role_table;
		NameTable _name_table;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		bool initialize(const QueryStream& q);

	public:
		virtual bool initialize();

		virtual bool reopen(const HintTable& hint_table = {});

		virtual bool open();
		virtual void close();
		virtual void finalize();

		virtual bool traverse(bool all = false);
		virtual void traverseMore();
		virtual void traverseAll();
		virtual void traverseOffset(int value = Rowset::FetchSize);

		virtual bool seek(int row_index);
		virtual QSqlRecord seekRecord(int row_index);
		virtual QVariantMap seekKey(int row_index);
		virtual QSqlIndex seekIndex(int row_index);
		virtual QVariant seekData(int row_index, int column_index);
		virtual QSqlField seekField(int row_index, int column_index);

		virtual bool mutate(const MutationList& mutation_list);

		virtual bool update(const QSqlRecord& record, int row_index);
		virtual bool update(const QSqlRecord& record);
		virtual bool insert(const QSqlRecord& record);
		virtual bool remove(int row_index);
		virtual bool remove(const QSqlRecord& record);

		virtual bool patch(const QSqlRecord& record);
		virtual bool ignore(const QSqlRecord& record);
		virtual bool append(const QSqlRecord& record);
		virtual bool prepend(const QSqlRecord& record);
		virtual bool suspend(const QSqlRecord& record);

	protected:
		virtual bool mutate(const MutationList& mutation_list, QSet<int>& inserted_index_set, QSet<int>& removed_index_set, QSet<int>& updated_index_set);
		virtual bool mutate(Mutation mutation, const QSqlRecord& record, QSet<int>& transformation_set);

		virtual bool update(const QSqlRecord& record, int row_index, QSet<int>& transformation_set);
		virtual bool update(const QSqlRecord& record, QSet<int>& transformation_set);
		virtual bool insert(const QSqlRecord& record, QSet<int>& transformation_set);
		virtual bool remove(int row_index, QSet<int>& transformation_set);
		virtual bool remove(const QSqlRecord& record, QSet<int>& transformation_set);

		virtual bool patch(const QSqlRecord& record, QSet<int>& transformation_set);
		virtual bool ignore(const QSqlRecord& record, QSet<int>& transformation_set);
		virtual bool append(const QSqlRecord& record, QSet<int>& transformation_set);
		virtual bool prepend(const QSqlRecord& record, QSet<int>& transformation_set);
		virtual bool suspend(const QSqlRecord& record, QSet<int>& transformation_set);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		virtual void setName(const QString& name);
		virtual void setIndex(const QSqlIndex& index);
		virtual void setFilter(const AbstractPredicate& predicate);
		virtual void setLimit(qsizetype limit);
		virtual void setReverse(bool onoff = true);
		virtual void addSort(const Field& field, Qt::SortOrder order);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QSqlQuery& getQuery() const;
		const QString& getFilter() const;
		const QStringList& getSortList() const;
		const qsizetype& getLimit() const;
		const bool& getReverse() const;
		const QSqlError& getError() const;

		const QSqlDatabase& getDatabase() const;
		const QString& getName() const;
		QString getEscapedName() const;
		const QSqlRecord& getRecord() const;
		const QSqlIndex& getIndex() const;
		const RoleTable& getRoleTable() const;
		const NameTable& getNameTable() const;

		const QVector<QSqlRecord>& getAppendix() const;
		const QSet<QSqlRecord>& getPatchSet() const;
		const QSet<QSqlRecord>& getIgnoreSet() const;

		int getWidth() const;
		virtual int getOffset() const;
		virtual int getLength() const;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QSqlDatabase makeDatabase() const;
		QSqlRecord makeRecord() const;
		QStringList makeFilterList(const QSqlRecord& record, const Field::Condition& condition) const;
		virtual QSqlIndex makeIndex() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void reset();
		void fetched(int begin_offset, int end_offset);
		void transformed(QSet<int> inserted_index_set, QSet<int> removed_index_set, QSet<int> updated_index_set, QMap<int, int> moved_index_map);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isLoaded() const;
		virtual bool isEmpty() const;
		virtual bool isEOF() const;
		virtual bool hasIndex(const QList<QSqlIndex>& index_list, bool all = false) const;
	/** @} */

	/** @name Friends */
	/** @{ */
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(QSharedPointer<Cadabra::Queen::Core::Data::Rowset>)

#endif
