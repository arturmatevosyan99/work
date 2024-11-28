#include "Rowset.hpp"

#include <array>
#include <QListIterator>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	const int Rowset::FetchSize = 250;

	Rowset::Rowset(Provider* provider)
	:
		QObject(),

		_database(provider ? provider->makeDatabase() : QSqlDatabase()),

		_reverse(false),
		_limit(0),

		_query(_database),
		_offset(-1),
		_eof(true)
	{

	}

	bool Rowset::initialize()
	{
		_error = QSqlError();
		_index.clear();
		_record.clear();
		_sort_list.removeDuplicates();

		if (_name.isEmpty()) {
			_error = QSqlError(QLatin1String("No table name given"), QString(), QSqlError::StatementError);
			return false;
		} else {
			_record = _database.record(_name);
			_index = !_custom_index.isEmpty() ? _custom_index : _database.primaryIndex(_name);
//			initColOffsets(rec.count());
		}

		if (_record.isEmpty()) {
			_error = QSqlError(QLatin1String("Unable to find table ") + _name, QString(), QSqlError::StatementError);
			return false;
		}

		QueryStream q;
		q << _database.driver()->sqlStatement(QSqlDriver::SelectStatement, _name, _record, false) << Qt::endl;

		if (q.isEmpty()) {
			_error = QSqlError(QLatin1String("Unable to select fields from table ") + _name, QString(), QSqlError::StatementError);
			return false;
		}

		if (!_user_filter.isEmpty()) {
			q << "WHERE " << _user_filter << (_system_filter.isEmpty() ? "" : (" AND (" + _system_filter + ")")) << Qt::endl;
		} else if (!_system_filter.isEmpty()) {
			q << "WHERE " << _system_filter << Qt::endl;
		}
		if (!_sort_list.isEmpty()) {
			q << "ORDER BY " << _sort_list.join(", ") << Qt::endl;
		}
		if (_limit > 0) {
			q << "LIMIT " << _limit  << Qt::endl;
		}

		return initialize(q);
	}

	bool Rowset::initialize(const QueryStream& q)
	{
		_offset = -1;
		_eof = true;
		_role_table.clear();
		_name_table.clear();
		_query.finish();
		if (!_query.prepare(q)) {
			_error = _query.lastError();
			return false;
		}
		return true;
	}

	bool Rowset::reopen(const HintTable& hint_table)
	{
		return false;
	}

	bool Rowset::open()
	{
		return (initialize() && traverse(false));
	}

	void Rowset::finalize()
	{
		if (!_query.isActive()) {
			return;
		}

		_error = QSqlError();
		_offset = -1;
		_eof = true;
		_query.finish();
		_appendix_vector.clear();
		_patch_set.clear();
		_ignore_set.clear();

		emit reset();
	}

	void Rowset::close()
	{
		_error = QSqlError();
		_index.clear();
		_record.clear();
		_offset = -1;
		_eof = true;
		_role_table.clear();
		_name_table.clear();
		_appendix_vector.clear();
		_patch_set.clear();
		_ignore_set.clear();
		_query.clear();

		emit reset();
	}

	bool Rowset::traverse(bool all)
	{
		if (_query.isActive() || _error.isValid()) {
			return false;
		}

//		beginResetModel();
		if (!_query.exec()) {
			_error = _query.lastError();
			emit reset();
			return false;
		}

		_record = _query.record();
		for (int i = 0; i < _record.count(); ++i) {
			_name_table.insert(_record.fieldName(i), Qt::UserRole + i + 1);
			_role_table.insert(Qt::UserRole + i + 1, _record.fieldName(i).toUtf8());
		}
		emit reset();

		if (_query.driver()->hasFeature(QSqlDriver::QuerySize) && _query.size() > 0) {
			_offset = _query.size() - 1 + _appendix_vector.length();
		} else {
			_offset = -1;
			_eof = false;
			if (all) {
				traverseAll();
			} else {
				traverseMore();
			}
		}
		return true;
	}

	void Rowset::traverseMore()
	{
		traverseOffset(_offset + Rowset::FetchSize);
	}

	void Rowset::traverseAll()
	{
		while (!_eof) {
			traverseMore();
		}
	}

	void Rowset::traverseOffset(int value)
	{
		if (_eof || value <= _offset) {
			return ;
		}

		int offset(-1);
		if (_appendix_vector.length() > value) {
			offset = value;
		} else if (_query.seek(value - _appendix_vector.length())) {
			offset = value;
		} else {
			offset = qMax(_offset, _appendix_vector.length());
			if (_query.seek(offset - _appendix_vector.length())) {
				while (_query.next()) {
					++offset;
				}
			} else {
				/// @note empty or invalid query
				offset = -1;
			}
			_eof = true;
		}

		if (offset >= 0 && offset > _offset) {
			int __offset(_offset);
			_offset = offset;
			emit fetched(__offset + 1, offset);
		} else {
			_offset = offset;
		}
	}

	bool Rowset::seek(int row_index)
	{
		if (row_index < 0) {
			return false;
		} else if (row_index < _appendix_vector.length()) {
			return true;
		} else if (row_index > _offset) {
			traverseOffset(row_index);
		}
		return _query.seek(row_index - _appendix_vector.length());
	}

	QSqlRecord Rowset::seekRecord(int row_index)
	{
		return seek(row_index) ? (row_index < _appendix_vector.length() ? _appendix_vector.at(row_index) : _query.record()) : QSqlRecord();
	}

	QVariantMap Rowset::seekKey(int row_index)
	{
		const QSqlRecord record(seekRecord(row_index));
		if (record.isEmpty()) {
			return QVariantMap();
		}

		QVariantMap key;
		QSqlIndex index(makeIndex());
		for (int i = 0; i < index.count(); ++i) {
			key[index.fieldName(i)] = record.value(index.fieldName(i));
		}
		return key;
	}

	QSqlIndex Rowset::seekIndex(int row_index)
	{
		const QSqlRecord record(seekRecord(row_index));
		if (record.isEmpty()) {
			return QSqlIndex();
		}

		QSqlIndex index(makeIndex());
		for (int i = 0; i < index.count(); ++i) {
			index.setValue(i, record.value(index.fieldName(i)));
		}
		return index;
	}

	QVariant Rowset::seekData(int row_index, int column_index)
	{
		return seek(row_index) ? (row_index < _appendix_vector.length() ? _appendix_vector.at(row_index).value(column_index) : _query.value(column_index)) : QVariant();
	}

	QSqlField Rowset::seekField(int row_index, int column_index)
	{
		QSqlField retval(_record.field(column_index));
		retval.setValue(seekData(row_index, column_index));
		return retval;
	}

	bool Rowset::mutate(const MutationList& mutation_list)
	{
		QSet<int> inserted_index_set, removed_index_set, updated_index_set;
		if (!mutate(mutation_list, inserted_index_set, removed_index_set, updated_index_set)) {
			return false;
		} else if (!inserted_index_set.isEmpty() || !removed_index_set.isEmpty() || !updated_index_set.isEmpty()) {
			emit transformed(inserted_index_set, removed_index_set, updated_index_set, {});
		}
		return true;
	}

	bool Rowset::update(const QSqlRecord& record, int row_index)
	{
		QSet<int> transformation_set;
		if (!update(record, row_index, transformation_set)) {
			return false;
		}
		emit transformed({}, {}, transformation_set, {});
		return true;
	}

	bool Rowset::update(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!update(record, transformation_set)) {
			return false;
		}
		emit transformed({}, {}, transformation_set, {});
		return true;
	}

	bool Rowset::insert(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!insert(record, transformation_set)) {
			return false;
		}
		emit transformed(transformation_set, {}, {}, {});
		return true;
	}

	bool Rowset::remove(int row_index)
	{
		QSet<int> transformation_set;
		if (!remove(row_index, transformation_set)) {
			return false;
		}
		emit transformed({}, transformation_set, {}, {});
		return true;
	}

	bool Rowset::remove(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!remove(record, transformation_set)) {
			return false;
		}
		emit transformed({}, transformation_set, {}, {});
		return true;
	}

	bool Rowset::patch(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!patch(record, transformation_set)) {
			return false;
		}
		emit transformed({}, {}, transformation_set, {});
		return true;
	}

	bool Rowset::ignore(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!ignore(record, transformation_set)) {
			return false;
		}
		emit transformed({}, transformation_set, {}, {});
		return true;
	}

	bool Rowset::append(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!append(record, transformation_set)) {
			return false;
		}
		emit transformed(transformation_set, {}, {}, {});
		return true;
	}

	bool Rowset::prepend(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!prepend(record, transformation_set)) {
			return false;
		}
		emit transformed(transformation_set, {}, {}, {});
		return true;
	}

	bool Rowset::suspend(const QSqlRecord& record)
	{
		QSet<int> transformation_set;
		if (!suspend(record, transformation_set)) {
			return false;
		}
		emit transformed({}, transformation_set, {}, {});
		return true;
	}

	bool Rowset::mutate(const MutationList& mutation_list, QSet<int>& inserted_index_set, QSet<int>& removed_index_set, QSet<int>& updated_index_set)
	{
		/// @bug Порядок важен! (Suspend, Ignore, Remove) -> (Patch, Update) -> (Append, Insert)
		bool retval(true);
		for (const MutationList::value_type& mutation : mutation_list) {
			QSet<int> transformation_set;
			switch (mutation.first) {
				case Mutation::Suspend:
					retval &= suspend(mutation.second, transformation_set);
					removed_index_set.unite(transformation_set);
					break;

				case Mutation::Ignore:
					retval &= ignore(mutation.second, transformation_set);
					removed_index_set.unite(transformation_set);
					break;

				case Mutation::Remove:
					retval &= remove(mutation.second, transformation_set);
					removed_index_set.unite(transformation_set);
					break;

				case Mutation::Patch:
					retval &= patch(mutation.second, transformation_set);
					updated_index_set.unite(transformation_set);
					break;

				case Mutation::Update:
					retval &= update(mutation.second, transformation_set);
					updated_index_set.unite(transformation_set);
					break;

				case Mutation::Append:
					retval &= append(mutation.second, transformation_set);
					inserted_index_set.unite(transformation_set);
					break;

				case Mutation::Prepend:
					retval &= prepend(mutation.second, transformation_set);
					inserted_index_set.unite(transformation_set);
					break;

				case Mutation::Insert:
					retval &= insert(mutation.second, transformation_set);
					inserted_index_set.unite(transformation_set);
					break;

			}
		}
		return retval;
	}

	bool  Rowset::mutate(Mutation mutation, const QSqlRecord& record, QSet<int>& transformation_set)
	{
		switch (mutation) {
			case Mutation::Suspend:
				return suspend(record, transformation_set);

			case Mutation::Ignore:
				return ignore(record, transformation_set);

			case Mutation::Remove:
				return remove(record, transformation_set);

			case Mutation::Patch:
				return patch(record, transformation_set);

			case Mutation::Update:
				return update(record, transformation_set);

			case Mutation::Append:
				return append(record, transformation_set);

			case Mutation::Prepend:
				return prepend(record, transformation_set);

			case Mutation::Insert:
				return insert(record, transformation_set);

		}
		return false;
	}

	bool Rowset::update(const QSqlRecord& record, int row_index, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		/// @todo
		return false;
	}

	bool Rowset::update(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		/// @todo
		return false;
	}

	bool Rowset::insert(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		/// @todo
		return false;
	}

	bool Rowset::remove(int row_index, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		/// @todo
		return false;
	}

	bool Rowset::remove(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		/// @todo
		return false;
	}

	bool Rowset::patch(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		/// @todo
		return false;
	}

	bool Rowset::ignore(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		/// @todo
		return false;
	}

	bool Rowset::append(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		_appendix_vector.append(record);
		transformation_set.insert(_appendix_vector.length() - 1);
		return true;
	}

	bool Rowset::prepend(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		_appendix_vector.prepend(record);
		transformation_set.insert(0);
		return true;
	}

	bool Rowset::suspend(const QSqlRecord& record, QSet<int>& transformation_set)
	{
		transformation_set.clear();
		for (int t = 0, a = 0; a < _appendix_vector.count(); ++t, ++a) {
			for (int r = 0; r < record.count(); ++r) {
				if (record.isGenerated(r)) {
					if (record.value(r) != _appendix_vector.at(a).value(record.fieldName(r))) {
						transformation_set.remove(t);
						break;
					} else {
						transformation_set.insert(t);
					}
				}
			}
			if (transformation_set.contains(t)) {
				_appendix_vector.remove(a);
				--a;
			}
		}
		return true;
	}

	void Rowset::setName(const QString& name)
	{
		_name = name;
	}

	void Rowset::setIndex(const QSqlIndex& index)
	{
		_custom_index = index;
	}

	void Rowset::setFilter(const AbstractPredicate& predicate)
	{
		_user_filter = predicate;
	}

	void Rowset::setLimit(qsizetype limit)
	{
		_limit = limit;
	}

	void Rowset::setReverse(bool onoff)
	{
		if (_reverse != onoff) {
			for (QString& sorter : _sort_list) {
				if (sorter.contains("ASC")) {
					sorter.replace("ASC", "DESC");
				} else {
					sorter.replace("DESC", "ASC");
				}
			}
			_reverse = onoff;
		}
	}

	void Rowset::addSort(const Field& field, Qt::SortOrder order)
	{
		_sort_list.push_back(field.getAlias().append(Qt::SortOrder::AscendingOrder == order ? (!_reverse ? " ASC" : " DESC") : (!_reverse ? " DESC" : " ASC")));
	}

	const QSqlQuery& Rowset::getQuery() const
	{
		return _query;
	}

	const QString& Rowset::getFilter() const
	{
		return _user_filter;
	}

	const QStringList& Rowset::getSortList() const
	{
		return _sort_list;
	}

	const qsizetype& Rowset::getLimit() const
	{
		return _limit;
	}

	const bool& Rowset::getReverse() const
	{
		return _reverse;
	}

	const QSqlError& Rowset::getError() const
	{
		return _error;
	}

	const QSqlDatabase& Rowset::getDatabase() const
	{
		return _database;
	}

	const QString& Rowset::getName() const
	{
		return _name;
	}

	QString Rowset::getEscapedName() const
	{
		return (_database.driver()->isIdentifierEscaped(_name, QSqlDriver::TableName)) ? _name : _database.driver()->escapeIdentifier(_name, QSqlDriver::TableName);
	}

	const QSqlRecord& Rowset::getRecord() const
	{
		return _record;
	}

	const QSqlIndex& Rowset::getIndex() const
	{
		return _index;
	}

	const Rowset::RoleTable& Rowset::getRoleTable() const
	{
		return _role_table;
	}

	const Rowset::NameTable& Rowset::getNameTable() const
	{
		return _name_table;
	}

	const QVector<QSqlRecord>& Rowset::getAppendix() const
	{
		return _appendix_vector;
	}

	const QSet<QSqlRecord>& Rowset::getPatchSet() const
	{
		return _patch_set;
	}

	const QSet<QSqlRecord>& Rowset::getIgnoreSet() const
	{
		return _ignore_set;
	}

	int Rowset::getWidth() const
	{
		return _record.count();
	}

	int Rowset::getOffset() const
	{
		return _offset < 0 ? 0 : _offset;
	}

	int Rowset::getLength() const
	{
		return _offset < 0 ? 0 : _offset + 1;
	}

	QSqlDatabase Rowset::makeDatabase() const
	{
		return _database;
	}

	QSqlRecord Rowset::makeRecord() const
	{
		return _record;
	}

	QStringList Rowset::makeFilterList(const QSqlRecord& record, const Field::Condition& condition) const
	{
		QStringList retval;
		for (int i = 0; i < _index.count(); ++i) {
			const QSqlField field(record.field(_index.fieldName(i)));
			retval.push_back(field.name());
			if (!_database.driver()->isIdentifierEscaped(retval.back(), QSqlDriver::FieldName)) {
				retval.back() = _database.driver()->escapeIdentifier(retval.back(), QSqlDriver::FieldName);
			}
			retval.back().prepend(getName() + ".");

			static constexpr std::array<Field::Condition, 4> null_condition_array = {Field::Condition::Equal, Field::Condition::Is, Field::Condition::IsNot, Field::Condition::NotEqual};
			if (field.isNull() && std::any_of(std::cbegin(null_condition_array), std::cend(null_condition_array), [&condition](const Field::Condition& c){ return (condition == c); })) {
				switch (condition) {
					case Field::Condition::Equal:
						retval.last().append(" IS "); /// @??? Обрыв
						break;
					case Field::Condition::NotEqual:
						retval.last().append(" IS NULL ");
						break;
					default:
						retval.append(" " + Field::ConditionMap.value(condition, "IS") + " "); /// @??? Обрыв
						break;
				}
			} else {
				const QMetaType::Type type(static_cast<QMetaType::Type>(field.type()));
				retval.last().append(" " + Field::ConditionMap.value(condition, "=") + " ");
				switch (type) {
					case QMetaType::Type::QString:
						retval.back().append("'" +  field.value().toString() + "'");
						break;

					default:
						/// @todo Остальные типы: дата/время, uuid, etc
	//					filter_list.last().append(field.value().toString());
						retval.back().append(_database.driver()->formatValue(field));
				}
			}
		}
		return retval;
	}

	QSqlIndex Rowset::makeIndex() const
	{
		return _index;
	}

	bool Rowset::isLoaded() const
	{
		return _query.isActive();
	}

	bool Rowset::isEmpty() const
	{
		return _offset < 0 && _eof;
	}

	bool Rowset::isEOF() const
	{
		return _eof;
	}

	bool Rowset::hasIndex(const QList<QSqlIndex>& index_list, bool all) const
	{
		/// @xxx makeFilterList dublicate?
		QStringList filter_list;
		for (const QSqlIndex& index : index_list) {
			for (int f=0; f < index.count(); ++f) {
				QSqlField field(index.field(f));
				filter_list.append(field.name());
				if (!_database.driver()->isIdentifierEscaped(filter_list.last(), QSqlDriver::FieldName)) {
					filter_list.last() = _database.driver()->escapeIdentifier(filter_list.last(), QSqlDriver::FieldName);
				}
				filter_list.last().prepend(_name + ".");

				if (field.isNull()) {
					filter_list.last().append(" IS NULL ");
				} else {
					filter_list.last().append(" = ");
					const QMetaType::Type meta_type(static_cast<QMetaType::Type>(field.type()));
					switch (meta_type) {
						case QMetaType::QString:
							filter_list.last().append("'" + field.value().toString() + "'");
							break;
						default:
							/// @todo Остальные типы: дата/время, uuid, etc
//							filter_list.last().append(field.value().toString());
							filter_list.last().append(_database.driver()->formatValue(field));
							break;
					}
				}
			}
		}
		if (filter_list.isEmpty()) {
			return false;
		}
		QSqlQuery query(_database.exec(QString("SELECT COUNT(*) FROM %1 WHERE %2").arg(_name, filter_list.join(all ? " AND " : " OR "))));
		return (!query.isActive() || !query.next()) ? false : query.value(0).toInt() > 0;
	}
} } } }
