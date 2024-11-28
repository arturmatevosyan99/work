#include "TextMessage.hpp"

#include <QJsonObject>
#include <QGuiApplication>
#include <QRegularExpression>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	bool TextMessage::EmailStyle(QString& input)
	{
		QRegularExpression expression(R"(\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*)", QRegularExpression::CaseInsensitiveOption);
		QRegularExpressionMatchIterator m(expression.globalMatch(input));
		QSet<QString> match_set;
		while (m.hasNext()) {
			match_set += m.next().captured(0);
		}
		if (match_set.isEmpty()) {
			return false;
		}
		for (const QString& match : qAsConst(match_set)) {
			input.replace(match, QString(match).prepend("<a href=\"mailto:" + match + "\">").append("</a>"));
		}
		return true;
	}

	bool TextMessage::HyperlinkStyle(QString& input)
	{
		QRegularExpression hyperlink_regex(R"((?:https?|ftp)://\S+)", QRegularExpression::CaseInsensitiveOption);
		QRegularExpressionMatchIterator u(hyperlink_regex.globalMatch(input));
		QSet<QString> match_set;
		while (u.hasNext()) {
			match_set += u.next().captured(0);
		}
		if (match_set.isEmpty()) {
			return false;
		}
		for (const QString& match : qAsConst(match_set)) {
			input.replace(match, QString(match.size() > 70 ? match.left(70) + "..." : match).prepend("<a href=\"" + match + "\">").append("</a>"));
		}
		return true;
	}

	bool TextMessage::GeneralStyle(QString& input)
	{
		input.replace('\n', "<br />");
		return true;
	}

	TextMessage::TextMessage(QObject* parent)
	:
		AbstractMessage(parent),

		_format(Format::Auto)
	{
	}

	QJsonObject TextMessage::exportBody() const
	{
		return QJsonObject{
							{"text", _text},
							{"format", static_cast<char>(_format)}
		};
	}

	bool TextMessage::importBody(const QJsonObject& body)
	{
		_format = static_cast<Format>(body["format"].toString("A").at(0).toLatin1());
		setText(body["text"].toString());

		return true;
	}

	bool TextMessage::clipBody() const
	{
		QGuiApplication::clipboard()->setText(_text);
		return true;
	}

	QString TextMessage::getPreviewText() const
	{
		return _text;
	}

	QString TextMessage::getText() const
	{
		return _formatted_text.isEmpty() ? _text : _formatted_text;
	}

	TextMessage::Format TextMessage::getFormat() const
	{
		return _format;
	}

	void TextMessage::setText(const QString& text)
	{
		if (_text != text) {
			_formatted_text = text;
			if (Format::Auto != _format) {
				_text = text;
				_text.remove(QRegExp("<[^>]*>"));
			} else if (_formatted_text.contains(QRegExp("<[^>]*>"))) {
				if (_formatted_text.contains("://")) {
					_content.setFlag(AbstractMessage::Content::Hyperlink);
				}
				if (_formatted_text.contains("@")) {
					_content.setFlag(AbstractMessage::Content::Hyperlink);
				}
				_format = TextMessage::Format::Styled;
				_text = text;
				_text.remove(QRegExp("<[^>]*>"));
			} else if (!_formatted_text.isEmpty()) {
				if (_formatted_text.contains("://") && HyperlinkStyle(_formatted_text)) {
					_content.setFlag(AbstractMessage::Content::Hyperlink);
				}
				if (_formatted_text.contains("@") && EmailStyle(_formatted_text)) {
					_content.setFlag(AbstractMessage::Content::Hyperlink);
				}

				if (_content.testFlag(AbstractMessage::Content::Hyperlink) && GeneralStyle(_formatted_text)) {
					_format = TextMessage::Format::Styled;
					_text = text;
				} else {
					_text.clear();
					_text.swap(_formatted_text);
				}
			}
			emit textChanged();
		}
	}

	void TextMessage::setFormat(const Format& format)
	{
		if (_format != format) {
			_format = format;
			emit textChanged();
		}
	}

	bool TextMessage::isEditable() const
	{
		return isImported() && getChannelDescriptor()->getSubjectId() == getAuthorSubjectId();
	}

	bool TextMessage::hasClipboard() const
	{
		return true;
	}
} } } }
