#include "TemplatedRiddle.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<QString>::importAnswer(const QVariant& answer)
	{
		if (answer.canConvert<QString>()) {
			const QString converted_answer(answer.toString());
			if (_answer != converted_answer) {
				_answer = converted_answer;
				emit answerChanged();
			}
		} else {
			qWarning() << "TemplatedRiddle<QString>::importAnswer > Something is wrong with the answer, the value has not changed";
			return false;
		}

		return true;
	}

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<Qaos::Date>::importAnswer(const QVariant& answer)
	{
		Qaos::Date converted_answer;

		if (answer.canConvert<Qaos::Date>()) {
			converted_answer = answer.value<Qaos::Date>();
		} else if (answer.canConvert<QDate>()) {
			converted_answer = Qaos::Date(answer.value<QDate>().day(), answer.value<QDate>().month(), answer.value<QDate>().year());
		} else {
			qWarning() << "TemplatedRiddle<Qaos::Date>::importAnswer > Something is wrong with the answer, the value has not changed";
			return false;
		}

		if (_answer != converted_answer) {
			_answer = converted_answer;
			emit answerChanged();
		}

		return true;
	}

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<bool>::importAnswer(const QVariant& answer)
	{
		if (answer.canConvert<bool>()) {
			const bool converted_answer(answer.toBool());
			if (_answer != converted_answer) {
				_answer = converted_answer;
				emit answerChanged();
			}
		} else {
			qWarning() << "TemplatedRiddle<bool>::importAnswer > Something is wrong with the answer, the value has not changed";
			return false;
		}

		return true;
	}

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<std::nullptr_t>::importAnswer(const QVariant& answer)
	{
		if (_answer != nullptr) {
			emit answerChanged();
		}

		return true;
	}

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<double>::importAnswer(const QVariant& answer)
	{
		if (answer.canConvert<double>()) {
			const double converted_answer(answer.toDouble());
			if (_answer != converted_answer) {
				_answer = converted_answer;
				emit answerChanged();
			}
		} else {
			qWarning() << "TemplatedRiddle<double>::importAnswer > Something is wrong with the answer, the value has not changed";
			return false;
		}

		return true;
	}

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<QSet<QString>>::importAnswer(const QVariant& answer)
	{
		if (answer.canConvert<QStringList>()) {
			QSet<QString> converted_answer;
			const QStringList string_list(answer.value<QStringList>());

			for (const QString& element : string_list) {
				converted_answer.insert(element);
			}

			if (_answer != converted_answer) {
				_answer = converted_answer;
				emit answerChanged();
			}
		} else {
			qWarning() << "TemplatedRiddle<QSet<QString>>::importAnswer > Something is wrong with the answer, the value has not changed";
			return false;
		}

		return true;
	}

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<int>::importAnswer(const QVariant& answer)
	{
		if (answer.canConvert<int>()) {
			const int converted_answer(answer.toInt());
			if (_answer != converted_answer) {
				_answer = converted_answer;
				emit answerChanged();
			}
		} else {
			qWarning() << "TemplatedRiddle<int>::importAnswer > Something is wrong with the answer, the value has not changed";
			return false;
		}

		return true;
	}

	template<>
	QVariant Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<Qaos::Date>::exportAnswer() const
	{
		QJsonObject answer;

		answer.insert("day", _answer.getDay());
		answer.insert("month", _answer.getMonth());
		answer.insert("year", _answer.getYear());

		return QVariant::fromValue(answer);
	}

	template<>
	QVariant Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<std::nullptr_t>::exportAnswer() const
	{
		return QVariant::fromValue(nullptr);
	}

	template<>
	QVariant Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<QSet<QString>>::exportAnswer() const
	{
		return QVariant(_answer.values());
	}
} } } }
