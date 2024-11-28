/**
 @file Theme.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Theme.hpp"

#include "../System/Native.hpp"
#include "../../Skeleton.hpp"

#include <QQuickStyle>
#include <QFontDatabase>
#include <QJsonDocument>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	const QMap<QString, Theme::FontStyle> Theme::FontStyleMap =
	{
		{"default", Theme::FontStyle::Default},
		{"header1", Theme::FontStyle::Header1},
		{"header2", Theme::FontStyle::Header2},
		{"header3", Theme::FontStyle::Header3},
		{"header4", Theme::FontStyle::Header4},
		{"header5", Theme::FontStyle::Header5},
		{"header6", Theme::FontStyle::Header6},
		{"title1", Theme::FontStyle::Title1},
		{"title2", Theme::FontStyle::Title2},
		{"title3", Theme::FontStyle::Title3},
		{"title4", Theme::FontStyle::Title4},
		{"body1", Theme::FontStyle::Body1},
		{"body2", Theme::FontStyle::Body2},
		{"button", Theme::FontStyle::Button},
		{"caption", Theme::FontStyle::Caption},
		{"overline", Theme::FontStyle::Overline},
		{"headerline", Theme::FontStyle::Headerline}
	};

	const QMap<QString, Theme::ColorStyle> Theme::ColorStyleMap =
	{
		{"accent", Theme::ColorStyle::Accent},
		{"primary", Theme::ColorStyle::Primary},
		{"background", Theme::ColorStyle::Background},
		{"foreground", Theme::ColorStyle::Foreground}
	};

	bool Theme::Font(const QString& file_name)
	{
		const int id(QFontDatabase::addApplicationFont(file_name));
		if (id < 0) {
			qWarning() << "Could not load font style:" << file_name;
			return false;
		}
		QFontDatabase font_database;
		const QStringList font_list(font_database.applicationFontFamilies(id));
		for (const QString& font : font_list) {
			qInfo() << font << "styles:" << font_database.styles(font);
		}
		return true;
	}

	Theme* Theme::Instance()
	{
		static Theme instance;

		return &instance;
	}

	QObject* Theme::QInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
	{
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		Theme* retval(Cadabra::Queen::Core::View::Theme::Instance());

		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	Theme::Theme(QObject *parent)
	:
		QObject(parent),

		_font_scale(System::Native::Instance()->getDefaultFontScale()),

		_unknown_font(QFont()),

		_unknown_color(QColor()),
		_accent_color(243, 244, 246),
		_primary_color(73, 141, 242),
		_background_color(0, 0, 0),
		_foreground_color(255, 255, 255)
	{
		/// @todo QQuickStyle::setStyle(":/Guiness");
		QQuickStyle::setStyle("Material");
	}

	bool Theme::load(const QString& name, Mode mode)
	{
		QFile file(":/Core/Asset/Style/" + name + ".json");
		if (!file.open(QFile::ReadOnly)) {
			qWarning("Theme load error: Could not open a file");
			return false;
		}

		QJsonParseError error;
		const QJsonDocument document(QJsonDocument::fromJson(file.readAll(), &error));
		if (error.error != QJsonParseError::NoError || !document.isObject()) {
			qWarning() << "Theme JSON parse error:" << error.errorString();
			return false;
		} else if (Mode::Auto == mode) {
			/// @todo
			mode = Mode::Light;
		}

		QJsonObject root_object(document.object());
		root_object = root_object.value(Mode::Light == mode ? "light" : "dark").toObject({});

		QJsonObject::ConstIterator c(root_object.find("color"));
		if (c != root_object.constEnd() && c->isObject()) {
			QJsonObject color_object(c->toObject());
			QList<QString> color_list(color_object.keys());
			for (const QString& alias : qAsConst(color_list)) {
				setColor(alias.toLower(), QColor(color_object.value(alias).toString("#FFFFFF")));
			}
		}

		QJsonObject::ConstIterator f(root_object.find("font"));
		if (f != root_object.constEnd() && f->isObject()) {
			QJsonObject font_object(f->toObject());
			QList<QString> font_list(font_object.keys());
			QFont default_font(_unknown_font);
			if (font_list.removeOne("default")) {
				default_font = Meta::Font::Parse(default_font, font_object.value("default").toObject());
				default_font.setPointSizeF(default_font.pointSize() * _font_scale);
				setFont(FontStyle::Default, default_font);
			}
			for (const QString& alias : qAsConst(font_list)) {
				QFont font(Meta::Font::Parse(default_font, font_object.value(alias).toObject()));
				font.setPointSizeF(font.pointSize() * _font_scale);
				setFont(alias.toLower(), font);
			}
		}

		return true;
	}

	Meta::Font Theme::customizeFont(QFont origin) const
	{
		return Meta::Font(origin);
	}

	qreal Theme::getFontScale() const
	{
		return _font_scale;
	}

	const QFont& Theme::getFont(const QString& alias) const
	{
		return getFont(Theme::FontStyleMap.value(alias.toLower(), FontStyle::Unknown));
	}

	const QFont& Theme::getFont(const FontStyle& alias) const
	{
		switch(alias) {
			case FontStyle::Default: return _default_font;
			case FontStyle::Header1: return _header1_font;
			case FontStyle::Header2: return _header2_font;
			case FontStyle::Header3: return _header3_font;
			case FontStyle::Header4: return _header4_font;
			case FontStyle::Header5: return _header5_font;
			case FontStyle::Header6: return _header6_font;
			case FontStyle::Title1: return _title1_font;
			case FontStyle::Title2: return _title2_font;
			case FontStyle::Title3: return _title3_font;
			case FontStyle::Title4: return _title4_font;
			case FontStyle::Body1: return _body1_font;
			case FontStyle::Body2: return _body2_font;
			case FontStyle::Button: return _button_font;
			case FontStyle::Caption: return _caption_font;
			case FontStyle::Overline: return _overline_font;
			case FontStyle::Headerline: return _headerline_font;
			default: return _unknown_font;
		}
	}

	const QFont& Theme::getSystemFont() const
	{
		return _unknown_font;
	}

	const QFont& Theme::getDefaultFont() const
	{
		return _default_font;
	}

	const QFont& Theme::getHeader1Font() const
	{
		return _header1_font;
	}

	const QFont& Theme::getHeader2Font() const
	{
		return _header2_font;
	}

	const QFont& Theme::getHeader3Font() const
	{
		return _header3_font;
	}

	const QFont& Theme::getHeader4Font() const
	{
		return _header4_font;
	}

	const QFont& Theme::getHeader5Font() const
	{
		return _header5_font;
	}

	const QFont& Theme::getHeader6Font() const
	{
		return _header6_font;
	}

	const QFont& Theme::getTitle1Font() const
	{
		return _title1_font;
	}

	const QFont& Theme::getTitle2Font() const
	{
		return _title2_font;
	}

	const QFont& Theme::getTitle3Font() const
	{
		return _title3_font;
	}

	const QFont& Theme::getTitle4Font() const
	{
		return _title4_font;
	}

	const QFont& Theme::getBody1Font() const
	{
		return _body1_font;
	}

	const QFont& Theme::getBody2Font() const
	{
		return _body2_font;
	}

	const QFont& Theme::getButtonFont() const
	{
		return _button_font;
	}

	const QFont& Theme::getCaptionFont() const
	{
		return _caption_font;
	}

	const QFont& Theme::getOverlineFont() const
	{
		return _overline_font;
	}

	const QFont& Theme::getHeaderlineFont() const
	{
		return _headerline_font;
	}

	const QColor& Theme::getColor(const QString& alias) const
	{
		return getColor(Theme::ColorStyleMap.value(alias.toLower(), ColorStyle::Primary));
	}

	const QColor& Theme::getColor(const ColorStyle& alias) const
	{
		switch(alias) {
			case ColorStyle::Primary: return _primary_color;
			case ColorStyle::Accent: return _accent_color;
			case ColorStyle::Background: return _background_color;
			case ColorStyle::Foreground: return _foreground_color;

			default: return _unknown_color;
		}
	}

	const QColor& Theme::getAccentColor() const
	{
		return _accent_color;
	}

	const QColor& Theme::getPrimaryColor() const
	{
		return _primary_color;
	}

	const QColor& Theme::getBackgroundColor() const
	{
		return _background_color;
	}

	const QColor& Theme::getForegroundColor() const
	{
		return _foreground_color;
	}

	void Theme::setFontScale(qreal value)
	{
		if (!qFuzzyCompare(value, _font_scale)) {
			_font_scale = value;
			emit fontScaleChanged();
			/// @todo need to rescale all fonts
		}
	}

	void Theme::setFont(const QString& alias, const QFont& value)
	{
		setFont(Theme::FontStyleMap.value(alias.toLower(), FontStyle::Unknown), value);
	}

	void Theme::setFont(const FontStyle& alias, const QFont& value)
	{
		switch (alias) {
			case FontStyle::Default: setDefaultFont(value); break;
			case FontStyle::Header1: setHeader1Font(value); break;
			case FontStyle::Header2: setHeader2Font(value); break;
			case FontStyle::Header3: setHeader3Font(value); break;
			case FontStyle::Header4: setHeader4Font(value); break;
			case FontStyle::Header5: setHeader5Font(value); break;
			case FontStyle::Header6: setHeader6Font(value); break;
			case FontStyle::Title1: setTitle1Font(value); break;
			case FontStyle::Title2: setTitle2Font(value); break;
			case FontStyle::Title3: setTitle3Font(value); break;
			case FontStyle::Title4: setTitle4Font(value); break;
			case FontStyle::Body1: setBody1Font(value); break;
			case FontStyle::Body2: setBody2Font(value); break;
			case FontStyle::Button: setButtonFont(value); break;
			case FontStyle::Caption: setCaptionFont(value); break;
			case FontStyle::Overline: setOverlineFont(value); break;
			default: return;
		}
	}

	void Theme::setDefaultFont(const QFont& value)
	{
		if (value != _default_font) {
			Skeleton::setFont(value);
			_default_font = value;
			emit defaultFontChanged();
		}
	}

	void Theme::setHeader1Font(const QFont& value)
	{
		if (value != _header1_font) {
			_header1_font = value;
			emit header1FontChanged();
		}
	}

	void Theme::setHeader2Font(const QFont& value)
	{
		if (value != _header2_font) {
			_header2_font = value;
			emit header2FontChanged();
		}
	}

	void Theme::setHeader3Font(const QFont& value)
	{
		if (value != _header3_font) {
			_header3_font = value;
			emit header3FontChanged();
		}
	}

	void Theme::setHeader4Font(const QFont& value)
	{
		if (value != _header4_font) {
			_header4_font = value;
			emit header4FontChanged();
		}
	}

	void Theme::setHeader5Font(const QFont& value)
	{
		if (value != _header5_font) {
			_header5_font = value;
			emit header5FontChanged();
		}
	}

	void Theme::setHeader6Font(const QFont& value)
	{
		if (value != _header6_font) {
			_header6_font = value;
			emit header6FontChanged();
		}
	}

	void Theme::setTitle1Font(const QFont& value)
	{
		if (value != _title1_font) {
			_title1_font = value;
			emit title1FontChanged();
		}
	}

	void Theme::setTitle2Font(const QFont& value)
	{
		if (value != _title2_font) {
			_title2_font = value;
			emit title2FontChanged();
		}
	}

	void Theme::setTitle3Font(const QFont& value)
	{
		if (value != _title3_font) {
			_title3_font = value;
			emit title3FontChanged();
		}
	}

	void Theme::setTitle4Font(const QFont& value)
	{
		if (value != _title4_font) {
			_title4_font = value;
			emit title4FontChanged();
		}
	}

	void Theme::setBody1Font(const QFont& value)
	{
		if (value != _body1_font) {
			_body1_font = value;
			emit body1FontChanged();
		}
	}

	void Theme::setBody2Font(const QFont& value)
	{
		if (value != _body2_font) {
			_body2_font = value;
			emit body2FontChanged();
		}
	}

	void Theme::setButtonFont(const QFont& value)
	{
		if (value != _button_font) {
			_button_font = value;
			emit buttonFontChanged();
		}
	}

	void Theme::setCaptionFont(const QFont& value)
	{
		if (value != _caption_font) {
			_caption_font = value;
			emit captionFontChanged();
		}
	}

	void Theme::setOverlineFont(const QFont& value)
	{
		if (value != _overline_font) {
			_overline_font = value;
			emit overlineFontChanged();
		}
	}

	void Theme::setHeaderlineFont(const QFont& value)
	{
		if (value != _headerline_font) {
			_headerline_font = value;
			emit headerlineFontChanged();
		}
	}

	void Theme::setColor(const QString& alias, const QColor& value)
	{
		setColor(Theme::ColorStyleMap.value(alias.toLower(), ColorStyle::Unknown), value);
	}

	void Theme::setColor(const ColorStyle& alias, const QColor& value)
	{
		switch (alias) {
			case ColorStyle::Accent: setAccentColor(value); break;
			case ColorStyle::Primary: setPrimaryColor(value); break;
			case ColorStyle::Background: setBackgroundColor(value); break;
			case ColorStyle::Foreground: setForegroundColor(value); break;
			default: return;
		}
	}

	void Theme::setAccentColor(const QColor& value)
	{
		if (value != _accent_color) {
			_accent_color = value;
			emit accentColorChanged();
		}
	}

	void Theme::setPrimaryColor(const QColor& value)
	{
		if (value != _primary_color) {
			_primary_color = value;
			emit primaryColorChanged();
		}
	}

	void Theme::setBackgroundColor(const QColor& value)
	{
		if (value != _background_color) {
			_background_color = value;
			emit backgroundColorChanged();
		}
	}

	void Theme::setForegroundColor(const QColor& value)
	{
		if (value != _foreground_color) {
			_foreground_color = value;
			emit foregroundColorChanged();
		}
	}
} } } }
