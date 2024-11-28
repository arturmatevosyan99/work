#ifndef Cadabra_Queen_Core_Data_AbstractEntity_hpp
#define Cadabra_Queen_Core_Data_AbstractEntity_hpp

#include "Rowset.hpp"
#include "Rowset/StaticRowset.hpp"

#include "Field.hpp"
#include "ForeignKey.hpp"
#include "Predicate/CompoundPredicate.hpp"

#include <QScopedPointer>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QAbstractListModel>
#include <QSqlField>
#include <QSqlIndex>
#include <QString>
#include <QHash>
#include <QPair>
#include <QSet>
#include <QPair>
#include <QList>
#include <QCache>
#include <QEvent>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class Provider;

	class AbstractEntity : public QAbstractListModel
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(bool isValid READ isValid NOTIFY validated)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class CursorDirection
		{
			Backward = -1,
			Moveless = 0,
			Forward = 1
		};
	/** @} */

	/** @name Aliasses */
	/** @{ */
	public:
		using Field = Data::Field;
		using FieldMap = QMap<QString, std::reference_wrapper<Field>>;
		using FieldSet = QSet<std::reference_wrapper<Field>>;
		using ForeignList = QList<std::reference_wrapper<ForeignKey>>;
		using Constraint = QPair<QString, FieldSet>;
		using ConstraintList = QList<Constraint>;
		using OrderList = QList<QPair<std::reference_wrapper<const Field>, Qt::SortOrder>>;
		using RoleTable = Rowset::RoleTable;
		using NameTable = Rowset::NameTable;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QVariant FieldVariant(const Field::Type& value_type, const QMetaType::Type& meta_type, const QSqlField& sql_field);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractEntity(const QString& name, const QString& suffix = "");
		virtual ~AbstractEntity() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _name;
		const QString _sufix;
		const QString _alias;
		QString _user_alias;

		FieldMap _field_map;
		Constraint _primary_key;
		Constraint _primary_index;
		ConstraintList _unique_list;
		ForeignList _foreign_list;
		OrderList _order_list;
		CompoundPredicate _compound_predicate;

		int _row_index;
		qsizetype _row_limit;
		QSharedPointer<Rowset> _row_set;
		bool _valid;
		bool _auto_fetch;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool open(Provider* provider, bool fetch_all = true);
		virtual bool open(Provider* provider, QFlags<Provider::OpenMode> open_mode, Provider::RowsetType rowset_type = Provider::RowsetType::Regular);
		virtual bool open(QSharedPointer<Rowset> row_set);
		virtual bool reopen(const Rowset::HintTable& hint_table = {});
		virtual bool close();
		virtual bool previous();
		virtual bool first();
		virtual bool last();
		virtual bool next();
		virtual bool move(CursorDirection direction);
		virtual bool relocate();
		virtual bool locate(int row_index);
		QSqlRecord record(std::nullptr_t);
		QSqlRecord record(bool default_generated);
		QSqlRecord record(int row_index);
		virtual QSqlRecord record();
		virtual void fetchMore(const QModelIndex& index) override;

		virtual bool mutate(const Rowset::MutationList& list);
		virtual bool insert(const QSqlRecord& record);
		virtual bool update(const QSqlRecord& record);
		virtual bool remove(const QSqlRecord& record);
		virtual bool patch(const QSqlRecord& record);
		virtual bool ignore(const QSqlRecord& record);
		virtual bool append(const QSqlRecord& record);
		virtual bool prepend(const QSqlRecord& record);
		virtual bool suspend(const QSqlRecord& record);

	private:
		void validate(bool onoff);
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
		void setUserAlias(const QString& alias);
		void setPrimaryKey(FieldSet&& set, const QString& alias = "");
		void setPrimaryIndex(FieldSet&& set, const QString& alias = "");
		bool addField(Field& field);
		bool addField(Field& field, Qt::SortOrder sort);
		void addUniqueKey(FieldSet&& set, const QString& alias = "");
		void addForeignKey(ForeignKey& key);
		void permutateRows(int row1_index, int row2_index);

	public:
		virtual void resetCompoundPredicate(CompoundPredicate::Logic logic = CompoundPredicate::Logic::And);
		virtual void addPredicate(QSharedPointer<WherePredicate> predicate);
		template<typename P, typename ...PArgs>
		typename std::enable_if<std::is_convertible_v<P*, WherePredicate*>, void>::type emplacePredicate(PArgs&& ...args)
		{
			_compound_predicate.append(QSharedPointer<P>::create(std::forward<PArgs>(args)...));
		}
		virtual void resetOrder();
		virtual bool addOrder(const Field& field, Qt::SortOrder sort);
		virtual void setRowLimit(qsizetype limit);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual const QString& getName() const;
		virtual const QString& getSuffix() const;
		virtual const QString& getAlias() const;
		virtual const QString& getUserAlias(bool failover = true) const;
		virtual const int& getRowIndex() const;
		virtual const RoleTable& getRoleTable() const;
		virtual const NameTable& getNameTable() const;
		virtual const FieldMap& getFieldMap() const;
		virtual const Constraint& getPrimaryKey() const;
		virtual const ConstraintList& getUniqueKeyList() const;
		virtual const ForeignList& getForeignKeyList() const;
		virtual const CompoundPredicate& getCompoundPredicate() const;
		virtual QVariant get(const QModelIndex& index, int role) const;

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual int rowIndex() const;
		virtual QVariantMap key(const QModelIndex& index) const;
		virtual QVariant data(const QModelIndex &index, int role) const override;
		Q_INVOKABLE QVariant data(const QModelIndex &index, const QString& role) const;
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
		Q_INVOKABLE QVariantMap roleKeys() const;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QSqlRecord makeRowsetRecord() const;
		QSqlIndex makeRowsetIndex() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void validated(bool onoff);
		void rowsetOpened(int row_count);
		void rowsetTransformed(QSet<int> inserted_index_set, QSet<int> removed_index_set, QSet<int> updated_index_set, QMap<int, int> moved_index_map);
		void rowsetFetched(QSharedPointer<Cadabra::Queen::Core::Data::Rowset> row_set, int begin_offset, int end_offset);
		void keyMapped(QModelIndex index, QVariantMap key_map);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onRowsetTransformed(QSet<int> inserted_index_set, QSet<int> removed_index_set, QSet<int> updated_index_set, QMap<int, int> moved_index_map);
		void onRowsetFetched(int begin_offset, int end_offset);
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
		virtual bool event(QEvent* event) override;
		virtual void customEvent(QEvent* event) override;
		virtual void onSelect() = 0;
		virtual void onCreate();
		virtual void onForeign();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasRowset() const;
		virtual bool hasPrimaryKey() const;
		virtual bool hasUserAlias() const;
		virtual bool isOpened() const;
		virtual bool isEmpty() const;
		/**
		 * @brief is End of Fetch
		 * @return
		 */
		virtual bool isEOF() const;

		/**
		 * @brief is End of Set
		 * @return
		 */
		virtual bool isEOS() const;

		virtual bool isBOF() const;
		virtual bool isBOS() const;

		bool isValid() const;

		virtual bool hasIndex(const QSqlIndex& index) const;
		virtual bool hasAnyIndex(const QList<QSqlIndex>& index_list) const;
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(QSharedPointer<Cadabra::Queen::Core::Data::AbstractEntity>)

#endif
