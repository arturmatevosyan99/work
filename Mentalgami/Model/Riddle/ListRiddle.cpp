#include "ListRiddle.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	ListRiddle::ListRiddle(const QString& text, const QString& descriptor, Validator* validator)
	:
		TemplatedRiddle<QSet<QString>>(text, descriptor, validator),

		_answer_model(new Mentalgami::Model::AnswerModel)
	{

	}

	QJsonObject ListRiddle::exportBody() const
	{
		QJsonObject retval;
		QJsonArray answer_vector;

		for (int i = 0; i != getAnswerModel()->rowCount(); i++) {
			QJsonArray pair;
			pair.append(getAnswerModel()->index(i, 0).data(AnswerModel::Role::AnswerKey).toJsonValue());
			pair.append(getAnswerModel()->index(i, 0).data(AnswerModel::Role::AnswerValue).toJsonValue());
			answer_vector.append(QJsonValue(pair));
		}

		retval.insert("answers", answer_vector);

		return retval;
	}

	bool ListRiddle::importBody(const QJsonObject& body)
	{
		const QJsonValue answer_vector(body["answers"]);
		if (answer_vector.isArray()) {
			const QJsonArray answer_vector_array(answer_vector.toArray());
			for (QJsonArray::ConstIterator i = answer_vector_array.constBegin(); i != answer_vector_array.constEnd(); i++) {
				if (i->isArray()) {
					const QJsonArray answer_pair(i->toArray());
					if (answer_pair.at(0).isString() && answer_pair.at(1).isString()) {
						_answer_model->addAnswer(answer_pair.at(0).toString(), answer_pair.at(1).toString());
						emit answerModelChanged();
					} else {
						qWarning() << "Something is wrong with the elements of the pair of the 'answer_vector' parameter, the pair is not added";
						return false;
					}
				} else {
					qWarning() << "Something is wrong with the element of the 'answer_vector', it is not added";
					return false;
				}
			}
		} else {
			qWarning() << "Something is wrong with the 'answer_vector' parameter, the value has not changed";
			return false;
		}

		return true;
	}

	QString ListRiddle::getType() const
	{
		return QString("List");
	}

	QAbstractListModel* ListRiddle::getAnswerModel() const
	{
		return _answer_model.get();
	}
} } } }
