#include "AbstractEntity.hpp"

#include "Provider.hpp"
#include "Event.hpp"
#include "Rowset/ShadedRowset.hpp"
#include "Rowset/StaticRowset.hpp"
#include "Rowset/TeleRowset.hpp"
#include "Command/EntityOpen.hpp"
#include "Command/EntityReopen.hpp"
#include "Command/EntityClose.hpp"
#include "Command/EntityFetch.hpp"
#include "Command/EntityMutate.hpp"
#include "../Meta/Qd.hpp"
#include "../../Skeleton.hpp"

#include "../../Qaos/MakeDateTime.hpp"

#include <functional>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QUuid>
#include <QList>
#include <QPair>
#include <QThread>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	QVariant AbstractEntity::FieldVariant(const Field::Type& value_type, const QMetaType::Type& meta_type, const QSqlField& sql_field)
	{
		switch (meta_type) {
			case QMetaType::LongLong:
				return QVariant::fromValue(Meta::Qd(sql_field.value()));

			case QMetaType::QJsonDocument:
				return QVariant(QJsonDocument::fromJson(sql_field.value().toByteArray()).object());

			case QMetaType::QDateTime:
				return sql_field.value().isNull() ? QDateTime() : Field::Type::Timestamp == value_type ? QDateTime::fromSecsSinceEpoch(sql_field.value().toLongLong(), Qt::UTC) : Qaos::MakeDateTime(sql_field.value());

			case QMetaType::QUuid:
				return sql_field.value().toUuid();

			default:
				return sql_field.value();
		}
	}

	AbstractEntity::AbstractEntity(const QString& name, const QString& suffix)
	:
		QAbstractListModel(nullptr),

		_name(name),
		_sufix(suffix),
		_alias(suffix.isEmpty() ? name : name + "_" + suffix),
		_row_limit(0),
		_valid(false)
	{
	}

	bool AbstractEntity::open(Provider* provider, bool fetch_all)
	{
		return open(provider, fetch_all ? QFlags<Provider::OpenMode>(Provider::OpenMode::FetchAll) : QFlags<Provider::OpenMode>());
	}

	bool AbstractEntity::open(Provider* provider, QFlags<Provider::OpenMode> open_mode, Provider::RowsetType rowset_type)
	{
		if (!provider) {
			return false;
		} else if (thread() != provider->thread()) {
			qCritical() << "Rowset thread error: wrong thread";
			return false;
		} else if (thread() != QThread::currentThread()) {
			QCoreApplication::postEvent(this, new Data::Event(new EntityOpen(nullptr, provider, open_mode, rowset_type)));
			return true;
		}

		beginResetModel();
		validate(false);
		if (!open_mode.testFlag(Provider::OpenMode::WithoutReset)) {
			if (_row_set) {
				QObject::disconnect(_row_set.get(), &Rowset::transformed, nullptr, nullptr);
				QObject::disconnect(_row_set.get(), &Rowset::fetched, nullptr, nullptr);
			}

			switch (rowset_type) {
				case Provider::RowsetType::Shade:
					_row_set.reset(new ShadedRowset(provider));
					break;

				case Provider::RowsetType::Tele:
					_row_set.reset(new TeleRowset(provider));
					break;

				default:
					_row_set.reset(new Rowset(provider));
					/// beginInsertRows on rowsFetched
					break;
			}

			_row_set->setLimit(_row_limit);
			_row_set->setName(_alias);
			if (open_mode.testFlag(Provider::OpenMode::Reverse)) {
				_row_set->setReverse(!_row_set->getReverse());
			}

			onSelect();
			if (!_order_list.isEmpty()) {
				for (const OrderList::value_type& order : _order_list) {
					_row_set->addSort(order.first.get(), order.second);
				}
			} else {
				for (const Field& key_field : _primary_key.second) {
					_row_set->addSort(key_field, Qt::SortOrder::AscendingOrder);
				}
			}
			if (!_compound_predicate.isEmpty()) {
				_row_set->setFilter(_compound_predicate);
			}
			if (!_primary_index.second.empty()) {
				QSqlIndex primary_index("", _primary_index.first);
				for (const QSqlField& field : _primary_index.second) {
					primary_index.append(field);
				}
				_row_set->setIndex(primary_index);
			}

			if (!_row_set->initialize()) {
				qCritical() << "Rowset initialization error: " << _row_set->getError();
				endResetModel();
				return false;
			}

			QObject::connect(_row_set.get(), &Rowset::transformed, this, &AbstractEntity::onRowsetTransformed);

			if (open_mode.testFlag(Provider::OpenMode::FetchThrow)) {
				QObject::connect(_row_set.get(), &Rowset::fetched, std::bind(&AbstractEntity::onRowsetFetched, this, std::placeholders::_1, std::placeholders::_2));
			} else {
				QObject::connect(_row_set.get(), &Rowset::fetched, std::bind(&AbstractEntity::rowsetFetched, this, _row_set, std::placeholders::_1, std::placeholders::_2));
			}
		} else if (!_row_set) {
			endResetModel();
			return false;
		} else {
			_row_set->close();
		}

		_row_index = 0;

		qDebug() << "Rowset query: " << _row_set->getQuery().lastQuery();
		if (!_row_set->traverse(open_mode.testFlag(Provider::OpenMode::FetchAll))) {
			qCritical() << "Rowset fetch error: " << _row_set->getError();
			_row_index = 0;
			endResetModel();
			return false;
		}
		validate(true);

		qDebug() << "Rowset total size:" << _row_set->getLength() << " fetch offset:" << _row_set->getOffset();
		first();

		if (open_mode.testFlag(Provider::OpenMode::AutoReopen) && !open_mode.testFlag(Provider::OpenMode::WithoutReset)) {
			open_mode.setFlag(Provider::OpenMode::WithoutReset);
			open_mode.setFlag(Provider::OpenMode::AutoReopen, false);
			QObject::connect(provider, &Provider::connected, std::bind((bool(AbstractEntity::*)(Provider*, QFlags<Provider::OpenMode>, Provider::RowsetType))&AbstractEntity::open, this, provider, open_mode, rowset_type));
			QObject::connect(provider, &Provider::disconnected, this, &AbstractEntity::close);
		}
		emit rowsetOpened(_row_set->getLength());
		endResetModel();
		return true;
	}

	bool AbstractEntity::open(QSharedPointer<Rowset> row_set)
	{
		if (!row_set) {
			return false;
		} else if (_row_set) {
			QObject::disconnect(_row_set.get(), &Rowset::transformed, nullptr, nullptr);
			QObject::disconnect(_row_set.get(), &Rowset::fetched, nullptr, nullptr);
		}

		validate(false);
		beginResetModel();
		_row_set = row_set;
		_row_index = 0;
		if (!_row_set->initialize()) {
			endResetModel();
			return false;
		}
		QObject::connect(_row_set.get(), &Rowset::transformed, this, &AbstractEntity::onRowsetTransformed);
		validate(true);

		qDebug() << "Rowset total size:" << _row_set->getLength() << " fetch offset:" << _row_set->getOffset();
		first();

		emit rowsetOpened(_row_set->getLength());
		endResetModel();
		return true;
	}

	bool AbstractEntity::reopen(const Rowset::HintTable& hint_table)
	{
		if (!_row_set) {
			return false;
		} else if (thread() != QThread::currentThread()) {
			if (hint_table.isEmpty()) {
				QCoreApplication::postEvent(this, new QEvent(QEvent::Type::UpdateRequest));
			} else {
				QCoreApplication::postEvent(this, new Data::Event(new EntityReopen(nullptr, hint_table)));
			}
			return true;
		}

		validate(false);
		validate(_row_set->reopen(hint_table));

		return _valid;
	}

	bool AbstractEntity::close()
	{
		if (!_row_set) {
			return false;
		}

		if (thread() != QThread::currentThread()) {
			QCoreApplication::postEvent(this, new Data::Event(new EntityClose(nullptr)));
			return true;
		}

		beginResetModel();
		_row_set->close();
		_row_index = 0;
		endResetModel();

		return true;
	}

	bool AbstractEntity::first()
	{
		return locate(0);
	}

	bool AbstractEntity::last()
	{
		if (!_row_set) {
			return false;
		}
		_row_set->traverseAll();
		return locate(rowCount() - 1);
	}

	bool AbstractEntity::previous()
	{
		return locate(_row_index - 1);
	}

	bool AbstractEntity::next()
	{
		return locate(_row_index + 1);
	}

	bool AbstractEntity::move(CursorDirection direction)
	{
		switch (direction) {
			case CursorDirection::Forward:
				return next();

			case CursorDirection::Backward:
				return previous();

			default:
				return relocate();
		}
	}

	bool AbstractEntity::relocate()
	{		
		return locate(_row_index);
	}

	bool AbstractEntity::locate(int row_index)
	{
		if (!_row_set || row_index < 0) {
			return false;
		}
		QSqlRecord record(_row_set->seekRecord(row_index));
		if (record.isEmpty() && _row_set->isEOF()) {
			return false;
		}
		QMapIterator<AbstractEntity::FieldMap::key_type, FieldMap::mapped_type> f(_field_map);
		while (f.hasNext()) {
			Field& field(f.next().value().get());

			const QVariant value(record.value(field.name()));
			const QMetaType::Type meta_type(static_cast<QMetaType::Type>(field.type()));
			const Field::Type value_type(field.getValueType());
			switch (meta_type) {
				case QMetaType::LongLong:
					field.setValue(QVariant::fromValue(Meta::Qd(value)));
					break;

				case QMetaType::QJsonDocument:
					field.setValue(QJsonDocument::fromJson(value.toString().toUtf8()).object());
					break;

				case QMetaType::QDateTime:
					field.setValue(Field::Type::Timestamp == value_type ? QDateTime::fromSecsSinceEpoch(value.toLongLong(), Qt::UTC) : Qaos::MakeDateTime(value));
					break;

				case QMetaType::QUuid:
					field.setValue(value.toUuid());
					break;

				default:
					field.setValue(value);
			}
		}
		_row_index = row_index;
		return true;
	}

	QSqlRecord AbstractEntity::record(std::nullptr_t)
	{
		return _row_set->makeRecord();
	}

	QSqlRecord AbstractEntity::record(bool default_generated)
	{
		QSqlRecord retval(_row_set->makeRecord());
		if (!default_generated) {
			for (int f=retval.count() - 1; f >= 0; --f) {
				retval.setGenerated(f, false);
			}
		}
		return retval;
	}

	QSqlRecord AbstractEntity::record(int row_index)
	{
		QSqlRecord retval(_row_set->seekRecord(row_index));

		for (int v = retval.count()-1; v >= 0; --v) {
			FieldMap::ConstIterator f(_field_map.find(retval.fieldName(v)));
			if (f == _field_map.constEnd()) {
				continue;
			}
			const QMetaType::Type meta_type(static_cast<QMetaType::Type>(f.value().get().type()));
			const Field::Type value_type(f.value().get().getValueType());
			switch (meta_type) {
				case QMetaType::LongLong:
					retval.setValue(v, QVariant::fromValue(Meta::Qd(retval.value(v))));
					break;

				case QMetaType::QJsonDocument:
					retval.setValue(v, QJsonDocument::fromJson(retval.value(v).toString().toUtf8()).object());
					break;

				case QMetaType::QDateTime:
					retval.setValue(v, Field::Type::Timestamp == value_type ? QDateTime::fromSecsSinceEpoch(retval.value(v).toLongLong(), Qt::UTC) : retval.value(v).toDateTime());
					break;

				case QMetaType::QUuid:
					retval.setValue(v, retval.value(v).toUuid());
					break;

				default:
					continue;
			}
		}
		return retval;
	}

	QSqlRecord AbstractEntity::record()
	{
		return record(_row_index);
	}

	void AbstractEntity::fetchMore(const QModelIndex& index)
	{
		if (QThread::currentThread() != thread()) {
			QCoreApplication::postEvent(this, new Event(new EntityFetch(nullptr, index.row())));
		} else {
			locate(index.row());
		}
	}

	bool AbstractEntity::mutate(const Rowset::MutationList& list)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->mutate(list);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, list)));
		return true;
	}

	bool AbstractEntity::insert(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->insert(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Insert)));
		return true;
	}

	bool AbstractEntity::update(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->update(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Update)));
		return true;
	}

	bool AbstractEntity::remove(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->remove(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Remove)));
		return true;
	}

	bool AbstractEntity::patch(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->patch(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Patch)));
		return true;
	}

	bool AbstractEntity::ignore(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->ignore(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Ignore)));
		return true;
	}

	bool AbstractEntity::append(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->append(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Append)));
		return true;
	}

	bool AbstractEntity::prepend(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->prepend(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Prepend)));
		return true;
	}

	bool AbstractEntity::suspend(const QSqlRecord& record)
	{
		if (!_row_set) {
			return false;
		} else if (QThread::currentThread() == thread()) {
			return _row_set->suspend(record);
		}
		QCoreApplication::postEvent(this, new Data::Event(new EntityMutate(nullptr, record, Rowset::Mutation::Suspend)));
		return true;
	}

	void AbstractEntity::validate(bool onoff)
	{
		if (onoff != _valid) {
			_valid = onoff;
			emit validated(onoff);
		}
	}

	void AbstractEntity::setUserAlias(const QString& alias)
	{
		_user_alias = alias;
	}

	void AbstractEntity::setPrimaryKey(FieldSet&& set, const QString& alias)
	{
		_primary_key.first = alias.isEmpty() ? _name + "_PK" : alias;
		_primary_key.second = std::move(set);
	}

	void AbstractEntity::setPrimaryIndex(FieldSet&& set, const QString& alias)
	{
		_primary_index.first = alias.isEmpty() ? _name + "_IDX" : alias;
		_primary_index.second = std::move(set);
	}

	bool AbstractEntity::addField(Field& field)
	{
		FieldMap::const_iterator f(_field_map.insert(field.name(), std::ref(field)));
		if (_field_map.cend() != f) {
			field.setEntity(this);
			return true;
		}
		return false;
	}

	bool AbstractEntity::addField(Field& field, Qt::SortOrder sort)
	{
		FieldMap::const_iterator f(_field_map.insert(field.name(), std::ref(field)));		
		if (_field_map.cend() != f) {
			_order_list.push_back(qMakePair(std::cref(field), sort));
			field.setEntity(this);
			return true;
		}
		return false;
	}

	void AbstractEntity::addUniqueKey(FieldSet&& set, const QString& alias)
	{
		_unique_list.push_back(qMakePair<QString, FieldSet>(alias.isEmpty() ? _name + "_PK" : alias, std::move(set)));
	}

	void AbstractEntity::addForeignKey(ForeignKey& key)
	{
		_foreign_list.push_back(std::ref(key));
	}

	void AbstractEntity::permutateRows(int row1_index, int row2_index)
	{
		if (row1_index == row2_index) {
			return ;
		} else if (row1_index > row2_index) {
			std::swap(row1_index, row2_index);
		}

		if (!beginMoveRows(QModelIndex(), row2_index, row2_index, QModelIndex(), row1_index)) {
			return ;
		}
		endMoveRows();

		if (beginMoveRows(QModelIndex(), row1_index + 1, row1_index + 1, QModelIndex(), row2_index + 1)) {
			endMoveRows();
		}
	}

	void AbstractEntity::resetCompoundPredicate(CompoundPredicate::Logic logic)
	{
		_compound_predicate.clear();
		_compound_predicate.setLogic(logic);
	}

	void AbstractEntity::addPredicate(QSharedPointer<WherePredicate> predicate)
	{
		_compound_predicate.append(predicate);
	}

	void AbstractEntity::resetOrder()
	{
		_order_list.clear();
	}

	bool AbstractEntity::addOrder(const Field& field, Qt::SortOrder sort)
	{
		FieldMap::const_iterator f(_field_map.find(field.name()));
		if (_field_map.cend() != f && f.value().get() == field) {
			_order_list.push_back(qMakePair(std::ref(field), sort));
			return true;
		}
		return false;
	}

	void AbstractEntity::setRowLimit(qsizetype limit)
	{
		_row_limit = limit;
	}

	const QString& AbstractEntity::getName() const
	{
		return _name;
	}

	const QString& AbstractEntity::getSuffix() const
	{
		return _sufix;
	}

	const QString& AbstractEntity::getAlias() const
	{
		return _alias;
	}

	const QString& AbstractEntity::getUserAlias(bool failover) const
	{
		return (_user_alias.isEmpty() & failover) ? _alias : _user_alias;
	}

	const int& AbstractEntity::getRowIndex() const
	{
		return _row_index;
	}

	const AbstractEntity::RoleTable& AbstractEntity::getRoleTable() const
	{
		static RoleTable dummy{};
		return !_row_set ? dummy : _row_set->getRoleTable();
	}

	const AbstractEntity::NameTable& AbstractEntity::getNameTable() const
	{
		static NameTable dummy{};
		return !_row_set ? dummy : _row_set->getNameTable();
	}

	const AbstractEntity::FieldMap& AbstractEntity::getFieldMap() const
	{
		return _field_map;
	}

	const AbstractEntity::Constraint& AbstractEntity::getPrimaryKey() const
	{
		return _primary_key;
	}

	const AbstractEntity::ConstraintList& AbstractEntity::getUniqueKeyList() const
	{
		return _unique_list;
	}

	const AbstractEntity::ForeignList& AbstractEntity::getForeignKeyList() const
	{
		return  _foreign_list;
	}

	const CompoundPredicate& AbstractEntity::getCompoundPredicate() const
	{
		return _compound_predicate;
	}

	QVariant AbstractEntity::get(const QModelIndex& index, int role) const
	{
		if (!_row_set || !index.isValid() || Qt::UserRole > role) {
			return QVariant();
		} else if (Qt::UserRole == role) {
			return key(index);
		}

		const QSqlField field(_row_set->seekField(index.row(), role - Qt::UserRole - 1));
		const FieldMap::ConstIterator f(_field_map.find(field.name()));
		return (_field_map.constEnd() != f) ? AbstractEntity::FieldVariant(f.value().get().getValueType(), static_cast<QMetaType::Type>(f.value().get().type()), field) : QVariant();
	}

	int AbstractEntity::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() || !_row_set ? 0 : _row_set->getLength();
	}

	int AbstractEntity::rowIndex() const
	{
		return _row_index;
	}

	QVariantMap AbstractEntity::key(const QModelIndex& index) const
	{
		QVariantMap retval;
		for (const std::reference_wrapper<Field>& field : _primary_key.second) {
			retval[field.get().getAlias()] = get(index, _row_set->getNameTable().value(field.get().getAlias(), 0));
		}
		return retval;
	}

	QVariant AbstractEntity::data(const QModelIndex& index, int role) const
	{
		return get(index, role);
	}

	QVariant AbstractEntity::data(const QModelIndex &index, const QString& role) const
	{
		return get(index, getNameTable().value(role, Qt::UserRole - 1));
	}

	Qt::ItemFlags AbstractEntity::flags(const QModelIndex& index) const
	{
		return (!index.isValid() || !_valid) ? Qt::NoItemFlags : Qt::ItemIsEnabled;
	}

	QHash<int, QByteArray> AbstractEntity::roleNames() const
	{
		return getRoleTable();
	}

	QVariantMap AbstractEntity::roleKeys() const
	{
		QVariantMap retval;
		for (Rowset::NameTable::ConstIterator r = getNameTable().constBegin(); r != getNameTable().constEnd(); ++r) {
			retval.insert(r.key(), r.value());
		}
		return retval;
	}

	QSqlRecord AbstractEntity::makeRowsetRecord() const
	{
		return _row_set->makeRecord();
	}

	QSqlIndex AbstractEntity::makeRowsetIndex() const
	{
		return _row_set->makeIndex();
	}

	void AbstractEntity::onRowsetTransformed(QSet<int> inserted_index_set, QSet<int> removed_index_set, QSet<int> updated_index_set, QMap<int, int> moved_index_map)
	{
		static const std::function<QList<QPair<int, int>>(const QSet<int>&, Qt::SortOrder)> continuous_sequencer([](const QSet<int>& set, Qt::SortOrder order = Qt::SortOrder::AscendingOrder) -> QList<QPair<int, int>>
		{
			if (set.isEmpty()) {
				return {};
			} else if (set.count() == 1) {
				return {qMakePair(*(set.begin()), *(set.begin()))};
			}

			QList<int> sorted_list(set.values());
			std::sort(sorted_list.begin(), sorted_list.end());
			QList<QPair<int, int>> retval_list({qMakePair(sorted_list.first(), sorted_list.first() - 1)});

			for (const int& index : sorted_list) {
				if (index - retval_list.last().second == 1) {
					retval_list.last().second = index;
				} else {
					retval_list.push_back(qMakePair(index, index));
				}
			}
			if (Qt::SortOrder::DescendingOrder == order) {
				std::reverse(retval_list.begin(), retval_list.end());
			}
			return retval_list;
		});

		/**
		 * @xxx Так нельзя, потому что происходит скорей наоборот, 1 - обновляется, а 0 - добавляется
		 * @sample inserted{0, 1} & removed{0} -> updated{0} & inserted{1} обновиться, а после него вставят, но старый
		 * @todo Нужно залечить для оптимизации
		 */
/*
		const QSet<int> updated2_index_set(inserted_index_set & removed_index_set);
		inserted_index_set.subtract(updated2_index_set);
		removed_index_set.subtract(updated2_index_set);
		updated_index_set.unite(updated2_index_set);

		qCritical() << "Rowset transformed:" << updated2_index_set << inserted_index_set << removed_index_set << updated_index_set;
*/

		/// @todo Игнорировать непрофетченные области
		emit rowsetTransformed(inserted_index_set, removed_index_set, updated_index_set, moved_index_map);

		const QList<QPair<int, int>> removed_sequence_list(continuous_sequencer(removed_index_set, Qt::SortOrder::DescendingOrder));
		for (const QPair<int, int>& sequence : removed_sequence_list) {
			beginRemoveRows(QModelIndex(), sequence.first, sequence.second);
			endRemoveRows();
		}

		const QList<QPair<int, int>> inserted_sequence_list(continuous_sequencer(inserted_index_set, Qt::SortOrder::AscendingOrder));
		for (const QPair<int, int>& sequence : inserted_sequence_list) {
			beginInsertRows(QModelIndex(), sequence.first, sequence.second);
			endInsertRows();
		}

		for (const int& row_index : updated_index_set) {
			QModelIndex model_index(createIndex(row_index, 0));
			emit dataChanged(model_index, model_index, {});
		}

		if (moved_index_map.count() > 5000) {
			beginResetModel();
			endResetModel();
		} else if (!moved_index_map.isEmpty()) {
			emit layoutAboutToBeChanged();

			QModelIndexList index1_list;
			QModelIndexList index2_list;

			for (QMap<int, int>::Iterator m = moved_index_map.begin(); m != moved_index_map.end(); ++m) {
				index1_list.push_back(createIndex(m.key(), 0));
				index2_list.push_back(createIndex(m.value(), 0));
			}
			changePersistentIndexList(index1_list, index2_list);
			emit layoutChanged({}, QAbstractItemModel::VerticalSortHint);
		}
	}

	void AbstractEntity::onRowsetFetched(int begin_offset, int end_offset)
	{
		QSharedPointer<StaticRowset> row_set(new StaticRowset(_row_set->makeIndex(), _row_set->makeRecord(), end_offset - begin_offset + 1)) ;
		for (int r = begin_offset; r <= end_offset; ++r) {
			row_set->update(_row_set->seekRecord(r), r - begin_offset);
		}
		emit rowsetFetched(row_set, begin_offset, end_offset);
	}

	bool AbstractEntity::event(QEvent* event)
	{
		switch (event->type()) {
			case QEvent::Type::UpdateRequest:
				reopen();
				break;

			case QEvent::Type::LayoutRequest:
				beginResetModel();
				if (_row_set) {
					onRowsetFetched(0, _row_set->getOffset());
				}
				emit rowsetOpened(_row_set ? _row_set->getLength() : 0);
				endResetModel();
				break;

			default:
				return QAbstractListModel::event(event);
		}
		return true;
	}

	void AbstractEntity::customEvent(QEvent* event)
	{
		if (event->type() == Core::Data::Event::Action<EntityFetch>::RegisterEventType()) {
			locate(Core::Data::Event::TypeCast<EntityFetch>(event)->getRowIndex());
		} else if (event->type() == Core::Data::Event::Action<EntityOpen>::RegisterEventType()) {
			EntityOpen* command(Core::Data::Event::TypeCast<EntityOpen>(event));
			open(command->getProvider(), command->getModeSet(), command->getRowsetType());
		} else if (event->type() == Core::Data::Event::Action<EntityReopen>::RegisterEventType()) {
			EntityReopen* command(Core::Data::Event::TypeCast<EntityReopen>(event));
			reopen(command->getHintTable());
		} else if (event->type() == Core::Data::Event::Action<EntityClose>::RegisterEventType()) {
			/*EntityClose* command(*/Core::Data::Event::TypeCast<EntityClose>(event)/*)*/;
			close();
		} else if (event->type() == Core::Data::Event::Action<EntityMutate>::RegisterEventType()) {
			EntityMutate* command(Core::Data::Event::TypeCast<EntityMutate>(event));
			mutate(command->getList());
		}
	}

	void AbstractEntity::onCreate()
	{

	}

	void AbstractEntity::onForeign()
	{

	}

	bool AbstractEntity::hasRowset() const
	{
		return nullptr != _row_set;
	}

	bool AbstractEntity::hasPrimaryKey() const
	{
		return !_primary_key.second.empty();
	}

	bool AbstractEntity::hasUserAlias() const
	{
		return !_user_alias.isEmpty();
	}

	bool AbstractEntity::isOpened() const
	{
		return hasRowset() && _row_set->isLoaded();
	}

	bool AbstractEntity::isEmpty() const
	{
		return rowCount() <= 0;
	}

	bool AbstractEntity::isEOF() const
	{
		return (isEmpty() || (_row_index >= _row_set->getOffset()));
	}

	bool AbstractEntity::isEOS() const
	{
		return (isEmpty() || (_row_index >= _row_set->getLength() - 1));
	}

	bool AbstractEntity::isBOF() const
	{
		return (0 == rowCount() || 0 == _row_index);
	}

	bool AbstractEntity::isBOS() const
	{
		return (0 == rowCount() || 0 == _row_index);
	}

	bool AbstractEntity::isValid() const
	{
		return _valid;
	}

	bool AbstractEntity::hasIndex(const QSqlIndex& index) const
	{
		return hasAnyIndex({index});
	}

	bool AbstractEntity::hasAnyIndex(const QList<QSqlIndex>& index_list) const
	{
		return _row_set->hasIndex(index_list);
	}
} } } }
