#ifndef Cadabra_Queen_EC_Model_Acquiring_PlayMarket_hpp
#define Cadabra_Queen_EC_Model_Acquiring_PlayMarket_hpp

#include "../AbstractAcquirer.hpp"

#include "../../Meta/InappReceipt.hpp"

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	class InappCharger : public AbstractAcquirer
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QString Code;
		static Meta::InappReceipt Receipt();
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		InappCharger(const QString& name, const QJsonObject& specification, QObject* parent);
		virtual ~InappCharger() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _shared_secret;
		bool _debug;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total) override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual const QString& getEngine() const override;
		virtual QSet<QString> getCurrencyCodeSet() const override;
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
