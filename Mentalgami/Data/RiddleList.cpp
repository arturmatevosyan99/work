#include "RiddleList.hpp"

#include "../Meta/RiddleRender.hpp"
#include "../Meta/RiddleInformation.hpp"

#include <QJsonArray>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Data {
	QVector<RiddleList::Tuple> RiddleList::Parse(const QJsonValue& value)
	{
		if (!value.isArray()) {
			qWarning() << "The received data is not an array";
			return {};
		}
		QVector<RiddleList::Tuple> retval;
		const QJsonArray input(value.toArray());
		for (QJsonArray::ConstIterator i = input.constBegin(); i != input.constEnd(); i++) {
			if (!i->isObject()) {
				qWarning() << "The array element is malformed";
				continue;
			}
			const QJsonObject object(i->toObject());
			retval.append({Core::Meta::Qd(object["scheme"].toString("-1")), object, nullptr});
		}
		return retval;
	}

	RiddleList::RiddleList(QObject* parent)
	:
		QAbstractListModel(parent)
	{

	}

	bool RiddleList::append(QVector<Tuple>&& vector)
	{
		if (vector.isEmpty()) {
			return false;
		}
		beginInsertRows(QModelIndex(), _vector.size(), _vector.size() + vector.size() - 1);
		_vector.append(std::move(vector));
		endInsertRows();
		return true;
	}

	bool RiddleList::compose(QQmlComponent* component)
	{
		if (!component) {
			return false;
		}
		beginInsertRows(QModelIndex(), _vector.size(), _vector.size());
		_vector.append({0, {}, component});
		endInsertRows();
		return true;
	}

	bool RiddleList::removeTail(int index)
	{
		const int start_index(index + 1);
		if (start_index < 0 || start_index >= _vector.size()) {
			return false;
		}
		beginRemoveRows(QModelIndex(), start_index, _vector.size() - 1);
		_vector.remove(start_index, _vector.size() - start_index);
		endRemoveRows();
		return true;
	}

	int RiddleList::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _vector.count();
	}

	QVariant RiddleList::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {
			case Meta::RiddleInformation::Role::Scheme: return QVariant::fromValue(std::get<Core::Meta::Qd>(_vector.at(index.row())));
			case Meta::RiddleInformation::Role::Object: return QVariant::fromValue(std::get<QJsonObject>(_vector.at(index.row())));
			case Meta::RiddleInformation::Role::Delegate: return QVariant::fromValue(std::get<QQmlComponent*>(_vector.at(index.row())));

			default: return QVariant();
		}
	}

	QHash<int, QByteArray> RiddleList::roleNames() const
	{
		QHash<int, QByteArray> retval;

		retval[Meta::RiddleInformation::Role::Scheme] = "scheme";
		retval[Meta::RiddleInformation::Role::Object] = "object";
		retval[Meta::RiddleInformation::Role::Delegate] = "delegate";

		return retval;
	}

	bool RiddleList::isEmpty() const
	{
		return _vector.isEmpty();
	}
} } } }
