#include "GroupMap.hpp"

#include "../../Data/Entity/Tag/GroupTag.hpp"

#include "../../../Core/Meta/Qd.hpp"
#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"
#include "../../../Core/Data/Predicate/InPredicate.hpp"

#include "../../../Qaos/MakeUnique.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	GroupMap::GroupMap(QObject* parent)
	:
		AbstractMap(parent)
	{

	}

	GroupMap::~GroupMap()
	{
		unload();
	}

	bool GroupMap::load(Roaster* roaster, bool lazy_mode)
	{
		if (AbstractMap::isLoaded() || !AbstractMap::load(roaster, lazy_mode)) {
			return false;
		}

		beginResetModel();
		_map.clear();
		while (!isLazy()) {
			const QSharedPointer<Data::Group> subject_data(new Data::Group());
			const QSharedPointer<Data::GroupTag> subject_tag_data(new Data::GroupTag());
			if (!subject_data->open(getDataProvider()) || !subject_tag_data->open(getDataProvider())) {
				break;
			}
			fetch(subject_data);
			if (!subject_tag_data->isEmpty()) {
				do {
					AbstractMap::tag(subject_tag_data->target_id.value().toLongLong(), subject_tag_data->tag_id.value().toLongLong(), true);
				} while (subject_tag_data->next());
			}
			break;
		}
		endResetModel();
		return true;
	}

	bool GroupMap::unload()
	{		
		if (AbstractMap::unload()) {
			beginResetModel();
			_map.clear();
			endResetModel();
			return true;
		}
		return false;
	}

	bool GroupMap::apply(Meta::Group::Id id, const QDateTime& modification_datetime, const QJsonObject& dump)
	{
		QJsonObject::ConstIterator cts(dump.find("creation_timestamp"));
		if (cts != dump.constEnd()) {
			if (!upsert(
						id,
						dump.value("name").toString(""),
						dump.value("description").toString(""),
						dump.value("picture").toString(""),
						cts->toInt(0),
						modification_datetime.toSecsSinceEpoch()
			)) {
				qCritical() << "Group(" << id << ") upsert error on patching";
				return false;
			}
			emit altered(id, EssenceAlter::InsertAlter);
			return true;
		}

		QJsonObject::ConstIterator name(dump.find("name"));
		QJsonObject::ConstIterator description(dump.find("description"));
		QJsonObject::ConstIterator picture(dump.find("picture"));

		QSharedPointer<Data::Group> entity(new Data::Group());
		entity->modification_timestamp.setValue(modification_datetime.toSecsSinceEpoch());
		entity->creation_timestamp.setGenerated(false);
		entity->id.setGenerated(false);
		entity->meta.setGenerated(false);

		if (dump.constEnd() != name) {
			entity->name.setValue(name->toString(""));
		} else {
			entity->name.setGenerated(false);
		}

		if (dump.constEnd() != picture) {
			entity->picture.setValue(picture->toString(""));
		} else {
			entity->picture.setGenerated(false);
		}

		if (dump.constEnd() != description) {
			entity->description.setValue(description->toString(""));
		} else {
			entity->description.setGenerated(false);
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
			qCritical() << "Group(" << id << ") update error on patching";
			return false;
		}
		Map::Iterator s(_map.find(id));
		if (_map.end() != s) {
			QVector<int> role_vector;
			s.value()->setModificationTimestamp(modification_datetime.toSecsSinceEpoch());
			if (dump.constEnd() != name) {
				s.value()->setName(entity->name.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Group::Role::Name));
			}
			if (dump.constEnd() != description) {
				s.value()->setDescription(entity->description.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Group::Role::Description));
			}
			if (dump.constEnd() != picture) {
				s.value()->setPicture(entity->picture.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Group::Role::Picture));
			}
			QModelIndex index(createIndex(std::distance(_map.begin(), s), 0));
			emit dataChanged(index, index, role_vector);
		}
		emit altered(id, EssenceAlter::UpdateAlter);
		return true;
	}

	bool GroupMap::tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff)
	{
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		Data::GroupTag entity;

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

	bool GroupMap::opt(Meta::Essence::Id id, const QString& key, const QVariant& value)
	{
		QVariant backup;
		if (!AbstractMap::opt(id, key, value, backup)) {
			return false;
		}

		Data::Group entity;
		entity.meta.setValue(QJsonDocument(optionObject(id)));
		entity.id.setGenerated(false);
		entity.name.setGenerated(false);
		entity.description.setGenerated(false);
		entity.picture.setGenerated(false);
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

	QSharedPointer<const Meta::Group> GroupMap::upsert(Meta::Group::Id id, const QString& name, const QString& description, const QString& picture, int creation_timestamp, int modification_timestamp)
	{
		Map::Iterator s(_map.find(id));
		if (_map.end() == s && isLazy() && fetch({id})) {
			s = _map.find(id);
		}
		QSharedPointer<Meta::Group> retval;
		if (_map.end() == s) {
			const std::size_t index(std::distance(_map.begin(), s));
			beginInsertRows(QModelIndex(), index, index);
			s =_map.insert(
							id,
							QSharedPointer<Meta::Group>::create
							(
								id,
								name,
								description,
								picture,
								creation_timestamp,
								modification_timestamp
							)
			);
			AbstractMap::opt(id);
			retval = s.value();
			endInsertRows();
		} else if (s.value()->getModificationDateTime().currentSecsSinceEpoch() >= modification_timestamp) {
			return s.value();
		} else {
			retval = s.value();
			retval->setName(name);
			retval->setDescription(description);
			retval->setPicture(picture);
			retval->setCreationTimestamp(creation_timestamp);
			retval->setModificationTimestamp(modification_timestamp);
			QModelIndex index(createIndex(std::distance(_map.begin(), s), 0));
			emit dataChanged(index, index);
		}

		QSharedPointer<Data::Group> entity(new Data::Group());
		entity->id.setValue(id);
		entity->name.setValue(name);
		entity->description.setValue(description);
		entity->picture.setValue(picture);
		entity->creation_timestamp.setValue(retval->getCreationDateTime());
		entity->modification_timestamp.setValue(retval->getModificationDateTime());
		entity->meta.setGenerated(false);

		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		return data_factory.insert(entity.get(), Core::Data::Factory::InsertionPolicy::Replace) ? retval : nullptr;
	}

	bool GroupMap::remove(Meta::Essence::Id id)
	{
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		const Core::Data::Field id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(id));
		Data::Group entity;
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

	bool GroupMap::fetch(const QList<Meta::Essence::Id>& id_list)
	{
		const QSharedPointer<Data::Group> data(new Data::Group());
		data->addPredicate(
							QSharedPointer<Core::Data::InPredicate>::create(
																			data->id,
																			QVariantList(id_list.begin(), id_list.end())
							)
		);
		return data->open(getDataProvider(), true) ? fetch(data) : false;
	}

	bool GroupMap::fetch(QSharedPointer<Data::Group> data)
	{
		if (data->isEmpty()) {
			return false;
		}
		/// @todo max_id & min_id and load tags
		do {
			const Meta::Group::Id id(data->id.value().toLongLong());
			/// @??? нужно ли делать insertRows
			_map.insert(
						id,
						QSharedPointer<Meta::Group>::create
						(
							id,
							data->name.value().toString(),
							data->description.value().toString(),
							data->picture.value().toString(),
							data->creation_timestamp.value().toDateTime(),
							data->modification_timestamp.value().toDateTime()
						)
			);
			AbstractMap::opt(id, data->meta.value().toJsonObject());
		} while (data->next());
//		AbstractMap::tag(id);
		return true;
	}

	bool GroupMap::fetch(QSharedPointer<Core::Data::Rowset> row_set)
	{
		return false;
	}

	QSharedPointer<Data::AbstractRecord> GroupMap::makeDataRecord() const
	{
		return QSharedPointer<Data::Group>::create();
	}

	QSharedPointer<const Meta::Group> GroupMap::getGroup(Meta::Group::Id id)
	{
		Map::Iterator s(_map.find(id));
		if (_map.end() != s) {
			return s.value();
		} else if (!isLazy()) {
			return nullptr;
		} else if (fetch({id})) {
			s = _map.find(id);
		}
		return _map.end() == s ? nullptr : s.value();
	}

	int GroupMap::rowCount(const QModelIndex& parent) const
	{
		if (parent.isValid()) {
			return 0;
		}
		return _map.size();
	}

	QVariant GroupMap::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {
			case Meta::Group::Role::EssenceId: return QVariant::fromValue(Core::Meta::Qd((_map.constBegin() + index.row()).value()->getId()));
			case Meta::Group::Role::Name: return (_map.constBegin() + index.row()).value()->getName();
			case Meta::Group::Role::Description: return (_map.constBegin() + index.row()).value()->getDescription();
			case Meta::Group::Role::Picture: return (_map.constBegin() + index.row()).value()->getPicture();
			case Meta::Group::Role::CreationDateTime: return (_map.constBegin() + index.row()).value()->getCreationDateTime();
			case Meta::Group::Role::ModificationDateTime: return (_map.constBegin() + index.row()).value()->getModificationDateTime();

			default: return AbstractMap::data(index, role);
		}
	}

	QVariant GroupMap::data(const Meta::Essence::Id& id, int role) const
	{
		Map::ConstIterator m(_map.find(id));
		if (_map.constEnd() == m) {
			return QVariant();
		}

		switch (role) {
			case Meta::Group::Role::EssenceId: return QVariant::fromValue(Core::Meta::Qd(m.value()->getId()));
			case Meta::Group::Role::Name: return m.value()->getName();
			case Meta::Group::Role::Description: return m.value()->getDescription();
			case Meta::Group::Role::Picture: return m.value()->getPicture();
			case Meta::Group::Role::CreationDateTime: return m.value()->getCreationDateTime();
			case Meta::Group::Role::ModificationDateTime: return m.value()->getModificationDateTime();

			default: return AbstractMap::data(id, role);
		}
	}

	QHash<int, QByteArray> GroupMap::roleNames() const
	{
		QHash<int, QByteArray> retval(AbstractMap::roleNames());

		retval[Meta::Group::Role::EssenceId] = "id";
		retval[Meta::Group::Role::Name] = "name";
		retval[Meta::Group::Role::Description] = "description";
		retval[Meta::Group::Role::Picture] = "picture";
		retval[Meta::Group::Role::CreationDateTime] = "creation_datetime";
		retval[Meta::Group::Role::ModificationDateTime] = "modification_datetime";

		return retval;
	}

	bool GroupMap::isExist(const Meta::Essence::Id& id) const
	{
		return _map.contains(id);
	}
} } } }
