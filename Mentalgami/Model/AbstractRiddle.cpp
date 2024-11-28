#include "AbstractRiddle.hpp"

#include "../Meta/RiddleRender.hpp"

#include <cmath>

#include <QJsonArray>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	bool AbstractRiddle::isInt(const QJsonValue& value)
	{
		return value.isDouble() && (std::trunc(value.toDouble()) == value.toDouble());
	}

	AbstractRiddle::AbstractRiddle(const QString& text, const QString& description, RiddleValidator* validator)
	:
		_text(text),
		_description(description),

		_key(-1),
		_component(-1),

		_specifications(Meta::RiddleInformation::Specification::Empty),
		_validator(validator),
		_is_busy(false)
	{
		if (_validator) {
			_validator->setParent(this);
		}
	}

	QJsonObject AbstractRiddle::dump(bool all_inclusive) const
	{
		/// @note Если all_inclusive == true, то экспортируем все данные Riddle'а
		/// В противном случае - экспортируем только то, что могло измениться на Queen'е или необходимо для записи в БД

		QJsonObject retval;

		if (all_inclusive) {
			retval = exportBody();

			retval.insert("text", _text);
			retval.insert("description", _description);

			retval.insert("component", _component);
			retval.insert("specifications", static_cast<int>(_specifications));

			QJsonArray media_array;
			for (const QUrl& media_element : _media_list) {
				media_array.push_back(QJsonValue(media_element.toString()));
			}

			retval.insert("media_list", media_array);
		}

		retval.insert("answer", QJsonValue::fromVariant(exportAnswer()));
		retval.insert("key", _key);
		retval.insert("type", getType());

		return retval;
	}

	/// Параметр all_inclusive сейчас не используется
	bool AbstractRiddle::parse(const QJsonObject& body, bool all_inclusive)
	{
		/// @note Если all_inclusive == true, то импортируем все данные Riddle'а
		/// В противном случае - импортируем только то, что необходимо Queen'у

		if (!importBody(body)) {
			return false;
		}

		/// @note Принимать ответ с Туза не всегда имеет смысл
//		if (!importAnswer(body["answer"].toVariant())) {
//			return false;
//		}

		const QJsonValue text(body["text"]);
		if (text.isString()) {
			_text = text.toString();
			emit textChanged();
		} else {
			qWarning() << "Something is wrong with the 'text' parameter, the value has not changed";
			return false;
		}

		const QJsonValue description(body["description"]);
		if (description.isString()) {
			_description = description.toString();
			emit descriptionChanged();
		} else {
			qWarning() << "Something is wrong with the 'description' parameter, the value has not changed";
			return false;
		}

		const QJsonValue key(body["key"]);
		if (isInt(key)) {
			_key = key.toInt();
		} else {
			qWarning() << "Something is wrong with the 'key' parameter, the value has not changed";
			return false;
		}

		const QJsonValue component(body["component"]);
		if (isInt(component)) {
			_component = component.toInt();
			emit componentChanged();
		} else {
			qWarning() << "Something is wrong with the 'component' parameter, the value has not changed";
			return false;
		}

		const QJsonValue specifications(body["specifications"]);
		if (isInt(specifications)) {
			_specifications = static_cast<Meta::RiddleInformation::Specification>(specifications.toInt());
			emit specificationsChanged();
		} else {
			qWarning() << "Something is wrong with the 'specifications' parameter, the value has not changed";
			return false;
		}

		const QJsonValue media_list(body["media_list"]);
		if (media_list.isArray()) {
			const QJsonArray media_array(media_list.toArray());
			for (QJsonArray::ConstIterator i = media_array.constBegin(); i != media_array.constEnd(); i++) {
				if (i->isString()) {
					_media_list.push_back(i->toString());
					emit mediaListChanged();
				}
			}
		}

		if (!_validator->import(body)) {
			return false;
		}

		return true;
	}

	bool AbstractRiddle::validate(const QVariant& answer)
	{
		if (!_validator->validate(answer)) {
			return false;
		}

		return importAnswer(answer);
	}

	const QString& AbstractRiddle::getText() const
	{
		return _text;
	}

	const QString& AbstractRiddle::getDescription() const
	{
		return _description;
	}

	int AbstractRiddle::getComponent() const
	{
		return _component;
	}

	QFlags<Meta::RiddleInformation::Specification> AbstractRiddle::getSpecifications() const
	{
		return _specifications;
	}

	const QVector<QUrl>& AbstractRiddle::getMediaList() const
	{
		return _media_list;
	}

	RiddleValidator* AbstractRiddle::getValidator() const
	{
		return _validator;
	}

	Cadabra::Queen::Core::Meta::Qd AbstractRiddle::getScheme() const
	{
		const int type_id(QMetaType::type(QString("*").prepend(this->metaObject()->className()).toLatin1().data()));
		return QMetaType::UnknownType == type_id ? 0 : Core::Meta::Scheme<Meta::RiddleRender>::Ident(QMetaType(type_id));
	}

	void AbstractRiddle::setText(const QString& text)
	{
		if (_text != text) {
			_text = text;
			emit textChanged();
		}
	}

	void AbstractRiddle::setDescription(const QString& description)
	{
		if (_description != description) {
			_description = description;
			emit descriptionChanged();
		}
	}

	void AbstractRiddle::setSpecifications(const QFlags<Meta::RiddleInformation::Specification>& specifications)
	{
		if (_specifications != specifications) {
			_specifications = specifications;
			emit specificationsChanged();
		}
	}

	void AbstractRiddle::setMediaList(const QVector<QUrl>& media_list)
	{
		if (_media_list != media_list) {
			_media_list = media_list;
			emit mediaListChanged();
		}
	}

	void AbstractRiddle::setBusy(bool busy)
	{
		if (_is_busy != busy) {
			_is_busy = busy;
			emit busyChanged();
		}
	}

	bool AbstractRiddle::isReadOnly() const
	{
		return _specifications & Meta::RiddleInformation::Specification::ReadOnly;
	}

	bool AbstractRiddle::isBusy() const
	{
		return _is_busy;
	}
} } } }
