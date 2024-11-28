#include "TinkoffAcquirer.hpp"

#include "../../../Core/System/Native.hpp"

#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	#ifdef APP_MERCHANT_ID
		const QString TinkoffAcquirer::AppMerchantId = APP_MERCHANT_ID;
	#else
		const QString TinkoffAcquirer::AppMerchantId = "";
	#endif
	#ifdef APP_MERCHANT_NAME
		const QString TinkoffAcquirer::AppMerchantName = APP_MERCHANT_NAME;
	#else
		const QString TinkoffAcquirer::AppMerchantName = "";
	#endif

	TinkoffAcquirer::TinkoffAcquirer(const QString& name, const QJsonObject& specification, QObject* parent)
	:
		AbstractAcquirer(name, parent),

		_public_key(specification.value("public_key").toString("")),
		_terminal_key(specification.value("terminal_key").toString("")),
		_password(specification.value("terminal_password").toString("")),

		_is_recurrent(false)
	{

	}
#ifndef TKS_ASDK
	const QString TinkoffAcquirer::WalletPaymentIntefaceName = "";

	bool TinkoffAcquirer::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, PaymentInterface payment_interface)
	{
		return false;
	}
#endif
	bool TinkoffAcquirer::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, bool ui)
	{
		if (_payment_inteface_connection) {
			return false;
		}
		_payment_inteface_connection = QObject::connect(Core::System::Native::Instance(), &Core::System::Native::actionAlertDone, std::bind((bool(TinkoffAcquirer::*)(const SD::Meta::LegalCounterparty&, const QVector<SD::Meta::PurchaseItem>&, const SD::Meta::Money&, int))&TinkoffAcquirer::pay, this, legal_counterparty, item_vector, total, std::placeholders::_1));
		Core::System::Native::Instance()->alertAction("", tr("Please choose payment method"), {TinkoffAcquirer::WalletPaymentIntefaceName, "VISA / Mastercard / Maestro / МИР"}, {Core::System::Native::AlertProperty::ActionCancel, Core::System::Native::AlertProperty::ActionSheet});
		return true;
	}

	bool TinkoffAcquirer::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, int payment_interface)
	{
		if (_payment_inteface_connection) {
			QObject::disconnect(_payment_inteface_connection);
		}
		if (-1 == payment_interface) {
			resolve(false);
			return true;
		}
		return pay(legal_counterparty, item_vector, total, static_cast<PaymentInterface>(payment_interface));
	}

	bool TinkoffAcquirer::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total)
	{
		return pay(legal_counterparty, item_vector, total, PaymentInterface::Auto);
	}

	bool TinkoffAcquirer::test(const QFlags<Option>& option_flagset)
	{
		return (option_flagset.testFlag(TinkoffAcquirer::Option::PriceAdjustment)) ? false : true;
	}

	void TinkoffAcquirer::setCustomerKey(const QString& value)
	{
		_customer_key = value;
	}

	void TinkoffAcquirer::setCustomerCardId(const QString& value)
	{
		_customer_card_id = value;
	}

	void TinkoffAcquirer::setPublicKey(const QString& value)
	{
		_public_key = value;
	}

	void TinkoffAcquirer::setTerminalKey(const QString& value)
	{
		_terminal_key = value;
	}

	void TinkoffAcquirer::setPassword(const QString& value)
	{
		_password = value;
	}

	void TinkoffAcquirer::setRecurrentMode(bool onoff)
	{
		_is_recurrent = onoff;
	}

	const QString& TinkoffAcquirer::getEngine() const
	{
		static const QString retval("ec.tinkoffacquirer");
		return retval;
	}

	QSet<QString> TinkoffAcquirer::getCurrencyCodeSet() const
	{
		return {"RUB"};
	}

	QFlags<AbstractAcquirer::Option> TinkoffAcquirer::getOptionFlagSet() const
	{
		return {};
	}
} } } }
