#ifndef Cadabra_Queen_EC_Model_Application_OnlineShop_hpp
#define Cadabra_Queen_EC_Model_Application_OnlineShop_hpp

#include "../../../Repository/Model/AbstractApplication.hpp"

#include "../../EC.hpp"
#include "../AbstractAcquirer.hpp"
#include "../../Data/Showcase.hpp"
#include "../../Data/Basket.hpp"
#include "../../Meta/InappReceipt.hpp"
#include "../../Meta/CustomerAccount.hpp"
#include "../../../SD/Model/Document/PurchaseOrder.hpp"
#include "../../../SD/Meta/PaymentPreference.hpp"

#include <QMap>
#include <QMultiMap>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QScopedPointer>
#include <QQmlListProperty>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	class OnlineShop : public Repository::Model::AbstractApplication
	{
	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::EC::Data::Showcase> showroom READ getShowcaseList)
		Q_PROPERTY(Cadabra::Queen::EC::Data::Basket* purchaseBasket READ getPurchaseBasket CONSTANT)
		Q_PROPERTY(Cadabra::Queen::SD::Model::PurchaseOrder* purchaseOrder READ getPurchaseOrder CONSTANT)
		Q_PROPERTY(bool stockout READ isStockout NOTIFY showcaseLoaded)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		template <typename A>
		static inline typename std::enable_if<std::is_convertible<A*, AbstractAcquirer*>::value, AbstractAcquirer*>::type
		AcquirerFactory(const QString& service_name, const QJsonObject& service_specification, QObject* parent = nullptr)
		{
			return new A(service_name, service_specification, parent);
		}
		template <typename A>
		static inline typename std::enable_if<std::is_same<A, std::nullptr_t>::value, AbstractAcquirer*>::type
		AcquirerFactory(const QString&, const QJsonObject&, QObject* = nullptr)
		{
			return nullptr;
		}

		static const QMap<QString, std::function<AbstractAcquirer*(const QString&, const QJsonObject&, QObject*)>> AcquirerFactoryMap;
		static bool Acquire(QScopedPointer<AbstractAcquirer>& acquirer, const QJsonArray& array);
		static bool Acquire(QScopedPointer<AbstractAcquirer>& acquirer, const QJsonObject& service);
		static bool Acquire(QScopedPointer<AbstractAcquirer>& acquirer, const QString& service_name, const QString& service_code, const QJsonObject& service_specification);

	private:
		static void AppendShowcaseProperty(QQmlListProperty<Data::Showcase>* list, Data::Showcase* showcase);
		static void ClearShowcasePropertyList(QQmlListProperty<Data::Showcase>* list);
		static int ShowcasePropertyListLength(QQmlListProperty<Data::Showcase>* list);
		static Data::Showcase* ShowcaseProperty(QQmlListProperty<Data::Showcase>* list, int index);
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit OnlineShop();
		virtual ~OnlineShop() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Data::Basket* _purchase_basket;
		SD::Model::PurchaseOrder* _purchase_order;
		QVector<Data::Showcase*> _showcase_vector;
		QSet<Data::Showcase*> _showcase_loadset;
		QMap<QSharedPointer<const Repository::Meta::Subject>, QSharedPointer<const SD::Meta::Counterparty>> _account_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool loadShowcase(Data::Showcase* showcase = nullptr);
		Q_INVOKABLE bool issuePurchaseOrder(const QString& title, const QString& comment, const QList<QUrl>& delivery_option_list = {});
		Q_INVOKABLE bool resolvePurchaseOrder(Cadabra::Queen::SD::Model::PurchaseOrder* purchase_order = nullptr);
		Q_INVOKABLE bool assetDispatch();
		Q_INVOKABLE bool extractAccount(QVariant customer_subject_id);
		Q_INVOKABLE Cadabra::Queen::EC::Meta::InappReceipt restoreInappReceipt();

		void appendShowcase(Data::Showcase* show_case);
		void clearShowcaseList();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QQmlListProperty<Data::Showcase> getShowcaseList();
		int getShowcaseCount() const;
		Data::Showcase* getShowcase(int index) const;
		Data::Basket* getPurchaseBasket() const;
		SD::Model::PurchaseOrder* getPurchaseOrder() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void showcaseLoaded(bool success);
		void purchaseOrderIssued(bool success);
		void purchaseOrderResolved(bool success);
		void paymentAcquired(bool success);
		void paymentFaulted(EC::PaymentFault incident, const QString& message);
		void accountExtracted(Cadabra::Queen::EC::Meta::CustomerAccount account);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onShowcaseLoaded(Data::Showcase* showcase, quint8 domain, quint32 code, QString message);
		void onShowcaseLoaded(Data::Showcase* showcase, QJsonValue payload);

		void onPurchaseOrderIssued(quint8 domain, quint32 code, QString message);
		void onPurchaseOrderIssued(QJsonValue payload);

		void onPurchaseOrderResolved(quint8 domain, quint32 code, QString message);
		void onPurchaseOrderResolved(QJsonValue payload);

		void onAssetDispatched(quint8 domain, quint32 code, QString message);
		void onAssetDispatched(QJsonValue payload);

		void onAccountExtracted(quint8 domain, quint32 code, QString message);
		void onAccountExtracted(QJsonValue payload);

		void onAcquiringFaulted(AbstractAcquirer* acquirer, const QString& error_message);
		void onAcquiringResolved(AbstractAcquirer* acquirer, bool yesno);
		void onAcquiringDismised(AbstractAcquirer* acquirer);
	/** @} */

	/** @name Hooks */
	/** @{ */	
	protected:
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
//		virtual bool onClose() override;
//		virtual bool onTeardown() override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isStockout() const;
	/** @} */
	};
} } } }
#endif
