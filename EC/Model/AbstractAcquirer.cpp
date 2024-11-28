#include "AbstractAcquirer.hpp"

#include "../../Core/System/UserTracker.hpp"

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	AbstractAcquirer::AbstractAcquirer(const QString& name, QObject* parent)
	:
		QObject(parent),

		_name(name),
		_shop_endpoint(nullptr),
		_shop_id(0),
		_journal_id(0),
		_order_id(0)
	{

	}

	bool AbstractAcquirer::reprice(const QMap<Repository::Meta::Essence::Id, QString>&)
	{
		return false;
	}

	bool AbstractAcquirer::test(const QFlags<Option>& option_flagset)
	{
		return (getOptionFlagSet() & option_flagset) == option_flagset;
	}

	void AbstractAcquirer::track(const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total)
	{
		_track = qMakePair(item_vector, total);
	}

	void AbstractAcquirer::resolve(const QString& error_message)
	{
		emit faulted(error_message);
	}

	void AbstractAcquirer::resolve(bool yesno)
	{
		if (yesno) {
			emit successed();
			if (!_track.first.isEmpty() && !_track.second.isZero()) {
				Core::System::UserTracker::PurchaseEvent
				(
					getEngine(),
					_track.first.front().getSKU(),
					_track.first.front().getQuantity(),
					_track.second.getCentum(),
					_track.second.getCurrencySign()
				);
			}
		} else {
			emit canceled();
		}
		_track.first.clear();
		_track.second.zero();
	}

	void AbstractAcquirer::resolve(Repository::Meta::Essence::Id id, const SD::Meta::Money& price)
	{
		emit repriced({{id, price}});
	}

	void AbstractAcquirer::resolve(const QMap<Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money>& price_map)
	{
		emit repriced(price_map);
	}

	QString AbstractAcquirer::makeOrderNo() const
	{
		return QString("%1-%2")
				.arg(_order_id)
				.arg(_order_date.toString("ddMMyyyy"))
		;
/*
		return QString("%1-%2-%3")
				.arg(_journal_id)
				.arg(_order_id)
				.arg(_order_date.toString("ddMMyyyy"))
		;
*/
/*
		return QString("urn:cadabra:platform:ec:invoice:%1:%2:%3:%4")
				.arg(_shop_endpoint ? _shop_endpoint->getHost() : "localhost")
				.arg(_shop_endpoint ? _shop_endpoint->getPort() : 80)
				.arg(_journal_id)
				.arg(_order_id)
		;
*/
	}

	const QString& AbstractAcquirer::getName() const
	{
		return _name;
	}

	const QString& AbstractAcquirer::getEngine() const
	{
		static const QString retval("unknown");
		return retval;
	}

	QSet<QString> AbstractAcquirer::getCurrencyCodeSet() const
	{
		return {};
	}

	QFlags<AbstractAcquirer::Option> AbstractAcquirer::getOptionFlagSet() const
	{
		return {};
	}

	void AbstractAcquirer::setShopEndpoint(Core::Net::Endpoint* shop_endpoint, Repository::Meta::Object::Id shop_id)
	{
		_shop_endpoint = shop_endpoint;
		_shop_id = shop_id;
	}

	void AbstractAcquirer::setJournalId(const Repository::Meta::Object::Id& value)
	{
		_journal_id = value;
	}

	void AbstractAcquirer::setOrderId(const Repository::Meta::Object::Id& value)
	{
		_order_id = value;
	}

	void AbstractAcquirer::setOrderDate(const QDateTime& value)
	{
		_order_date = value;
	}

	void AbstractAcquirer::setOrderTitle(const QString& value)
	{
		_order_title = value;
	}

	void AbstractAcquirer::setOrderDescription(const QString& value)
	{
		_order_description = value;
	}
} } } }
