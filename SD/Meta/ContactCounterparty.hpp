/**
 @file ContactCounterparty.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_SD_Meta_ContactCounterparty_hpp
#define Cadabra_Queen_SD_Meta_ContactCounterparty_hpp

#include "../../Repository/Meta/Essence.hpp"
#include "../../Repository/Meta/Subject.hpp"

#include "../SD.hpp"

#include <QObject>
#include <QJsonObject>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	class ContactCounterparty : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		using Sex = SD::PersonSex;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Sex)
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
		ContactCounterparty();
		ContactCounterparty(const ContactCounterparty& origin);
		ContactCounterparty(ContactCounterparty&& origin);
		virtual ~ContactCounterparty() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:		
		Repository::Meta::Essence::Id _id;
		Repository::Meta::Subject::Id _subject_id;
		QString _first_name;
		QString _last_name;
		QString _middle_name;
		Sex _sex;
		qulonglong _phone;
		QString _email;
		QString _country_code;
		QString _address;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		ContactCounterparty& operator=(const ContactCounterparty& rhs);
		ContactCounterparty& operator=(ContactCounterparty&& rhs);
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
		void setSubjectId(Repository::Meta::Subject::Id subject_id);
		void setFirstName(const QString& value);
		void setLastName(const QString& value);
		void setMiddleName(const QString& value);
		void setSex(const Sex& sex);
		void setCountryCode(const QString& value);
		void setAddress(const QString& value);
		void setEmail(const QString& value);
		void setPhoneNumber(qulonglong value);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Repository::Meta::Essence::Id& getId();
		const Repository::Meta::Subject::Id& getSubjectId();
		const QString& getFirstName() const;
		const QString& getMiddleName() const;
		const QString& getLastName() const;
		const ContactCounterparty::Sex& getSex() const;
		const QString& getCountryCode() const;
		const QString& getAddress() const;
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
	friend QDataStream& operator<<(QDataStream& out, const ContactCounterparty& legal_counterparty);
	friend QDataStream& operator>>(QDataStream& in, ContactCounterparty& legal_counterparty);
	friend QDebug operator<<(QDebug debug, const ContactCounterparty& legal_counterparty);
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::SD::Meta::ContactCounterparty)

#endif
