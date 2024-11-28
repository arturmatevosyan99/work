/**
 @file Group.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_Group_hpp
#define Cadabra_Queen_Repository_Meta_Group_hpp

#include "Essence.hpp"

#include <QString>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Group
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Id = Essence::Id;
		using AvatarScheme = Essence::Scheme;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			EssenceId = Meta::Essence::Role::EssenceId,
			Name,
			Description,
			Picture,
			CreationDateTime,
			ModificationDateTime
		};

		enum class Operation : std::int8_t
		{
			Flush = '0',
			Internal = 'I',
			Touch = 'T',  /// @note business logic operation
			Emplacement = 'E',
			Dissolution = 'S',
			Refusal = 'R',
			Renaming = 'N',
			Description = 'D',
			Metafication = 'M',
			Picture = 'p',
			Lock = 'l',
			Tag = 't',
			Attribute = 'a',
			Super = 's'
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Group(Group::Id id, const QString& name, const QString& description, const QString& picture, int creation_timestamp, int modification_timestamp);
		Group(Group::Id id, const QString& name, const QString& description, const QString& picture, const QDateTime& creation_datetime, const QDateTime& modification_datetime);
		Group(const Group& origin);
		Group(Group&& origin);
		~Group() = default;
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
		const Group::Id _id;
		QString _name;
		QString _description;
		QString _picture;
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
		void setName(const QString& name);
		void setDescription(const QString& description);
		void setPicture(const QString& picture);
		void setCreationTimestamp(int timestamp);
		void setModificationTimestamp(int timestamp);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Group::Id& getId() const;
		const QString& getName() const;
		const QString& getDescription() const;
		const QString& getPicture() const;
		const QDateTime& getCreationDateTime() const;
		const QDateTime& getModificationDateTime() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator Meta::Group::Id() const;
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
