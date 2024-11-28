/**
 @file Subject.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_Subject_hpp
#define Cadabra_Queen_Repository_Meta_Subject_hpp

#include "Essence.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QString>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Subject
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Id = Essence::Id;
		using AvatarScheme = Essence::Scheme;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const Subject::Id DummyId;

		static QMap<Id, QSharedPointer<const Subject>> Parse(const QJsonValue& value);
		static QMap<Id, QSharedPointer<const Subject>> Parse(const QJsonArray& array);
		static QSharedPointer<const Subject> Parse(const QJsonObject& object);
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			EssenceId = Meta::Essence::Role::EssenceId,
			AvatarSchema,
			Name,
			Description,
			Picture,
			Locale,
			Timezone,
			CreationDateTime,
			ModificationDateTime
		};

		enum class Operation : std::int8_t
		{
			Flush = '0',
			Authorization = 'A',
			Internal = 'I',
			Touch = 'T',  /// @note business logic operation
			Emplacement = 'E',
			Refusal = 'R',
			Cremation = 'C',
			Renaming = 'N',
			Metafication = 'M',
			Description = 'D',
			Localization = 'L',
			Timeshift = 'Z',
			Membership = 'G', /// < Group
			Picture = 'p',
			Lock = 'l',
			Root = 'r',
			Home = 'h',
			Tag = 't',
			Attribute = 'a',
			Super = 's'
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Subject(Subject::Id id, Subject::AvatarScheme avatar_scheme, const QString& name, const QString& description, const QString& picture, const QString& locale, const QString& timezone, int creation_timestamp, int modification_timestamp);
		Subject(Subject::Id id, Subject::AvatarScheme avatar_scheme, const QString& name, const QString& description, const QString& picture, const QString& locale, const QString& timezone, const QDateTime& creation_datetime, const QDateTime& modification_timestamp);
		Subject(const Subject& origin);
		Subject(Subject&& origin);
		~Subject() = default;
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
		const Subject::Id _id;
		Subject::AvatarScheme _avatar_scheme;
		QString _name;
		QString _description;
		QString _picture;
		QString _locale;
		QString _timezone;
		QDateTime _creation_datetime;
		QDateTime _modification_datetime;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setAvatarScheme(Subject::AvatarScheme avatar_scheme);
		void setName(const QString& name);
		void setDescription(const QString& description);
		void setPicture(const QString& picture);
		void setLocale(const QString& locale);
		void setTimezone(const QString& timezone);
		void setCreationTimestamp(int timestamp);
		void setModificationTimestamp(int timestamp);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Subject::Id& getId() const;
		const Subject::AvatarScheme& getAvatarScheme() const;
		const QString& getName() const;
		const QString& getDescription() const;
		const QString& getPicture() const;
		const QString& getLocale() const;
		const QString& getTimezone() const;
		const QDateTime& getCreationDateTime() const;
		const QDateTime& getModificationDateTime() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator Subject::Id() const;
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
