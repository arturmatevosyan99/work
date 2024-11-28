#ifndef Cadabra_Queen_Core_View_Theme_hpp
#define Cadabra_Queen_Core_View_Theme_hpp

#include <QObject>

#include "../Meta/Font.hpp"

#include <QFont>
#include <QColor>
#include <QString>
#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace View {	
	/**
	 * @brief The Theme class
	 * @todo Должно как-то взаимодействовать(унаследовать, переопределять параметры...) с Material
	 */
	class Theme : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Mode
		{
			Auto,
			Light,
			Dark
		};

		enum class FontStyle : quint8
		{
			Unknown,
			Default,
			Header1,
			Header2,
			Header3,
			Header4,
			Header5,
			Header6,
			Title1,
			Title2,
			Title3,
			Title4,
			Body1,
			Body2,
			Button,
			Caption,
			Overline,
			Headerline,
		};

		enum class ColorStyle : quint8
		{
			Unknown,
			Accent,
			Primary,
			Background,
			Foreground
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QMap<QString, FontStyle> FontStyleMap;
		static const QMap<QString, ColorStyle> ColorStyleMap;

		static bool Font(const QString& file_name);

		static Theme* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Mode)
		Q_ENUM(FontStyle)
		Q_ENUM(ColorStyle)
		Q_PROPERTY(QFont systemFont READ getSystemFont CONSTANT)
		Q_PROPERTY(QFont defaultFont READ getDefaultFont WRITE setDefaultFont NOTIFY defaultFontChanged)
		Q_PROPERTY(QFont header1Font READ getHeader1Font WRITE setHeader1Font NOTIFY header1FontChanged)
		Q_PROPERTY(QFont header2Font READ getHeader2Font WRITE setHeader2Font NOTIFY header2FontChanged)
		Q_PROPERTY(QFont header3Font READ getHeader3Font WRITE setHeader3Font NOTIFY header3FontChanged)
		Q_PROPERTY(QFont header4Font READ getHeader4Font WRITE setHeader4Font NOTIFY header4FontChanged)
		Q_PROPERTY(QFont header5Font READ getHeader5Font WRITE setHeader5Font NOTIFY header5FontChanged)
		Q_PROPERTY(QFont header6Font READ getHeader6Font WRITE setHeader6Font NOTIFY header6FontChanged)
		Q_PROPERTY(QFont title1Font READ getTitle1Font WRITE setTitle1Font NOTIFY title1FontChanged)
		Q_PROPERTY(QFont title2Font READ getTitle2Font WRITE setTitle2Font NOTIFY title2FontChanged)
		Q_PROPERTY(QFont title3Font READ getTitle3Font WRITE setTitle3Font NOTIFY title3FontChanged)
		Q_PROPERTY(QFont title4Font READ getTitle4Font WRITE setTitle4Font NOTIFY title4FontChanged)
		Q_PROPERTY(QFont body1Font READ getBody1Font WRITE setBody1Font NOTIFY body1FontChanged)
		Q_PROPERTY(QFont body2Font READ getBody2Font WRITE setBody2Font NOTIFY body2FontChanged)
		Q_PROPERTY(QFont buttonFont READ getButtonFont WRITE setButtonFont NOTIFY buttonFontChanged)
		Q_PROPERTY(QFont captionFont READ getCaptionFont WRITE setCaptionFont NOTIFY captionFontChanged)
		Q_PROPERTY(QFont overlineFont READ getOverlineFont WRITE setOverlineFont NOTIFY overlineFontChanged)
		Q_PROPERTY(QFont headerlineFont READ getHeaderlineFont WRITE setHeaderlineFont NOTIFY headerlineFontChanged)
		Q_PROPERTY(qreal fontScale READ getFontScale WRITE setFontScale NOTIFY fontScaleChanged)
		Q_PROPERTY(QColor accentColor READ getAccentColor WRITE setAccentColor NOTIFY accentColorChanged)
		Q_PROPERTY(QColor primaryColor READ getPrimaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
		Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
		Q_PROPERTY(QColor foregroundColor READ getForegroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	private:
		explicit Theme(QObject* parent = nullptr);
		virtual ~Theme() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		qreal _font_scale;

		const QFont _unknown_font;
		QFont _default_font;
		QFont _header1_font;
		QFont _header2_font;
		QFont _header3_font;
		QFont _header4_font;
		QFont _header5_font;
		QFont _header6_font;
		QFont _title1_font;
		QFont _title2_font;
		QFont _title3_font;
		QFont _title4_font;
		QFont _body1_font;
		QFont _body2_font;
		QFont _button_font;
		QFont _caption_font;
		QFont _overline_font;
		QFont _headerline_font;

		const QColor _unknown_color;
		QColor _accent_color;
		QColor _primary_color;
		QColor _background_color;
		QColor _foreground_color;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool load(const QString& name, Mode mode);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE Cadabra::Queen::Core::Meta::Font customizeFont(QFont origin) const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		qreal getFontScale() const;

		const QFont& getFont(const QString& alias) const;
		const QFont& getFont(const FontStyle& alias) const;
		const QFont& getSystemFont() const;
		const QFont& getDefaultFont() const;
		const QFont& getHeader1Font() const;
		const QFont& getHeader2Font() const;
		const QFont& getHeader3Font() const;
		const QFont& getHeader4Font() const;
		const QFont& getHeader5Font() const;
		const QFont& getHeader6Font() const;
		const QFont& getTitle1Font() const;
		const QFont& getTitle2Font() const;
		const QFont& getTitle3Font() const;
		const QFont& getTitle4Font() const;
		const QFont& getBody1Font() const;
		const QFont& getBody2Font() const;
		const QFont& getButtonFont() const;
		const QFont& getCaptionFont() const;
		const QFont& getOverlineFont() const;
		const QFont& getHeaderlineFont() const;

		const QColor& getColor(const QString& alias) const;
		const QColor& getColor(const ColorStyle& alias) const;
		const QColor& getAccentColor() const;
		const QColor& getPrimaryColor() const;
		const QColor& getBackgroundColor() const;
		const QColor& getForegroundColor() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setFontScale(qreal value);

		void setFont(const QString& alias, const QFont& value);
		void setFont(const FontStyle& alias, const QFont& value);
		void setDefaultFont(const QFont& value);
		void setHeader1Font(const QFont& value);
		void setHeader2Font(const QFont& value);
		void setHeader3Font(const QFont& value);
		void setHeader4Font(const QFont& value);
		void setHeader5Font(const QFont& value);
		void setHeader6Font(const QFont& value);
		void setTitle1Font(const QFont& value);
		void setTitle2Font(const QFont& value);
		void setTitle3Font(const QFont& value);
		void setTitle4Font(const QFont& value);
		void setBody1Font(const QFont& value);
		void setBody2Font(const QFont& value);
		void setButtonFont(const QFont& value);
		void setCaptionFont(const QFont& value);
		void setOverlineFont(const QFont& value);
		void setHeaderlineFont(const QFont& value);

		void setColor(const QString& alias, const QColor& value);
		void setColor(const ColorStyle& alias, const QColor& value);
		void setAccentColor(const QColor& value);
		void setPrimaryColor(const QColor& value);
		void setBackgroundColor(const QColor& value);
		void setForegroundColor(const QColor& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void defaultFontChanged();
		void header1FontChanged();
		void header2FontChanged();
		void header3FontChanged();
		void header4FontChanged();
		void header5FontChanged();
		void header6FontChanged();
		void title1FontChanged();
		void title2FontChanged();
		void title3FontChanged();
		void title4FontChanged();
		void body1FontChanged();
		void body2FontChanged();
		void buttonFontChanged();
		void captionFontChanged();
		void overlineFontChanged();
		void headerlineFontChanged();

		void fontScaleChanged();

		void accentColorChanged();
		void primaryColorChanged();
		void backgroundColorChanged();
		void foregroundColorChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
