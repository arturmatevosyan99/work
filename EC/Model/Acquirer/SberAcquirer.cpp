#include "SberAcquirer.hpp"

#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	SberAcquirer::SberAcquirer(const QString& name, const QJsonObject& specification, QObject* parent)
	:
		AbstractAcquirer(name, parent),

		_public_key(specification.value("public_key").toString("")),
		_terminal_key(specification.value("terminal_key").toString("")),
		_password(specification.value("terminal_password").toString(""))
	{

	}
#ifndef SBRF_ASDK
	bool SberAcquirer::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total)
	{
		return false;
	}
#endif
	void SberAcquirer::setCustomerKey(const QString& value)
	{
		_customer_key = value;
	}

	void SberAcquirer::setCustomerCardId(const QString& value)
	{
		_customer_card_id = value;
	}

	void SberAcquirer::setPublicKey(const QString& value)
	{
		_public_key = value;
	}

	void SberAcquirer::setTerminalKey(const QString& value)
	{
		_terminal_key = value;
	}

	void SberAcquirer::setPassword(const QString& value)
	{
		_password = value;
	}

	void SberAcquirer::setRecurrentMode(bool onoff)
	{
		_is_recurrent = onoff;
	}

	QSet<QString> SberAcquirer::getCurrencyCodeSet() const
	{
		return {"RUB"};
	}

	QFlags<AbstractAcquirer::Option> SberAcquirer::getOptionFlagSet() const
	{
		return {};
	}
} } } }
