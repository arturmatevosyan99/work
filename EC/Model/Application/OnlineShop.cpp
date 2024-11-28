#include "OnlineShop.hpp"

#include "../Acquirer/TinkoffAcquirer.hpp"
#include "../Acquirer/SberAcquirer.hpp"

#ifdef INAPP_SDK
#if INAPP_SDK == APS
#include "../Acquirer/AppStore.hpp"
#elif INAPP_SDK == GPM
#include "../Acquirer/PlayMarket.hpp"
#elif INAPP_SDK == HAG
#include "../Acquirer/PlayMarket.hpp"
#endif
#else
#include "../Acquirer/InappCharger.hpp"
#endif

#include <QTimeZone>
#include <QDateTime>
#include <QLocale>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	const QMap<QString, std::function<AbstractAcquirer*(const QString&, const QJsonObject&, QObject*)>> OnlineShop::AcquirerFactoryMap =
	{
#ifdef TKS_ASDK
		{
			"ec.tinkoffacquirer",
			&OnlineShop::AcquirerFactory<TinkoffAcquirer>
		},
#endif

#ifdef STRIPE_SDK
		{
			"ec.stripeacquirer",
			&OnlineShop::AcquirerFactory<StripeAcquirer>
		},
#endif

#ifdef INAPP_SDK
		{
			"ec.inappcharger",
			&OnlineShop::AcquirerFactory<InappCharger>
		},
#if INAPP_SDK == APS
		{
			"ec.appstore",
			&OnlineShop::AcquirerFactory<InappCharger>
		},
#elif INAPP_SDK == GPM
		{
			"ec.playmarket",
			&OnlineShop::AcquirerFactory<InappCharger>
		},
#elif INAPP_SDK == HAG
		{
			"ec.appgallery",
			&OnlineShop::AcquirerFactory<InappCharger>
		},
#endif
#endif

#ifdef SBRF_ASDK
		{
			"ec.sberacquirer",
			&OnlineShop::AcquirerFactory<SberAcquirer>
		},
#endif

		{
			"",
			&OnlineShop::AcquirerFactory<std::nullptr_t>
		}
	};

	bool OnlineShop::Acquire(QScopedPointer<AbstractAcquirer>& acquirer, const QJsonArray& array)
	{
		foreach (const QJsonValue& service, array) {
			if (service.isObject() && Acquire(acquirer, service.toObject())) {
				return true;
			}
		}
		return false;
	}

	bool OnlineShop::Acquire(QScopedPointer<AbstractAcquirer>& acquirer, const QJsonObject& service)
	{
		return Acquire(acquirer, service["service_name"].toString("noname"), service["service_code"].toString("unknown"), service["service_specification"].toObject());
	}

	bool OnlineShop::Acquire(QScopedPointer<AbstractAcquirer>& acquirer, const QString& service_name, const QString& service_code, const QJsonObject& service_specification)
	{
		acquirer.reset(nullptr);
		if (service_code.isEmpty() || service_code == "unknown") {
			return false;
		}

		QMap<QString, std::function<AbstractAcquirer*(const QString&, const QJsonObject&, QObject*)>>::ConstIterator a(OnlineShop::AcquirerFactoryMap.find(service_code));
		if (OnlineShop::AcquirerFactoryMap.constEnd() == a) {
			acquirer.reset();
			return false;
		}
		acquirer.reset(a.value()(service_name, service_specification, nullptr));
		return true;
	}

	void OnlineShop::AppendShowcaseProperty(QQmlListProperty<Data::Showcase>* list, Data::Showcase* showcase)
	{
		if (OnlineShop* shop = qobject_cast<OnlineShop*>(list->object)) {
			shop->appendShowcase(showcase);
		}
	}

	void OnlineShop::ClearShowcasePropertyList(QQmlListProperty<Data::Showcase>* list)
	{
		if (OnlineShop* shop = qobject_cast<OnlineShop*>(list->object)) {
			shop->clearShowcaseList();
		}
	}

	int OnlineShop::ShowcasePropertyListLength(QQmlListProperty<Data::Showcase>* list)
	{
		OnlineShop* shop = qobject_cast<OnlineShop*>(list->object);
		return shop ? shop->_showcase_vector.count() : 0;
	}

	Data::Showcase* OnlineShop::ShowcaseProperty(QQmlListProperty<Data::Showcase>* list, int index)
	{
		OnlineShop* shop = qobject_cast<OnlineShop*>(list->object);
		return shop ? shop->_showcase_vector[index] : nullptr;
	}

	OnlineShop::OnlineShop()
	:
		AbstractApplication(nullptr),

		_purchase_basket(new Data::Basket(this)),
		_purchase_order(new SD::Model::PurchaseOrder(this))
	{
	}

	bool OnlineShop::loadShowcase(Data::Showcase* showcase)
	{
		QFlags<AbstractAcquirer::Option> option_flagset{AbstractAcquirer::Option::WithoutDiscount, AbstractAcquirer::Option::SinglePurchaseItem};
		QList<Data::Showcase*> showcase_predicate_list;
		for (QList<Data::Showcase*>::value_type& target_showcase : nullptr == showcase ? _showcase_vector : QVector<Data::Showcase*>{showcase}) {
			if (target_showcase->isActive()) {
				showcase_predicate_list.append(target_showcase);
				if (
					showcase_predicate_list.count() > 1 ||
					(
						target_showcase->getCatalogue() == Data::Showcase::Catalogue::Bundle ||
						target_showcase->getCatalogue() == Data::Showcase::Catalogue::Category ||
						(
							target_showcase->getCatalogue() == Data::Showcase::Catalogue::Nomenclature && target_showcase->getFilterStringList().length() > 1
						)
					)
				) {
					option_flagset.setFlag(AbstractAcquirer::Option::SinglePurchaseItem, false);
					break;
				}
			}
		}
		if (showcase_predicate_list.isEmpty() || !_purchase_basket->setPaymentMethod(option_flagset)) {
			return false;
		}
		for (const QList<Data::Showcase*>::value_type& target_showcase : showcase_predicate_list) {
			QSharedPointer<Core::Net::CRPCall> call(makeCall());
			connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), std::bind((void(OnlineShop::*)(Data::Showcase*, QJsonValue))&OnlineShop::onShowcaseLoaded, this, target_showcase, std::placeholders::_1));
			connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), std::bind((void(OnlineShop::*)(Data::Showcase*, quint8, quint32, QString))&OnlineShop::onShowcaseLoaded, this, target_showcase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			call->act(
						"Cadabra::Ace::EC::Model::PriceInterface",
						"Cadabra::Ace::EC::Model::ShowCase",
						{
							{"catalogue", static_cast<char>(target_showcase->getCatalogue())},
							{"filter", QJsonArray::fromStringList(target_showcase->getFilterStringList())},
							{"currency", _purchase_basket->getCurrencyCode()}
						}
			);
			_showcase_loadset.insert(target_showcase);
		}
		return true;
	}

	bool OnlineShop::issuePurchaseOrder(const QString& title, const QString& comment, const QList<QUrl>& delivery_option_list)
	{
		if (_purchase_basket->isEmpty()) {
			return false;
		}

		QVector<SD::Meta::PaymentPreference> payment_preference(_purchase_basket->makePaymentPreferenceVector());

		QJsonArray basket;
		for (Data::Stock::SourceMap::ConstIterator b(_purchase_basket->getSourceMap().constBegin()); b != _purchase_basket->getSourceMap().constEnd(); ++b) {
			basket.push_back(
								QJsonObject(
											{
												{"id", QString::number(b->price_id)},
												{"count", _purchase_basket->getQuantity(b)}
											}
								)
			);
		}

		QJsonArray preferences;
		for (QVector<SD::Meta::PaymentPreference>::ConstIterator p = payment_preference.constBegin(); p != payment_preference.constEnd(); ++p) {
			preferences.push_back(
									QJsonObject(
												{
													{"type", static_cast<char>(p->getType())},
													{"alias", p->getAlias()}
												}
									)
			);
		}

		QJsonArray delivery_options;
		for (const QUrl& delivery_option : delivery_option_list) {
			delivery_options.push_back(delivery_option.toString());
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&OnlineShop::onPurchaseOrderIssued));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&OnlineShop::onPurchaseOrderIssued));
		call->act(
					"Cadabra::Ace::SD::Model::ShopInterface",
					"Cadabra::Ace::SD::Model::OrderPurchase",
					{
						{"title", title},
						{"comment", comment},
						{"payment_preferences", preferences},
						{"delivery_options", delivery_options},
						{"basket", basket}
					}
		);
		return true;
	}

	bool OnlineShop::resolvePurchaseOrder(Cadabra::Queen::SD::Model::PurchaseOrder* purchase_order)
	{
		if (_purchase_order->getState() != ERP::DocumentState::Issued) {
			return false;
		}
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&OnlineShop::onPurchaseOrderResolved));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&OnlineShop::onPurchaseOrderResolved));
		call->act(
				   "Cadabra::Ace::SD::Model::ShopInterface",
				   "Cadabra::Ace::SD::Model::OrderResolve",
				   {
					   {"id", QString::number(_purchase_order->getId())}
				   }
		);
		return true;
	}

	bool OnlineShop::assetDispatch()
	{
		if (!hasDescriptor()) {
			return false;
		}
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&OnlineShop::onAssetDispatched));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&OnlineShop::onAssetDispatched));
		call->act(
					"Cadabra::Ace::EC::Model::PriceInterface",
					"Cadabra::Ace::EC::Model::AssetDispatch",
					{
					}
		);
		return true;
	}

	bool OnlineShop::extractAccount(QVariant customer_subject_id)
	{
		if (!hasDescriptor()) {
			return false;
		}

		Cadabra::Queen::Core::Meta::Qd customer_subject_qd(customer_subject_id);

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&OnlineShop::onAccountExtracted));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&OnlineShop::onAccountExtracted));
		call->act(
				   "Cadabra::Ace::SD::Model::ShopInterface",
				   "Cadabra::Ace::SD::Model::AccountExtract",
				   {
					   {"customer_subject_id", customer_subject_qd.toString()}
				   }
		);
		return true;
	}

	Cadabra::Queen::EC::Meta::InappReceipt OnlineShop::restoreInappReceipt()
	{
		return InappCharger::Receipt();
	}

	void OnlineShop::appendShowcase(Data::Showcase* show_case)
	{
		_showcase_vector.append(show_case);
	}

	void OnlineShop::clearShowcaseList()
	{
		_showcase_vector.clear();
	}

	QQmlListProperty<Data::Showcase> OnlineShop::getShowcaseList()
	{
		return QQmlListProperty<Data::Showcase>(this, this, &OnlineShop::AppendShowcaseProperty, &OnlineShop::ShowcasePropertyListLength, &OnlineShop::ShowcaseProperty, &OnlineShop::ClearShowcasePropertyList);
	}

	int OnlineShop::getShowcaseCount() const
	{
		return _showcase_vector.count();
	}

	Data::Showcase* OnlineShop::getShowcase(int index) const
	{
		return index >= _showcase_vector.count() ? nullptr : _showcase_vector.at(index);
	}

	Data::Basket* OnlineShop::getPurchaseBasket() const
	{
		return _purchase_basket;
	}

	SD::Model::PurchaseOrder* OnlineShop::getPurchaseOrder() const
	{
		return _purchase_order;
	}

	bool OnlineShop::onHandle()
	{
		return true;
	}

	bool OnlineShop::onOpen()
	{
		return true;
	}

	bool OnlineShop::onSetup()
	{
		return true;
	}

	void OnlineShop::onShowcaseLoaded(Data::Showcase* showcase, quint8 domain, quint32 code, QString message)
	{
		if (_showcase_loadset.remove(showcase) && _showcase_loadset.isEmpty()) {
			emit showcaseLoaded(false);
		}
	}

	void OnlineShop::onShowcaseLoaded(Data::Showcase* showcase, QJsonValue payload)
	{
		showcase->add(payload.toObject());
		if (_showcase_loadset.remove(showcase) && _showcase_loadset.isEmpty()) {
			emit showcaseLoaded(true);
		}
	}

	void OnlineShop::onPurchaseOrderIssued(quint8 domain, quint32 code, QString message)
	{
		emit purchaseOrderIssued(false);
	}

	void OnlineShop::onPurchaseOrderIssued(QJsonValue payload)
	{
		QJsonObject object(payload.toObject());
		if (object.isEmpty()) {
			emit purchaseOrderIssued(false);
			return ;
		}

		QJsonObject::const_iterator o(object.find("order"));
		if (object.constEnd() == o || !_purchase_order->parse(o->toObject())) {
			emit purchaseOrderIssued(false);
			return ;
		}

		QJsonObject::const_iterator s(object.find("state"));
		if (object.constEnd() == s) {
			emit purchaseOrderIssued(false);
			return ;
		}
		QJsonObject state(s->toObject());

		QJsonObject::const_iterator p(state.find("payed"));
		if (state.constEnd() == p) {
			emit purchaseOrderIssued(false);
			return ;
		}else if (p.value().toBool()) {
			/// @todo message dialog is payed
			emit purchaseOrderIssued(false);
			return ;
		}

		QJsonObject::const_iterator m(state.find("payment_methods"));
		if (state.constEnd() == p) {
			emit purchaseOrderIssued(false);
			return ;
		}

		QJsonArray method_array(m->toArray());
		if (method_array.size() <= 0) {
			emit purchaseOrderIssued(false);
			return ;
		}

		emit purchaseOrderIssued(true);

		QScopedPointer<AbstractAcquirer> acquirer;
		if (!OnlineShop::Acquire(acquirer, method_array)) {
			emit paymentFaulted(EC::PaymentFault::UnreachableService, tr("Payment method is not supported by this platform"));
		} else {
			acquirer->setShopEndpoint(getDescriptor()->getRealm()->getEndpoint(), getDescriptor()->getObjectId());
			acquirer->setJournalId(_purchase_order->getJournalId());
			acquirer->setOrderId(_purchase_order->getId());
			acquirer->setOrderDate(_purchase_order->getIssueDate());
			acquirer->setOrderTitle(_purchase_order->getTitle());
			acquirer->setOrderDescription(_purchase_order->getDescription());

			QObject::connect(acquirer.get(), &AbstractAcquirer::faulted, std::bind(&OnlineShop::onAcquiringFaulted, this, acquirer.get(), std::placeholders::_1));
			QObject::connect(acquirer.get(), &AbstractAcquirer::canceled, std::bind(&OnlineShop::onAcquiringResolved, this, acquirer.get(), false));
			QObject::connect(acquirer.get(), &AbstractAcquirer::successed, std::bind(&OnlineShop::onAcquiringResolved, this, acquirer.get(), true));
			QObject::connect(acquirer.get(), &AbstractAcquirer::dismissed, std::bind(&OnlineShop::onAcquiringDismised, this, acquirer.get()));

			if (acquirer->pay(_purchase_order->getLegal(), _purchase_order->getItemVector(), _purchase_order->getTotal())) {
				acquirer.take()->setParent(this);
			} else {
				onAcquiringFaulted(acquirer.take(), "Payment process error");
			}
		}
	}

	void OnlineShop::onPurchaseOrderResolved(quint8 domain, quint32 code, QString message)
	{
		emit paymentFaulted(EC::PaymentFault::UnknownError, message);
	}

	void OnlineShop::onPurchaseOrderResolved(QJsonValue payload)
	{
		QJsonObject object(payload.toObject());

		QJsonObject::const_iterator p(object.find("payed"));
		if (object.constEnd() == p) {
			emit purchaseOrderResolved(false);
			/// @note Не важно, на самом деле, как именно Resolved = false, потому что это совершенно точно связанно с отсутсивем платежных способов в резолве
//			emit paymentFaulted(Cadabra::Queen::EC::PaymentFault::UnknownError, "Malformed payment specification");
			return ;
		} else if (p.value().toBool()) {
			emit purchaseOrderResolved(true);
			emit paymentAcquired(true);
			return ;
		}

		QJsonObject::const_iterator m(object.find("payment_methods"));
		if (object.constEnd() == m) {
			emit purchaseOrderResolved(false);
//			emit paymentFaulted(Cadabra::Queen::EC::PaymentFault::UnreachableService, tr("No payment method is specified"));
			return ;
		}

		QJsonArray method_array(m->toArray());
		if (method_array.size() <= 0) {
			emit purchaseOrderResolved(false);
//			emit paymentFaulted(Cadabra::Queen::EC::PaymentFault::UnreachableService, tr("No payment method is available"));
			return ;
		}

		emit purchaseOrderResolved(true);

		QScopedPointer<AbstractAcquirer> acquirer;
		if (!OnlineShop::Acquire(acquirer, method_array)) {
			emit paymentFaulted(Cadabra::Queen::EC::PaymentFault::UnreachableService, tr("Payment method is not supported by this platform"));
		} else {
			acquirer->setShopEndpoint(getDescriptor()->getRealm()->getEndpoint(), getDescriptor()->getObjectId());
			acquirer->setOrderId(_purchase_order->getId());
			acquirer->setOrderDate(_purchase_order->getIssueDate());
			acquirer->setOrderTitle(_purchase_order->getTitle());
			acquirer->setOrderDescription(_purchase_order->getComment());

			QObject::connect(acquirer.get(), &AbstractAcquirer::faulted, std::bind(&OnlineShop::onAcquiringFaulted, this, acquirer.get(), std::placeholders::_1));
			QObject::connect(acquirer.get(), &AbstractAcquirer::canceled, std::bind(&OnlineShop::onAcquiringResolved, this, acquirer.get(), false));
			QObject::connect(acquirer.get(), &AbstractAcquirer::successed, std::bind(&OnlineShop::onAcquiringResolved, this, acquirer.get(), true));
			QObject::connect(acquirer.get(), &AbstractAcquirer::dismissed, std::bind(&OnlineShop::onAcquiringDismised, this, acquirer.get()));

			if (acquirer->pay(_purchase_order->getLegal(), _purchase_order->getItemVector(), _purchase_order->getTotal())) {
				acquirer.take()->setParent(this);
			} else {
				onAcquiringFaulted(acquirer.take(), "Payment process error");
			}
		}
	}

	void OnlineShop::onAssetDispatched(quint8 domain, quint32 code, QString message)
	{
	}

	void OnlineShop::onAssetDispatched(QJsonValue payload)
	{
	}

	void OnlineShop::onAccountExtracted(quint8 domain, quint32 code, QString message)
	{
//		emit accountExtracted();
	}

	void OnlineShop::onAccountExtracted(QJsonValue payload)
	{
		if (!payload.isObject()) {
			return ;
		}

		const QJsonObject account(payload.toObject());
		emit accountExtracted(Meta::CustomerAccount(Repository::Meta::Subject::Parse(account.value("subject").toObject()), SD::Meta::Counterparty::Parse(account.value("counterparty").toObject())));
	}

	void OnlineShop::onAcquiringFaulted(AbstractAcquirer* acquirer, const QString& error_message)
	{
		QObject::disconnect(acquirer);
		acquirer->deleteLater();
		emit paymentFaulted(EC::PaymentFault::AcquiringError, error_message);
	}

	void OnlineShop::onAcquiringResolved(AbstractAcquirer* acquirer, bool yesno)
	{
		QObject::disconnect(acquirer);
		acquirer->deleteLater();
		emit paymentAcquired(yesno);
	}

	void OnlineShop::onAcquiringDismised(AbstractAcquirer* acquirer)
	{
		QObject::disconnect(acquirer);
		acquirer->deleteLater();
	}

	bool OnlineShop::isStockout() const
	{
		return _showcase_vector.empty();
	}
} } } }
