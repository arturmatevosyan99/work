#ifndef Cadabra_Queen_EC_Model_Message_PaymentMessage_hpp
#define Cadabra_Queen_EC_Model_Message_PaymentMessage_hpp

#include "../../../ERP/Model/Message/DocumentMessage.hpp"

#include "../AbstractAcquirer.hpp"
#include "../Application/OnlineShop.hpp"
#include "../../EC.hpp"
#include "../../../SD/Meta/Counterparty.hpp"
#include "../../../SD/Meta/PurchaseItem.hpp"
#include "../../../SD/Meta/PaymentPreference.hpp"
#include "../../../SD/Meta/Money.hpp"

#include <QScopedPointer>
#include <QVector>
#include <QQmlListProperty>
#include <QString>
#include <QMetaType>
#include <QMap>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	class PaymentMessage : public ERP::Model::DocumentMessage
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		PaymentMessage(QObject* parent = nullptr);
		virtual ~PaymentMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		const QScopedPointer<OnlineShop> _online_shop;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE bool resolveOrder();
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getDescription() const override;
		virtual ERP::Model::AbstractDocument* getDocument() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void faulted(QString message);
		void completed(bool success);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onPurchaseOrderResolved(bool success);
		void onPaymentAcquired(bool success);
		void onPaymentFaulted(EC::PaymentFault incident, const QString& message);
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
