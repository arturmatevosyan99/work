/**
 @file Essence.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_Essence_hpp
#define Cadabra_Queen_Repository_Meta_Essence_hpp

#include "../Repository.hpp"

#include <QtGlobal>
#include <QObject>
#include <QJsonValue>
#include <QDataStream>
#include <QDebug>
#include <QMap>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Handle
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(QString value READ getValue WRITE setValue)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Handle fromUInt64(const quint64& value);
		static Handle fromString(const QString& value);
		static Handle fromJson(const QJsonValue& value);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Handle();
		Handle(const Handle& origin);
		Handle(const QString& value);
		Handle(Handle&& origin);
		Handle(const quint64& value);
		Handle(quint64&& value);
		~Handle() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		quint64 _value;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:		
		void setValue(const QString& value);
		bool increment(const Handle& delta);
		bool decrement(const Handle& delta);
		Handle& ment(const Handle& delta, bool positive);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QString getValue() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		Handle& operator=(const Handle& rhs);
		Handle& operator=(const quint64& rhs);
		Handle& operator=(Handle&& rhs);
		Handle& operator=(quint64&& rhs);
		Handle& operator-=(const Handle& rhs);
		Handle& operator+=(const Handle& rhs);
		Handle operator-(const Handle& rhs);
		Handle operator+(const Handle& rhs);
		bool operator==(const quint64& rhs) const;
		bool operator==(const int& rhs) const;
		bool operator!() const;
		operator QString() const;
		operator bool() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		Q_INVOKABLE bool isZero() const;
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const Handle& handle);
	friend QDataStream& operator>>(QDataStream& in, Handle& handle);
	friend QDebug operator<<(QDebug debug, const Handle& handle);
	/** @} */
	};

	class Essence
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Id = qint64;
		using Handle = Meta::Handle;
		using Scheme = QString;
		using SchemeIdent = qint64;
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		static Essence::Id ToId(const QString& value, bool exception_safe = false);
		static Essence::Id ToId(const QJsonValue& value, bool exception_safe = false);
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			OptMap = Qt::UserRole + 1,
			TagSet,
			EssenceId
		};

		enum class Target : char
		{
			Zilch = '0',
			Subject = 's',
			Object = 'o',
			Group = 'g',
			Tag = 't',
			Session = 'S'
		};

		enum class Impact : qint8
		{
			Success = 'S',
			Error = 'E',
			Forbidden = 'F'
		};

		enum class TouchTime : qint8
		{
			Access = 'S',
			Modification = 'H',
			Creation = 'C'
		};
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
	/** @} */
		
	/** @name Properties */
	/** @{ */
	public:
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	protected:			
	/** @} */
		
	/** @name Converters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Formatters */
	/** @{ */
	public:
	/** @} */
		
	/** @name States */
	/** @{ */
	public:
	/** @} */
		
	/** @name Hooks */
	/** @{ */
	public:
	/** @} */
		
	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};

	class Cache
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Essence::Id id READ getId WRITE setId)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Essence::Handle handle READ getHandle WRITE setHandle)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Essence::Scheme scheme READ getScheme WRITE setScheme)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Cache();
		Cache(const Cache& origin);
		Cache(Essence::Id id, Essence::Handle handle, Essence::Scheme scheme);
		Cache(Cache&& origin);
		~Cache() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Essence::Id _id;
		Essence::Handle _handle;
		Essence::Scheme _scheme;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setId(const Essence::Id& value);
		void setHandle(const Essence::Handle& value);
		void setScheme(const Essence::Scheme& value);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Essence::Id getId() const;
		Essence::Handle getHandle() const;
		Essence::Scheme getScheme() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		Cache& operator=(const Cache& rhs);
		Cache& operator=(const Essence::Id& rhs);
		Cache& operator=(const Essence::Handle& rhs);
		Cache& operator=(const Essence::Scheme& rhs);
		Cache& operator=(Cache&& rhs);
		Cache& operator=(Essence::Id&& rhs);
		Cache& operator=(Essence::Handle&& rhs);
		Cache& operator=(Essence::Scheme&& rhs);
		bool operator!() const;
		operator Essence::Id() const;
		operator Essence::Handle() const;
		operator Essence::Scheme() const;
		operator bool() const;
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const Cache& handle);
	friend QDataStream& operator>>(QDataStream& in, Cache& handle);
	friend QDebug operator<<(QDebug debug, const Cache& handle);
	/** @} */
	};

	using JobId = Essence::Id;
	using SubjectId = Essence::Id;
	using GroupId = Essence::Id;
	using ObjectId = Essence::Id;
	using ObjectHandle = Essence::Handle;
	using SessionHandle = Essence::Handle;
	using SessionScheme = Essence::Scheme;
	using ObjectScheme = Essence::Scheme;
	using ObjectCache = Cache;
	using SessionPool = Cache;

	struct RealmTarget
	{
	/** @name Constructors */
	/** @{ */
		RealmTarget();
		RealmTarget(Meta::Essence::Target target, Meta::Essence::Id);
		RealmTarget(const RealmTarget& origin);
		RealmTarget(RealmTarget&& origin);
	/** @} */

	/** @name Operators */
	/** @{ */
		RealmTarget& operator=(const RealmTarget& origin);
	/** @} */

	/** @name Properties */
	/** @{ */
		Meta::Essence::Target target;
		Meta::Essence::Id id;
	/** @} */

	/** @name Getters */
	/** @{ */
		const Meta::Essence::Target& geTarget() const;
		const Meta::Essence::Id& getId() const;
	/** @} */
	};

	inline bool operator<(const RealmTarget& lhs, const RealmTarget& rhs)
	{
		return (lhs.target == rhs.target) ? (lhs.id < rhs.id) : lhs.target < rhs.target;
	}

	using EssenceAlterMap = QMap<Cadabra::Queen::Repository::Meta::Essence::Id, Cadabra::Queen::Repository::EssenceAlterFlags>;
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::Handle)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::Cache)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::RealmTarget)

#endif
