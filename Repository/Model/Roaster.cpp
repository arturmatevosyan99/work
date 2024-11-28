#include "Roaster.hpp"

#include "../Meta/Tag.hpp"
#include "../Data/Entity/Record/Tag.hpp"

#include "../../Core/Data/Factory.hpp"
#include "../../Core/Data/Predicate/ConditionPredicate.hpp"

#include <QDateTime>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Roaster::Roaster()
	:
		QObject(nullptr),

		_data_provider(nullptr),
		_last_load_id(0)
	{

	}

	Roaster::~Roaster()
	{
	}

	Roaster::TagMap::ConstIterator Roaster::operator[](const Meta::Essence::Id& id)
	{
		return get(id);
	}

	Roaster::TagMap::ConstIterator Roaster::operator[](const Meta::Essence::Id& id) const
	{
		return get(id);
	}

	TagSet Roaster::makeSet(const QSet<Meta::Essence::Id>& id_set)
	{
		TagSet retval;
		for (const Meta::Essence::Id& id : id_set) {
			retval.add(new Meta::Tag(this, get(id)));
		}
		return retval;
	}

	QSet<Meta::Essence::Id> Roaster::filterSet(const QString& pattern) const
	{
		QSet<Meta::Essence::Id> retval;
		for (TagMap::ConstIterator t = _tag_map.constBegin(); t != _tag_map.constEnd(); ++t) {
			if (t.value()) {
				if (pattern.contains('*') && QRegExp(pattern, Qt::CaseSensitivity::CaseInsensitive, QRegExp::PatternSyntax::Wildcard).exactMatch(t.value()->objectName())) {
					retval.insert(t.key());
				} else if (pattern.compare(t.value()->objectName()) == 0) {
					retval.insert(t.key());
				}
			}
		}
		return retval;
	}

	bool Roaster::load(Core::Data::Provider* provider)
	{
		if (_data_provider || !provider) {
			return false;
		}

		const QSharedPointer<Data::Tag> tag_data(new Data::Tag());
		tag_data->addOrder(tag_data->id, Qt::SortOrder::AscendingOrder);

		if (!tag_data->open(provider)) {
			return false;
		}

		_data_provider = provider;

		if (!tag_data->isEmpty()) {
			do {
				_tag_map.insert(tag_data->id.value().toLongLong(), new QObject(this)).value()->setObjectName(tag_data->alias.value().toString());
			} while (tag_data->next());
			_last_load_id = _tag_map.lastKey();
		}

		return true;
	}

	bool Roaster::load(qsizetype& count)
	{
		count = 0;
		if (!_data_provider) {
			return false;
		}

		const QSharedPointer<Data::Tag> tag_data(new Data::Tag());
		tag_data->addOrder(tag_data->id, Qt::SortOrder::AscendingOrder);

		if (!_tag_map.empty()) {
			const Core::Data::Field id_field(Core::Data::Field::Type::Integer, _last_load_id);
			tag_data->addPredicate(QSharedPointer<Core::Data::ConditionPredicate>::create(tag_data->id, id_field, Core::Data::ConditionPredicate::Condition::Greater));
		}
		if (!tag_data->open(_data_provider)) {
			return false;
		} else if (tag_data->isEmpty()) {
			return true;
		}

		do {
			_tag_map.insert(tag_data->id.value().toLongLong(), new QObject(this)).value()->setObjectName(tag_data->alias.value().toString());
			++count;
		} while (tag_data->next());
		_last_load_id = _tag_map.lastKey();

		return true;
	}

	bool Roaster::unload()
	{
		_data_provider = nullptr;
		_tag_map.clear();

		return true;
	}

	Core::Data::Provider* Roaster::getDataProvider() const
	{
		return _data_provider;
	}

	Roaster::TagMap::ConstIterator Roaster::get(const Meta::Essence::Id& id) const
	{
		TagMap::ConstIterator t(_tag_map.find(id));
		return t;
	}

	Roaster::TagMap::ConstIterator Roaster::get(const Meta::Essence::Id& id)
	{
		TagMap::ConstIterator t(_tag_map.find(id));
		return _tag_map.cend() == t ? _tag_map.insert(id, new QObject(this)) : t;
	}

	bool Roaster::emplace(Meta::Essence::Id id, const QString& alias)
	{
		TagMap::Iterator t(_tag_map.find(id));
		if (_tag_map.end() == t || !t.value()->objectName().isEmpty()) {
			return false;
		}

		const QSharedPointer<Data::Tag> data(new Data::Tag());
		data->id.setValue(id);
		data->alias.setValue(alias);
		data->timestamp.setValue(QDateTime::currentSecsSinceEpoch());
		if (!getDataProvider()->makeFactory().insert(data.get(), Core::Data::Factory::InsertionPolicy::Replace)) {
			return false;
		}

		t.value()->setObjectName(alias);
		return true;
	}

	bool Roaster::import(const QJsonArray& rowset)
	{
		Core::Data::Factory data_factory(getDataProvider()->makeFactory());
		const QSharedPointer<Data::Tag> data_enity(new Data::Tag());

		if (!data_factory.insert(data_enity.get(), rowset, Core::Data::Factory::InsertionPolicy::Replace)) {
			return false;
		} else if (data_factory.getAffectedRowsCount() > 0) {
			/// @???
			qsizetype count;
			return load(count) ? (data_factory.getAffectedRowsCount() == count) : false;
		}
		return false;
	}

	bool Roaster::isLoaded() const
	{
		return !_data_provider;
	}

	bool Roaster::isValid(const TagMap::ConstIterator& iteartor) const
	{
		return _tag_map.constEnd() != iteartor;
	}
} } } }
