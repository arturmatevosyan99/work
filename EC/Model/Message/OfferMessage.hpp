#ifndef Cadabra_Queen_EC_Model_Message_OfferMessage_hpp
#define Cadabra_Queen_EC_Model_Message_OfferMessage_hpp

#include "../../../Conversation/Model/AbstractMessage.hpp"

#include "../AbstractAcquirer.hpp"
#include "../Application/OnlineShop.hpp"
#include "../../EC.hpp"
#include "../../../SD/Meta/Counterparty.hpp"
#include "../../../SD/Meta/PurchaseItem.hpp"
#include "../../../SD/Meta/PaymentPreference.hpp"
#include "../../../SD/Meta/Money.hpp"

#include <QScopedPointer>
#include <QVector>
#include <QMultiMap>
#include <QMap>
#include <QString>
#include <QMetaType>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	class OfferMessage : public Conversation::Model::AbstractMessage
	{
	/** @name Aliasses */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum State {
			Price,
			Basket,
			Order,
			Busy,
			Payment,
			Invalid
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(State)
		Q_PROPERTY(State state READ getState NOTIFY stateChanged)
		Q_PROPERTY(QString note READ getNote NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd shopObjectId READ getShopObjectId NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd payerSubjectId READ getPayerSubjectId NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::EC::Model::OnlineShop* onlineShop READ getOnlineShop NOTIFY imported)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		OfferMessage(QObject* parent = nullptr);
		virtual ~OfferMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		State _state;
		QScopedPointer<OnlineShop> _online_shop;
		QScopedPointer<Data::Showcase> _unit_showcase;
		QScopedPointer<Data::Showcase> _bundle_showcase;

		Repository::Meta::Object::Id _shop_object_id;
		Repository::Meta::Subject::Id _payer_subject_id;

		QString _note;
		QMap<QString, double> _unit_map;
		QSet<QString> _bundle_set;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool loadPrice();
		Q_INVOKABLE bool issueOrder();
		Q_INVOKABLE bool resolveOrder();

		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		State getState() const;
		QString getNote() const;
		Cadabra::Queen::Core::Meta::Qd getShopObjectId() const;
		Cadabra::Queen::Core::Meta::Qd getPayerSubjectId() const;
		Cadabra::Queen::EC::Model::OnlineShop* getOnlineShop() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	private:
		void setState(State state);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void stateChanged();
		void faulted(QString message);
		void completed(bool success);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onPriceLoaded(bool success);
		void onBasketFaulted(Cadabra::Queen::EC::PaymentFault incident, const QString& error);
		void onBasketRepriced(QSet<Repository::Meta::Essence::Id> set);
		void onPurchaseOrderIssued(bool success);
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
