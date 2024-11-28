#include "OfferMessage.hpp"

#include "../Application/OnlineShop.hpp"
#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"
#include "../../../Repository/Model/Descriptor/StatelessDescriptor.hpp"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	OfferMessage::OfferMessage(QObject* parent)
	:
		Conversation::Model::AbstractMessage(parent),

		_state(State::Price),
		_online_shop(new OnlineShop()),
		_unit_showcase(new Data::Showcase(Data::Showcase::Catalogue::Nomenclature)),
		_bundle_showcase(new Data::Showcase(Data::Showcase::Catalogue::Bundle)),

		_shop_object_id(0),
		_payer_subject_id(0)
	{
		QObject::connect(_online_shop.get(), &OnlineShop::showcaseLoaded, this, &OfferMessage::onPriceLoaded);
		QObject::connect(_online_shop.get(), &OnlineShop::purchaseOrderIssued, this, &OfferMessage::onPurchaseOrderIssued);
		QObject::connect(_online_shop.get(), &OnlineShop::purchaseOrderResolved, this, &OfferMessage::onPurchaseOrderResolved);
		QObject::connect(_online_shop.get(), &OnlineShop::paymentAcquired, this, &OfferMessage::onPaymentAcquired);
		QObject::connect(_online_shop.get(), &OnlineShop::paymentFaulted, this, &OfferMessage::onPaymentFaulted);
		QObject::connect(_online_shop->getPurchaseBasket(), &Data::Basket::itemsRepriced, this, &OfferMessage::onBasketRepriced);
		QObject::connect(_online_shop->getPurchaseBasket(), &Data::Basket::faulted, this, &OfferMessage::onBasketFaulted);

		_online_shop->appendShowcase(_unit_showcase.get());
		_online_shop->appendShowcase(_bundle_showcase.get());
	}

	bool OfferMessage::loadPrice()
	{
		if (!isImported() || !_online_shop->hasDescriptor() || getChannelDescriptor()->getSubjectId() != _payer_subject_id) {
			return false;
		} else if (!_online_shop->loadShowcase()) {
			setState(State::Invalid);
		}
		return true;
	}

	bool OfferMessage::issueOrder()
	{
		/// @obsolete Должно быть удалено в пользу стандартного способа определения опций доставки через DeliveryCapture навык на backend
		if ((!isImported() || _online_shop->getPurchaseBasket()->isEmpty()) ? false : _online_shop->issuePurchaseOrder(_note, "Personal offer message", {QUrl(QString("capc://%1?subject_id=%2").arg(getChannelDescriptor()->getObjectId()).arg(getChannelDescriptor()->getSubjectId()))})) {
			setState(State::Busy);
			return true;
		}
		return false;
	}

	bool OfferMessage::resolveOrder()
	{
		if ((!isImported() || _online_shop->getPurchaseOrder()->getItemVector().isEmpty()) ? false : _online_shop->resolvePurchaseOrder()) {
			setState(State::Busy);
			return true;
		}
		return false;
	}

	QJsonObject OfferMessage::exportBody() const
	{
		return QJsonObject {
							{"purchase_basket", _online_shop->getPurchaseBasket()->dump()},
							{"purchase_order", _online_shop->getPurchaseOrder()->compose()},
		};
	}

	bool OfferMessage::importBody(const QJsonObject& body)
	{
		State state(State::Price);

		_shop_object_id = Repository::Meta::Essence::ToId(body["shop_object_id"]);
		_payer_subject_id = Repository::Meta::Essence::ToId(body["payer_subject_id"]);
		if (_shop_object_id > 0 && !_online_shop->hasDescriptor()) {
			 QScopedPointer<Repository::Model::StatelessDescriptor> descriptor(
																				new Repository::Model::StatelessDescriptor(
																															getChannelDescriptor()->getRealm()->getSessionDescriptor(),
																															QSharedPointer<Repository::Meta::Object>::create(
																																												_shop_object_id,
																																												0,
																																												"",
																																												"",
																																												false,
																																												false,
																																												"",
																																												493,
																																												QDateTime::currentDateTimeUtc(),
																																												QDateTime::currentDateTimeUtc(),
																																												QDateTime::currentDateTimeUtc()
																															)
																				)
			);
			if (!_online_shop->open(descriptor.data())) {
				return false;
			}
			descriptor->setParent(_online_shop.get());
			descriptor.take();
		}

		QJsonObject::ConstIterator t(body.find("purchase_term"));
		if (body.constEnd() != t && t->isObject()) {
			const QJsonObject term_body(t->toObject());

			_note = term_body["note"].toString("");

			_online_shop->getPurchaseBasket()->resetPaymentPreference();
			const QJsonArray payment_method_array(term_body["paymethods"].toArray());
			foreach (const QJsonValue& payment_method, payment_method_array) {
				_online_shop->getPurchaseBasket()->addPaymentPreference(
																		SD::Meta::PaymentPreference{
																									static_cast<char>(payment_method["type"].toInt('T')),
																									payment_method["alias"].toString("faulty")
																		}
				);
			}

			_unit_map.clear();
			const QJsonArray basket_unit_array(term_body["units"].toArray());
			foreach (const QJsonValue& basket_unit, basket_unit_array) {
				if (basket_unit.isObject()) {
					const QJsonObject unit_item(basket_unit.toObject());
					if (unit_item.contains("sku") && unit_item.contains("quantity")) {
						_unit_showcase->getFilterStringList().append(unit_item.value("sku").toString(""));
						_unit_map.insert(
											_unit_showcase->getFilterStringList().last(),
											unit_item.value("quantity").toDouble()
						);
					}
				}
			}
			_unit_showcase->setActive(!_unit_showcase->getFilterStringList().isEmpty());

			_bundle_set.clear();
			const QJsonArray bundle_array(term_body["bundles"].toArray());
			foreach (const QJsonValue& bundle, bundle_array) {
				_bundle_showcase->getFilterStringList().append(bundle.toString(""));
				_bundle_set.insert(_bundle_showcase->getFilterStringList().last());
			}
			_bundle_showcase->setActive(!_bundle_showcase->getFilterStringList().isEmpty());
		}

		QJsonObject::ConstIterator b(body.find("purchase_basket"));
		if (body.constEnd() != b && b->isObject()) {
			if (!_online_shop->getPurchaseBasket()->parse(b->toObject())) {
				_online_shop->getPurchaseBasket()->clear();
				return false;
			} else {
				state = State::Basket;
			}
		}

		QJsonObject::ConstIterator o(body.find("purchase_order"));
		if (body.constEnd() != o && o->isObject()) {
			QJsonObject order_body(o->toObject());
			if (!order_body.isEmpty() && _online_shop->getPurchaseOrder()->parse(o->toObject())) {
				state = State::Order;
				/// @todo убрать из корзины отсутствующие в заказе
			}
		}
		if (State::Busy != _state) {
			setState(state);
		}
		return true;
	}

	QString OfferMessage::getPreviewText() const
	{
		return _note.isEmpty() ? tr("Special offer") : _note;
	}

	OfferMessage::State OfferMessage::getState() const
	{
		return _state;
	}

	QString OfferMessage::getNote() const
	{
		return _note;
	}

	Cadabra::Queen::Core::Meta::Qd OfferMessage::getShopObjectId() const
	{
		return _shop_object_id;
	}

	Cadabra::Queen::Core::Meta::Qd OfferMessage::getPayerSubjectId() const
	{
		return _payer_subject_id;
	}

	Cadabra::Queen::EC::Model::OnlineShop* OfferMessage::getOnlineShop() const
	{
		return _online_shop.get();
	}

	void OfferMessage::setState(State state)
	{
		if (_state != state) {
			_state = state;
			emit stateChanged();
		}
	}

	void OfferMessage::onPriceLoaded(bool success)
	{
		if (!success) {
			setState(State::Price);
			return ;
		}
		const int showcase_count(_online_shop->getShowcaseCount());
		for (int s=0; s<showcase_count; ++s) {
			Data::Showcase* showcase(_online_shop->getShowcase(s));
			for (const Data::Showcase::Map::mapped_type& showcase_item : showcase->getItemMap()) {
				if (showcase->getCatalogue() == Data::Showcase::Catalogue::Bundle && !_bundle_set.contains(showcase_item.code)) {
					_online_shop->getPurchaseBasket()->addStock(showcase_item.stock.get(), 1);
				} else if (showcase->getCatalogue() == Data::Showcase::Catalogue::Nomenclature) {
					for (const Data::Stock::SourceMap::mapped_type& stock_item : showcase_item.stock->getSourceMap()) {
						QMap<QString, double>::ConstIterator u(_unit_map.find(stock_item.sku));
						if (_unit_map.constEnd() != u) {
							_online_shop->getPurchaseBasket()->addItem(stock_item, u.value());
						}
					}
				}
			}
		}
		if (!_online_shop->getPurchaseBasket()->isEmpty()) {
			setState(State::Basket);
			emit modified();
		}
	}

	void OfferMessage::onBasketFaulted(Cadabra::Queen::EC::PaymentFault incident, const QString& error)
	{
		setState(State::Invalid);
	}

	void OfferMessage::onBasketRepriced(QSet<Repository::Meta::Essence::Id> set)
	{
		if (State::Price == _state && !_online_shop->getPurchaseBasket()->isEmpty()) {
			setState(State::Basket);
			emit modified();
		}
	}

	void OfferMessage::onPurchaseOrderIssued(bool success)
	{
		if (!success) {
			setState(State::Basket);
			emit faulted(tr("Purchase order cannot be issed, please check your internet connection and try again"));
		} else {
			/// @brief Иначе, кнопка, переходит лишний раз в enabled состояние, на какое-то время, и пользователь может нажать еще раз
			setState(State::Busy);
			emit modified();
		}
	}

	void OfferMessage::onPurchaseOrderResolved(bool success)
	{
		if (!success) {
			setState(State::Order);
			emit faulted(tr("Purchase order cannot be resolved, please check your internet connection and try again"));
		} else {
			setState(State::Busy);
		}
	}

	void OfferMessage::onPaymentAcquired(bool success)
	{
		setState(success ? State::Payment : State::Order);
		emit completed(success);
	}

	void OfferMessage::onPaymentFaulted(EC::PaymentFault incident, const QString& message)
	{
		setState(State::Order);
		if (message.isEmpty()) {
			emit faulted(tr("Payment cannot be completed, please check your internet connection and try again"));
		} else {
			emit faulted(tr("Payment cannot be completed due to unexpectede error") + ": " + message);
		}
	}
} } } }
