#ifndef Cadabra_Queen_EC_Model_Acquiring_SberAcquirer_hpp
#define Cadabra_Queen_EC_Model_Acquiring_SberAcquirer_hpp

#include "../AbstractAcquirer.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	class SberAcquirer : public AbstractAcquirer
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SberAcquirer(const QString& name, const QJsonObject& specification, QObject* parent);
		virtual ~SberAcquirer() override = default;
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
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total) override;
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
		virtual QSet<QString> getCurrencyCodeSet() const override;
		virtual QFlags<Option> getOptionFlagSet() const override;
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
