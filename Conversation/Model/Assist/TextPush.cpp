#include "TextPush.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	TextPush::TextPush(QObject* parent)
	:
		AbstractAssist(-8034428181974876158, parent)
	{
	}

	QJsonObject TextPush::exportBody() const
	{
		return QJsonObject{
                            {"prefix", _prefix},
							{"label", _label},
							{"value", _value}
		};
	}

	bool TextPush::importBody(const QJsonObject& body)
	{
		setLabel(body.value("label").toString(""));
		if (_label.isEmpty()) {
			return false;
		}

		setValue(body.value("value").toString(""));
		setPrefix(body.value("prefix").toString(""));
		return true;
	}

    QString TextPush::text()
    {
        return  _prefix.isEmpty() ? _label : QString("%1 %2").arg(_prefix, _label);
    }

	const QString& TextPush::getLabel() const
	{
		return _label;
	}

	const QString& TextPush::getValue() const
	{
		return _value.isEmpty() ? _label : _value;
	}

    const QString& TextPush::getPrefix() const
    {
        return _prefix;
    }

	Meta::Assist::TargetLayout TextPush::getDefaultTargetLayout() const
	{
		return Meta::Assist::TargetLayout::FeedFooter;
	}

	QString TextPush::getPreviewText() const
	{
		return _label;
	}

	void TextPush::setLabel(const QString& label)
	{
		if (_label != label) {
			_label = label;
			emit labelChanged();
		}
	}

	void TextPush::setValue(const QString& value)
	{
		if (_value != value) {
			_value = value;
			emit valueChanged();
		}
	}

    void TextPush::setPrefix(const QString& prefix)
    {
        if (_prefix != prefix) {
            _prefix = prefix;
            emit prefixChanged();
        }
    }
} } } }
