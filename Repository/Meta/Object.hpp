/**
 @file Object.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_Object_hpp
#define Cadabra_Queen_Repository_Meta_Object_hpp

#include "Essence.hpp"

#include <QString>
#include <QVector>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>
#include <set>
#include <cstdint>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Object
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Id = Essence::Id;
		using Handle = Essence::Handle;
		using Cache = ObjectCache;
		using ApplicationScheme = Essence::Scheme;
		using ApplicationSchemeIdent = Essence::SchemeIdent;
		using PermissionRule = qint32;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const Object::Id BrandNewId;
		static const Object::Id DeadlockId;
		static const Object::Id DomainId;

		static Object::Handle ToHandle(const QString& value, bool exception_safe = false);
		static Object::Handle ToHandle(const QJsonValue& value, bool exception_safe = false);

		static QMap<Id, QSharedPointer<const Meta::Object>> Parse(const QJsonValue& value);
		static QMap<Id, QSharedPointer<const Meta::Object>> Parse(const QJsonArray& array);
		static QSharedPointer<const Meta::Object> Parse(const QJsonObject& object);
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			EssenceId = Meta::Essence::Role::EssenceId,
			ParentId,
			Name,
			Favourite,
			Localization,
			Picture,
			Permission,
			ApplicationSchema,
			CreationDateTime,
			ModificationDateTime,
			AccessDateTime
		};

		enum class Condition : std::uint8_t
		{
			Removed = 0,
			Destructed = 1,
			URO = 4, /// @note UFO
			Isolated = 10,
			Locked = 100,
			Actual = 255
		};

		enum class AccessMode : std::uint8_t
		{
			None = 0x000,
			Read = 0b001,
			Execute = 0b010,
			Write = 0b100
		};

		enum class AccessRight : std::uint8_t
		{
			Subject = 7,
			Group	= 4,
			World	= 1
		};

		enum class Link : std::int16_t
		{
			None = 0,
			Shortcut = 1,
			Soft = 2,
			Hard = 3,
			Dependant = 4
		};

		enum class Lifecycle : std::uint8_t
		{
			Permanent = 'P',
			Temporary = 'G',
			Session = 'S'
		};

		using AccessSet = std::set<AccessMode>;

		enum class Operation : std::int8_t
		{
			Flush = '0',
			Internal = 'I',
			Touch = 'T',  /// @note business logic operation
			Emplacement = 'E',
			Metafication = 'M',
			Renaming = 'N',
			Let = 'L', /// @note AppServ/Let (former O: Opening)
			Refusal = 'R',
			Destruction = 'D',
			Copying = 'C',
			Movement = 'm',
			Permit = 'P',
			Picture = 'p',
			Own = 'o',
			Tag = 't',
			Link = 'l',
			Attribute = 'a',
			Super = 's'
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Object(Object::Id id, Object::Id parent_id, Object::ApplicationScheme application_scheme, const QString& name, bool favourite, bool localization, const QString& picture, Object::PermissionRule permission, const QDateTime& creation_datetime, const QDateTime& modification_datetime, const QDateTime& access_datetime = QDateTime::currentDateTimeUtc(), const QVariant& option = QVariant());
		Object(Object::Id id, Object::Id parent_id, Object::ApplicationScheme application_scheme, const QString& name, bool favourite, bool localization, const QString& picture, Object::PermissionRule permission, qint64 creation_timestamp, qint64 modification_timestamp, qint64 access_timestamp = QDateTime::currentSecsSinceEpoch(), const QVariant& option = QVariant());
		~Object() = default;
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
	private:
		const Object::Id _id;
		Object::Id _parent_id;
		Object::ApplicationScheme _application_scheme;
		QString _name;
		bool _favourite;
		bool _localization;
		QString _picture;
		Object::PermissionRule _permission;
		QDateTime _creation_datetime;
		QDateTime _modification_datetime;
		QDateTime _access_datetime;
		QVariant _option;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setParentId(Object::Id object_id);
		void setFavourite(bool onoff);
		void setName(const QString& name);
		void setLocalization(bool onoff);
		void setApplicationScheme(Object::ApplicationScheme scheme);
		void setOption(const QVariant& option);
		void setPicture(const QString& picture);
		void setPermission(Object::PermissionRule permission);
		void setCreationDateTime(const QDateTime& datetime);
		void setCreationTimestamp(qint64 timestamp);
		void setModificationDateTime(const QDateTime& datetime);
		void setModificationTimestamp(qint64 timestamp);
		void setAccessDateTime(const QDateTime& datetime);
		void setAccessTimestamp(qint64 timestamp);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Object::Id& getId() const;
		const Object::Id& getParentId() const;
		const bool& getFavourite() const;
		const QString& getName() const;
		const bool& getLocalization() const;
		QString getLocalizedName() const;
		const Object::ApplicationScheme& getApplicationScheme() const;
		const QVariant& getOption() const;
		const QString& getPicture() const;
		const Object::PermissionRule& getPermission() const;
		const QDateTime& getCreationDateTime() const;
		const QDateTime& getModificationDateTime() const;
		const QDateTime& getAccessDateTime() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator Meta::Object::Id() const;
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
} } } }

#endif
