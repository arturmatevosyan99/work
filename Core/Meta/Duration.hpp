/**
 @file Duration.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Duration_hpp
#define Cadabra_Queen_Core_Meta_Duration_hpp

#include <QVariant>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>
#include <QMap>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class Duration
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Span {
			Second = 's',
			Minute = 'm',
			Hour = 'h',
			Day = 'd',
			Week = 'W',
			Month = 'M',
			Year = 'Y'
		};
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		static const QMap<QString, Duration::Span> Vocabulary;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Duration();
		Duration(const QString& value);
		Duration(const Duration& origin);
		Duration(Duration&& origin);
		Duration(const QVariant& v);
		~Duration() = default;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator QString() const;
		operator QVariant() const;
		bool operator<(const qint64& rhs) const;
		bool operator>(const qint64& rhs) const;
		bool operator==(const qint64& rhs) const;
		bool operator<(const Duration& rhs) const;
		bool operator>(const Duration& rhs) const;
		bool operator==(const Duration& rhs) const;
		Duration& operator=(const Duration& rhs);
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
		QMap<Span, int> _map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool parse(const QString& value);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void add(Duration::Span span, int value);
	/** @} */

	/** @name Friends */
	/** @{ */
	public:
		Q_INVOKABLE QString toString() const noexcept;
		Q_INVOKABLE QStringList toStringList() const noexcept;
		Q_INVOKABLE QVariant toVariant() const noexcept;
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const Duration& duration);
	friend QDataStream& operator>>(QDataStream& in, Duration& duration);
	friend QDebug operator<<(QDebug debug, const Duration& duration);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		Q_INVOKABLE bool isZero() const;
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Core::Meta::Duration)

#endif
