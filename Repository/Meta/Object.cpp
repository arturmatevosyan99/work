/**
 @file Object.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Object.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	const Object::Id Object::BrandNewId = -1;
	const Object::Id Object::DeadlockId = 0;
	const Object::Id Object::DomainId = 1;

	Object::Handle Object::ToHandle(const QString& value, bool exception_safe)
	{
		if (!exception_safe) {
			return static_cast<Object::Handle>(value.toULongLong());
		}
		try {
			return static_cast<Object::Handle>(value.toULongLong());
		} catch (const std::exception&) {
			return 0;
		}
	}

	Object::Handle Object::ToHandle(const QJsonValue& value, bool exception_safe)
	{
		if (!exception_safe) {
			return ToHandle(value.toString());
		}
		try {
			return ToHandle(value.toString("0"));
		} catch (const std::exception&) {
			return 0;
		}
	}

	QMap<Object::Id, QSharedPointer<const Meta::Object>> Object::Parse(const QJsonValue& value)
	{
		if (value.isArray()) {
			return Object::Parse(value.toArray());
		} else if (value.isObject()) {
			QSharedPointer<const Meta::Object> object(Object::Parse(value.toObject()));
			return {{object->getId(), object}};
		}
		return {};
	}

	QMap<Object::Id, QSharedPointer<const Meta::Object>> Object::Parse(const QJsonArray& array)
	{
		QMap<Object::Id, QSharedPointer<const Meta::Object>> retval;
		foreach (const QJsonValue& value, array) {
			if (value.isObject()) {
				if (QSharedPointer<const Meta::Object> object = Object::Parse(value.toObject())) {
					retval.insert(object->getId(), object);
				}
			}
		}
		return retval; /// retval.insert(object->getId(), object);
	}

	QSharedPointer<const Meta::Object> Object::Parse(const QJsonObject& object)
	{
		qDebug() << "Parsing ace object" << object;

		const QJsonValue o(object.value("option"));
		return QSharedPointer<const Meta::Object>::create(
															Essence::ToId(object.value("id")),
															Essence::ToId(object.value("parent_id")),
															object.value("application_scheme").toString("0"),
															object.value("name").toString(),
															object.value("favourite").toBool(),
															object.value("localization").toBool(),
															object.value("picture").toString(),
															object.value("permission").toInt(),
															object.value("creation_timestamp").toInt(),
															object.value("modification_timestamp").toInt(),
															object.value("access_timestamp").toInt(),
															o.isObject() ? o.toObject().toVariantMap() : (o.isArray() ? o.toArray().toVariantList() : o.toVariant())
		);
	}

	Object::Object(Object::Id id, Object::Id parent_id, Object::ApplicationScheme application_scheme, const QString& name, bool favourite, bool localization, const QString& picture, Object::PermissionRule permission, const QDateTime& creation_datetime, const QDateTime& modification_datetime, const QDateTime& access_datetime, const QVariant& option)
	:
		_id(id),
		_parent_id(parent_id),
		_application_scheme(application_scheme),
		_name(name),
		_favourite(favourite),
		_localization(localization),
		_picture(picture),
		_permission(permission),
		_creation_datetime(creation_datetime),
		_modification_datetime(modification_datetime),
		_access_datetime(access_datetime),
		_option(option)
	{
	}

	Object::Object(Object::Id id, Object::Id parent_id, Object::ApplicationScheme application_scheme, const QString& name, bool favourite, bool localization, const QString& picture, Object::PermissionRule permission, qint64 creation_timestamp, qint64 modification_timestamp, qint64 access_timestamp, const QVariant& option)
	:
		_id(id),
		_parent_id(parent_id),
		_application_scheme(application_scheme),
		_name(name),
		_favourite(favourite),
		_localization(localization),
		_picture(picture),
		_permission(permission),
		_creation_datetime(QDateTime::fromSecsSinceEpoch(creation_timestamp, Qt::UTC)),
		_modification_datetime(QDateTime::fromSecsSinceEpoch(modification_timestamp, Qt::UTC)),
		_access_datetime(QDateTime::fromSecsSinceEpoch(access_timestamp, Qt::UTC)),
		_option(option)
	{
	}

	void Object::setParentId(Object::Id object_id)
	{
		_parent_id = object_id;
	}

	void Object::setName(const QString& name)
	{
		_name = name;
	}

	void Object::setFavourite(bool onoff)
	{
		_favourite = onoff;
	}

	void Object::setLocalization(bool onoff)
	{
		_localization = onoff;
	}

	void Object::setApplicationScheme(Object::ApplicationScheme scheme)
	{
		_application_scheme = scheme;
	}

	void Object::setOption(const QVariant& option)
	{
		_option = option;
	}

	void Object::setPicture(const QString& picture)
	{
		_picture = picture;
	}

	void Object::setPermission(Object::PermissionRule permission)
	{
		_permission = permission;
	}

	void Object::setCreationDateTime(const QDateTime& datetime)
	{
		_creation_datetime = datetime;
	}

	void Object::setCreationTimestamp(qint64 timestamp)
	{
		_creation_datetime.setSecsSinceEpoch(timestamp);
	}

	void Object::setModificationDateTime(const QDateTime& datetime)
	{
		_modification_datetime = datetime;
	}

	void Object::setModificationTimestamp(qint64 timestamp)
	{
		_modification_datetime.setSecsSinceEpoch(timestamp);
	}

	void Object::setAccessDateTime(const QDateTime& datetime)
	{
		_access_datetime = datetime;
	}

	void Object::setAccessTimestamp(qint64 timestamp)
	{
		_access_datetime.setSecsSinceEpoch(timestamp);
	}

	const Object::Id& Object::getId() const
	{
		return _id;
	}

	const Object::Id& Object::getParentId() const
	{
		return _parent_id;
	}

	const Object::ApplicationScheme& Object::getApplicationScheme() const
	{
		return _application_scheme;
	}

	const QVariant& Object::getOption() const
	{
		return _option;
	}

	const QString& Object::getName() const
	{
		return _name;
	}

	QString Object::getLocalizedName() const
	{
		return !_localization ? _name : QObject::tr(qPrintable(_name), "Repository::Meta::Object");
	}

	const bool& Object::getFavourite() const
	{
		return _favourite;
	}

	const bool& Object::getLocalization() const
	{
		return _localization;
	}

	const QString& Object::getPicture() const
	{
		return _picture;
	}

	const Object::PermissionRule& Object::getPermission() const
	{
		return _permission;
	}

	const QDateTime& Object::getCreationDateTime() const
	{
		return _creation_datetime;
	}

	const QDateTime& Object::getModificationDateTime() const
	{
		return _modification_datetime;
	}

	const QDateTime& Object::getAccessDateTime() const
	{
		return _access_datetime;
	}

	Object::operator Meta::Object::Id() const
	{
		return _id;
	}
} } } }
