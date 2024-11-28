/**
 @file Subject.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Subject.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	const Subject::Id Subject::DummyId = 0;

	QMap<Subject::Id, QSharedPointer<const Subject>> Subject::Parse(const QJsonValue& value)
	{
		if (value.isArray()) {
			return Subject::Parse(value.toArray());
		} else if (value.isObject()) {
			QSharedPointer<const Subject> subject(Subject::Parse(value.toObject()));
			return {{subject->getId(), subject}};
		}
		return {};
	}

	QMap<Subject::Id, QSharedPointer<const Subject>> Subject::Parse(const QJsonArray& array)
	{
		QMap<Subject::Id, QSharedPointer<const Subject>> retval;
		foreach (const QJsonValue& value, array) {
			if (value.isObject()) {
				if (QSharedPointer<const Subject> subject = Subject::Parse(value.toObject())) {
					retval.insert(subject->getId(), subject);
				}
			}
		}
		return retval; /// retval.insert(object->getId(), object);
	}

	QSharedPointer<const Subject> Subject::Parse(const QJsonObject& object)
	{
		return QSharedPointer<const Subject>::create
		(
			Essence::ToId(object.value("id")),
			object.value("avatar_scheme").toString("0"),
			object.value("name").toString(),
			object.value("description").toString(),
			object.value("picture").toString(),
			object.value("locale").toString("en_US"),
			object.value("timezone").toString("UTC"),
			object.value("creation_timestamp").toInt(),
			object.value("modification_timestamp").toInt()
		);
	}

	Subject::Subject(Subject::Id id, Subject::AvatarScheme avatar_scheme, const QString& name, const QString& description, const QString& picture, const QString& locale, const QString& timezone, int creation_timestamp, int modification_timestamp)
	:
		_id(id),
		_avatar_scheme(avatar_scheme),
		_name(name),
		_description(description),
		_picture(picture),
		_locale(locale),
		_timezone(timezone),
		_creation_datetime(QDateTime::fromSecsSinceEpoch(creation_timestamp, Qt::UTC)),
		_modification_datetime(QDateTime::fromSecsSinceEpoch(modification_timestamp, Qt::UTC))
	{

	}

	Subject::Subject(Subject::Id id, Subject::AvatarScheme avatar_scheme, const QString& name, const QString& description, const QString& picture, const QString& locale, const QString& timezone, const QDateTime& creation_datetime, const QDateTime& modification_timestamp)
	:
		_id(id),
		_avatar_scheme(avatar_scheme),
		_name(name),
		_description(description),
		_picture(picture),
		_locale(locale),
		_timezone(timezone),
		_creation_datetime(creation_datetime),
		_modification_datetime(modification_timestamp)
	{

	}

	Subject::Subject(const Subject& origin)
	:
		_id(origin._id),
		_avatar_scheme(origin._avatar_scheme),
		_name(origin._name),
		_description(origin._description),
		_picture(origin._picture),
		_locale(origin._locale),
		_timezone(origin._timezone),
		_creation_datetime(origin._creation_datetime),
		_modification_datetime(origin._modification_datetime)
	{

	}

	Subject::Subject(Subject&& origin)
	:
		_id(std::move(origin._id)),
		_avatar_scheme(std::move(origin._avatar_scheme)),
		_name(std::move(origin._name)),
		_description(std::move(origin._description)),
		_picture(std::move(origin._picture)),
		_locale(std::move(origin._locale)),
		_timezone(std::move(origin._timezone)),
		_creation_datetime(std::move(origin._creation_datetime)),
		_modification_datetime(std::move(origin._modification_datetime))
	{

	}

	void Subject::setAvatarScheme(Subject::AvatarScheme avatar_scheme)
	{
		_avatar_scheme = avatar_scheme;
	}

	void Subject::setName(const QString& name)
	{
		_name = name;
	}

	void Subject::setDescription(const QString& description)
	{
		_description = description;
	}

	void Subject::setPicture(const QString& picture)
	{
		_picture = picture;
	}

	void Subject::setLocale(const QString& locale)
	{
		_locale = locale;
	}

	void Subject::setTimezone(const QString& timezone)
	{
		_timezone = timezone;
	}

	void Subject::setCreationTimestamp(int timestamp)
	{
		_creation_datetime.setSecsSinceEpoch(timestamp);
	}

	void Subject::setModificationTimestamp(int timestamp)
	{
		_modification_datetime.setSecsSinceEpoch(timestamp);
	}

	const Subject::Id& Subject::getId() const
	{
		return _id;
	}

	const Subject::AvatarScheme& Subject::getAvatarScheme() const
	{
		return _avatar_scheme;
	}

	const QString& Subject::getName() const
	{
		return _name;
	}

	const QString& Subject::getDescription() const
	{
		return _description;
	}

	const QString& Subject::getPicture() const
	{
		return _picture;
	}

	const QString& Subject::getLocale() const
	{
		return _locale;
	}

	const QString& Subject::getTimezone() const
	{
		return _timezone;
	}

	const QDateTime& Subject::getCreationDateTime() const
	{
		return _creation_datetime;
	}

	const QDateTime& Subject::getModificationDateTime() const
	{
		return _modification_datetime;
	}

	Subject::operator Subject::Id() const
	{
		return _id;
	}
} } } }
