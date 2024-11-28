/**
 @file PaymentPreference.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_SD_Meta_PaymentPreference_hpp
#define Cadabra_Queen_SD_Meta_PaymentPreference_hpp

#include <QObject>
#include <QMetaType>
#include <QDataStream>
#include <QJsonObject>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	class PaymentPreference : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Type : char
		{
			Currency = 'C',
			Voucher = 'V', /// @note Coupon with 100% discount (when ec_coupon.code is not unique)
			Terminal = 'T', /// @note tinkoff_1, stripe_cadabra
			Engine = 'E' /// @note sd.cashbox, sd.banktransfer, ec.tinkoffacquiring, ec.stripe, ec.appstore, ec.playmarket
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Type)
		Q_PROPERTY(Type type READ getType CONSTANT)
		Q_PROPERTY(QString alias READ getAlias CONSTANT)
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
		PaymentPreference();
		PaymentPreference(PaymentPreference::Type type, const QString& alias);
		PaymentPreference(const QString& type, const QString& alias);
		PaymentPreference(const char& type, const QString& alias);
		PaymentPreference(const PaymentPreference& origin);
		PaymentPreference(PaymentPreference&& origin);
		virtual ~PaymentPreference() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Type _type;
		QString _alias;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		PaymentPreference& operator=(const PaymentPreference& rhs);
		PaymentPreference& operator=(PaymentPreference&& rhs);
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
		const QString& getAlias() const;
		const Type& getType() const;
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
	friend QDataStream& operator<<(QDataStream& out, const PaymentPreference& purchase_item);
	friend QDataStream& operator>>(QDataStream& in, PaymentPreference& purchase_item);
	friend QDebug operator<<(QDebug debug, const PaymentPreference& purchase_item);
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::SD::Meta::PaymentPreference)

#endif
