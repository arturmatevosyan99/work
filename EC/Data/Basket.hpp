#ifndef Cadabra_Queen_EC_Data_Basket_hpp
#define Cadabra_Queen_EC_Data_Basket_hpp

#include "Stock.hpp"

#include "../EC.hpp"
#include "../Model/AbstractAcquirer.hpp"
#include "../../SD/Meta/PaymentPreference.hpp"

#include <QVector>
#include <QFlag>

namespace Cadabra { namespace Queen { namespace EC { namespace Data {
	class Basket : public Stock
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Quantity = Stock::Role::Action + 1,
			Discount
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QMap<QString, int> CurrencyFixedPriorityMap;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using PaymentPreferenceVector = QVector<SD::Meta::PaymentPreference>;
		using QuantityMap = QMap<Repository::Meta::Essence::Id, double>;
		using DiscountMap = QMap<Repository::Meta::Essence::Id, double>;
		using RepriceMap = QMap<Repository::Meta::Essence::Id, SD::Meta::Money>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Basket(QObject* parent = nullptr);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _currency_code;
		QScopedPointer<Model::AbstractAcquirer> _acquirer;
		PaymentPreferenceVector _payment_preference_vector;
		QuantityMap _quantity_map;
		DiscountMap _discount_map;
		RepriceMap _reprice_map;
		bool _reprice_force;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool parse(const QJsonObject& object) override;
		virtual QJsonObject dump() const override;
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
		virtual bool insert(const Item& item) override;

		bool repriceItem(const Cadabra::Queen::Repository::Meta::Essence::Id& id, const Cadabra::Queen::SD::Meta::Money& price);
		bool discountItem(const Cadabra::Queen::Repository::Meta::Essence::Id& id, const double& value);

	public:
		bool setPaymentMethod();
		bool setPaymentMethod(QFlags<Model::AbstractAcquirer::Option>& option_flagset);

		bool setCurrency(const QString& code);
		bool setAcquirer(const QString& engine_alias);
		bool setAcquirer(QScopedPointer<Model::AbstractAcquirer>&& acquirer);

		void enableRepriceForce(bool onoff = true);

		void addItem(const Item& item, double quantity = 1.0);
		Q_INVOKABLE void addStock(Cadabra::Queen::EC::Data::Stock* stock, double quantity = 1.0);
		Q_INVOKABLE void addStockItem(Cadabra::Queen::EC::Data::Stock* stock, QVariant price_qd, double quantity);

		Q_INVOKABLE void resetPaymentPreference();
		Q_INVOKABLE void addPaymentPreference(SD::Meta::PaymentPreference::Type type, const QString& alias);
		void addPaymentPreference(const SD::Meta::PaymentPreference& preference);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getCurrencyCode() const;
		const QVector<SD::Meta::PaymentPreference>& getPaymentPreferenceVector() const;		
		double getQuantity(const SourceMap::ConstIterator& m) const;
		double getQuantity(const int& index) const;
		double getDiscountRate(const SourceMap::ConstIterator& m) const;
		double getDiscountRate(const int& index) const;
		SD::Meta::Money getDiscountedCost(const SourceMap::ConstIterator& m) const;
		SD::Meta::Money getDiscountedCost(const int& index) const;
		SD::Meta::Money getImportCost(const SourceMap::ConstIterator& m) const;
		SD::Meta::Money getImportCost(const int& index) const;
		virtual SD::Meta::Money getCost(const SourceMap::ConstIterator& m) const override;

		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QVector<SD::Meta::PaymentPreference> makePaymentPreferenceVector() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void currencyChanged();
		void acquirerChanged();
		void faulted(Cadabra::Queen::EC::PaymentFault incident, const QString& error);
		void acquired(bool success);
		void itemsRepriced(QSet<Repository::Meta::Essence::Id> set);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onAcquirerChanged();
		void onSourceMapInserted(SourceMap::ConstIterator b, SourceMap::ConstIterator e);
		void onSourceMapRepriced(QMap<Cadabra::Queen::Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money> map);

	protected slots:
		virtual void onModelBeforeReset() override;
		virtual void onModelAfterReset() override;
		virtual void onModelDataChanged(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& roles) override;
		virtual void onModelRowsInserted(const QModelIndex& parent, int first, int last) override;
		virtual void onModelRowsRemoved(const QModelIndex& parent, int first, int last) override;
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
		virtual bool isEmpty() const override;
	/** @} */
	};
} } } }

#endif
