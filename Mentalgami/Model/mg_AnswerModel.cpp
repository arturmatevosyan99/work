#include "mg_AnswerModel.hpp"

#include <QJsonArray>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	AnswerModel::AnswerModel(QObject* parent)
	:
		QAbstractListModel(parent)
	{

	}

	AnswerModel::AnswerModel(const Model::AnswerModel& answer_model)
	{

	}

	void AnswerModel::addAnswer(const QString& key, const QString& value)
	{
		beginInsertRows(QModelIndex(), _vector.size(), _vector.size());
		_vector.append(QPair<QString, QString>(key, value));
		endInsertRows();
	}

	int AnswerModel::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _vector.count();
	}

	QVariant AnswerModel::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {
			case Role::AnswerKey: return QVariant::fromValue(_vector.at(index.row()).first);
			case Role::AnswerValue: return QVariant::fromValue(_vector.at(index.row()).second);

			default: return QVariant();
		}
	}

	QHash<int, QByteArray> AnswerModel::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractListModel::roleNames());

		retval[Role::AnswerKey] = "answerKey";
		retval[Role::AnswerValue] = "answerValue";

		return retval;
	}
} } } }
