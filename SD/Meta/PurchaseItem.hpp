/**
 @file PurchaseItem.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_SD_Meta_PurchaseItem_hpp
#define Cadabra_Queen_SD_Meta_PurchaseItem_hpp

#include "Money.hpp"

#include "../../Repository/Meta/Essence.hpp"

#include <QObject>
#include <QMetaType>
#include <QDataStream>
#include <QJsonObject>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	class PurchaseItem : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString name READ getName CONSTANT)
		Q_PROPERTY(QString sku READ getSKU CONSTANT)
		Q_PROPERTY(QString cost READ getCost CONSTANT)
		Q_PROPERTY(qint64 centum READ getCentum CONSTANT)
		Q_PROPERTY(double price READ getPrice CONSTANT)
		Q_PROPERTY(double quantity READ getQuantity CONSTANT)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
//		using Id = qint64;
//		using Scheme = qint64;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		PurchaseItem();
		PurchaseItem(const QString& name, const QString& sku, const SD::Meta::Money& cost, double quantity, Repository::Meta::Essence::Id unit_id = 0);
		PurchaseItem(const PurchaseItem& origin);
		PurchaseItem(PurchaseItem&& origin);
		virtual ~PurchaseItem();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _name;
		QString _sku;
		SD::Meta::Money _cost;
		double _quantity;
		Repository::Meta::Essence::Id _unit_id;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		PurchaseItem& operator=(const PurchaseItem& rhs);
		PurchaseItem& operator=(PurchaseItem&& rhs);
		operator QJsonObject() const;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getName() const;
		const QString& getSKU() const;
		const double& getQuantity() const;

		const qint64& getCentum() const;
		const SD::Meta::Money& getCost() const;
		const Repository::Meta::Essence::Id& getUnitId() const;
		double getPrice() const;
		Q_INVOKABLE QString getTotal(bool coins, bool currency_sign);
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Templates */
	/** @{ */
	public:
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const PurchaseItem& purchase_item);
	friend QDataStream& operator>>(QDataStream& in, PurchaseItem& purchase_item);
	friend QDebug operator<<(QDebug debug, const PurchaseItem& purchase_item);
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::SD::Meta::PurchaseItem)

#endif
