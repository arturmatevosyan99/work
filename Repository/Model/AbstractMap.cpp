#include "AbstractMap.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	const QSet<Meta::Essence::Id> AbstractMap::DummyTagSet = {};

	AbstractMap::AbstractMap(QObject* parent)
	:
		QAbstractItemModel(parent),

		_lazy_mode(false),
		_roaster(nullptr)
	{

	}

	AbstractMap::~AbstractMap()
	{

	}

	bool AbstractMap::load(Roaster* roaster, bool lazy_mode)
	{
		_roaster = roaster;
		if (nullptr != _roaster) {
			_lazy_mode = lazy_mode;
			return true;
		}
		return false;
	}

	bool AbstractMap::unload()
	{
		_roaster = nullptr;
		return true;
	}

	qsizetype AbstractMap::bulk(const QJsonArray& array)
	{
		if (!_roaster) {
			return 0;
		}
		/**
		 * @todo Возможно, правильней:
		 * 1) добавлять во временную таблицу
		 * 2) из которой добавлять в основную
		 * 3) а потом временный rowset отдавать в
		 */

		Core::Data::Factory data_factory(_roaster->getDataProvider()->makeFactory());
		if (!data_factory.insert(makeDataRecord().data(), array)) {
			qWarning() << "Bulk insert error";
			return 0;
		}
		return data_factory.getAffectedRowsCount();
	}

	bool AbstractMap::remove(Meta::Essence::Id id)
	{
		_option_map.remove(id);
		_tag_map.remove(id);
		emit altered(id, EssenceAlter::DeleteAlter);

		return true;
	}

	bool AbstractMap::opt(Meta::Essence::Id id, const QString& key, const QVariant& value)
	{
		OptionMap::Iterator o(_option_map.find(id));
		if (_option_map.end() == o) {
			o = _option_map.insert(id, QJsonObject());
		}
		o->insert(key, QJsonValue::fromVariant(value));

		QModelIndex index(createIndex(std::distance(_option_map.begin(), o), 0));
		emit dataChanged(index, index, {Meta::Essence::Role::OptMap});
		emit altered(id, EssenceAlter::UpdateAlter);

		return true;
	}

	bool AbstractMap::tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff)
	{
		TagMap::Iterator t(_tag_map.find(id));
		if (_tag_map.end() == t) {
			t = _tag_map.insert(id, {});
		}

		if (onoff) {
			t->insert(tag_id);
		} else {
			t->remove(tag_id);
		}

		QModelIndex index(createIndex(std::distance(_tag_map.begin(), t), 0));
		emit dataChanged(index, index, {Meta::Essence::Role::TagSet});
		emit altered(id, EssenceAlter::UpdateAlter);

		return true;
	}

	bool AbstractMap::tag(Meta::Essence::Id id)
	{
		_tag_map.insert(id, {});
		return true;
	}

	bool AbstractMap::observe(EssenceIncidentFlags flag_set)
	{
		if (!flag_set) {
			QScopedPointer<MapObserver> observer(_map_observer.fetchAndStoreAcquire(nullptr));
			return nullptr != observer;
		}

		QScopedPointer<MapObserver> observer(new MapObserver(this));
		if (!_map_observer.testAndSetRelaxed(nullptr, observer.get())) {
			return false;
		}
		observer.take();
		return true;
	}

	bool AbstractMap::fetch(const QList<Meta::Essence::Id>& id_list)
	{
		return false;
	}

	bool AbstractMap::opt(Meta::Essence::Id id)
	{
		_option_map.insert(id, QJsonObject());
		return true;
	}

	bool AbstractMap::opt(Meta::Essence::Id id, const QJsonObject& object)
	{
		_option_map.insert(id, object);
		return true;
	}

	bool AbstractMap::opt(Meta::Essence::Id id, const QString& key, const QVariant& value, QVariant& backup)
	{
		OptionMap::Iterator o(_option_map.find(id));
		if (_option_map.end() == o) {
			return false;
		}
		backup = o->contains(key) ? o->value(key) : QVariant();
		o->insert(key, QJsonValue::fromVariant(value));

		QModelIndex index(createIndex(std::distance(_option_map.begin(), o), 0));
		emit dataChanged(index, index, {Meta::Essence::Role::OptMap});

		return true;
	}

	bool AbstractMap::fetch(QSharedPointer<Core::Data::Rowset> row_set)
	{
		return false;
	}

	Core::Data::Provider* AbstractMap::getDataProvider() const
	{
		return !_roaster ? nullptr : _roaster->getDataProvider();
	}

	Roaster* AbstractMap::getRoaster() const
	{
		return _roaster;
	}

	QModelIndex AbstractMap::index(int row, int column, const QModelIndex& parent) const
	{
		return createIndex(row, column);
	}

	QModelIndex AbstractMap::parent(const QModelIndex& child) const
	{
		return QModelIndex();
	}

	int AbstractMap::columnCount(const QModelIndex& parent) const
	{
		return 1;
	}

	QJsonObject AbstractMap::optionObject(const Meta::Essence::Id& id) const
	{
		OptionMap::ConstIterator o(_option_map.find(id));
		return _option_map.constEnd() == o ? QJsonObject() : o.value();
	}

	QVariant AbstractMap::option(Meta::Essence::Id id, const QString& key) const
	{
		OptionMap::ConstIterator o(_option_map.find(id));
		return (_option_map.end() == o) ? QVariant() : o.value().value(key).toVariant();
	}

	const AbstractMap::TagMap::mapped_type& AbstractMap::tagSet(const Meta::Essence::Id& id) const
	{
		TagMap::ConstIterator t(_tag_map.find(id));
		return _tag_map.constEnd() == t ? AbstractMap::DummyTagSet : t.value();
	}

	const AbstractMap::TagMap::mapped_type& AbstractMap::tagSet(const Meta::Essence::Id& id)
	{
		return _tag_map[id];
	}

	const AbstractMap::TagMap::mapped_type& AbstractMap::tagSet(const QModelIndex& index) const
	{
		return !index.isValid() ? AbstractMap::DummyTagSet : tagSet((_option_map.constBegin() + index.row()).key());
	}

	const AbstractMap::TagMap::mapped_type& AbstractMap::tagSet(const QModelIndex& index)
	{
		return !index.isValid() ? AbstractMap::DummyTagSet : tagSet((_option_map.constBegin() + index.row()).key());
	}

	QVariant AbstractMap::key(const QModelIndex& index) const
	{
		return !index.isValid() ? QVariant() : QVariant((_option_map.constBegin() + index.row()).key());
	}

	Meta::Essence::Id AbstractMap::mapKey(const int& row) const
	{
		return _option_map.count() <= row ? 0 : (_option_map.constBegin() + row).key();
	}

	QVariant AbstractMap::data(const QVariantMap& key, int role) const
	{
		return data(key.value("id", -1).toLongLong(), role);
	}

	QVariant AbstractMap::data(const QVariant& key, int role) const
	{
		return (static_cast<QMetaType::Type>(key.type()) == QMetaType::QVariantMap)
				? data(key.toMap(), role)
				: (static_cast<QMetaType::Type>(key.type()) == QMetaType::LongLong)
					? data(key.toLongLong(), role)
					: QVariant();
	}

	QVariant AbstractMap::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		} else if (Meta::Essence::Role::OptMap == role) {
			return QVariant((_option_map.constBegin() + index.row()).value());
		} else if (Meta::Essence::Role::TagSet == role) {
			TagMap::ConstIterator t(_tag_map.find(mapKey(index.row())));
			return  QVariant::fromValue(_roaster->makeSet(_tag_map.constEnd() != t ? t.value() : AbstractMap::DummyTagSet));
		} else if (Qt::UserRole == role) {
			return key(index);
		}
		return QVariant();
	}

	QVariant AbstractMap::data(const Meta::Essence::Id& id, int role) const
	{
		if (Meta::Essence::Role::OptMap == role) {
			OptionMap::ConstIterator o(_option_map.find(id));
			return (_option_map.constEnd() != o) ? QVariant(o.value()) : QVariant();
		} else if (Meta::Essence::Role::TagSet == role) {
			TagMap::ConstIterator t(_tag_map.find(id));
			return  QVariant::fromValue(_roaster->makeSet(_tag_map.constEnd() != t ? t.value() : AbstractMap::DummyTagSet));
		} else if (Qt::UserRole == role) {
			return _option_map.contains(id) ? QVariant(id) : QVariant();
		}
		return QVariant();
	}

	QHash<int, QByteArray> AbstractMap::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractItemModel::roleNames());
		retval[Meta::Essence::Role::OptMap] = "opt_map";
		retval[Meta::Essence::Role::TagSet] = "tag_set";
		retval[Meta::Essence::Role::EssenceId] = "id";

		return retval;
	}

	bool AbstractMap::isLoaded() const
	{
		return _roaster;
	}

	bool AbstractMap::isLazy() const
	{
		return _lazy_mode;
	}

	MapObserver::MapObserver(AbstractMap* map)
	:
		QObject(map)
	{
		QObject::connect(map, &AbstractMap::altered, this, &MapObserver::onAltered, Qt::ConnectionType::DirectConnection);
	}

	MapObserver::~MapObserver()
	{
		flush();
	}

	void MapObserver::flush()
	{
		QMap<Meta::Essence::Id, EssenceAlterFlags> map;
		_alter_map.swap(map);
		if (!map.isEmpty()) {
			if (AbstractMap* target = qobject_cast<AbstractMap*>(parent())) {
				emit target->patched(map);
			}
		}
	}

	const QMap<Meta::Essence::Id, EssenceAlterFlags>& MapObserver::getMap() const
	{
		return _alter_map;
	}

	void MapObserver::onAltered(Meta::Essence::Id id, EssenceAlter alter)
	{
		_alter_map[id].setFlag(alter);
	}
} } } }
