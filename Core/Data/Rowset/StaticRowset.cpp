#include "StaticRowset.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	StaticRowset::StaticRowset(const QSqlIndex& index, const QSqlRecord& record, int cache_size)
	:
		Rowset(nullptr),

		_vector(cache_size > 0 ? cache_size : 0),
		_v(_vector.constEnd())
	{
		_index = index;
		_record = record;
		for (int i = 0; i < _record.count(); ++i) {
			_name_table.insert(_record.fieldName(i), Qt::UserRole + i + 1);
			_role_table.insert(Qt::UserRole + i + 1, _record.fieldName(i).toUtf8());
		}
	}

	StaticRowset::~StaticRowset()
	{
	}

	bool StaticRowset::initialize()
	{
		_v = _vector.constBegin();
		return true;
	}

	void StaticRowset::close()
	{
		_vector.clear();
		_v = _vector.constEnd();
		emit reset();
	}

	bool StaticRowset::traverse(bool all)
	{
		all ? traverseAll() : traverseMore();
		return true;
	}

	void StaticRowset::traverseMore()
	{
		traverseOffset();
	}

	void StaticRowset::traverseAll()
	{
		_v = _vector.constEnd();
	}

	void StaticRowset::traverseOffset(int value)
	{
		_v += value;
	}

	bool StaticRowset::seek(int row_index)
	{
		if (_vector.length() <= row_index) {
			return false;
		}
		_v = _vector.constBegin() + row_index;
		return true;
	}

	QSqlRecord StaticRowset::seekRecord(int row_index)
	{
		return seek(row_index) ? *_v : QSqlRecord();
	}

	QSqlField StaticRowset::seekField(int row_index, int column_index)
	{
		return seek(row_index) ? _v->field(column_index) : QSqlField();
	}

	QVariant StaticRowset::seekData(int row_index, int column_index)
	{
		return seek(row_index) ? _v->value(column_index) : QVariant();
	}

	bool StaticRowset::update(const QSqlRecord& record, int row_index)
	{
		if (_vector.length() <= row_index) {
			return false;
		}
		_vector[row_index] = record;
		return true;
	}

	bool StaticRowset::insert(const QSqlRecord& record)
	{
		_vector.append(record);
		return true;
	}

	bool StaticRowset::remove(int row_index)
	{
		if (_vector.length() <= row_index) {
			return false;
		}
		_vector.remove(row_index);
		return true;
	}

	void StaticRowset::setFilter(const AbstractPredicate& predicate)
	{
		/// @todo
	}

	void StaticRowset::setLimit(qsizetype limit)
	{
		/// @todo
	}

	void StaticRowset::addSort(const Field& field, Qt::SortOrder order)
	{
		/// @todo
	}

	int StaticRowset::getOffset() const
	{
		return _vector.isEmpty() ? 0 : _vector.length() - 1;
	}

	int StaticRowset::getLength() const
	{
		return _vector.length();
	}

	bool StaticRowset::isEmpty() const
	{
		return _vector.isEmpty();
	}

	bool StaticRowset::isEOF() const
	{
		return _vector.constEnd() == _v;
	}

	bool StaticRowset::hasIndex(const QList<QSqlIndex>& index_list, bool all) const
	{
		/// @todo
		return false;
	}
} } } }
