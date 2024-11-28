/**
 @file Qd.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Qd_hpp
#define Cadabra_Queen_Core_Meta_Qd_hpp

#include <QVariant>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class Qd
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Qd();
		Qd(qint64 v);
		Qd(const Qd& origin);
		Qd(Qd&& origin);
		Qd(const QVariant& v);
		~Qd() = default;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static bool IsValid(const QVariant& v);
		static Qd Make(const QVariant& v);
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator qint64() const;
		operator QVariant() const;
		bool operator<(const qint64& rhs) const;
		bool operator>(const qint64& rhs) const;
		bool operator==(const qint64& rhs) const;
		bool operator<(const Qd& rhs) const;
		bool operator>(const Qd& rhs) const;
		bool operator==(const Qd& rhs) const;
		Qd& operator=(const Qd& rhs);
	/** @} */

	/** @name Setters/Getters */
	/** @{ */
	public:
		void set(qint64 v);
		qint64 get() const;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
		qint64 value;
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
		Q_INVOKABLE qint64 toInt64() const noexcept;
		Q_INVOKABLE QString toString() const noexcept;
		Q_INVOKABLE QVariant toVariant() const noexcept;
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const Qd& handle);
	friend QDataStream& operator>>(QDataStream& in, Qd& handle);
	friend QDebug operator<<(QDebug debug, const Qd& handle);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		Q_INVOKABLE bool isZero() const;
		Q_INVOKABLE bool isAny(const QList<Cadabra::Queen::Core::Meta::Qd>& list) const;
		bool isAny(std::initializer_list<qint64> list) const;
	/** @} */
	};

	inline uint qHash(const Qd& key)
	{
		return static_cast<uint>(key.value);
	}
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Core::Meta::Qd)

#endif
