#include "TeleRowset.hpp"

#include "../Predicate/CompoundPredicate.hpp"
#include "../Predicate/ConditionPredicate.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <array>
#include <numeric>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	TeleRowset::TeleRowset(Provider* parent)
	:
		Rowset(parent),

		_head_length(0),
		_tail_length(0)
	{
		setReverse(true);
	}

	TeleRowset::~TeleRowset()
	{

	}

	bool TeleRowset::initialize()
	{
		_head_length = 0;
		_tail_length = 0;
		_zero_record = QSqlRecord();
		_zero_filter.clear();

		if (!Rowset::initialize()) {
			return false;
		}

		QSqlDatabase database(makeDatabase());
		QSqlQuery query(database);
		QueryStream q;

		/// @brief Текущая отсечка с самым свежим - "нулевой километр"
		q << database.driver()->sqlStatement(QSqlDriver::SelectStatement, getName(), _record, false) << Qt::endl;
		if (!getFilter().isEmpty()) {
			q << "WHERE " << getFilter() << "" << (_system_filter.isEmpty() ? "" : (" AND (" + _system_filter + ")")) << Qt::endl;
		} else if (!_system_filter.isEmpty()) {
			q << "WHERE " << _system_filter << Qt::endl;
		}

		if (!getSortList().isEmpty()) {
			q << "ORDER BY " << getSortList().join(", ") << Qt::endl;
		}
		q << "LIMIT 1" << Qt::endl;

		if (!query.exec(q)) {
			_error = QSqlError(QLatin1String("Unable to select zero record of %1 table").arg(getName()), QString(), QSqlError::StatementError);
			return false;
		} else if (query.first()) {
			_zero_record = query.record();
			_zero_filter = makeFilterList(_zero_record, Field::Condition::Unknown);
		}
		if (_zero_filter.empty()) {
			qWarning() << "No index is specified: tail cutoff and sequential increment are not supported";
			_zero_filter << "1 = 1";
		}
		query.finish();

		const QString op(getReverse() ? "<=" : ">=");

		/// @brief От "нулевого километра" двигаемся назад(в хвост)
		q.clear();
		q << database.driver()->sqlStatement(QSqlDriver::SelectStatement, getName(), _record, false) << Qt::endl;

		if (!getFilter().isEmpty()) {
			q << "WHERE " << getFilter() << "" << (_system_filter.isEmpty() ? "" : (" AND (" + _system_filter + ")")) << " AND (" << _zero_filter.join(" AND ").replace('?', op) << ")" << Qt::endl;
		} else if (!_system_filter.isEmpty()) {
			q << "WHERE " << _system_filter << " AND (" << _zero_filter.join(" AND ").replace('?', op) << ")"<< Qt::endl;
		} else {
			q << "WHERE " << _zero_filter.join(" AND ").replace('?', op) << Qt::endl;
		}

		if (!getSortList().isEmpty()) {
			q << "ORDER BY " << getSortList().join(", ") << Qt::endl;
		}
		if (getLimit() > 0) {
			q << "LIMIT " << getLimit() << Qt::endl;
		}

		if (!Rowset::initialize(q)) {
			_error = QSqlError(QLatin1String("Unable to select initial rowset of %1 table").arg(getName()), QString(), QSqlError::StatementError);
			return false;
		}
		/// @note Считаем сколько есть записей назад(в хвосте)
		QMap<End, qsizetype> count_map{{End::Tail, 0}};
		count(count_map);
		_tail_length = count_map.value(End::Tail, 0);
		return true;
	}

	bool TeleRowset::reopen(const Rowset::HintTable& hint_table)
	{
		if (!isLoaded()) {
			return false;
		}

		/// @note По идее, если в hint_table есть delete внутри уже загруженной части или вставка не в хвост/голову, то сразу возвращаем false
		if (hint_table.contains(Mutation::Remove) || hint_table.contains(Mutation::Insert) || _zero_record.isEmpty()) {
			if (!Rowset::open()) {
				return false;
			}
			QVector<int> index_vector(getLength()); std::iota(index_vector.begin(), index_vector.end(), 0);
			emit transformed(QSet<int>(index_vector.constBegin(), index_vector.constEnd()), {}, {}, {});
			return true;
		}

		/// @brief Считаем прирост головы, хвоста и получаем разницу докачанных назад(рост хвоста) и добавленных вперед(рост головы)
		QMap<End, qsizetype> count_map{{End::Tail, 0}, {End::Head, 0}};
		count(count_map);

		/// @bug Когда удалил zero_record?
		/// @brief но меньше(хвост) и больше(голова) должны остаться и поэтому нам нужно научиться определять факт удаления zero_record для +1 или -1 смещения головы

		/// @brief
		QueryStream q;
		QSqlDatabase database(makeDatabase());

		/// @note Дальнейший порядок обработки в порядке объявления
		QSet<int> removed_index_set, inserted_index_set, updated_index_set;
		QMap<int, int> moved_index_map;

		/// @brief Отцепляем
		Rowset::HintTable::ConstIterator s(hint_table.find(Mutation::Suspend));
		if (hint_table.constEnd() != s) {
			do {
				QSet<int> transformation_set;
				if (suspend(s.value(), transformation_set)) {
					removed_index_set.unite(transformation_set);
				}
			} while (++s != hint_table.constEnd() && s.key() == Mutation::Suspend);
		}

		/// @brief Прицепляем
		constexpr std::array<Mutation, 2> pend_array{Mutation::Append, Mutation::Prepend};
		for (const Mutation& m : pend_array) {
			Rowset::HintTable::ConstIterator a(hint_table.find(m));
			if (hint_table.constEnd() != a) {
				do {
					QSet<int> transformation_set;
					if (mutate(m, a.value(), transformation_set)) {
						inserted_index_set.unite(transformation_set);
					}
				} while (++a != hint_table.constEnd() && a.key() == m);
			}
		}

		const QString op(getReverse() ? "<=" : ">=");

		/// @brief Определеяем индексы(актуальные после остальных мутаций) измененных записей по заданным подсказкам
		/// @note Измененными могут быть только записи в хвосте вплоть до(включая) нулевую запись
		Rowset::HintTable::ConstIterator u(hint_table.find(Mutation::Update));
		if (!_zero_record.isEmpty() && hint_table.constEnd() != u) {
			QSqlRecord hint_record(_index);
			hint_record.insert(0, QSqlField("no", QVariant::LongLong));
			q << database.driver()->sqlStatement(QSqlDriver::SelectStatement, "", hint_record, false) << Qt::endl;
			q << "(SELECT ";
			for (int i = _index.count() - 1; i >= 0; --i) {
				if (_index.isGenerated(i)) {
					q << (database.driver()->isIdentifierEscaped(_index.fieldName(i), QSqlDriver::FieldName) ? _index.fieldName(i) : database.driver()->escapeIdentifier(_index.fieldName(i), QSqlDriver::FieldName)) << ", " << Qt::endl;
				}
			}
			q << "ROW_NUMBER() OVER(";
			if (!getSortList().isEmpty()) {
				q << "ORDER BY " << getSortList().join(", ");
			}
			q << ") AS no" << Qt::endl;
			q << "FROM " << getEscapedName() << Qt::endl;
			q << "WHERE (" << Qt::endl;
			if (!getFilter().isEmpty()) {
				q << getFilter() << "" << (_system_filter.isEmpty() ? "" : (" AND (" + _system_filter + ")")) << " AND (" << _zero_filter.join(" AND ").replace('?', op) << ")" << Qt::endl;
			} else if (!_system_filter.isEmpty()) {
				q << _system_filter << " AND (" << _zero_filter.join(" AND ").replace('?', op) << ")"<< Qt::endl;
			} else {
				q << _zero_filter.join(" AND ").replace('?', op) << Qt::endl;
			}
			q << ")" << Qt::endl;
			if (!getSortList().isEmpty()) {
				q << "ORDER BY " << getSortList().join(", ") << Qt::endl;
			}
			/// @bug Нужно учитывать новые в голове, которые заберут свою часть общего лимита
			if (getLimit() > 0) {
				q << "LIMIT " << getLimit() << Qt::endl;
			}

			q << ") AS " << getName() + "_U WHERE" << Qt::endl;
			do {
				q << "(" << (makeFilterList(u.value(), Field::Condition::Equal).join(" AND ").replace(getName(), getName() + "_U")) << ")";
			} while (++u != hint_table.constEnd() && u.key() == Mutation::Update && q.append(" OR "));

			QSqlQuery query(database);
			if (!query.exec(q)) {
				qWarning() << "Updated record could not be determinated beacause of an error: `" << query.lastError() << "` for the query: '" << query.lastQuery() << "'";
				return false;
			}

			while (query.next()) {
				updated_index_set.insert(query.value(0).toInt() - 1 + count_map.value(End::Head, 0));
			}
			query.finish();
			q.clear();
		}

		qsizetype limit(getLimit());

		if (count_map.value(End::Tail, 0) > 0) {
			q << database.driver()->sqlStatement(QSqlDriver::SelectStatement, getName(), _record, false) << Qt::endl;
			if (!getFilter().isEmpty()) {
				q << "WHERE " << getFilter() << "" << (_system_filter.isEmpty() ? "" : (" AND (" + _system_filter + ")")) << " AND (" << _zero_filter.join(" AND ").replace('?', op) << ")" << Qt::endl;
			} else if (!_system_filter.isEmpty()) {
				q << "WHERE " << _system_filter << " AND (" << _zero_filter.join(" AND ").replace('?', op) << ")"<< Qt::endl;
			} else {
				q << "WHERE " << _zero_filter.join(" AND ").replace('?', op) << Qt::endl;
			}
			if (limit > 0) {
				q << "LIMIT " << (count_map[End::Tail] = qMin(limit, count_map[End::Tail])) << Qt::endl;
				if ((limit -= count_map[End::Tail]) <= 0) {
					count_map[End::Head] = 0;
				}
			}
		}

		if (count_map.value(End::Head, 0) > 0) {
			if (!q.isEmpty()) {
				q << "UNION" << Qt::endl;
			}
			q << database.driver()->sqlStatement(QSqlDriver::SelectStatement, getName(), _record, false) << Qt::endl;
			if (!getFilter().isEmpty()) {
				q << "WHERE " << getFilter() << "" << (_system_filter.isEmpty() ? "" : (" AND (" + _system_filter + ")")) << " AND (" << _zero_filter.join(" AND ").replace('?', ">") << ")" << Qt::endl;
			} else if (!_system_filter.isEmpty()) {
				q << "WHERE " << _system_filter << " AND (" << _zero_filter.join(" AND ").replace('?', ">") << ")"<< Qt::endl;
			} else {
				q << "WHERE " << _zero_filter.join(" AND ").replace('?', ">") << Qt::endl;
			}
			if (limit > 0) {
				q << "LIMIT " << (count_map[End::Head] = qMin(limit, count_map[End::Head])) << Qt::endl;
			}
		}

		if (!getSortList().isEmpty()) {
			q << "ORDER BY " << getSortList().join(", ") << Qt::endl;
		}

		/// @note х----0-----г (от конца хвоста до начала головы)

		/// @bug Необходимо уточнить, что zero record еще сушествует, иначе -1
		/// @note Но считаем, что удаление из середины не кейс TeleRowset, поэтому пока не заморачиваемся
		if (_head_length <= count_map.value(End::Head, 0)) {
			for (int count = count_map.value(End::Head, 0) - _head_length, h = 0; h < count; ++h) {
				inserted_index_set.insert(getAppendix().length() + h);
			}
		} else {
			/// @ocd Вряд ли это вообще можно хоть как-то гарантировать, но считаем, что если удалены, то только в начале старой головы
			for (int count = _head_length - count_map.value(End::Head, 0), h = 0; h < count; ++h) {
				removed_index_set.insert(getAppendix().length() + h);
			}
		}

		/// @ocd Вряд ли это вообще можно хоть как-то гарантировать, но считаем, что если удалены/доблавены, то только в конце хвоста
		if (_tail_length <= count_map.value(End::Tail, 0)) {
			for (int limit = count_map.value(End::Tail, 0) + count_map.value(End::Head, 0) + 1, t = count_map.value(End::Head, 0) + _tail_length + 1; t < limit ; ++t) {
				inserted_index_set.insert(t);
			}
		} else {
			for (int limit = _head_length + _tail_length - (_tail_length - count_map.value(End::Tail, 0)), t = _head_length + _tail_length; t > limit; --t) {
				removed_index_set.insert(t);
			}
		}

		_tail_length = 0;
		_head_length = 0;
		if (!Rowset::initialize(q)) {
			_error = QSqlError(QLatin1String("Unable to select initial rowset of %1 table").arg(getName()), QString(), QSqlError::StatementError);
			return false;
		}
		_tail_length = count_map.value(End::Tail, 0) + count_map.value(End::Head, 0);

		if (!traverse(false)) {
			return false;
		} else if (isEmpty()) {
			_zero_record = QSqlRecord();
			_zero_filter << "1 = 1";
		} else {
			_zero_record = seekRecord(0);
			_zero_filter = makeFilterList(_zero_record, Field::Condition::Unknown);
		}

		emit transformed(inserted_index_set, removed_index_set, updated_index_set, moved_index_map);
		return true;
	}

	bool TeleRowset::traverse(bool all)
	{
		return Rowset::traverse(all);
	}

	void TeleRowset::close()
	{
		_head_length = 0;
		_tail_length = 0;
		_zero_record.clear();

		return Rowset::close();
	}

	void TeleRowset::count(QMap<End, qsizetype>& count_map)
	{
		QSqlDatabase database(makeDatabase());
		QSqlQuery query(database);
		QueryStream q;

		/// @note Нулевой записи нет потому что данных нет
		if (_zero_record.isEmpty()) {
			q << "SELECT COUNT(*) FROM " << getName() << Qt::endl;
			if (!getFilter().isEmpty()) {
				q << "WHERE " << getFilter() << Qt::endl;
			}
			count_map[End::Tail] = 0;
			count_map[End::Head] = (query.exec(q) && query.first()) ? query.value(0).toInt() : 0;
			return;
		}

		qsizetype limit(getLimit() - 1);
		/// @note Получаем общее количество от нулевой отсечки вперед и назад
		for (QMap<End, qsizetype>::iterator c = count_map.begin(); c != count_map.end(); ++c) {
			q.clear();
			q << "SELECT COUNT(*) FROM " << getName() << Qt::endl;

			if (!getFilter().isEmpty()) {
				q << "WHERE " << getFilter() << " AND " << Qt::endl;
			} else {
				q << "WHERE " << Qt::endl;
			}

			const char op(getReverse() ? static_cast<char>(c.key()) : (c.key() == End::Head ? '<' : '>'));

			q << " (" << _zero_filter.join(" AND ").replace('?', op) << ")" << Qt::endl;

			if (!getSortList().isEmpty()) {
				q << "ORDER BY " << getSortList().join(", ") << Qt::endl;
			}
			if (limit > 0) {
				q << "LIMIT " << limit << Qt::endl;
			}

			if (query.exec(q) && query.first()) {
				c.value() = query.value(0).toInt();
				if (limit > 0) {
					if ((limit -= c.value()) <= 0) {
						break;
					}
				}
				query.finish();
			} else {
				c.value() = 0;
			}
		}
	}

	bool TeleRowset::update(const QSqlRecord& record, int row_index)
	{
		/// @todo
		return false;
	}

	bool TeleRowset::insert(const QSqlRecord& record)
	{
		/// @brief Не поддерживается в монотонно приростающих rowsets
		return false;
	}

	bool TeleRowset::remove(int row_index)
	{
		/// @brief Не поддерживается в монотонно приростающих rowsets
		return false;
	}

	void TeleRowset::setName(const QString& table_name)
	{
		return Rowset::setName(table_name);
	}

	void TeleRowset::setFilter(const AbstractPredicate& predicate)
	{
		return Rowset::setFilter(predicate);
	}

	void TeleRowset::setLimit(qsizetype limit)
	{
		return Rowset::setLimit(limit);
	}

	void TeleRowset::addSort(const Field& field, Qt::SortOrder order)
	{
		return Rowset::addSort(field, order);
	}

	int TeleRowset::getLength() const
	{
		return _zero_record.isEmpty() ? 0 : 1 + _head_length + _tail_length + getAppendix().count();
	}
} } } }
