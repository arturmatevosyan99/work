/**
 @file PurchaseItem.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "PurchaseItem.hpp"

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	PurchaseItem::PurchaseItem()
	:
	  _name(""),
	  _sku(""),
	  _quantity(0),
	  _unit_id(0)
	{

	}

	PurchaseItem::PurchaseItem(const QString& name, const QString& sku, const SD::Meta::Money& cost, double quantity, Repository::Meta::Essence::Id unit_id)
	:
		_name(name),
		_sku(sku),
		_cost(cost),
		_quantity(quantity),
		_unit_id(unit_id)
	{

	}

	PurchaseItem::PurchaseItem(const PurchaseItem& origin)
	:
		_name(origin._name),
		_sku(origin._sku),
		_cost(origin._cost),
		_quantity(origin._quantity),
		_unit_id(origin._unit_id)
	{

	}

	PurchaseItem::PurchaseItem(PurchaseItem&& origin)
	:
		_name(std::move(origin._name)),
		_sku(std::move(origin._sku)),
		_cost(std::move(origin._cost)),
		_quantity(std::move(origin._quantity)),
		_unit_id(std::move(origin._unit_id))
	{

	}

	PurchaseItem::~PurchaseItem()
	{

	}

	PurchaseItem& PurchaseItem::operator=(const PurchaseItem& rhs)
	{
		_name = rhs._name;
		_sku = rhs._sku;
		_cost = rhs._cost;
		_quantity = rhs._quantity;
		_unit_id = rhs._unit_id;

		return *this;
	}

	PurchaseItem& PurchaseItem::operator=(PurchaseItem&& rhs)
	{
		_name = std::move(rhs._name);
		_sku = std::move(rhs._sku);
		_cost = std::move(rhs._cost);
		_quantity = std::move(rhs._quantity);
		_unit_id = std::move(rhs._unit_id);

		return *this;
	}

	PurchaseItem::operator QJsonObject() const
	{
		return {
				{"unit_id", QString::number(_unit_id)},
				{"sku", _sku},
				{"name", _name},
				{"price", _cost.getCentum()},
				{"quantity", _quantity}
		};
	}

	const QString& PurchaseItem::getName() const
	{
		return _name;
	}

	const QString& PurchaseItem::getSKU() const
	{
		return _sku;
	}

	const double& PurchaseItem::getQuantity() const
	{
		return _quantity;
	}

	const qint64& PurchaseItem::getCentum() const
	{
		return _cost.getCentum();
	}

	const SD::Meta::Money& PurchaseItem::getCost() const
	{
		return _cost;
	}

	double PurchaseItem::getPrice() const
	{
		return _cost.multiply(_quantity).getCost();
	}

	const Repository::Meta::Essence::Id& PurchaseItem::getUnitId() const
	{
		return _unit_id;
	}

	QString PurchaseItem::getTotal(bool coins, bool currency_sign)
	{
		return _cost.multiply(_quantity).getPrice(coins, currency_sign);
	}

	QDataStream& operator<<(QDataStream& out, const PurchaseItem& purchase_item)
	{
		out << purchase_item._name << purchase_item._cost << purchase_item._quantity;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, PurchaseItem& purchase_item)
	{
		in >> purchase_item._name >> purchase_item._cost >> purchase_item._quantity;
		return in;
	}

	QDebug operator<<(QDebug debug, const PurchaseItem& purchase_item)
	{
		debug << purchase_item._name << purchase_item._quantity << purchase_item._cost;
		return debug;
	}
} } } }
