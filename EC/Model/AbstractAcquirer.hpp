#ifndef Cadabra_Queen_EC_Model_AbstractAcquirer_hpp
#define Cadabra_Queen_EC_Model_AbstractAcquirer_hpp

#include "../../Core/Net/Endpoint.hpp"
#include "../../Repository/Meta/Object.hpp"
#include "../../SD/Meta/Counterparty.hpp"
#include "../../SD/Meta/PurchaseItem.hpp"
#include "../../SD/Meta/Money.hpp"

#include <QObject>
#include <QString>
#include <QDateTime>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	class AbstractAcquirer : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Option
		{
			SinglePurchaseItem = 0b000001,
			PriceAdjustment = 0b000010,
			WithoutDiscount = 0b000100
		};
		Q_DECLARE_FLAGS(OptionFlags, Option)
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAG(OptionFlags)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractAcquirer(const QString& name, QObject* parent = nullptr);
		virtual ~AbstractAcquirer() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	protected:
		const QString _name;

		Core::Net::Endpoint* _shop_endpoint;
		Repository::Meta::Object::Id _shop_id;
		Repository::Meta::Object::Id _journal_id;
		Repository::Meta::Object::Id _order_id;
		QDateTime _order_date;
		QString _order_title;
		QString _order_description;

	private:
		QPair<QVector<SD::Meta::PurchaseItem>, SD::Meta::Money> _track;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total) = 0;
		virtual bool reprice(const QMap<Repository::Meta::Essence::Id, QString>& item_map);
		virtual bool test(const QFlags<Option>& option_flagset);

	protected:
		void track(const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total);
		void resolve(const QString& error);
		void resolve(bool yesno);
		void resolve(Repository::Meta::Essence::Id key, const SD::Meta::Money& price);
		void resolve(const QMap<Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money>& price_map);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QString makeOrderNo() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getName() const;
		virtual const QString& getEngine() const;
		virtual QSet<QString> getCurrencyCodeSet() const;
		virtual QFlags<Option> getOptionFlagSet() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setShopEndpoint(Core::Net::Endpoint* shop_endpoint, Repository::Meta::Object::Id shop_id);
		void setJournalId(const Repository::Meta::Object::Id& value);
		void setOrderId(const Repository::Meta::Object::Id& value);
		void setOrderDate(const QDateTime& value);
		void setOrderTitle(const QString& value);
		void setOrderDescription(const QString& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void dismissed();
		void canceled();
		void successed();
		void faulted(QString message);
		void repriced(QMap<Cadabra::Queen::Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money> map);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::EC::Model::AbstractAcquirer::OptionFlags)
Q_DECLARE_METATYPE(Cadabra::Queen::EC::Model::AbstractAcquirer::OptionFlags)

#endif
