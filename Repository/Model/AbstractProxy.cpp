#include "AbstractProxy.hpp"

#include "../../Core/Data/Event.hpp"

#include <QEvent>
#include <QCoreApplication>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Cadabra::Queen::Repository::Meta::EssenceToken* AbstractProxy::DynamicToken(QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken>* list, int index)
	{
		return &(reinterpret_cast<QVector<Meta::EssenceToken>*>(list->data)->operator[](index));
	}

	int AbstractProxy::DynamicTokenListSize(QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken>* list)
	{
		return reinterpret_cast<QVector<Meta::EssenceToken>*>(list->data)->count();
	}

	int* AbstractProxy::DynamicOrder(QQmlListProperty<int>* list, int index)
	{
		return &(reinterpret_cast<QVector<int>*>(list->data)->operator[](index));
	}

	int AbstractProxy::DynamicOrderListSize(QQmlListProperty<int>* list)
	{
		return reinterpret_cast<QVector<int>*>(list->data)->count();
	}

	AbstractProxy::AbstractProxy(Realm* parent)
	:
		QAbstractItemModel(),

		_realm(nullptr),
		_is_ready(false),
		_is_live(false),
		_is_reverse(false),
		_refresh_flagset(RefreshFlag::Manual),
		_preferred_index({ProxyIndex::MapIterator, ProxyIndex::DataRecord, ProxyIndex::NetCursor}),
		_active_cache(0),
		_active_model(nullptr)
	{
		QObject::connect(this, &AbstractProxy::permanentTokenChanged, std::bind(&AbstractProxy::refresh, this, RefreshFlag::PermanentToken));
		QObject::connect(this, &AbstractProxy::permanentOrderChanged, std::bind(&AbstractProxy::refresh, this, RefreshFlag::PermanentToken));
		QObject::connect(this, &AbstractProxy::dynamicTokenChanged, std::bind(&AbstractProxy::refresh, this, RefreshFlag::DynamicToken));
		QObject::connect(this, &AbstractProxy::dynamicOrderChanged, std::bind(&AbstractProxy::refresh, this, RefreshFlag::DynamicToken));
	}

	AbstractProxy::~AbstractProxy()
	{

	}

	bool AbstractProxy::load()
	{
		if (!_realm) {
			return false;
		}

		resetActiveModel(filter());
		return (nullptr != _active_model);
	}

	bool AbstractProxy::unload()
	{
		if (!_active_model) {
			return false;
		}

		resetActiveModel();
		return true;
	}

	bool AbstractProxy::reload()
	{
		if (!_realm/* || !_active_model*/) {
			return false;
		}

		resetActiveModel(filter());
		return (nullptr != _active_model);
	}

	bool AbstractProxy::update()
	{
		if (!_active_model || !_is_ready) {
			return false;
		}
		setReady(false);
		QCoreApplication::postEvent(_active_model.get(), new QEvent(QEvent::Type::UpdateRequest), Qt::HighEventPriority);
		return true;
	}

	bool AbstractProxy::refresh(RefreshFlag flag)
	{
		return (_refresh_flagset.testFlag(flag)) ? reload() : false;
	}

	void AbstractProxy::realize(Realm* realm, bool background_mode)
	{
		if (hasSourceModel()) {
			QObject::disconnect(getSourceModel(), &QAbstractItemModel::modelReset, this, &AbstractProxy::onSourceReseted);
			QObject::disconnect(getSourceModel(), &QAbstractItemModel::dataChanged, this, &AbstractProxy::onSourceChanged);
		}

		unload();
		_realm = realm;

		if (hasSourceModel()) {
			_map_iterator.reset(allocateIterator(), &QObject::deleteLater);
			if (!_map_iterator) {
				qDebug("MapIterator does not support this source model");
			} else {
				_map_iterator->blockSignals(true);
			}

			_data_record.reset(allocateRecord(), &QObject::deleteLater);
			if (!_data_record) {
				qDebug("DataRecord does not support this source model");
			} else {
				QObject::connect(this, &AbstractProxy::liveChanged, _data_record.get(), &Data::Object::setLive);
				QObject::connect(_data_record.get(), &Data::Object::validated, this, &AbstractProxy::setReady);
				QObject::connect(_data_record.get(), &Data::AbstractRecord::rowsetOpened, this, &AbstractProxy::onActiveResized);
				QObject::connect(_data_record.get(), &Data::AbstractRecord::rowsetFetched, this, &AbstractProxy::onActiveIndexed);
				QObject::connect(_data_record.get(), &Data::AbstractRecord::rowsetTransformed, this, &AbstractProxy::onActiveTransformed);
				_data_record->setLive(_is_live);
				if (background_mode) {
					_data_record->moveToThread(Skeleton::ThreadInstance("Core::ModelProxy"));
				}
				_data_record->blockSignals(true);
			}

			_net_cursor.reset(allocateCursor(), &QObject::deleteLater);
			if (!_net_cursor) {
				qDebug("NetCursor does not support this source model");
			} else {
//				_net_cursor->setLive(_is_live);
				if (background_mode) {
					_net_cursor->moveToThread(Skeleton::ThreadInstance("Core::ModelProxy"));
				}
//				QObject::connect(_net_cursor.get(), &Net::AbstractCursor::rowsetOpened, this, &AbstractProxy::onCacheResized);
				_net_cursor->blockSignals(true);
			}

			QObject::connect(getSourceModel(), &QAbstractItemModel::modelReset, this, &AbstractProxy::onSourceReseted);
			QObject::connect(getSourceModel(), &QAbstractItemModel::dataChanged, this, &AbstractProxy::onSourceChanged);
		}
	}

	Model::AbstractIterator* AbstractProxy::allocateIterator() const
	{
		return nullptr;
	}

	Data::AbstractRecord* AbstractProxy::allocateRecord() const
	{
		return nullptr;
	}

	Net::AbstractCursor* AbstractProxy::allocateCursor() const
	{
		return nullptr;
	}

	bool AbstractProxy::filterIterator(Model::AbstractIterator* iterator) const
	{
		if (!iterator) {
			return false;
		}
		iterator->clear();
		return (iterator->filter(_permanent_token_vector) && iterator->filter(_dynamic_token_vector) && iterator->order(_permanent_order_vector, _is_reverse) && iterator->order(_dynamic_order_vector, _is_reverse));
	}

	bool AbstractProxy::filterRecord(Data::AbstractRecord* record) const
	{
		if (!record) {
			return false;
		}
		record->clear();
		return (record->filter(_permanent_token_vector) && record->filter(_dynamic_token_vector) && record->order(_permanent_order_vector, _is_reverse) && record->order(_dynamic_order_vector, _is_reverse));
	}

	bool AbstractProxy::filterCursor(Net::AbstractCursor* cursor) const
	{
		if (!cursor) {
			return false;
		}
		cursor->clear();
		return (cursor->filter(_permanent_token_vector) && cursor->filter(_dynamic_token_vector) && cursor->order(_permanent_order_vector, _is_reverse) && cursor->order(_dynamic_order_vector, _is_reverse));
	}

	void AbstractProxy::resetActiveModel(QSharedPointer<QAbstractItemModel> candidate)
	{
		/// @brief Активная модель могла обновить данные, например, получив уведомление о том, что источник обновился
		/// @note Пробрасываем наружу (до ListView, например) dataChanged, rowsInserted, rowsMoved, rowsRemoved
		beginResetModel();

		if (_active_model) {
			_active_model->blockSignals(true);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::dataChanged, this, &AbstractProxy::dataChanged);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::layoutAboutToBeChanged, this, &AbstractProxy::layoutAboutToBeChanged);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::layoutChanged, this, &AbstractProxy::layoutChanged);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::rowsAboutToBeInserted, this, &AbstractProxy::rowsAboutToBeInserted);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::rowsInserted, this, &AbstractProxy::rowsInserted);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::rowsAboutToBeRemoved, this, &AbstractProxy::rowsAboutToBeRemoved);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::rowsRemoved, this, &AbstractProxy::rowsRemoved);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::rowsAboutToBeMoved, this, &AbstractProxy::rowsAboutToBeMoved);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::rowsMoved, this, &AbstractProxy::rowsMoved);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::modelAboutToBeReset, this, &AbstractProxy::modelAboutToBeReset);
			QObject::disconnect(_active_model.get(), &QAbstractItemModel::modelReset, this, &AbstractProxy::modelReset);
		}

		_active_model.swap(candidate);

		if (_active_model) {
			_active_model->blockSignals(false);
			QObject::connect(_active_model.get(), &QAbstractItemModel::dataChanged, this, &AbstractProxy::dataChanged);
			QObject::connect(_active_model.get(), &QAbstractItemModel::layoutAboutToBeChanged, this, &AbstractProxy::layoutAboutToBeChanged);
			QObject::connect(_active_model.get(), &QAbstractItemModel::layoutChanged, this, &AbstractProxy::layoutChanged);
			QObject::connect(_active_model.get(), &QAbstractItemModel::rowsAboutToBeInserted, this, &AbstractProxy::rowsAboutToBeInserted);
			QObject::connect(_active_model.get(), &QAbstractItemModel::rowsInserted, this, &AbstractProxy::rowsInserted);
			QObject::connect(_active_model.get(), &QAbstractItemModel::rowsAboutToBeRemoved, this, &AbstractProxy::rowsAboutToBeRemoved);
			QObject::connect(_active_model.get(), &QAbstractItemModel::rowsRemoved, this, &AbstractProxy::rowsRemoved);
			QObject::connect(_active_model.get(), &QAbstractItemModel::rowsAboutToBeMoved, this, &AbstractProxy::rowsAboutToBeMoved);
			QObject::connect(_active_model.get(), &QAbstractItemModel::rowsMoved, this, &AbstractProxy::rowsMoved);
			QObject::connect(_active_model.get(), &QAbstractItemModel::modelAboutToBeReset, this, &AbstractProxy::modelAboutToBeReset);
			QObject::connect(_active_model.get(), &QAbstractItemModel::modelReset, this, &AbstractProxy::modelReset);
			if (candidate.get() != _active_model.get()) {
				_active_cache.clear();
				QCoreApplication::postEvent(_active_model.get(), new QEvent(QEvent::Type::LayoutRequest), Qt::HighEventPriority);
			}
		}

		endResetModel();
	}

	QSharedPointer<QAbstractItemModel> AbstractProxy::filter()
	{
		if (!hasRealm()) {
			return nullptr;
		}

		_alternative_index = QFlags<ProxyIndex>();
		/// @brief Most appropriate
		if (_preferred_index.testFlag(ProxyIndex::MapIterator) && filterIterator(_map_iterator.get())) {
			_alternative_index.setFlag(ProxyIndex::MapIterator);
		} else if (_preferred_index.testFlag(ProxyIndex::DataRecord) && filterRecord(_data_record.get())) {
			_alternative_index.setFlag(ProxyIndex::DataRecord);
		} else if (_preferred_index.testFlag(ProxyIndex::NetCursor) && filterCursor(_net_cursor.get())) {
			_alternative_index.setFlag(ProxyIndex::NetCursor);
		} else if (_map_iterator && !_preferred_index.testFlag(ProxyIndex::MapIterator) && filterIterator(_map_iterator.get())) { /// < failover
			_alternative_index.setFlag(ProxyIndex::MapIterator);
		} else if (_data_record && !_preferred_index.testFlag(ProxyIndex::DataRecord) && filterRecord(_data_record.get())) { /// < failover
			_alternative_index.setFlag(ProxyIndex::DataRecord);
		} else if (_net_cursor && !_preferred_index.testFlag(ProxyIndex::NetCursor) && filterCursor(_net_cursor.get())) { /// < failover
			_alternative_index.setFlag(ProxyIndex::NetCursor);
		} else if (_preferred_index.testFlag(ProxyIndex::MapIterator) && _map_iterator) { /// < failover2
			_alternative_index.setFlag(ProxyIndex::MapIterator);
		} else if (_preferred_index.testFlag(ProxyIndex::DataRecord) && _data_record) { /// < failover2
			_alternative_index.setFlag(ProxyIndex::DataRecord);
		} else if (_preferred_index.testFlag(ProxyIndex::NetCursor) && _net_cursor) { /// < failover2
			_alternative_index.setFlag(ProxyIndex::NetCursor);
		}
		emit indexAltered();

		setReady(false);

		/// @brief Load data
		if (_map_iterator && _alternative_index.testFlag(ProxyIndex::MapIterator) && _map_iterator->validate()) {
			return _map_iterator;
		} else if (_data_record && _alternative_index.testFlag(ProxyIndex::DataRecord) && getRealm()->getDataProvider()->open(_data_record.get(), {Core::Data::Provider::OpenMode::FetchThrow}, Core::Data::Provider::RowsetType::Shade)) {
			return _data_record;
		} else if (_net_cursor && _alternative_index.testFlag(ProxyIndex::NetCursor) && _net_cursor->page(false)) {
			return _net_cursor;
		}
		return nullptr;
	}

	bool AbstractProxy::tokenize(const QString& raw_token, bool permanent)
	{
		QStringList raw_list(raw_token.split(" ", Qt::SkipEmptyParts));
		QList<Meta::EssenceToken> token_list;
		for (const QString& raw : raw_list) {
			Meta::EssenceToken token;
			if (!token.parse(raw)) {
				continue;
			} else if (!token_list.isEmpty() && token.type == Meta::EssenceToken::Type::Any) {
				token_list.last().value += " " + token.value;
			} else {
				token_list.push_back(token);
			}
		}
		return addToken(token_list, permanent);
	}

	bool AbstractProxy::assort(const QString& role_name, Qt::SortOrder sort_order, bool permanent)
	{
		if (!hasSourceModel()) {
			return false;
		}
		QList<int> role_list(getSourceModel()->roleNames().keys(role_name.toUtf8()));
		if (role_list.isEmpty()) {
			return false;
		}
		return addOrder(role_list.first(), sort_order, permanent);
	}

	QStringList AbstractProxy::makeTokenList() const
	{
		QStringList retval;
		for (const QVector<Meta::EssenceToken>& token_vector : {_permanent_token_vector, _dynamic_token_vector}) {
			for (const Meta::EssenceToken& token : token_vector) {
				if (token.active && !token.isInvalid()) {
					retval.push_back(token.dump());
				}
			}
		}
		return retval;
	}

	Realm* AbstractProxy::getRealm() const
	{
		return _realm;
	}

	QAbstractItemModel* AbstractProxy::getActiveModel() const
	{
		return _active_model.get();
	}

	ProxyIndexFlags AbstractProxy::getAlternativeIndexFlagSet() const
	{
		return _alternative_index;
	}

	ProxyIndexFlags AbstractProxy::getPreferredIndexFlagSet() const
	{
		return _preferred_index;
	}

	AbstractProxy::RefreshFlags AbstractProxy::getRefreshFlagSet() const
	{
		return _refresh_flagset;
	}

	const AbstractProxy::TokenVector& AbstractProxy::getTokenVector(bool permanent) const
	{
		return permanent ? _permanent_token_vector : _dynamic_token_vector;
	}

	const AbstractProxy::TokenVector& AbstractProxy::getPermanentTokenVector() const
	{
		return _permanent_token_vector;
	}

	const AbstractProxy::TokenVector& AbstractProxy::getDynamicTokenVector() const
	{
		return _dynamic_token_vector;
	}

	const AbstractProxy::OrderVector& AbstractProxy::getOrderVector(bool permanent) const
	{
		return permanent ? _permanent_order_vector : _dynamic_order_vector;
	}

	const AbstractProxy::OrderVector& AbstractProxy::getPermanentOrderVector() const
	{
		return _permanent_order_vector;
	}

	const AbstractProxy::OrderVector& AbstractProxy::getDynamicOrderVector() const
	{
		return _dynamic_order_vector;
	}

	QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken> AbstractProxy::getDynamicTokenList()
	{
		return QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken>(&_property_list_parent, &_dynamic_token_vector, &AbstractProxy::DynamicTokenListSize, &AbstractProxy::DynamicToken);
	}

	QQmlListProperty<int> AbstractProxy::getDynamicOrderList()
	{
		return QQmlListProperty<int>(&_property_list_parent, &_dynamic_order_vector, &AbstractProxy::DynamicOrderListSize, &AbstractProxy::DynamicOrder);
	}

	QModelIndex AbstractProxy::index(int row, int column, const QModelIndex& parent) const
	{
		/// @brief Вернуть индекс по которому обратятся к конкретной модели напрямую
//		return hasActiveModel() ? _active_model->index(row, column, parent) : hasSourceModel() ? getSourceModel()->index(row, column, parent) : createIndex(row, column);
		return createIndex(row, column);
	}

	QModelIndex AbstractProxy::parent(const QModelIndex& child) const
	{
		return QModelIndex();
	}

	int AbstractProxy::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() || !hasActiveModel() ? 0 : _active_cache.maxCost();
	}

	int AbstractProxy::columnCount(const QModelIndex& parent) const
	{
		/// @??? q: Слои в многослойных Proxy реализовывать через колонки? a: Не получится из-за потенциально различного rowCount у моделей
		return hasSourceModel() ? getSourceModel()->columnCount(parent) : 0;
	}

	QVariant AbstractProxy::data(const QModelIndex& index, int role) const
	{
		if (!hasSourceModel()) {
			return QVariant();
		} else if (hasActiveModel()) {
			QVariantMap* key(_active_cache[index]);
			if (nullptr != key) {
				return getSourceModel()->data(*key, role);
			}
			/// @todo Не фетчить одно и то же
			/// @todo Если живет в том же потоке - можно взять сразу же
			_active_model->fetchMore(index);
			return QVariant();
		} else {
			return getSourceModel()->data(getSourceModel()->index(index.row(), index.column()), role);
		}
	}

	Qt::ItemFlags AbstractProxy::flags(const QModelIndex& index) const
	{
		return !_active_cache[index] ? Qt::NoItemFlags : Qt::ItemIsEnabled;
	}

	QHash<int, QByteArray> AbstractProxy::roleNames() const
	{
		return hasSourceModel() ? getSourceModel()->roleNames() : QHash<int, QByteArray>{};
	}

	void AbstractProxy::setReady(bool onoff)
	{
		if (_is_ready != onoff) {
			_is_ready = onoff;
			emit readyChanged(onoff);
		}
	}

	void AbstractProxy::setLive(bool onoff)
	{
		if (_is_live != onoff) {
			_is_live = onoff;
			emit liveChanged(onoff);
		}
	}

	void AbstractProxy::setReverse(bool onoff)
	{
		if (onoff != _is_reverse) {
			_is_reverse = onoff;
			emit reverseChanged(onoff);
			if (!_permanent_order_vector.isEmpty() || !_dynamic_order_vector.isEmpty()) {
				reload();
			}
		}
	}

	void AbstractProxy::setPreferredIndex(ProxyIndex preference, bool onoff)
	{
		if (_preferred_index.testFlag(preference) != onoff) {
			_preferred_index.setFlag(preference, onoff);
			emit indexPreferred();
		}
	}

	void AbstractProxy::setPreferredIndex(ProxyIndex preference)
	{
		return setPreferredIndexFlagSet(QFlags<ProxyIndex>{preference});
	}

	void AbstractProxy::setPreferredIndexFlagSet(ProxyIndexFlags preference)
	{
		if (_preferred_index != preference) {
			_preferred_index = preference;
			emit indexPreferred();
		}
	}

	void AbstractProxy::setRefreshMode(RefreshFlag flag, bool onoff)
	{
		_refresh_flagset.setFlag(flag, onoff);
	}

	bool AbstractProxy::addToken(const QList<Meta::EssenceToken> token_list, bool permanent)
	{
		TokenVector& token_vector(permanent ? _permanent_token_vector : _dynamic_token_vector);
		const int index(token_vector.count());

		for (const Meta::EssenceToken& token : qAsConst(token_list)) {
			if (token.type != Meta::EssenceToken::Type::Invalid) {
				token_vector.push_back(token);
			}
		}

		const int count(token_vector.count() - index);
		if (0 == count) {
			return false;
		} else if (permanent) {
			emit permanentTokenChanged(index, count, true);
		} else {
			emit dynamicTokenChanged(index, count, true);
		}
		return true;
	}

	bool AbstractProxy::addToken(Meta::EssenceToken::Type token_type, const QString& token_value, bool permanent)
	{
		if (token_type == Meta::EssenceToken::Type::Invalid || token_value.isEmpty()) {
			return false;
		}
		if (permanent) {
			_permanent_token_vector.push_back(Meta::EssenceToken{});
			_permanent_token_vector.last().value = token_value;
			_permanent_token_vector.last().type = token_type;
			emit permanentTokenChanged(_permanent_token_vector.length(), 1, true);
		} else {
			_dynamic_token_vector.push_back(Meta::EssenceToken{});
			_dynamic_token_vector.last().value = token_value;
			_dynamic_token_vector.last().type = token_type;
			emit dynamicTokenChanged(_permanent_token_vector.length(), 1, true);
		}
		return true;
	}

	bool AbstractProxy::replaceToken(int index, Meta::EssenceToken::Type token_type, const QString& token_value, bool permanent)
	{
		if (token_value.isEmpty()) {
			return false;
		}
		if (permanent && _permanent_token_vector.length() > index) {
			_permanent_token_vector[index].type = token_type;
			_permanent_token_vector[index].value = token_value;
			emit permanentTokenChanged(index, 1, true);
		} else if (!permanent && _dynamic_token_vector.length() > index) {
			_dynamic_token_vector[index].type = token_type;
			_dynamic_token_vector[index].value = token_value;
			emit dynamicTokenChanged(index, 1, true);
		} else {
			return false;
		}
		return true;
	}

	bool AbstractProxy::removeToken(int index, bool permanent)
	{
		if (permanent) {
			if (_permanent_token_vector.length() <= index) {
				return false;
			}
			_permanent_token_vector.remove(index);
			emit permanentTokenChanged(index, 1, false);
		} else {
			if (_dynamic_token_vector.length() <= index) {
				return false;
			}
			_dynamic_token_vector.remove(index);
			emit dynamicTokenChanged(index, 1, false);
		}
		return true;
	}

	void AbstractProxy::clearTokenVector(bool permanent)
	{
		if (permanent) {
			if (!_permanent_token_vector.isEmpty()) {
				const int count(_permanent_token_vector.count());
				_permanent_token_vector.clear();
				emit permanentTokenChanged(0, count, false);
			}
		} else {
			if (!_dynamic_token_vector.isEmpty()) {
				const int count(_dynamic_token_vector.count());
				_dynamic_token_vector.clear();
				emit dynamicTokenChanged(0, count, false);
			}
		}
	}

	bool AbstractProxy::addOrder(int role, Qt::SortOrder order, bool permanent)
	{
		if (!hasSourceModel() || !getSourceModel()->roleNames().contains(role)) {
			return false;
		}
		if (permanent) {
			_permanent_order_vector.push_back(Qt::SortOrder::AscendingOrder == order ? -role : role);
			emit permanentOrderChanged(_permanent_order_vector.length(), true);
		} else {
			_dynamic_order_vector.push_back(Qt::SortOrder::AscendingOrder == order ? -role : role);
			emit dynamicOrderChanged(_dynamic_order_vector.length() - 1, true);
		}		
		return true;
	}

	bool AbstractProxy::resetOrder(int index, int role, Qt::SortOrder order, bool permanent)
	{
		if (!hasSourceModel() || !getSourceModel()->roleNames().contains(role)) {
			return false;
		}
		if (permanent && index < _permanent_order_vector.count()) {
			_permanent_order_vector.push_back(Qt::SortOrder::AscendingOrder == order ? -role : role);
			emit permanentOrderChanged(index, true);
		} else if (!permanent && index < _dynamic_order_vector.count()) {
			_dynamic_order_vector.push_back(Qt::SortOrder::AscendingOrder == order ? -role : role);
			emit dynamicOrderChanged(index, true);
		}
		return true;
	}

	bool AbstractProxy::removeOrder(int index, bool permanent)
	{
		if (permanent) {
			if (_permanent_order_vector.length() <= index) {
				return false;
			}
			_permanent_order_vector.remove(index);
			emit permanentOrderChanged(index, false);
		} else {
			if (_dynamic_order_vector.length() <= index) {
				return false;
			}
			_dynamic_order_vector.remove(index);
			emit dynamicOrderChanged(index, false);
		}
		return true;
	}

	void AbstractProxy::clearOrderVector(bool permanent)
	{
		if (permanent) {
			if (!_permanent_order_vector.isEmpty()) {
				_permanent_order_vector.clear();
				emit permanentOrderChanged(0, false);
			}
		} else {
			if (!_dynamic_order_vector.isEmpty()) {
				_dynamic_order_vector.clear();
				emit dynamicOrderChanged(0, false);
			}
		}
	}

	void AbstractProxy::onSourceReseted()
	{
		reload();
	}

	void AbstractProxy::onSourceChanged(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& role_vector)
	{
		/// @todo Нужно отслеживать изменения в источнике данных и оповещать активную модель если она сама, как AbstractIterator, об этом не знает
		/// @todo rowsRemoved, rowsInserted учитывать и передавать активной модели, если она сама, как AbstractIterator об этом не знает
		/// @note Порядок всегда один и тот же, т.е. в источнике rowsMoved события невозможны или, как минимум, не интересуют прокси
	}

	void AbstractProxy::onActiveResized(int row_count)
	{
		_active_cache.setMaxCost(row_count);
		setReady(true);
	}

	void AbstractProxy::onActiveIndexed(QSharedPointer<Core::Data::Rowset> row_set, int begin_offset, int end_offset)
	{
		getSourceModel()->fetch(row_set);
		const int length(end_offset - begin_offset + 1);
		if (_active_cache.maxCost() < length) {
			_active_cache.setMaxCost(length);
		}
		for (int o = 0; o < row_set->getLength(); ++o) {
			_active_cache.insert(createIndex(begin_offset + o, 0), new QVariantMap(row_set->seekKey(o)));
		}
		emit dataChanged(createIndex(begin_offset, 0), createIndex(end_offset, 0), {});
	}

	void AbstractProxy::onActiveTransformed(QSet<int> inserted_index_set, QSet<int> removed_index_set, QSet<int> updated_index_set, QMap<int, int> moved_index_map)
	{
		const int cost(_active_cache.maxCost() + inserted_index_set.count() - removed_index_set.count());
		_active_cache.setMaxCost(cost <= 0 ? 0 : cost);
		_active_cache.clear();
	}

	bool AbstractProxy::hasRealm() const
	{
		return _realm;
	}

	bool AbstractProxy::hasSourceModel() const
	{
		return nullptr != getSourceModel();
	}

	bool AbstractProxy::hasActiveModel() const
	{
		return nullptr != _active_model;
	}

	bool AbstractProxy::isReady() const
	{
		return !_active_model ? false : _is_ready;
	}

	bool AbstractProxy::isLive() const
	{
		return _is_live;
	}

	bool AbstractProxy::isReverse() const
	{
		return _is_reverse;
	}
} } } }
