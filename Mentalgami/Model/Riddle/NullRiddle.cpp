#include "NullRiddle.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	NullRiddle::NullRiddle(const QString& text, const QString& descriptor)
	:
		TemplatedRiddle<std::nullptr_t>(text, descriptor)
	{

	}

	QJsonObject NullRiddle::exportBody() const
	{
		QJsonObject retval;
		retval.insert("image_source", _image_source);

		return retval;
	}

	bool NullRiddle::importBody(const QJsonObject& body)
	{
		const QJsonValue image_source(body["image_source"]);
		if (image_source.isString()) {
			_image_source = image_source.toString();
			emit imageSourceChanged();
		} else {
			qWarning() << "Something is wrong with the 'source' parameter, the value has not changed";
			return false;
		}

		return true;
	}

	const QString& NullRiddle::getImageSource() const
	{
		return _image_source;
	}

	QString NullRiddle::getType() const
	{
		return QString("Null");
	}
} } } }
