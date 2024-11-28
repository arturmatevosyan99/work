#include "Showcase.hpp"

#include "../../Core/Meta/Qd.hpp"

#include <QQmlEngine>
#include <QUrl>
#include <QLocale>

namespace Cadabra { namespace Queen { namespace EC { namespace Data {
	/// @todo Перенести в Model, так как нереляционная модель

	Showcase::Showcase(QObject *parent)
	:
		QAbstractListModel(parent),

		_catalogue(Showcase::Catalogue::Category),
		_is_active(true)
	{
	}

	Showcase::Showcase(Showcase::Catalogue catalogue, QObject* parent)
	:
		QAbstractListModel(parent),

		_catalogue(catalogue),
		_is_active(true)
	{

	}

	void Showcase::add(const QJsonObject& object)
	{
		beginResetModel();
		foreach (const QJsonValue& i, object) {
			if (i.isObject()) {
				Item item;
				QJsonObject item_object(i.toObject());				
				item.code = item_object.value("code").toString("");
				item.caption = item_object.value("caption").toString("");
				item.description = item_object.value("description").toString("");
				item.stock.reset(new Stock());
				if (item_object.value("stock").isArray()) {
					item.stock->add(item_object.value("stock").toArray());
				}
				_map.insert(item.code, item);
			}
		}
		endResetModel();
	}

	void Showcase::add(const Item& item)
	{
		beginResetModel();
		if (item.stock) {
			item.stock->setParent(this);
		}
		_map.insert(item.code, item);
		endResetModel();
	}

	void Showcase::add(const Map& map)
	{
		if (map.empty()) {
			return ;
		}
		beginResetModel();
		foreach (const Item& i, map) {
			if (i.stock) {
				i.stock->setParent(this);
			}
			_map.insert(i.code, i);
		}
		endResetModel();
	}

	void Showcase::clear()
	{
		if (!_map.isEmpty()) {
			beginResetModel();
			_map.clear();
			endResetModel();
		}
	}

	void Showcase::setActive(bool onoff)
	{
		_is_active = onoff;
	}

	void Showcase::setCatalogue(const Catalogue& catalogue)
	{
		_catalogue = catalogue;
	}

	Cadabra::Queen::EC::Data::Stock* Showcase::stock(int index)
	{
		if (index >= _map.size()) {
			return nullptr;
		}

		QSharedPointer<Stock> retval((_map.constBegin() + index).value().stock);
		QQmlEngine::setObjectOwnership(retval.data(), QQmlEngine::CppOwnership);
		return retval.data();
	}

	const Showcase::Catalogue& Showcase::getCatalogue() const
	{
		return _catalogue;
	}

	const Showcase::Map& Showcase::getItemMap() const
	{
		return _map;
	}

	const QStringList& Showcase::getFilterStringList() const
	{
		return _filter;
	}

	QStringList& Showcase::getFilterStringList()
	{
		return _filter;
	}

	int Showcase::rowCount(const QModelIndex& parent) const
	{
		if (parent.isValid()) {
			return 0;
		}
		return _map.size();
	}

	QVariant Showcase::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {			
			case Role::Code: return (_map.constBegin() + index.row()).value().code;
			case Role::Caption: return (_map.constBegin() + index.row()).value().caption;
			case Role::Description: return (_map.constBegin() + index.row()).value().description;
			case Role::Banner: return QString("%1.%2").arg((_map.constBegin() + index.row()).value().code).arg(_catalogue == Catalogue::Bundle ? "bundle" : "category");
			case Role::Total: return (_map.constBegin() + index.row()).value().stock->getTotal().getPriceTag();

			default: return QVariant();
		}
	}

	QHash<int, QByteArray> Showcase::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractListModel::roleNames());		
		retval[Role::Code] = "code";
		retval[Role::Caption] = "caption";
		retval[Role::Description] = "description";
		retval[Role::Banner] = "banner";
		retval[Role::Total] = "total";

		return retval;
	}

	bool Showcase::isActive() const
	{
		return _is_active;
	}
} } } }
