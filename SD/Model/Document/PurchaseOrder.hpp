#ifndef Cadabra_Queen_SD_Model_Document_PurchaseOrder_hpp
#define Cadabra_Queen_SD_Model_Document_PurchaseOrder_hpp

#include "../../../ERP/Model/AbstractDocument.hpp"

#include "../../../EC/EC.hpp"
#include "../../Meta/Counterparty.hpp"
#include "../../Meta/PurchaseItem.hpp"
#include "../../Meta/PaymentPreference.hpp"
#include "../../Meta/Money.hpp"
#include "../../../Core/Meta/Qd.hpp"

#include <QVector>
//#include <QQmlListProperty>
#include <QString>

namespace Cadabra { namespace Queen { namespace SD { namespace Model {
    class PurchaseOrder : public ERP::Model::AbstractDocument
    {
    /** @name Qroperties */
    /** @{ */
    Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd shopId READ getShopId NOTIFY parsed)
		Q_PROPERTY(Cadabra::Queen::SD::Meta::LegalCounterparty legal READ getLegal NOTIFY parsed)
		Q_PROPERTY(QString currencyCode READ getCurrencyCode NOTIFY parsed)
		Q_PROPERTY(QString currencySign READ getCurrencySign NOTIFY parsed)
		Q_PROPERTY(QString total READ getTotal NOTIFY parsed)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem> itemList READ makeItemList NOTIFY parsed)
    /** @} */

	/** @name Statics */
	/** @{ */
	private:
		static Cadabra::Queen::SD::Meta::PurchaseItem* Item(QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem>* list, int index);
		static int ItemListSize(QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem>* list);
	/** @} */

	/** @name Constructors */
    /** @{ */
    public:
		PurchaseOrder(QObject* parent = nullptr);
		virtual ~PurchaseOrder() override = default;
    /** @} */

    /** @name Propreties */
    /** @{ */
    private:
		Core::Meta::Qd _shop_id;
		Meta::Counterparty _counteryparty;
		QVector<Meta::PurchaseItem> _item_vector;
		Meta::Money _total;
		QVector<Meta::PaymentPreference> _payment_preference_array;
    /** @} */

    /** @name Procedures */
    /** @{ */
	protected:
		virtual QJsonObject dump() const override;
		virtual bool scan(const QJsonObject& body) override;
    /** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<Cadabra::Queen::SD::Meta::PurchaseItem> makeItemList();
	/** @} */

    /** @name Getters */
    /** @{ */
    public:
		virtual QString getDescription() const override;
		Core::Meta::Qd getShopId() const;
		const Meta::Counterparty& getCounterparty() const;
		const Meta::LegalCounterparty& getLegal() const;
		const QString& getCurrencyCode() const;
		const QString& getCurrencySign() const;
		const Cadabra::Queen::SD::Meta::Money& getTotal() const;
		const QVector<Meta::PurchaseItem>& getItemVector() const;
    /** @} */

    /** @name Setters */
    /** @{ */
    public:
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
