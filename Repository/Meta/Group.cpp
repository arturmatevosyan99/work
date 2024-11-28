/**
 @file Group.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Group.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	Group::Group(Group::Id id, const QString& name, const QString& description, const QString& picture, int creation_timestamp, int modification_timestamp)
	:
		_id(id),
		_name(name),
		_description(description),
		_picture(picture),
		_creation_datetime(QDateTime::fromSecsSinceEpoch(creation_timestamp, Qt::UTC)),
		_modification_datetime(QDateTime::fromSecsSinceEpoch(modification_timestamp, Qt::UTC))
	{

	}

	Group::Group(Group::Id id, const QString& name, const QString& description, const QString& picture, const QDateTime& creation_datetime, const QDateTime& modification_datetime)
	:
		_id(id),
		_name(name),
		_description(description),
		_picture(picture),
		_creation_datetime(creation_datetime),
		_modification_datetime(modification_datetime)
	{

	}

	Group::Group(const Group& origin)
	:
		_id(origin._id),
		_name(origin._name),
		_description(origin._description),
		_picture(origin._picture),
		_creation_datetime(origin._creation_datetime),
		_modification_datetime(origin._modification_datetime)
	{

	}

	Group::Group(Group&& origin)
	:
		_id(std::move(origin._id)),
		_name(std::move(origin._name)),
		_description(std::move(origin._description)),
		_picture(std::move(origin._picture)),
		_creation_datetime(std::move(origin._creation_datetime)),
		_modification_datetime(std::move(origin._modification_datetime))
	{

	}

	void Group::setName(const QString& name)
	{
		_name = name;
	}

	void Group::setDescription(const QString& description)
	{
		_description = description;
	}

	void Group::setPicture(const QString& picture)
	{
		_picture = picture;
	}

	void Group::setCreationTimestamp(int timestamp)
	{
		_creation_datetime.setSecsSinceEpoch(timestamp);
	}

	void Group::setModificationTimestamp(int timestamp)
	{
		_modification_datetime.setSecsSinceEpoch(timestamp);
	}

	const Group::Id& Group::getId() const
	{
		return _id;
	}

	const QString& Group::getName() const
	{
		return _name;
	}

	const QString& Group::getDescription() const
	{
		return _description;
	}

	const QString& Group::getPicture() const
	{
		return _picture;
	}

	const QDateTime& Group::getCreationDateTime() const
	{
		return _creation_datetime;
	}

	const QDateTime& Group::getModificationDateTime() const
	{
		return _modification_datetime;
	}

	Group::operator Meta::Group::Id() const
	{
		return _id;
	}
} } } }
