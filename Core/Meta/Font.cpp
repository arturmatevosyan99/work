/**
 @file Font.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Font.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	QFont Font::Parse(const QFont& source, const QJsonObject& object)
	{
		QFont retval(source);
		QJsonObject::ConstIterator f(object.find("family"));
		QJsonObject::ConstIterator c(object.find("case"));
		QJsonObject::ConstIterator w(object.find("weight"));
		QJsonObject::ConstIterator i(object.find("italic"));
		QJsonObject::ConstIterator u(object.find("underline"));
		QJsonObject::ConstIterator s(object.find("strike"));
		QJsonObject::ConstIterator z(object.find("size"));
		QJsonObject::ConstIterator ls(object.find("letter-spacing"));
		QJsonObject::ConstIterator ws(object.find("word-spacing"));

		if (object.constEnd() != f && f->isString()) {
			const QString font_family(f->toString());
			retval.setFamily(font_family);
			if (retval.family() != font_family) {
				qWarning() << "Font " << font_family << " does not exist";
			}
		}
		if (object.constEnd() != c && c->isString()) {
			const QString case_style(c->toString("mixed").toLower());
			if ("mixed" == case_style) {
				retval.setCapitalization(QFont::Capitalization::MixedCase);
			} else if ("upper" == case_style) {
				retval.setCapitalization(QFont::Capitalization::AllUppercase);
			} else if ("lower" == case_style) {
				retval.setCapitalization(QFont::Capitalization::AllLowercase);
			} else if ("smallcaps" == case_style) {
				retval.setCapitalization(QFont::Capitalization::SmallCaps);
			} else if ("capitalize" == case_style) {
				retval.setCapitalization(QFont::Capitalization::Capitalize);
			}
		}
		if (object.constEnd() != w) {
			if (w->isDouble()) {
				retval.setWeight(w->toInt(0));
			} else if (w->isString()) {
				const QString weight(w->toString("regular").toLower());
				if (weight == "regular") {
					retval.setWeight(50);
				} else if (weight == "thin") {
					retval.setWeight(0);
				} else if (weight == "extralight") {
					retval.setWeight(12);
				} else if (weight == "light") {
					retval.setWeight(25);
				} else if (weight == "medium") {
					retval.setWeight(57);
				} else if (weight == "demibold") {
					retval.setWeight(63);
				} else if (weight == "bold") {
					retval.setWeight(75);
				} else if (weight == "extrabold") {
					retval.setWeight(81);
				} else if (weight == "black") {
					retval.setWeight(87);
				}
			}
		}
		if (object.constEnd() != i && i->isBool()) {
			retval.setItalic(i->toBool(false));
		}
		if (object.constEnd() != u && u->isBool()) {
			retval.setUnderline(u->toBool(false));
		}
		if (object.constEnd() != s && s->isBool()) {
			retval.setStrikeOut(s->toBool(false));
		}
		if (object.constEnd() != z && z->isDouble()) {
			retval.setPointSize(z->toInt());
		}
		if (object.constEnd() != ls && ls->isDouble()) {
			retval.setLetterSpacing(QFont::SpacingType::AbsoluteSpacing, ls->toDouble());
		}
		if (object.constEnd() != ws && ws->isDouble()) {
			retval.setWordSpacing(ws->toDouble());
		}

		return retval;
	}

	Font::Font(const QFont& font)
	:
		QFont(font)
	{

	}

	QFont Font::toFont() const noexcept
	{
		return *this;
	}

	Font Font::setBold(bool onoff) noexcept
	{
		QFont::setBold(onoff);
		return *this;
	}

	Font Font::setItalic(bool onoff) noexcept
	{
		QFont::setItalic(onoff);
		return *this;
	}

	Font Font::setUnderline(bool onoff) noexcept
	{
		QFont::setUnderline(onoff);
		return *this;
	}

	bool Font::isBold() const noexcept
	{
		return QFont::bold();
	}

	bool Font::isItalic() const noexcept
	{
		return QFont::italic();
	}

	bool Font::isUnderline() const noexcept
	{
		return QFont::underline();
	}
} } } }
