#include "Environment.hpp"

#include <QDebug>

namespace Cadabra { namespace Queen {
	Environment& Environment::GlobalInstance()
	{
		static Environment Instance(Environment(nullptr));
		return Instance;
	}

	Environment::Environment(QObject* parent)
	:
		QAbstractListModel(parent)
	{

	}

	void Environment::updateModel()
	{
		beginResetModel();
		endResetModel();
	}

	QVariant Environment::getVariable(const QString& key, const QVariant& default_value) const
	{
		return _map.value(key, default_value);
	}

	int Environment::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _map.size();
	}

	QVariant Environment::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {
			case Role::VariableRole: return (_map.constBegin() + index.row()).key();
			case Role::ValueRole: return (_map.constBegin() + index.row()).value();

			default: return QVariant();
		}
	}

	QHash<int, QByteArray> Environment::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractListModel::roleNames());

		retval[Role::VariableRole] = "variable";
		retval[Role::ValueRole] = "value";

		return retval;
	}

	bool Environment::setVariable(const QString& key, const QVariant& value)
	{
		if (key.isEmpty()) {
			return false;
		}
		QVariantMap::Iterator v(_map.find(key));
		if (value.isValid()) {
			if (_map.end() == v) {
				const int index(_map.isEmpty() ? 0 : std::distance(_map.begin(), _map.upperBound(key)));
				beginInsertRows(QModelIndex(), index, index);
				v = _map.insert(key, value);
				endInsertRows();
			} else {
				setData(createIndex(std::distance(_map.begin(), v), 0), value, Role::ValueRole);
			}
			return true;
		} else if (_map.end() != v) {
			const int index(std::distance(_map.begin(), v));
			beginRemoveRows(QModelIndex(), index, index);
			v = _map.erase(v);
			endRemoveRows();
			return true;
		}
		return false;
	}

	bool Environment::setData(const QModelIndex& index, const QVariant& value, int role)
	{
		if (!index.isValid()) {
			return false;
		} else if (role != ValueRole) {
			return false;
		}

		(_map.begin() + index.row()).value() = value;
		emit dataChanged(index, index, { role });

		return true;
	}
} }
