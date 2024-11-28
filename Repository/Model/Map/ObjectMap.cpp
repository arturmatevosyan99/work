#include "ObjectMap.hpp"

#include "../../Data/Entity/Tag/ObjectTag.hpp"

#include "../../../Core/Meta/Qd.hpp"
#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"
#include "../../../Core/Data/Predicate/InPredicate.hpp"

#include "../../../Qaos/MakeUnique.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	ObjectMap::ObjectMap(QObject* parent)
	:
		AbstractMap(parent)
	{

	}

	ObjectMap::~ObjectMap()
	{
		unload();
	}

	bool ObjectMap::load(Roaster* roaster, bool lazy_mode)
	{
		if (AbstractMap::isLoaded() || !AbstractMap::load(roaster, lazy_mode)) {
			return false;
		}

		beginResetModel();
		_map.clear();
		if (!isLazy()) {
			const QSharedPointer<Data::Object> object_data(new Data::Object());
			const QSharedPointer<Data::ObjectTag> object_tag_data(new Data::ObjectTag());
			if (object_data->open(getDataProvider()) && object_tag_data->open(getDataProvider())) {
				fetch(object_data);
				if (!object_tag_data->isEmpty()) {
					do {
						AbstractMap::tag(object_tag_data->target_id.value().toLongLong(), object_tag_data->tag_id.value().toLongLong(), true);
					} while (object_tag_data->next());
				}
			}
		}
		endResetModel();
		return true;
	}

	bool ObjectMap::unload()
	{		
		if (AbstractMap::unload()) {
			beginResetModel();
			_map.clear();
			endResetModel();
			return true;
		}
		return false;
	}

	bool ObjectMap::apply(Meta::Essence::Id id, const QDateTime& modification_datetime, const QJsonObject& dump)
	{
		QJsonObject::ConstIterator cts(dump.find("creation_timestamp"));
		if (cts != dump.constEnd()) {
			if (!upsert(
						id,
						dump.value("parent_id").toString("0").toLongLong(),
						dump.value("application_scheme").toString(""),
						dump.value("name").toString(""),
						false,
						dump.value("localization").toBool(),
						dump.value("picture").toString(""),
						dump.value("permission_rule").toInt(0),
						cts->toInt(0),
						modification_datetime.toSecsSinceEpoch()
			)) {
				qCritical() << "Object(" << id << ") upsert error on patching";
				return false;
			}
			emit altered(id, EssenceAlter::InsertAlter);
			return true;
		}

		QJsonObject::ConstIterator parent_id(dump.find("parent_id"));
		QJsonObject::ConstIterator name(dump.find("name"));
		QJsonObject::ConstIterator localization(dump.find("localization"));
		QJsonObject::ConstIterator picture(dump.find("picture"));
		QJsonObject::ConstIterator permission(dump.find("permission"));

		QSharedPointer<Data::Object> entity(new Data::Object());
		entity->modification_timestamp.setValue(modification_datetime);
		entity->creation_timestamp.setGenerated(false);
		entity->access_timestamp.setGenerated(false);
		entity->id.setGenerated(false);
		entity->application_scheme.setGenerated(false);
		entity->meta.setGenerated(false);
		entity->favourite.setGenerated(false);

		if (dump.constEnd() != parent_id) {
			entity->parent_id.setValue(parent_id->toString("0").toLongLong());
		} else {
			entity->parent_id.setGenerated(false);
		}

		if (dump.constEnd() != name) {
			entity->name.setValue(name->toString(""));
		} else {
			entity->name.setGenerated(false);
		}

		if (dump.constEnd() != localization) {
			entity->localization.setValue(localization->toBool(false));
		} else {
			entity->localization.setGenerated(false);
		}

		if (dump.constEnd() != picture) {
			entity->picture.setValue(picture->toString(""));
		} else {
			entity->picture.setGenerated(false);
		}

		if (dump.constEnd() != permission) {
			/// @todo Нужен BitwiseExpression
			Map::Iterator o(_map.find(id));
			if (_map.end() != o) {
				QJsonObject object(permission->toObject({}));
				const int on(object.value("+").toInt(-1));
				const int off(object.value("-").toInt(-1));
				Meta::Object::PermissionRule permission_rule((*o)->getPermission());
				if (on > -1) {
					permission_rule &= on;
				}
				if (off > -1) {
					permission_rule |= off;
				}
				entity->permission.setValue(permission_rule);
			} else {
				entity->permission.setGenerated(false);
			}
		} else {
			entity->permission.setGenerated(false);
		}

		const Core::Data::Field id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(id));
		const Core::Data::Field modification_field(Core::Data::Field::Type::DateTime, QVariant::fromValue(modification_datetime));
		QSharedPointer<Core::Data::CompoundPredicate> modification_predicate
		(
			new Core::Data::CompoundPredicate(Core::Data::CompoundPredicate::Logic::Or)
		);
		modification_predicate->append
		(
			QSharedPointer<Core::Data::ConditionPredicate>::create
			(
				entity->modification_timestamp,
				modification_field,
				Core::Data::ConditionPredicate::Condition::LessOrEqual
			)
		);
		modification_predicate->append
		(
			QSharedPointer<Core::Data::ConditionPredicate>::create
			(
				entity->modification_timestamp,
				Core::Data::Field(Core::Data::Field::Type::DateTime, QVariant(QVariant::Type::DateTime)),
				Core::Data::ConditionPredicate::Condition::Is
			)
		);

		const QSharedPointer<Core::Data::CompoundPredicate> root_predicate
		(
			new Core::Data::CompoundPredicate
			(
				{
					QSharedPointer<Core::Data::ConditionPredicate>::create
					(
						entity->id,
						id_field,
						Core::Data::ConditionPredicate::Condition::Equal
					),
					modification_predicate
				},
				Core::Data::CompoundPredicate::Logic::And
			)
		);

		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		if (!data_factory.update(entity.get(), root_predicate.get()) || data_factory.getAffectedRowsCount() <= 0) {
			qCritical() << "Object(" << id << ") update error on patching";
			return false;
		}
		Map::Iterator o(_map.find(id));
		if (_map.end() != o) {
			QVector<int> role_vector;
			o.value()->setModificationDateTime(modification_datetime);
			role_vector.push_back(static_cast<int>(Meta::Object::Role::ModificationDateTime));
			if (dump.constEnd() != parent_id) {
				o.value()->setParentId(entity->parent_id.value().toLongLong());
				role_vector.push_back(static_cast<int>(Meta::Object::Role::ParentId));
			}
			if (dump.constEnd() != name) {
				o.value()->setName(entity->name.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Object::Role::Name));
			}
			if (dump.constEnd() != localization) {
				o.value()->setLocalization(entity->localization.value().toBool());
				role_vector.push_back(static_cast<int>(Meta::Object::Role::Localization));
			}
			if (dump.constEnd() != picture) {
				o.value()->setPicture(entity->picture.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Object::Role::Picture));
			}
			if (dump.constEnd() != permission) {
				o.value()->setPermission(entity->permission.value().toInt());
				role_vector.push_back(static_cast<int>(Meta::Object::Role::Permission));
			}
			QModelIndex index(createIndex(std::distance(_map.begin(), o), 0));
			emit dataChanged(index, index, role_vector);
		}
		emit altered(id, EssenceAlter::UpdateAlter);
		return true;
	}

	bool ObjectMap::remove(Meta::Essence::Id id)
	{
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		const Core::Data::Field id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(id));
		Data::Object entity;
		if (!data_factory.remove(&entity, Qaos::MakeUnique<Core::Data::ConditionPredicate>(entity.id, id_field, Core::Data::ConditionPredicate::Condition::Equal).get())) {
			return false;
		}

		Map::Iterator o(_map.find(id));
		if (_map.end() != o) {
			const std::size_t row(std::distance(_map.begin(), o));
			beginRemoveRows(QModelIndex(), row, row);
			_map.erase(o);
			endRemoveRows();
		}
		return AbstractMap::remove(id);
	}

	bool ObjectMap::tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff)
	{
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		Data::ObjectTag entity;

		if (onoff) {
			entity.tag_id.setValue(tag_id);
			entity.target_id.setValue(id);
			if (!data_factory.insert(&entity, Core::Data::Factory::InsertionPolicy::Replace)) {
				return false;
			}
		} else {
			Core::Data::Field target_id_field(Core::Data::Field::Type::Integer, id);
			Core::Data::Field tag_id_field(Core::Data::Field::Type::Integer, tag_id);
			if (!data_factory.remove(
									&entity,
									Qaos::MakeUnique<Core::Data::CompoundPredicate>(
										std::initializer_list<QSharedPointer<const Core::Data::WherePredicate>>
										{
											QSharedPointer<Core::Data::ConditionPredicate>::create(entity.target_id, target_id_field, Core::Data::ConditionPredicate::Condition::Equal),
											QSharedPointer<Core::Data::ConditionPredicate>::create(entity.tag_id, tag_id_field, Core::Data::ConditionPredicate::Condition::Equal)
										},
										Core::Data::CompoundPredicate::Logic::And
									).get()
			)) {
				return false;
			}
		}
		return AbstractMap::tag(id, tag_id, onoff);
	}

	bool ObjectMap::opt(Meta::Essence::Id id, const QString& key, const QVariant& value)
	{
		QVariant backup;
		if (!AbstractMap::opt(id, key, value, backup)) {
			return false;
		}

		Data::Object entity;
		entity.meta.setValue(QJsonDocument(optionObject(id)));
		entity.id.setGenerated(false);
		entity.parent_id.setGenerated(false);
		entity.name.setGenerated(false);
		entity.favourite.setGenerated(false);
		entity.localization.setGenerated(false);
		entity.application_scheme.setGenerated(false);
		entity.picture.setGenerated(false);
		entity.permission.setGenerated(false);
		entity.creation_timestamp.setGenerated(false);
		entity.modification_timestamp.setGenerated(false);

		Core::Data::Field id_field(Core::Data::Field::Type::Integer, id);
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());

		if (data_factory.update(&entity, Qaos::MakeUnique<Core::Data::ConditionPredicate>(entity.id, id_field, Core::Data::ConditionPredicate::Condition::Equal).get())) {
			return true;
		}
		AbstractMap::opt(id, key, backup);
		return false;
	}

	QSharedPointer<const Meta::Object> ObjectMap::upsert(Meta::Object::Id id, Meta::Object::Id parent_id, Meta::Object::ApplicationScheme application_scheme, const QString& name, bool favourite, bool localization, const QString& picture, Meta::Object::PermissionRule permission, qint64 creation_timestamp, qint64 modification_timestamp)
	{
		Map::Iterator o(_map.find(id));
		if (_map.end() == o && isLazy() && fetch(id, Recursion::None)) {
			o = _map.find(id);
		}
		QSharedPointer<Meta::Object> retval(nullptr);
		if (_map.end() == o) {
			const std::size_t index(std::distance(_map.begin(), o));
			beginInsertRows(QModelIndex(), index, index);
			o = _map.insert(
							id,
							QSharedPointer<Meta::Object>::create
							(
								id,
								parent_id,
								application_scheme,
								name,
								favourite,
								localization,
								picture,
								permission,
								creation_timestamp,
								modification_timestamp,
								QDateTime::currentSecsSinceEpoch()
							)
			);
			AbstractMap::opt(id);
			retval = o.value();
			endInsertRows();
		} else if (o.value()->getModificationDateTime().toSecsSinceEpoch() >= modification_timestamp || (!o.value()->getModificationDateTime().isValid() && 0 == modification_timestamp)) {
			return o.value();
		} else {
			retval = o.value();
			retval->setName(name);
			retval->setFavourite(favourite);
			retval->setLocalization(localization);
			retval->setParentId(parent_id);
			retval->setApplicationScheme(application_scheme);
			retval->setPicture(picture);
			retval->setPermission(permission);
			retval->setCreationTimestamp(creation_timestamp);
			retval->setModificationTimestamp(modification_timestamp);
			QModelIndex index(createIndex(std::distance(_map.begin(), o), 0));
			emit dataChanged(index, index);
		}

		QSharedPointer<Data::Object> entity(new Data::Object());
		entity->id.setValue(id);
		entity->parent_id.setValue(parent_id);
		entity->name.setValue(name);
		entity->favourite.setValue(favourite);
		entity->localization.setValue(localization);
		entity->application_scheme.setValue(application_scheme);
		entity->picture.setValue(picture);
		entity->permission.setValue(permission);
		entity->creation_timestamp.setValue(retval->getCreationDateTime());
		entity->modification_timestamp.setValue(retval->getModificationDateTime());
		entity->access_timestamp.setGenerated(false);
		entity->meta.setGenerated(false);

		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		return data_factory.insert(entity.get(), Core::Data::Factory::InsertionPolicy::Replace) ? retval : nullptr;
	}

	void ObjectMap::access(Meta::Object::Id id)
	{
		Map::Iterator o(_map.find(id));
		while (_map.end() == o) {
			if (isLazy() && fetch(id, Recursion::None)) {
				o = _map.find(id);
				break;
			}
			return ;
		}
		const QSharedPointer<Meta::Object> object(o.value());
		Data::Object entity;
		entity.id.setGenerated(false);
		entity.parent_id.setGenerated(false);
		entity.name.setGenerated(false);
		entity.favourite.setGenerated(false);
		entity.localization.setGenerated(false);
		entity.application_scheme.setGenerated(false);
		entity.picture.setGenerated(false);
		entity.permission.setGenerated(false);
		entity.creation_timestamp.setGenerated(false);
		entity.modification_timestamp.setGenerated(false);
		entity.access_timestamp.setValue(QDateTime::currentDateTimeUtc());
		entity.meta.setGenerated(false);

		const Core::Data::Field id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(id));
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		if (data_factory.update(&entity, Qaos::MakeUnique<Core::Data::ConditionPredicate>(entity.id, id_field, Core::Data::ConditionPredicate::Condition::Equal).get())) {
			object->setAccessDateTime(entity.access_timestamp.value().toDateTime());
		}
	}

	bool ObjectMap::feed(QSharedPointer<Data::Object> data, int first_row_index, int last_row_index)
	{
		return (!data || first_row_index < 0 || data->isEmpty()) ? false : data->locate(first_row_index) ? fetch(data) : false;
	}

	bool ObjectMap::fetch(const QList<Meta::Essence::Id>& id_list)
	{
		const QSharedPointer<Data::Object> data(new Data::Object());
		data->addPredicate(
							QSharedPointer<Core::Data::InPredicate>::create(
																			data->id,
																			QVariantList(id_list.begin(), id_list.end())
							)
		);
		return data->open(getDataProvider(), true) ? fetch(data) : false;
	}

	bool ObjectMap::fetch(Meta::Essence::Id id, Recursion recursion)
	{
		const QSharedPointer<Data::Object> data(new Data::Object());
		const Core::Data::Field id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(id));
		/// @todo Descendant not fully implemented
		/// @todo Tree not fully implemented
		if (Recursion::Children == recursion || Recursion::Family == recursion || Recursion::Descendant == recursion || Recursion::Tree == recursion) {
			data->addPredicate(
								QSharedPointer<Core::Data::ConditionPredicate>::create(
																						data->parent_id,
																						id_field,
																						Core::Data::ConditionPredicate::Condition::Equal
								)
			);
		}
		if (Recursion::None == recursion || Recursion::Family == recursion || Recursion::Tree == recursion) {
			data->addPredicate(
								QSharedPointer<Core::Data::ConditionPredicate>::create(
																						data->id,
																						id_field,
																						Core::Data::ConditionPredicate::Condition::Equal
								)
			);
		}

		return data->open(getDataProvider(), true) ? fetch(data) : false;
	}

	bool ObjectMap::fetch(QSharedPointer<Data::Object> data)
	{
		if (data->isEmpty()) {
			return false;
		}
		/// @todo max_id & min_id and load tags
		do {
			const Meta::Object::Id id(data->id.value().toLongLong());
			Map::Iterator o(_map.find(id));
			if (
				_map.end() != o
				&&
				(
					(o.value()->getModificationDateTime().toSecsSinceEpoch() >= data->modification_timestamp.value().toDateTime().toSecsSinceEpoch())
					||
					(!o.value()->getModificationDateTime().isValid() && 0 == data->modification_timestamp.value().toDateTime().toSecsSinceEpoch())
				)
			) {
				continue;
			}
//			beginInsertRows(QModelIndex(), index, index);
			_map.insert(
						id,
						QSharedPointer<Meta::Object>::create
						(
							id,
							data->parent_id.value().toLongLong(),
							data->application_scheme.value().toString(),
							data->name.value().toString(),
							data->favourite.value().toBool(),
							data->localization.value().toBool(),
							data->picture.value().toString(),
							data->permission.value().toInt(),
							data->creation_timestamp.value().toDateTime(),
							data->modification_timestamp.value().toDateTime(),
							data->access_timestamp.value().toDateTime()
						)
			);
			AbstractMap::opt(id, data->meta.value().toJsonObject());
//			endInsertRows();
		} while (!data->isEOF() && data->next()); /// < Just till the end of a fetch
//		AbstractMap::tag(id);
		return true;
	}

	bool ObjectMap::fetch(QSharedPointer<Core::Data::Rowset> row_set)
	{
		const QSharedPointer<Data::Object> data(new Data::Object());
		return data->open(row_set) ? fetch(data) : false;
	}

	QSharedPointer<Data::AbstractRecord> ObjectMap::makeDataRecord() const
	{
		return QSharedPointer<Data::Object>::create();
	}

	QSharedPointer<const Meta::Object> ObjectMap::getObject(Meta::Object::Id id)
	{
		Map::Iterator o(_map.find(id));
		if (_map.end() != o) {
			return o.value();
		} else if (!isLazy()) {
			return nullptr;
		} else if (fetch({id})) {
			o = _map.find(id);
		}
		return _map.end() == o ? nullptr : o.value();
	}

	int ObjectMap::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _map.size();
	}

	QVariant ObjectMap::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {
			case Meta::Object::Role::EssenceId: return QVariant::fromValue(Core::Meta::Qd((_map.constBegin() + index.row()).value()->getId()));
			case Meta::Object::Role::ParentId: return QVariant::fromValue(Core::Meta::Qd((_map.constBegin() + index.row()).value()->getParentId()));
			case Meta::Object::Role::Name: return (_map.constBegin() + index.row()).value()->getName();
			case Meta::Object::Role::Favourite: return (_map.constBegin() + index.row()).value()->getFavourite();
			case Meta::Object::Role::Localization: return (_map.constBegin() + index.row()).value()->getLocalization();
			case Meta::Object::Role::Picture: return (_map.constBegin() + index.row()).value()->getPicture();
			case Meta::Object::Role::Permission: return (_map.constBegin() + index.row()).value()->getPermission();
			case Meta::Object::Role::ApplicationSchema: return (_map.constBegin() + index.row()).value()->getApplicationScheme();
			case Meta::Object::Role::CreationDateTime: return (_map.constBegin() + index.row()).value()->getCreationDateTime();
			case Meta::Object::Role::ModificationDateTime: return (_map.constBegin() + index.row()).value()->getModificationDateTime();
			case Meta::Object::Role::AccessDateTime: return (_map.constBegin() + index.row()).value()->getAccessDateTime();

			default: return AbstractMap::data(index, role);
		}
	}

	QVariant ObjectMap::data(const Meta::Essence::Id& id, int role) const
	{
		Map::ConstIterator m(_map.find(id));
		if (_map.constEnd() == m) {
			return QVariant();
		}

		switch (role) {
			case Meta::Object::Role::EssenceId: return QVariant::fromValue(Core::Meta::Qd(m.value()->getId()));
			case Meta::Object::Role::ParentId: return QVariant::fromValue(Core::Meta::Qd(m.value()->getParentId()));
			case Meta::Object::Role::Name: return m.value()->getName();
			case Meta::Object::Role::Favourite: return m.value()->getFavourite();
			case Meta::Object::Role::Localization: return m.value()->getLocalization();
			case Meta::Object::Role::Picture: return m.value()->getPicture();
			case Meta::Object::Role::Permission: return m.value()->getPermission();
			case Meta::Object::Role::ApplicationSchema: return m.value()->getApplicationScheme();
			case Meta::Object::Role::CreationDateTime: return m.value()->getCreationDateTime();
			case Meta::Object::Role::ModificationDateTime: return m.value()->getModificationDateTime();
			case Meta::Object::Role::AccessDateTime: return m.value()->getAccessDateTime();

			default: return AbstractMap::data(id, role);
		}
	}

	QHash<int, QByteArray> ObjectMap::roleNames() const
	{
		QHash<int, QByteArray> retval(AbstractMap::roleNames());

		retval[Meta::Object::Role::EssenceId] = "id";
		retval[Meta::Object::Role::ParentId] = "parent_id";
		retval[Meta::Object::Role::Name] = "name";
		retval[Meta::Object::Role::Favourite] = "favourite";
		retval[Meta::Object::Role::Localization] = "localization";
		retval[Meta::Object::Role::Picture] = "picture";
		retval[Meta::Object::Role::Permission] = "permission";
		retval[Meta::Object::Role::ApplicationSchema] = "application_scheme";
		retval[Meta::Object::Role::CreationDateTime] = "creation_datetime";
		retval[Meta::Object::Role::ModificationDateTime] = "modification_datetime";
		retval[Meta::Object::Role::AccessDateTime] = "access_datetime";

		return retval;
	}

	bool ObjectMap::isExist(const Meta::Essence::Id& id) const
	{
		return _map.contains(id);
	}
} } } }
