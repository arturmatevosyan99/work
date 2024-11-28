#ifndef Cadabra_Queen_Core_Meta_Font_hpp
#define Cadabra_Queen_Core_Meta_Font_hpp

#include <QObject>

#include <QFont>
#include <QColor>
#include <QString>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	/**
	 * @brief The Font class
	 * @todo real pointSize
	 * @todo enumeration weight
	 * @todo bool overline
	 * @todo bool strikeout
	 * @todo enumeration capitalization
	 * @todo real letterSpacing
	 * @todo real wordSpacing
	 * @todo bool kerning
	 * @todo bool preferShaping
	 * @todo enumeration hintingPreference
	 * @todo string styleName
	 */
	class Font : public QFont
	{
	/** @name Statics */
	/** @{ */
	public:
		static QFont Parse(const QFont& source, const QJsonObject& object);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Font(const QFont& origin = QFont());
		~Font() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
		Q_INVOKABLE QFont toFont() const noexcept;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE Cadabra::Queen::Core::Meta::Font setBold(bool onoff) noexcept;
		Q_INVOKABLE Cadabra::Queen::Core::Meta::Font setItalic(bool onoff) noexcept;
		Q_INVOKABLE Cadabra::Queen::Core::Meta::Font setUnderline(bool onoff) noexcept;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isBold() const noexcept;
		bool isItalic() const noexcept;
		bool isUnderline() const noexcept;
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Core::Meta::Font)

#endif
