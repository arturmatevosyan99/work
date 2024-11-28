#ifndef Cadabra_Queen_EC_Data_Stock_hpp
#define Cadabra_Queen_EC_Data_Stock_hpp

#include "../../Repository/Meta/Essence.hpp"
#include "../../Repository/Meta/Object.hpp"
#include "../../Core/Meta/Qd.hpp"

#include "../../SD/Meta/Money.hpp"
#include "../../SD/Meta/PurchaseItem.hpp"

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QSharedPointer>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

namespace Cadabra { namespace Queen { namespace EC { namespace Data {
	class Stock : public QAbstractListModel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			PriceId = Qt::UserRole + 1,
			CatalogueId,
			UnitId,
			SKU,
			Name,
			Area,
			Specification,
			RichSpecification,
			Thumbnail,
			Price,
			Cost,
			Sorter,
			Available,
			Asset,
			Action
		};

		enum ActivationPhase : char
		{
			None = '0',
			Purchase = 'P', /// < Проверить что-то: актуальности подписки, например
			Sale = 'S',
			Dispatch = 'D'
		};

		enum class NomenclatureArea : char
		{
			Common = '0',
			Service = 'S',
			Merchandise = 'M',
			Product = 'P',
			Material = 'L'
		};

		enum ProceedAction
		{
			NotApplicable,
			Buy,
			Open,
			Cancel,
			Manage
		};

		struct Hook
		{
			Repository::Meta::Object::Id id;
			Repository::Meta::Object::ApplicationScheme application_scheme;
			QString name;
		};

		struct Item
		{
			Repository::Meta::Essence::Id price_id;
			Repository::Meta::Essence::Id catalogue_id;
			Repository::Meta::Essence::Id unit_id;
			QString sku;
			QString name;
			QString specification;
			bool rich_specification;
			NomenclatureArea area;
			SD::Meta::Money cost;
			int sorter;
			bool available;			
			QMap<Stock::ActivationPhase, Hook> asset;
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(ProceedAction)
		Q_PROPERTY(int count READ getItemCount NOTIFY itemsChanged)
		Q_PROPERTY(QString total READ getTotal NOTIFY itemsChanged)
		Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY itemsChanged)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using SourceMap = QMap<Repository::Meta::Essence::Id, Item>;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Stock::Item ParseItem(const QJsonObject& object);
		static Stock::SourceMap ParseArray(const QJsonArray& array);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Stock(QObject* parent = nullptr);
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		SourceMap _source_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void clear();
		virtual bool parse(const QJsonObject& object);
		virtual QJsonObject dump() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
		virtual bool insert(const Item& item);

	public:
		virtual void add(const QJsonArray& array);
		virtual void add(const Item& item);
		virtual void add(const SourceMap& source_map);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const SourceMap& getSourceMap() const;
		int getItemCount() const;
		Cadabra::Queen::SD::Meta::Money getTotal() const;
		QList<Core::Meta::Qd> getPriceList() const;
		QVariantList getPriceVariantList() const;
		QVariant getAction(const int& index) const;
		SD::Meta::Money getCost(const int& index) const;
		virtual SD::Meta::Money getCost(const SourceMap::ConstIterator& index) const;

		virtual int rowCount(const QModelIndex& parent) const;
		virtual QVariant data(const QModelIndex& index, int role) const;
		virtual QHash<int, QByteArray> roleNames() const;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void itemsChanged();
	/** @} */

	/** @name Signals */
	/** @{ */
	protected slots:
		virtual void onModelBeforeReset();
		virtual void onModelAfterReset();
		virtual void onModelDataChanged(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& roles);
		virtual void onModelRowsInserted(const QModelIndex& parent, int first, int last);
		virtual void onModelRowsRemoved(const QModelIndex& parent, int first, int last);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool isEmpty() const;
	/** @} */
	};
} } } }

#endif
