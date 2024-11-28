#include "PurchaseOrder.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace SD { namespace Model {
	Cadabra::Queen::SD::Meta::PurchaseItem* PurchaseOrder::Item(QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem>* list, int index)
	{
		PurchaseOrder* message = qobject_cast<PurchaseOrder*>(list->object);
		return &(message->_item_vector[index]);
	}

	int PurchaseOrder::ItemListSize(QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem>* list)
	{
		PurchaseOrder* order = qobject_cast<PurchaseOrder*>(list->object);
		return order ? order->_item_vector.count() : 0;
	}

    PurchaseOrder::PurchaseOrder(QObject* parent)
    :
		ERP::Model::AbstractDocument(parent),

		_shop_id(0)
    {

    }

	bool PurchaseOrder::scan(const QJsonObject& body)
	{
		if (getState() == ERP::DocumentState::Unknown) {
			return false;
		} else if (getState() == ERP::DocumentState::Issued || getState() == ERP::DocumentState::Held) {
			_shop_id = body["shop_id"].toVariant();
			_total.setCurrencyCode(body["currency_code"].toString("XXX"));
			_counteryparty.parse(body["counterparty"].toObject());

			QJsonArray item_array(body["items"].toArray());
			_total.zero();
			_item_vector.clear();
			foreach (const QJsonValue& item, item_array) {
				_item_vector.push_back(SD::Meta::PurchaseItem{
																item["name"].toString("noname"),
																item["sku"].toString(""),
																SD::Meta::Money{item["price"].toInt(0), _total.getCurrencyCode()},
																item["quantity"].toDouble(0),
																Repository::Meta::Essence::ToId(item["unit_id"].toString("0"))
										}
				);
				_total += qRound64(_item_vector.back().getCentum() * _item_vector.back().getQuantity());
			}
			QJsonArray payment_preference_array(body["payment_preferences"].toArray());
			_payment_preference_array.clear();
			foreach (const QJsonValue& payment_preference, payment_preference_array) {
				_payment_preference_array.push_back(SD::Meta::PaymentPreference{payment_preference["type"].toString("T"), payment_preference["alias"].toString("default")});
			}
			return true;
		}

		return false;
	}

	QJsonObject PurchaseOrder::dump() const
	{
		QJsonArray payment_preference_array;
		for (const Meta::PaymentPreference& payment_preference : _payment_preference_array) {
			payment_preference_array.append(static_cast<QJsonObject>(payment_preference));
		}

		QJsonArray item_array;
		for (const SD::Meta::PurchaseItem& item : _item_vector) {
			item_array.append(static_cast<QJsonObject>(item));
		}

		return {
				{"counterparty", static_cast<QJsonObject>(_counteryparty)},
				{"shop_id", QString::number(_shop_id)},
				{"currency_code", _total.getCurrencyCode()},
				{"payment_preferences", std::move(payment_preference_array)},
				{"items", item_array}
		};
	}

	QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem> PurchaseOrder::makeItemList()
	{
		return QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem>(this, this, &PurchaseOrder::ItemListSize, &PurchaseOrder::Item);
	}

	QString PurchaseOrder::getDescription() const
	{
		if (getState() == ERP::DocumentState::Issued || getState() == ERP::DocumentState::Held) {
			return tr("Purchase order") + ((getNo() > 0 && getNo() != getId()) ? (" №" + QString::number(getNo())) : (" #" + QString::number(getId())));
		}
		return AbstractDocument::getDescription();
	}

	Core::Meta::Qd PurchaseOrder::getShopId() const
	{
		return _shop_id;
	}

	const Meta::Counterparty& PurchaseOrder::getCounterparty() const
	{
		return _counteryparty;
	}

	const SD::Meta::LegalCounterparty& PurchaseOrder::getLegal() const
	{
		return _counteryparty.getLegal();
	}

	const QString& PurchaseOrder::getCurrencyCode() const
	{
		return _total.getCurrencyCode();
	}

	const QString& PurchaseOrder::getCurrencySign() const
	{
		return _total.getCurrencySign();
	}

	const SD::Meta::Money& PurchaseOrder::getTotal() const
	{
		return _total;
	}

	const QVector<SD::Meta::PurchaseItem>& PurchaseOrder::getItemVector() const
	{
		return _item_vector;
	}
} } } }
