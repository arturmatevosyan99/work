#include "Stock.hpp"

#include "../../Core/Meta/Qd.hpp"

#include <QUrl>

namespace Cadabra { namespace Queen { namespace EC { namespace Data {
	/// @todo Перенести в Model, так как нереляционная модель
	///
	Stock::Item Stock::ParseItem(const QJsonObject& object)
	{
		Stock::Item retval;
		retval.price_id = Repository::Meta::Essence::ToId(object.value("price_id"));
		retval.catalogue_id = Repository::Meta::Essence::ToId(object.value("catalogue_id"));
		retval.unit_id = Repository::Meta::Essence::ToId(object.value("unit_id"));
		retval.sku = object.value("sku").toString("");
		retval.name = object.value("name").toString("");
		retval.area = static_cast<NomenclatureArea>(object.value("area").toInt('0'));
		retval.specification = object.value("specification").toString("");
		retval.rich_specification = object.value("rich_specification").toBool(false);
		retval.cost = SD::Meta::Money(object.value("cost").toObject());
		retval.available = object.value("available").toBool(false);
		retval.sorter = object.value("sorter").toInt(0);

		QJsonArray asset(object.value("asset").toArray());
		foreach (const QJsonValue& a, asset) {
			if (a.isObject()) {
				const QJsonObject hook(a.toObject());
				retval.asset.insert(
										static_cast<ActivationPhase>(hook.value("phase").toInt('0')),
										Hook {
												Repository::Meta::Essence::ToId(hook.value("id")),
												hook.value("application_scheme").toString("0"),
												hook.value("name").toString("")
										}
				);
			}
		}

		return retval;
	}

	Stock::SourceMap Stock::ParseArray(const QJsonArray& array)
	{
		SourceMap retval;
		for (const QJsonValue& i : array) {
			if (i.isObject()) {
				Item item(Stock::ParseItem(i.toObject()));
				retval.insert(item.price_id, item);
			}
		}
		return retval;
	}

	Stock::Stock(QObject* parent)
	:
		QAbstractListModel(parent)
	{
		QObject::connect(this, &QAbstractListModel::modelAboutToBeReset, this, &Stock::onModelBeforeReset);
		QObject::connect(this, &QAbstractListModel::modelReset, this, &Stock::onModelAfterReset);
		QObject::connect(this, &QAbstractListModel::dataChanged, this, &Stock::onModelDataChanged);
		QObject::connect(this, &QAbstractListModel::rowsInserted, this, &Stock::onModelRowsInserted);
		QObject::connect(this, &QAbstractListModel::rowsRemoved, this, &Stock::onModelRowsRemoved);
	}

	bool Stock::parse(const QJsonObject& object)
	{
		QJsonObject::ConstIterator a(object.find("items"));
		if (object.constEnd() == a || !a->isArray()) {
			return false;
		}
		beginResetModel();
		_source_map = ParseArray(a->toArray());
		endResetModel();
		return true;
	}

	void Stock::clear()
	{
		beginResetModel();
		_source_map.clear();
		endResetModel();
	}

	QJsonObject Stock::dump() const
	{
		QJsonArray basket_item_array;
		for (const SourceMap::mapped_type& basket_item : _source_map) {
			QJsonArray asset_array;
			for (QMap<ActivationPhase, Hook>::ConstIterator a = basket_item.asset.constBegin(); a != basket_item.asset.constEnd(); ++a) {
				asset_array.push_back(
										QJsonObject
										{
											{"phase", static_cast<char>(a.key())},
											{"id", QString::number(a->id)},
											{"application_scheme", a->application_scheme},
											{"name", a->name}
										}
				);
			}
			basket_item_array.push_back(
										QJsonObject
										{
											{"price_id", QString::number(basket_item.price_id)},
											{"catalogue_id", QString::number(basket_item.catalogue_id)},
											{"unit_id", QString::number(basket_item.unit_id)},
											{"sku", basket_item.sku},
											{"name", basket_item.name},
											{"specification", basket_item.specification},
											{"rich_specification", basket_item.rich_specification},
											{"area", static_cast<char>(basket_item.area)},
											{"cost", static_cast<QJsonObject>(basket_item.cost)},
											{"sorter", basket_item.sorter},
											{"available", basket_item.available},
											{"asset", asset_array},
										}
			);
		}

		QJsonObject retval;
		if (!basket_item_array.isEmpty()) {
			retval.insert("items", basket_item_array);
		}
		return retval;
	}

	bool Stock::insert(const Item& item)
	{
		_source_map.insert(item.price_id, item);
		return true;
	}

	void Stock::add(const QJsonArray& array)
	{
		add(ParseArray(array));
	}

	void Stock::add(const Item& item)
	{
		SourceMap::ConstIterator s(_source_map.lowerBound(item.price_id));
		if (_source_map.constEnd() == s || item.price_id != s->price_id) {
			QModelIndex index(createIndex(std::distance(_source_map.cbegin(), s), 0));
			insert(item);
			emit dataChanged(index, index, {});
		} else {
			std::iterator_traits<SourceMap::ConstIterator>::difference_type index(_source_map.isEmpty() == 0 || _source_map.cbegin() == s ? 0 : (std::distance(_source_map.cbegin(), s) + 1));
			beginInsertRows(QModelIndex(), index, index);
			insert(item);
			endInsertRows();
		}
	}

