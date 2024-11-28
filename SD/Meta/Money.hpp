/**
 @file Money.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_SD_Meta_Money_hpp
#define Cadabra_Queen_SD_Meta_Money_hpp

#include <QObject>
#include <QMetaType>
#include <QDataStream>
#include <QJsonValue>
#include <QJsonObject>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	class Money : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(qint64 centum READ getCentum NOTIFY recalculated)
		Q_PROPERTY(QString currencyCode READ getCurrencyCode NOTIFY recalculated)
		Q_PROPERTY(QString currencySign READ getCurrencySign NOTIFY recalculated)
		Q_PROPERTY(double cost READ getCost NOTIFY recalculated)
		Q_PROPERTY(QString price READ getPrice NOTIFY recalculated)
		Q_PROPERTY(QString priceTag READ getPriceTag NOTIFY recalculated)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QMap<QString, QString> CurrencySignMap;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Money();
		Money(qint64 centum, const QString& currency_code);
		Money(const QJsonObject& object);
		Money(const Money& origin);
		Money(Money&& origin);
		virtual ~Money();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:		
		qint64 _centum;
		QString _currency_code;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		Money& operator=(const Money& rhs);
		Money& operator=(Money&& rhs);
		Money& operator+=(const Money& rhs);
		Money& operator+=(const qint64& rhs);
		Money operator+(const Money& rhs) const;
		Money operator+(const qint64& rhs) const;
		operator QString() const;
		operator QJsonObject() const;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
		void zero();
		Money multiply(double quantity) const;
		Money discount(double percent) const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setCurrencyCode(const QString& value);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const qint64& getCentum() const;
		double getCost() const;
		const QString& getCurrencyCode() const;
		const QString& getCurrencySign() const;
		QString getPrice(bool coins = false, bool currency_sign = true) const;
		QString getPriceTag() const;
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isZero() const;
		bool isUndefined() const;
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void recalculated();
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const Money& purchase_item);
	friend QDataStream& operator>>(QDataStream& in, Money& purchase_item);
	friend QDebug operator<<(QDebug debug, const Money& purchase_item);
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::SD::Meta::Money)

#endif
