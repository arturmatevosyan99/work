/**
 @file LegalCounterparty.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_SD_Meta_LegalCounterparty_hpp
#define Cadabra_Queen_SD_Meta_LegalCounterparty_hpp

#include "../../Repository/Meta/Essence.hpp"

#include "../SD.hpp"

#include <QObject>
#include <QJsonObject>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	class LegalCounterparty : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		using Type = SD::LegalType;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Type)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		LegalCounterparty();
		LegalCounterparty(const LegalCounterparty& origin);
		LegalCounterparty(LegalCounterparty&& origin);
		virtual ~LegalCounterparty() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Repository::Meta::Essence::Id _id;
		Type _type;
		QString _name;
		QString _comment; /// @todo Не используется(почему-то) на стороне ace
		QString _post_code; /// @todo Не используется(почему-то) на стороне ace
		QString _country_code; /// @todo Не используется(почему-то) на стороне ace
		QString _address1; /// @todo Не используется(почему-то) на стороне ace
		QString _address2; /// @todo Не используется(почему-то) на стороне ace
		QString _website; /// @todo Не используется(почему-то) на стороне ace
		qulonglong _phone;
		QString _email;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		LegalCounterparty& operator=(const LegalCounterparty& rhs);
		LegalCounterparty& operator=(LegalCounterparty&& rhs);
		operator QJsonObject() const;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
		void parse(const QJsonObject& object);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setId(Repository::Meta::Essence::Id id);
		void setType(LegalCounterparty::Type value);
		void setName(const QString& value);
		void setComment(const QString& value);
		void setPostCode(const QString& value);
		void setCountryCode(const QString& value);
		void setAddress1(const QString& value);
		void setAddress2(const QString& value);
		void setEmail(const QString& value);
		void setPhoneNumber(qulonglong value);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Repository::Meta::Essence::Id& getId() const;
		const LegalCounterparty::Type& getType() const;
		const QString& getName() const;
		const QString& getComment() const;
		const QString& getPostCode() const;
		const QString& getCountryCode() const;
		const QString& getAddress1() const;
		const QString& getAddress2() const;
		const QString& getEmail() const;
		const qulonglong& getPhoneNumber() const;
		QString getPhone() const;
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasEmail() const;
		bool hasPhone() const;
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
	friend QDataStream& operator<<(QDataStream& out, const LegalCounterparty& legal_counterparty);
	friend QDataStream& operator>>(QDataStream& in, LegalCounterparty& legal_counterparty);
	friend QDebug operator<<(QDebug debug, const LegalCounterparty& legal_counterparty);
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::SD::Meta::LegalCounterparty)

#endif