	void Stock::add(const SourceMap& map)
	{
		if (map.empty()) {
			return ;
		}
		beginResetModel();
		foreach (const Stock::Item& i, map) {
			insert(i);
		}
		endResetModel();
	}

	const Stock::SourceMap& Stock::getSourceMap() const
	{
		return _source_map;
	}

	int Stock::getItemCount() const
	{
		return _source_map.count();
	}

	SD::Meta::Money Stock::getTotal() const
	{
		if (_source_map.isEmpty()) {
			return {};
		}
		SD::Meta::Money retval(getCost(_source_map.constBegin()));
		for (SourceMap::ConstIterator i = ++_source_map.constBegin(); i != _source_map.constEnd(); ++i) {
			retval += getCost(i);
		}
		return retval;
	}

	QList<Core::Meta::Qd> Stock::getPriceList() const
	{
		QList<Core::Meta::Qd> retval;
		if (!_source_map.isEmpty()) {
			for (SourceMap::ConstIterator i(_source_map.constBegin()); i != _source_map.constEnd(); ++i) {
				retval.push_back(Core::Meta::Qd(i->price_id));
			}
		}
		return retval;
	}

	QVariantList Stock::getPriceVariantList() const
	{
		QVariantList retval;
		if (!_source_map.isEmpty()) {
			for (SourceMap::ConstIterator i(_source_map.constBegin()); i != _source_map.constEnd(); ++i) {
				retval.push_back(Core::Meta::Qd(i->price_id));
			}
		}
		return retval;
	}

	SD::Meta::Money Stock::getCost(const int& index) const
	{
		return getCost(_source_map.constBegin() + index);
	}

	SD::Meta::Money Stock::getCost(const SourceMap::ConstIterator& m) const
	{
		return (_source_map.constEnd() == m) ? SD::Meta::Money{} : m->cost;
	}

	QVariant Stock::getAction(const int& index) const
	{
		const SourceMap::ConstIterator m(_source_map.constBegin() + index);
		if (_source_map.constEnd() == m || NomenclatureArea::Merchandise != m->area) {
			return QVariant::fromValue(nullptr);
		}

		const QMap<Stock::ActivationPhase, Hook>::ConstIterator d(m->asset.find(ActivationPhase::Dispatch));
		if (m->asset.constEnd() != d) {
			QVariantMap retval;
			retval.insert("id", d->id);
			retval.insert("application_scheme", d->application_scheme);
			retval.insert("name", d->name);
			return retval;
		}
		return QVariant::fromValue(nullptr);
	}

	int Stock::rowCount(const QModelIndex& parent) const
	{
		if (parent.isValid()) {
			return 0;
		}
		return _source_map.size();
	}

	QVariant Stock::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}
		switch (role) {
			case Role::PriceId: return Core::Meta::Qd((_source_map.constBegin() + index.row()).value().price_id);
			case Role::CatalogueId: return Core::Meta::Qd((_source_map.constBegin() + index.row()).value().catalogue_id);
			case Role::UnitId: return Core::Meta::Qd((_source_map.constBegin() + index.row()).value().unit_id);
			case Role::SKU: return (_source_map.constBegin() + index.row()).value().sku;
			case Role::Name: return (_source_map.constBegin() + index.row()).value().name;
//			case Role::Area: return (_map.constBegin() + index.row()).value().area;
			case Role::Specification: return (_source_map.constBegin() + index.row()).value().specification;
			case Role::RichSpecification: return (_source_map.constBegin() + index.row()).value().rich_specification;
			case Role::Thumbnail: return QString("%1.unit").arg((_source_map.constBegin() + index.row()).value().sku);
			case Role::Price: return (_source_map.constBegin() + index.row()).value().cost.getPriceTag();
			case Role::Cost: return getCost(index.row()).getPriceTag();
			case Role::Sorter: return (_source_map.constBegin() + index.row()).value().sorter;
			case Role::Available: return (_source_map.constBegin() + index.row()).value().available;
			case Role::Asset: return !(_source_map.constBegin() + index.row()).value().asset.isEmpty();
			case Role::Action: return getAction(index.row());

			default: return QVariant();
		}
	}

	QHash<int, QByteArray> Stock::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractListModel::roleNames());
		retval[Role::PriceId] = "price_id";
		retval[Role::CatalogueId] = "catalogue_id";
		retval[Role::UnitId] = "unit_id";
		retval[Role::SKU] = "sku";
		retval[Role::Name] = "name";
		retval[Role::Area] = "area";
		retval[Role::Specification] = "specification";
		retval[Role::RichSpecification] = "rich_specification";
		retval[Role::Thumbnail] = "thumbnail";
		retval[Role::Price] = "price"; /// < ценник/цена за штуку
		retval[Role::Cost] = "cost"; /// < стоимость
		retval[Role::Sorter] = "sorter";
		retval[Role::Available] = "available";
		retval[Role::Asset] = "asset";
		retval[Role::Action] = "action";
		return retval;
	}

	void Stock::onModelBeforeReset()
	{

	}

	void Stock::onModelAfterReset()
	{
		emit itemsChanged();
	}

	void Stock::onModelDataChanged(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& roles)
	{
		emit itemsChanged();
	}

	void Stock::onModelRowsInserted(const QModelIndex& parent, int first, int last)
	{
		emit itemsChanged();
	}

	void Stock::onModelRowsRemoved(const QModelIndex& parent, int first, int last)
	{
		emit itemsChanged();
	}

	bool Stock::isEmpty() const
	{
		return _source_map.isEmpty();
	}
} } } }
