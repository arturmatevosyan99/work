#include "InappCharger.hpp"

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	Meta::InappReceipt InappCharger::Receipt()
	{
		return Meta::InappReceipt();
	}

	InappCharger::InappCharger(const QString& name, const QJsonObject& specification, QObject* parent)
	:
		AbstractAcquirer(name, parent)
	{

	}

	bool InappCharger::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total)
	{
		return false;
	}

	const QString& InappCharger::getEngine() const
	{
		static const QString retval("ec.inappcharger");
		return retval;
	}
} } } }
