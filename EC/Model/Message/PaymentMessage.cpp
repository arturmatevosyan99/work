#include "PaymentMessage.hpp"

#include "../Application/OnlineShop.hpp"
#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"
#include "../../../Repository/Model/Descriptor/StatelessDescriptor.hpp"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	PaymentMessage::PaymentMessage(QObject* parent)
	:
		ERP::Model::DocumentMessage(parent),

		_online_shop(new OnlineShop())
	{
		QObject::connect(_online_shop.get(), &OnlineShop::purchaseOrderResolved, this, &PaymentMessage::onPurchaseOrderResolved);
		QObject::connect(_online_shop.get(), &OnlineShop::paymentAcquired, this, &PaymentMessage::onPaymentAcquired);
		QObject::connect(_online_shop.get(), &OnlineShop::paymentFaulted, this, &PaymentMessage::onPaymentFaulted);
	}

	bool PaymentMessage::resolveOrder()
	{
		return (!isImported() || _online_shop->getPurchaseOrder()->getItemVector().isEmpty()) ? false : _online_shop->resolvePurchaseOrder();
	}

	QJsonObject PaymentMessage::exportBody() const
	{
		return QJsonObject{};
	}

	bool PaymentMessage::importBody(const QJsonObject& body)
	{
		QJsonObject::ConstIterator d(body.find("document"));
		if (body.constEnd() == d || !d->isObject()) {
			return false;
		}

		Repository::Meta::Object::Id shop_object_id(Repository::Meta::Essence::ToId(d->toObject().value("shop_id")));
		if (shop_object_id <= 0 || _online_shop->hasDescriptor()) {
			return false;
		}
		QScopedPointer<Repository::Model::StatelessDescriptor> descriptor(
																		   new Repository::Model::StatelessDescriptor(
																														getChannelDescriptor()->getRealm()->getSessionDescriptor(),
																														QSharedPointer<Repository::Meta::Object>::create(
																																											shop_object_id,
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
		return DocumentMessage::importBody(body);
	}

	ERP::Model::AbstractDocument* PaymentMessage::getDocument() const
	{
		return _online_shop->getPurchaseOrder();
	}

	QString PaymentMessage::getDescription() const
	{
		return (_online_shop->getPurchaseOrder()->getState() == ERP::DocumentState::Unknown) ? tr("Special offer") : DocumentMessage::getDescription();
	}

	void PaymentMessage::onPurchaseOrderResolved(bool success)
	{
		if (!success) {
			emit faulted(tr("Purchase order cannot be resolved, please check your internet connection and try again"));
		}
	}

	void PaymentMessage::onPaymentAcquired(bool success)
	{
		emit completed(success);
	}

	void PaymentMessage::onPaymentFaulted(EC::PaymentFault incident, const QString& message)
	{
		if (message.isEmpty()) {
			emit faulted(tr("Payment cannot be completed, please check your internet connection and try again"));
		} else {
			emit faulted(tr("Payment cannot be completed due to unexpectede error") + ": " + message);
		}
	}
} } } }
