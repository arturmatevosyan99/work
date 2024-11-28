#ifndef Cadabra_Queen_EC_Model_Acquiring_TinkoffAcquirer_hpp
#define Cadabra_Queen_EC_Model_Acquiring_TinkoffAcquirer_hpp

#include "../AbstractAcquirer.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	class TinkoffAcquirer : public AbstractAcquirer
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class PaymentInterface : int
		{
			Auto = -1,
			Wallet = 0,
			Card = 1
		};
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		const static QString AppMerchantId;
		const static QString AppMerchantName;
		const static QString WalletPaymentIntefaceName;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TinkoffAcquirer(const QString& name, const QJsonObject& specification, QObject* parent);
		virtual ~TinkoffAcquirer() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _customer_key;
		QString _customer_card_id;

		QString _public_key;
		QString _terminal_key;
		QString _password;

		bool _is_recurrent;

		QMetaObject::Connection _payment_inteface_connection;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		bool pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, PaymentInterface payment_interface);
		bool pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, int payment_interface);
		bool pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, bool user_inteface);

	public:
		virtual bool pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total) override;
		virtual bool test(const QFlags<Option>& option_flagset) override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setCustomerKey(const QString& value);
		void setCustomerCardId(const QString& value);

		void setPublicKey(const QString& value);
		void setTerminalKey(const QString& value);
		void setPassword(const QString& value);

		void setRecurrentMode(bool onoff);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual const QString& getEngine() const override;
		virtual QSet<QString> getCurrencyCodeSet() const override;
		virtual QFlags<AbstractAcquirer::Option> getOptionFlagSet() const override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
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

#endif
