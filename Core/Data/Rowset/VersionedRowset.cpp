#include "VersionedRowset.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	VersionedRowset::VersionedRowset(Provider* parent)
	:
		Rowset(parent),

		_source_rowset(parent),
		_row_count(0),
		_revision(0)
	{

	}

	VersionedRowset::~VersionedRowset()
	{
		close();
	}

	bool VersionedRowset::initialize()
	{
		_row_count = 0;

		close();
		if (!_source_rowset.initialize()) {
			qWarning() << "Could not build source query: '" << _source_rowset.getQuery().lastQuery() << "'";
			_source_rowset.close();
			return false;
		}
		_source_rowset.close();

		QSqlQuery query(makeDatabase());
		if (!query.exec("CREATE TABLE " + getName() + " AS " + _source_rowset.getQuery().lastQuery())) {
			qWarning() << "Could not create temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
			return false;
		}
		return Rowset::initialize();
	}

	bool VersionedRowset::reopen(const Rowset::HintTable& hint_table)
	{
		Q_UNUSED(hint_table);
		return reopen(_revision);
	}

	bool VersionedRowset::reopen(quint64 revision)
	{
		if (!isLoaded()) {
			return false;
		}

		QSqlDatabase database(makeDatabase());
		QSqlQuery query(database);
		QString t2_name;

		if (revision > _revision) {
			return false;
		} else if (revision == _revision) {
			t2_name = QString("temp.%1_%2_%3").arg(_source_rowset.getName()).arg(reinterpret_cast<quintptr>(this)).arg(++_revision);
			/// @brief Новая времянка с теми же условиями и сортировкой
			if (!query.exec("CREATE TABLE " + t2_name + " AS " + _source_rowset.getQuery().lastQuery())) {
				qWarning() << "Could not create temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
				return false;
			}
			qDebug() << "Reopen: '" << query.lastQuery() << "'";
			query.finish();
		} else {
			t2_name = QString("temp.%1_%2_%3").arg(_source_rowset.getName()).arg(reinterpret_cast<quintptr>(this)).arg(_revision);
		}

		const QSqlIndex t_index(database.primaryIndex(_source_rowset.getName()));

		QSqlRecord t2_record(t_index);
		t2_record.insert(0, QSqlField("rowid", QVariant::LongLong));

		const QString t1_name(getName()); /// < Текущий слепок
		QSqlRecord t1_record(t_index);
		t1_record.insert(0, QSqlField("rowid", QVariant::LongLong));

		QStringList selection_field_list, saved_selection_field_list, t_join_predicate_list, saved_join_predicate_list, deleted_join_predicate_list, inserted_join_predicate_list;
		for (int i = 0; i < t_index.count(); ++i) {
			QString field_name(t_index.fieldName(i));
			if (!database.driver()->isIdentifierEscaped(field_name, QSqlDriver::FieldName)) {
				field_name = database.driver()->escapeIdentifier(field_name, QSqlDriver::FieldName);
			}
			t_join_predicate_list.append(QString("%1.%3 = %2.%3").arg(t1_name, t2_name, field_name));
			saved_join_predicate_list.append(QString("%1.%3 = %2.%3").arg("CTE_SAVED1", "CTE_SAVED2", field_name));
			deleted_join_predicate_list.append(QString("%1.%3 = %2.%3").arg("CTE_SAVED1", "CTE_DELETED_OFFSET", field_name));
			inserted_join_predicate_list.append(QString("%1.%3 = %2.%3").arg("CTE_SAVED1", "CTE_INSERTED_OFFSET", field_name));
			selection_field_list.append(field_name);
			saved_selection_field_list.append(QString("CTE_SAVED1.%1").arg(field_name));
		}

		QueryStream q;
		q	<< "WITH" << Qt::endl
		<< "CTE_DELETED AS (" << Qt::endl /// < Удаленные
			<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t1_name, t1_record, false) << Qt::endl
			<< "WHERE NOT EXISTS (" << Qt::endl
				<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t2_name, t2_record, false) << " WHERE " << t_join_predicate_list.join(" AND ") << Qt::endl
			<< ")" << Qt::endl
		<< ")," << Qt::endl

		<< "CTE_INSERTED AS (" << Qt::endl /// < Вставленные
			<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t2_name, t2_record, false) << Qt::endl
			<< "WHERE NOT EXISTS (" << Qt::endl
				<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t1_name, t1_record, false) << " WHERE " << t_join_predicate_list.join(" AND ") << Qt::endl
			<< ")" << Qt::endl
		<< ")," << Qt::endl

		<< "CTE_SAVED1 AS (" << Qt::endl /// < Оставшиеся (старые позиции)
			<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t1_name, t1_record, false) << Qt::endl
			<< "WHERE EXISTS (" << Qt::endl
				<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t2_name, t2_record, false) << " WHERE " << t_join_predicate_list.join(" AND ") << Qt::endl
			<< ")" << Qt::endl
		<< ")," << Qt::endl

		<< "CTE_SAVED2 AS (" << Qt::endl /// < Оставшиеся (новые позиции)
			<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t2_name, t2_record, false) << Qt::endl
			<< "WHERE exists (" << Qt::endl
				<< database.driver()->sqlStatement(QSqlDriver::SelectStatement, t1_name, t1_record, false) << " WHERE " << t_join_predicate_list.join(" AND ") << Qt::endl
			<< ")" << Qt::endl
		<< ")," << Qt::endl

		<< "CTE_DELETED_OFFSET AS (" << Qt::endl /// < Сдвиги от удалений для оставшихся на старых позициях
			<< "SELECT" << Qt::endl
				<< "rowid," << Qt::endl
				<< "(SUM (weight) OVER (ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)) AS offset," << Qt::endl
				<< selection_field_list.join(", ") << Qt::endl
			<< "FROM (" << Qt::endl
				<< "SELECT" << Qt::endl
					<< "rowid," << Qt::endl
					<< "0 AS weight," << Qt::endl
					<< selection_field_list.join(", ") << Qt::endl
				<< "FROM" << Qt::endl
					<< "CTE_SAVED1" << Qt::endl

				<< "UNION" << Qt::endl

				<< "SELECT" << Qt::endl
					<< "rowid," << Qt::endl
					<< "-1 AS weight," << Qt::endl
					<< selection_field_list.join(", ") << Qt::endl
				<< "FROM" << Qt::endl
					<< "CTE_DELETED" << Qt::endl
				<< "ORDER BY rowid" << Qt::endl
			<< ")" << Qt::endl
		<< ")," << Qt::endl

		<< "CTE_INSERTED_OFFSET AS (" << Qt::endl /// < Сдвиги от вставок относительно уже учтенных сдвиогов от удаления
			<< "SELECT" << Qt::endl
				<< "rowid," << Qt::endl
				<< "(SUM (weight) OVER (ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)) AS offset," << Qt::endl
				<< selection_field_list.join(", ") << Qt::endl
			<< "FROM (" << Qt::endl
				<< "SELECT" << Qt::endl
					<< "CTE_SAVED1.rowid + CTE_DELETED_OFFSET.offset AS rowid," << Qt::endl
					<< "0 AS weight," << Qt::endl
					<< saved_selection_field_list.join(", ") << Qt::endl
				<< "FROM CTE_SAVED1" << Qt::endl
				<< "LEFT OUTER JOIN CTE_DELETED_OFFSET ON " << deleted_join_predicate_list.join(" AND ") << Qt::endl

				<< "UNION" << Qt::endl

				<< "SELECT" << Qt::endl
					<< "rowid - 1 AS rowid," << Qt::endl
					<< "+1 AS weight," << Qt::endl
					<< selection_field_list.join(", ") << Qt::endl
				<< "FROM" << Qt::endl
					<< "CTE_INSERTED" << Qt::endl
				<< "ORDER BY rowid" << Qt::endl
			<< ")" << Qt::endl
		<< ")" << Qt::endl

		/// @brief Получаем набор перемещений
		<< "SELECT" << Qt::endl
			<< "CTE_SAVED1.rowid + CTE_DELETED_OFFSET.offset + CTE_INSERTED_OFFSET.offset AS rowid1," << Qt::endl
			<< "CTE_SAVED2.rowid AS rowid2" << Qt::endl
		<< "FROM CTE_SAVED1" << Qt::endl
			<< "LEFT OUTER JOIN CTE_INSERTED_OFFSET ON " << inserted_join_predicate_list.join(" AND ") << Qt::endl
			<< "LEFT OUTER JOIN CTE_DELETED_OFFSET ON " << deleted_join_predicate_list.join(" AND ") << Qt::endl
			<< "LEFT OUTER JOIN CTE_SAVED2 ON " << saved_join_predicate_list.join(" AND ") << Qt::endl
		<< "WHERE rowid1 <> rowid2" << Qt::endl

		<< "UNION" << Qt::endl

		/// @brief Получаем набор тех, кто остался на своих местах
		<< "SELECT" << Qt::endl
			<< "CTE_DELETED.rowid AS rowid1," << Qt::endl
			<< "NULL AS rowid2" << Qt::endl
		<< "FROM CTE_DELETED" << Qt::endl

		<< "UNION" << Qt::endl

		/// @brief Получаем набор тех, кто остался на своих местах
		<< "SELECT" << Qt::endl
			<< "NULL AS rowid1," << Qt::endl
			<< "CTE_INSERTED.rowid AS rowid2" << Qt::endl
		<< "FROM CTE_INSERTED" << Qt::endl
		;

		QElapsedTimer timer;
		timer.start();
		if (!query.exec(q)) {
			qWarning() << "Could not reopen and determine the incremental difference because of an error: `" << query.lastError() << "` during reload with query: '" << query.lastQuery() << "'";
			return false;
		} else {
			qDebug() << "VersionedRowset reopened during" << timer.elapsed() << "with query: '" << query.lastQuery() << "'";
		}

		QSet<int> inserted_index_set;
		QSet<int> removed_index_set;
		QSet<int> updated_index_set;
		QMap<int, int> moved_index_map;
		/// @brief Собираем изменения (id1: было; id2: стало)
		while (query.next()) {
			QVariant id1(query.value(0));
			QVariant id2(query.value(1));
			if (id1.isNull() && !id2.isNull()) { /// < добавление
				inserted_index_set.insert(id2.toInt() - 1);
			} else if (!id1.isNull() && id2.isNull()) { /// < удаление
				removed_index_set.insert(id1.toInt() - 1);
			} else if (!id1.isNull() && !id2.isNull()) { /// < перемещение или редактирование
				const int int_id1(id1.toInt() - 1);
				const int int_id2(id2.toInt() - 1);
				if (int_id1 == int_id2) {
					updated_index_set.insert(int_id1);
				} else {
					moved_index_map[int_id1] = int_id2;
				}
			}
		}
		query.finish();
		if (inserted_index_set.isEmpty() && removed_index_set.isEmpty() && updated_index_set.isEmpty() && moved_index_map.isEmpty()) {
			/// @todo Можно очистить времянку так как реальных изменений с текущим набором данных нет
			return true;
		}

		/// @brief Подменяем старую времянку на новую
		Rowset::setName(t2_name);
		const bool retcode(Rowset::initialize());
		if (!retcode) {
			_row_count = 0;
			qWarning() << "VersionedRowset reopen error: " << getError();
		} else {
			traverse(false);
			emit transformed(inserted_index_set, removed_index_set, updated_index_set, moved_index_map);
		}
		return retcode;
	}

	void VersionedRowset::close()
	{
		if (!isLoaded() || getName().isEmpty()) {
			return;
		}
		_row_count = 0;
		Rowset::close();
		QSqlQuery query(makeDatabase());

		for (quint64 r = 1; r <= _revision; ++r) {
			if (!query.exec("DROP TABLE IF EXISTS " + QString("temp.%1_%2_%3").arg(_source_rowset.getName()).arg(reinterpret_cast<quintptr>(this)).arg(r))) {
				qCritical() << "Could not drop temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
			}
		}
	}

	bool VersionedRowset::traverse(bool all)
	{
		QSqlQuery query("SELECT COUNT(*) FROM " + getName(), makeDatabase());
		_row_count = query.first() ? query.value(0).toInt() : 0;
		if (!Rowset::traverse(all)) {
			_row_count = 0;
			return false;
		}
		return true;
	}

	bool VersionedRowset::update(const QSqlRecord& record, int row_index)
	{
		return false;
	}

	bool VersionedRowset::insert(const QSqlRecord& record)
	{
		return false;
	}

	bool VersionedRowset::remove(int row_index)
	{
		return false;
	}

	void VersionedRowset::setName(const QString& table_name)
	{
		_source_rowset.setName(table_name);
		Rowset::setName(QString("temp.%1_%2_%3").arg(_source_rowset.getName()).arg(reinterpret_cast<quintptr>(this)).arg(_revision));
	}

	void VersionedRowset::setFilter(const AbstractPredicate& predicate)
	{
		_source_rowset.setFilter(predicate);
	}

	void VersionedRowset::setLimit(qsizetype limit)
	{
		_source_rowset.setLimit(limit);
	}

	void VersionedRowset::addSort(const Field& field, Qt::SortOrder order)
	{
		_source_rowset.addSort(field, order);
	}

	int VersionedRowset::getLength() const
	{
		return _row_count;
	}
} } } }
