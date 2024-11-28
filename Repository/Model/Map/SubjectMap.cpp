#include "SubjectMap.hpp"

#include "../../Data/Entity/Tag/SubjectTag.hpp"

#include "../../../Core/Meta/Qd.hpp"
#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"
#include "../../../Core/Data/Predicate/InPredicate.hpp"

#include "../../../Qaos/MakeUnique.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	SubjectMap::SubjectMap(QObject* parent)
	:
		AbstractMap(parent)
	{

	}

	SubjectMap::~SubjectMap()
	{
		unload();
	}

	bool SubjectMap::load(Roaster* roaster, bool lazy_mode)
	{
		if (AbstractMap::isLoaded() || !AbstractMap::load(roaster, lazy_mode)) {
			return false;
		}

		beginResetModel();
		_map.clear();
		while (!isLazy()) {
			const QSharedPointer<Data::Subject> subject_data(new Data::Subject());
			const QSharedPointer<Data::SubjectTag> subject_tag_data(new Data::SubjectTag());
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

	bool SubjectMap::unload()
	{		
		if (AbstractMap::unload()) {
			beginResetModel();
			_map.clear();
			endResetModel();
			return true;
		}
		return false;
	}

	bool SubjectMap::apply(Meta::Essence::Id id, const QDateTime& modification_datetime, const QJsonObject& dump)
	{
		QJsonObject::ConstIterator cts(dump.find("creation_timestamp"));
		if (cts != dump.constEnd()) {
			if (!upsert(
							id,
							dump.value("avatar_scheme").toString(""),
							dump.value("name").toString(""),
							dump.value("description").toString(""),
							dump.value("picture").toString(""),
							dump.value("locale").toString(""),
							dump.value("timezone").toString(""),
							cts->toInt(0),
							modification_datetime.toSecsSinceEpoch()
			)) {
				qCritical() << "Subject(" << id << ") upsert error on patching";
				return false;
			}
			emit altered(id, EssenceAlter::InsertAlter);
			return true;
		}

		QJsonObject::ConstIterator name(dump.find("name"));
		QJsonObject::ConstIterator description(dump.find("description"));
		QJsonObject::ConstIterator picture(dump.find("picture"));
		QJsonObject::ConstIterator locale(dump.find("locale"));
		QJsonObject::ConstIterator timezone(dump.find("timezone"));

		QSharedPointer<Data::Subject> entity(new Data::Subject());
		entity->modification_timestamp.setValue(modification_datetime.toSecsSinceEpoch());
		entity->avatar_scheme.setGenerated(false);
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

		if (dump.constEnd() != locale) {
			entity->locale.setValue(locale->toString(""));
		} else {
			entity->locale.setGenerated(false);
		}

		if (dump.constEnd() != timezone) {
			entity->timezone.setValue(timezone->toString(""));
		} else {
			entity->timezone.setGenerated(false);
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
			qCritical() << "Subject(" << id << ") update error on patching";
			return false;
		}
		Map::Iterator s(_map.find(id));
		if (_map.end() != s) {
			QVector<int> role_vector;
			s.value()->setModificationTimestamp(modification_datetime.toSecsSinceEpoch());
			if (dump.constEnd() != name) {
				s.value()->setName(entity->name.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Subject::Role::Name));
			}
			if (dump.constEnd() != description) {
				s.value()->setDescription(entity->description.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Subject::Role::Description));
			}
			if (dump.constEnd() != picture) {
				s.value()->setPicture(entity->picture.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Subject::Role::Picture));
			}
			if (dump.constEnd() != locale) {
				s.value()->setLocale(entity->locale.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Subject::Role::Locale));
			}
			if (dump.constEnd() != timezone) {
				s.value()->setTimezone(entity->timezone.value().toString());
				role_vector.push_back(static_cast<int>(Meta::Subject::Role::Timezone));
			}
			QModelIndex index(createIndex(std::distance(_map.begin(), s), 0));
			emit dataChanged(index, index, role_vector);
		}
		emit altered(id, EssenceAlter::UpdateAlter);
		return true;
	}

	bool SubjectMap::remove(Meta::Essence::Id id)
	{
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		const Core::Data::Field id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(id));
		Data::Subject entity;
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

	bool SubjectMap::tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff)
	{
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		Data::SubjectTag entity;

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

	bool SubjectMap::opt(Meta::Essence::Id id, const QString& key, const QVariant& value)
	{
		QVariant backup;
		if (!AbstractMap::opt(id, key, value, backup)) {
			return false;
		}

		Data::Subject entity;
		entity.meta.setValue(QJsonDocument(optionObject(id)));
		entity.id.setGenerated(false);
		entity.avatar_scheme.setGenerated(false);
		entity.locale.setGenerated(false);
		entity.name.setGenerated(false);
		entity.description.setGenerated(false);
		entity.timezone.setGenerated(false);
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

	QSharedPointer<const Meta::Subject> SubjectMap::upsert(Meta::Subject::Id id, Meta::Subject::AvatarScheme avatar_scheme, const QString& name, const QString& description, const QString& picture, const QString& locale, const QString& timezone, int creation_timestamp, int modification_timestamp)
	{
		Map::Iterator s(_map.find(id));
		if (_map.end() == s && isLazy() && fetch({id})) {
			s = _map.find(id);
		}
		QSharedPointer<Meta::Subject> retval;
		if (_map.end() == s) {
			const std::size_t index(std::distance(_map.begin(), s));
			beginInsertRows(QModelIndex(), index, index);
			s =_map.insert(
							id,
							QSharedPointer<Meta::Subject>::create
							(
								id,
								avatar_scheme,
								name,
								description,
								picture,
								locale,
								timezone,
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
			retval->setAvatarScheme(avatar_scheme);
			retval->setName(name);
			retval->setDescription(description);
			retval->setPicture(picture);
			retval->setLocale(locale);
			retval->setTimezone(timezone);
			retval->setCreationTimestamp(creation_timestamp);
			retval->setModificationTimestamp(modification_timestamp);
			QModelIndex index(createIndex(std::distance(_map.begin(), s), 0));
			emit dataChanged(index, index);
		}

		QSharedPointer<Data::Subject> entity(new Data::Subject());
		entity->id.setValue(id);
		entity->avatar_scheme.setValue(avatar_scheme);
		entity->name.setValue(name);
		entity->description.setValue(description);
		entity->picture.setValue(picture);
		entity->locale.setValue(locale);
		entity->timezone.setValue(timezone);
		entity->creation_timestamp.setValue(retval->getCreationDateTime());
		entity->modification_timestamp.setValue(retval->getModificationDateTime());
		entity->meta.setGenerated(false);

		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		return data_factory.insert(entity.get(), Core::Data::Factory::InsertionPolicy::Replace) ? retval : nullptr;
	}

	bool SubjectMap::fetch(const QList<Meta::Essence::Id>& id_list)
	{
		const QSharedPointer<Data::Subject> data(new Data::Subject());
		data->addPredicate(
							QSharedPointer<Core::Data::InPredicate>::create(
																			data->id,
																			QVariantList(id_list.begin(), id_list.end())
							)
		);
		return data->open(getDataProvider(), true) ? fetch(data) : false;
	}

	bool SubjectMap::fetch(QSharedPointer<Data::Subject> data)
	{
		if (data->isEmpty()) {
			return false;
		}
		/// @todo max_id & min_id and load tags
		do {
			const Meta::Subject::Id id(data->id.value().toLongLong());
			/// @??? нужно ли делать insertRows
			_map.insert(
						id,
						QSharedPointer<Meta::Subject>::create
						(
							id,
							data->avatar_scheme.value().toString(),
							data->name.value().toString(),
							data->description.value().toString(),
							data->picture.value().toString(),
							data->locale.value().toString(),
							data->timezone.value().toString(),
							data->creation_timestamp.value().toDateTime(),
							data->modification_timestamp.value().toDateTime()
						)
			);
			AbstractMap::opt(id, data->meta.value().toJsonObject());
		} while (data->next());
//		AbstractMap::tag(id);
		return true;
	}

	bool SubjectMap::fetch(QSharedPointer<Core::Data::Rowset> row_set)
	{
		return false;
	}

	QSharedPointer<Data::AbstractRecord> SubjectMap::makeDataRecord() const
	{
		return QSharedPointer<Data::Subject>::create();
	}

	QSharedPointer<const Meta::Subject> SubjectMap::getSubject(Meta::Subject::Id id)
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

	int SubjectMap::rowCount(const QModelIndex& parent) const
	{
		if (parent.isValid()) {
			return 0;
		}
		return _map.size();
	}

	QVariant SubjectMap::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {
			case Meta::Subject::Role::EssenceId: return QVariant::fromValue(Core::Meta::Qd((_map.constBegin() + index.row()).value()->getId()));
			case Meta::Subject::Role::AvatarSchema: return (_map.constBegin() + index.row()).value()->getAvatarScheme();
			case Meta::Subject::Role::Name: return (_map.constBegin() + index.row()).value()->getName();
			case Meta::Subject::Role::Description: return (_map.constBegin() + index.row()).value()->getDescription();
			case Meta::Subject::Role::Picture: return (_map.constBegin() + index.row()).value()->getPicture();
			case Meta::Subject::Role::Locale: return (_map.constBegin() + index.row()).value()->getLocale();
			case Meta::Subject::Role::Timezone: return (_map.constBegin() + index.row()).value()->getTimezone();
			case Meta::Subject::Role::CreationDateTime: return (_map.constBegin() + index.row()).value()->getCreationDateTime();
			case Meta::Subject::Role::ModificationDateTime: return (_map.constBegin() + index.row()).value()->getModificationDateTime();

			default: return AbstractMap::data(index, role);
		}
	}

	QVariant SubjectMap::data(const Meta::Essence::Id& id, int role) const
	{
		Map::ConstIterator m(_map.find(id));
		if (_map.constEnd() == m) {
			return QVariant();
		}

		switch (role) {
			case Meta::Subject::Role::EssenceId: return QVariant::fromValue(Core::Meta::Qd(m.value()->getId()));
			case Meta::Subject::Role::AvatarSchema: return m.value()->getAvatarScheme();
			case Meta::Subject::Role::Name: return m.value()->getName();
			case Meta::Subject::Role::Description: return m.value()->getDescription();
			case Meta::Subject::Role::Picture: return m.value()->getPicture();
			case Meta::Subject::Role::Locale: return m.value()->getLocale();
			case Meta::Subject::Role::Timezone: return m.value()->getTimezone();
			case Meta::Subject::Role::CreationDateTime: return m.value()->getCreationDateTime();
			case Meta::Subject::Role::ModificationDateTime: return m.value()->getModificationDateTime();

			default: return AbstractMap::data(id, role);
		}
	}

	QHash<int, QByteArray> SubjectMap::roleNames() const
	{
		QHash<int, QByteArray> retval(AbstractMap::roleNames());

		retval[Meta::Subject::Role::EssenceId] = "id";
		retval[Meta::Subject::Role::AvatarSchema] = "avatar_scheme";
		retval[Meta::Subject::Role::Name] = "name";
		retval[Meta::Subject::Role::Description] = "description";
		retval[Meta::Subject::Role::Picture] = "picture";
		retval[Meta::Subject::Role::Locale] = "locale";
		retval[Meta::Subject::Role::Timezone] = "timezone";
		retval[Meta::Subject::Role::CreationDateTime] = "creation_datetime";
		retval[Meta::Subject::Role::ModificationDateTime] = "modification_datetime";

		return retval;
	}

	bool SubjectMap::isExist(const Meta::Essence::Id& id) const
	{
		return _map.contains(id);
	}

	SubjectKey::SubjectKey(QWeakPointer<const Meta::Subject>&& subject)
	:
		_subject(subject)
	{

	}

	SubjectKey::SubjectKey(SubjectKey&& origin)
	:
		_subject(std::move(origin._subject))
	{

	}

	SubjectKey::SubjectKey(const SubjectKey& origin)
	:
		_subject(origin._subject)
	{

	}

	SubjectKey& SubjectKey::operator=(SubjectKey&& origin)
	{
		_subject = std::move(origin._subject);
		return *this;
	}

	SubjectKey& SubjectKey::operator=(const SubjectKey& origin)
	{
		_subject = origin._subject;
		return *this;
	}

	Cadabra::Queen::Core::Meta::Qd SubjectKey::getId() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getId() : -1;
	}

	QString SubjectKey::getDisplayId() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getName().isEmpty() ? QString::number(pointer->getId()) : pointer->getName() : QString();
	}

	QString SubjectKey::getAvatarScheme() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getAvatarScheme() : QString();
	}

	QString SubjectKey::getName() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getName() : QString();
	}

	QString SubjectKey::getDisplayName() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? (pointer->getDescription().isEmpty() ? (pointer->getName().isEmpty() ? QString("%1%2").arg(QObject::tr("User")).arg(getId()) : pointer->getName()) : pointer->getDescription()) : QString();
	}

	QString SubjectKey::getDescription() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getDescription() : QString();
	}

	QString SubjectKey::getPicture() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getPicture() : QString();
	}

	QString SubjectKey::getLocale() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getPicture() : QString();
	}

	QString SubjectKey::getTimezone() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getPicture() : QString();
	}

	QDateTime SubjectKey::getCreationDateTime() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getCreationDateTime() : QDateTime();
	}

	QDateTime SubjectKey::getModificationDateTime() const
	{
		const QSharedPointer<const Meta::Subject> pointer(_subject.lock());
		return !pointer.isNull() ? pointer->getModificationDateTime() : QDateTime();
	}

	bool SubjectKey::isValid() const
	{
		return !_subject.isNull() && _subject.lock();
	}
} } } }
