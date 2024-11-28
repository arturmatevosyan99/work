#include "ShadedRowset.hpp"

#include <QSqlQuery>
#include <QElapsedTimer>
#include <QSqlError>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	ShadedRowset::ShadedRowset(Provider* parent)
	:
		Rowset(parent),

		_source_rowset(parent),
		_row_count(0),
		_ab(nullptr)
	{

	}

	ShadedRowset::~ShadedRowset()
	{
		/// @brief DROP TABLE SQLite сделать не даст при наличии любого активного QSqlQuery даже по какой-то другой таблице
		close();
		if (nullptr != _ab) {
			delete _ab;
		}
	}

	bool ShadedRowset::initialize()
	{
		_row_count = 0;

		_source_query.clear();
		if (!_source_rowset.initialize()) {
			qWarning() << "Could not build source query: '" << _source_rowset.getQuery().lastQuery() << "'";
			_source_rowset.close();
			return false;
		}

		bool retval(true);
		do {
			QSqlDatabase database(makeDatabase());
			QSqlQuery query(database);
			if (nullptr == _ab) {
				if (!query.exec("CREATE TABLE IF NOT EXISTS " + makeShadeName(false) + " AS " + _source_rowset.getQuery().lastQuery())) {
					qWarning() << "Could not create first temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
					retval = false;
					break;
				} else {
					if (query.exec("SELECT COUNT(*) FROM " + makeShadeName(false)) && query.first() && (query.value(0).toInt() == 0 || query.exec("DELETE FROM " + makeShadeName(false)))) {
						if (query.exec("INSERT INTO " + makeShadeName(false) + " " + _source_rowset.getQuery().lastQuery())) {
							_ab = new bool(false);
							qDebug() << "ShadedRowset initialization query: '" << query.lastQuery() << "'";
						} else {
							qWarning() << "Could not refill first temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
							retval = false;
							break;
						}
					} else {
						qWarning() << "Could not reuse same temporary table for new ShadedRowset";
						retval = false;
						break;
					}
				}
				if (!query.exec("CREATE TABLE IF NOT EXISTS " + makeShadeName(true) + " AS " + database.driver()->sqlStatement(QSqlDriver::SelectStatement, _source_rowset.getName(), _source_rowset.getRecord(), false) + " LIMIT 0")) {
					qWarning() << "Could not create second temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
					retval = false;
					break;
				} else {
					if (query.exec("SELECT COUNT(*) FROM " + makeShadeName(true)) && query.first() && query.value(0).toInt() > 0) {
						if (query.exec("DELETE FROM " + makeShadeName(true))) {
							qDebug() << "ShadedRowset an initializiation error because of a query: '" << query.lastQuery() << "'";
						} else {
							qWarning() << "Could not truncate second temporary table because of a error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
							retval = false;
							break;
						}
					}
				}
			} else {
				close();
				if (!query.exec("INSERT INTO " + makeShadeName(*_ab) + " " + _source_rowset.getQuery().lastQuery())) {
					qWarning() << "Could not populate temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
					retval = false;
					break;
				} else {
					qDebug() << "ShadedRowset initialization query: '" << query.lastQuery() << "'";
				}
			}
		} while (false);
		_source_query = _source_rowset.getQuery().executedQuery();
		_source_rowset.close();

		return retval ? Rowset::initialize() : false;
	}

	bool ShadedRowset::reopen(const Rowset::HintTable& hint_table)
	{
		Q_UNUSED(hint_table);
		if (!isLoaded() || nullptr == _ab) {
			return false;
		}

		QSqlDatabase database(makeDatabase());
		QSqlQuery query(database);

		const QString t2_name(makeShadeName(!(*_ab))); /// < Новый слепок
		if (!query.exec("DELETE FROM " + t2_name)) {
			qWarning() << "Could not truncate temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
			return false;
		} else if (!query.exec("INSERT INTO " + t2_name + " " + _source_query)) {
			qWarning() << "Could not populate temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
			return false;
		}
		qDebug() << "Reopen query: '" << query.lastQuery() << "'";
		query.finish();

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
			qWarning() << "Could not determine the incremental difference during reload with query: '" << query.lastQuery() << "' because of an unexpected error: `" << query.lastError() << "`";
			return false;
		} else {
			qDebug() << "ShadedRowset reopened during" << timer.elapsed() << "with query: '" << query.lastQuery() << "'";
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
		Rowset::close();
		Rowset::setName(t2_name);
		*_ab = !(*_ab);
		const bool retcode(Rowset::initialize());
		if (!retcode) {
			_row_count = 0;
			qWarning() << "ShadedRowset reopen error: " << getError();
		} else {
			emit transformed(inserted_index_set, removed_index_set, updated_index_set, moved_index_map);
			traverse(false);
		}
		return retcode;
	}

	void ShadedRowset::close()
	{
		if (!isLoaded() || getName().isEmpty()) {
			return;
		}
		_source_query.clear();
		_row_count = 0;
		if (nullptr != _ab) {
			*_ab = false;
		}
		Rowset::close();
		QSqlQuery query(makeDatabase());
		if (!query.exec("DELETE FROM " + makeShadeName(false)) || !query.exec("DELETE FROM " + makeShadeName(true))) {
			qCritical() << "Could not clear temporary table because of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
		}
	}

	bool ShadedRowset::traverse(bool all)
	{
		QSqlQuery query("SELECT COUNT(*) FROM " + getName(), makeDatabase());
		_row_count = query.first() ? query.value(0).toInt() : 0;
		query.clear();
		if (!Rowset::traverse(all)) {
			_row_count = 0;
			return false;
		}
		return true;
	}

	bool ShadedRowset::update(const QSqlRecord& record, int row_index)
	{
		return false;
	}

	bool ShadedRowset::insert(const QSqlRecord& record)
	{
		return false;
	}

	bool ShadedRowset::remove(int row_index)
	{
		return false;
	}

	QSqlIndex ShadedRowset::makeIndex() const
	{
		return getDatabase().primaryIndex(_source_rowset.getName());
	}

	QString ShadedRowset::makeShadeName(bool ab) const
	{
		return QString("temp.%1_%2_%3").arg(_source_rowset.getName()).arg(reinterpret_cast<quintptr>(this)).arg(ab);
	}

	void ShadedRowset::setName(const QString& table_name)
	{
		_source_rowset.setName(table_name);
		Rowset::setName(makeShadeName(false));
	}

	void ShadedRowset::setFilter(const AbstractPredicate& predicate)
	{
		_source_rowset.setFilter(predicate);
	}

	void ShadedRowset::setLimit(qsizetype limit)
	{
		_source_rowset.setLimit(limit);
	}

	void ShadedRowset::addSort(const Field& field, Qt::SortOrder order)
	{
		_source_rowset.addSort(field, order);
	}

	int ShadedRowset::getLength() const
	{
		return _row_count;
	}
} } } }
