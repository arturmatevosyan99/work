#include "mg_TextRiddle.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	TextRiddle::TextRiddle(const QString& text, const QString& descriptor, Validator* validator)
	:
		TemplatedRiddle<QString>(text, descriptor, validator)
	{

	}

	QJsonObject TextRiddle::exportBody() const
	{
		QJsonObject retval;
		retval.insert("placeholder", _placeholder);

		return retval;
	}

	bool TextRiddle::importBody(const QJsonObject& body)
	{
		const QJsonValue placeholder(body["placeholder"]);
		if (placeholder.isString()) {
			_placeholder = placeholder.toString();
			emit placeholderChanged();
		} else {
			qWarning() << "Something is wrong with the 'placeholder' parameter, the value has not changed";
			return false;
		}

		return true;
	}

	const QString& TextRiddle::getPlaceholder() const
	{
		return _placeholder;
	}

	QString TextRiddle::getType() const
	{
		return QString("Text");
	}
} } } }
