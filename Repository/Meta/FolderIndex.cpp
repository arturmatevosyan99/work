#include "FolderIndex.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	FolderIndex::FolderIndex(QObject *parent)
	:
		QAbstractListModel(parent)
	{
	}

	bool FolderIndex::parse(const QJsonValue& value)
	{
		return value.isArray() ? parse(value.toArray()) : false;
	}

	bool FolderIndex::parse(const QJsonArray& value)
	{
		if (value.isEmpty() && _object_map.isEmpty()) {
			return false;
		}
		beginResetModel();
		_object_map.clear();
		Map object_map(Meta::Object::Parse(value));
		foreach (const QSharedPointer<const Meta::Object>& o, object_map) {
			_object_map.insert(o->getId(), o);
			qDebug() << "Option:" << o->getOption();
		}
		endResetModel();
		return true;
	}

	void FolderIndex::add(const QSharedPointer<const Meta::Object>& object)
	{
		Map::ConstIterator o(_object_map.lowerBound(object->getId()));
/*
		if (o == _object_map.cend()) { /// < Добавление в конец
			beginInsertRows(QModelIndex(), _object_map.size(), _object_map.size());
		} else {
			const int position(o - _object_map.constBegin());
			beginInsertRows(QModelIndex(), position, position);
		}
*/
		beginResetModel();
		_object_map.insert(object->getId(), object);
		endResetModel();
//		endInsertRows();
	}

	void FolderIndex::add(const Map& object_map)
	{
		if (object_map.empty()) {
			return ;
		}
//		beginInsertRows(QModelIndex(), _object_map.size(), _object_map.size() + object_vector.size() - 1);
		beginResetModel();
		foreach (const QSharedPointer<const Meta::Object>& o, object_map) {
			_object_map.insert(o->getId(), o);
		}
		endResetModel();
//		endInsertRows();
	}

	void FolderIndex::remap(const Map& object_map)
	{
		beginResetModel();
		_object_map = object_map;
		endResetModel();
	}

	void FolderIndex::swap(Map&& object_map)
	{
		beginResetModel();
		_object_map.clear();
		_object_map.swap(object_map);
		endResetModel();
	}

	int FolderIndex::rowCount(const QModelIndex& parent) const
	{
		if (parent.isValid()) {
			return 0;
		}
		return _object_map.size();
	}

	QVariant FolderIndex::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}
		switch (role) {
			case Role::Id: return (_object_map.constBegin() + index.row()).value()->getId();
			case Role::ParentId: return (_object_map.constBegin() + index.row()).value()->getParentId();
			case Role::Name: return (_object_map.constBegin() + index.row()).value()->getName();
			case Role::ApplicationScheme: return (_object_map.constBegin() + index.row()).value()->getApplicationScheme();
			case Role::Option: return (_object_map.constBegin() + index.row()).value()->getOption();
			case Role::Picture: return (_object_map.constBegin() + index.row()).value()->getPicture();
			case Role::CreationTimestamp: return (_object_map.constBegin() + index.row()).value()->getCreationDateTime();
			case Role::ModificationTimestamp: return (_object_map.constBegin() + index.row()).value()->getModificationDateTime();

			default: return QVariant();
		}
	}

	QHash<int, QByteArray> FolderIndex::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractListModel::roleNames());
		retval[Role::Id] = "id";
		retval[Role::ParentId] = "parent_id";
		retval[Role::Name] = "name";
		retval[Role::ApplicationScheme] = "application_scheme";
		retval[Role::Option] = "option";
		retval[Role::Picture] = "picture";
		retval[Role::CreationTimestamp] = "creation_timestamp";
		retval[Role::ModificationTimestamp] = "modification_timestamp";

		return retval;
	}
} } } }
