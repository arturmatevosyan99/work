/**
 @file ContactCounterparty.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "ContactCounterparty.hpp"

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	ContactCounterparty::ContactCounterparty()
	:
		_id(0),
		_subject_id(0),
		_sex(Sex::Unknown),
		_phone(0)
	{

	}

	ContactCounterparty::ContactCounterparty(const ContactCounterparty& origin)
	:
		_id(origin._id),
		_subject_id(origin._subject_id),
		_first_name(origin._first_name),
		_last_name(origin._last_name),
		_middle_name(origin._middle_name),
		_sex(origin._sex),
		_phone(origin._phone),
		_email(origin._email),
		_country_code(origin._country_code),
		_address(origin._address)
	{

	}

	ContactCounterparty::ContactCounterparty(ContactCounterparty&& origin)
	:
		_id(std::move(origin._id)),
		_subject_id(std::move(origin._subject_id)),
		_first_name(std::move(origin._first_name)),
		_last_name(std::move(origin._last_name)),
		_middle_name(std::move(origin._middle_name)),
		_sex(std::move(origin._sex)),
		_phone(std::move(origin._phone)),
		_email(std::move(origin._email)),
		_country_code(std::move(origin._country_code)),
		_address(std::move(origin._address))
	{

	}

	ContactCounterparty::~ContactCounterparty()
	{

	}

	ContactCounterparty& ContactCounterparty::operator=(const ContactCounterparty& rhs)
	{
		_id = rhs._id;
		_subject_id = rhs._subject_id;
		_first_name = rhs._first_name;
		_last_name = rhs._last_name;
		_middle_name = rhs._middle_name;
		_sex = rhs._sex;
		_phone = rhs._phone;
		_email = rhs._email;
		_country_code = rhs._country_code;
		_address = rhs._address;

		return *this;
	}

	ContactCounterparty& ContactCounterparty::operator=(ContactCounterparty&& rhs)
	{
		_id = std::move(rhs._id);
		_subject_id = std::move(rhs._subject_id);
		_first_name = std::move(rhs._first_name);
		_last_name = std::move(rhs._last_name);
		_middle_name = std::move(rhs._middle_name);
		_sex = std::move(rhs._sex);
		_phone = std::move(rhs._phone);
		_email = std::move(rhs._email);
		_country_code = std::move(rhs._country_code);
		_address = std::move(rhs._address);

		return *this;
	}

	ContactCounterparty::operator QJsonObject() const
	{
		return {
					{"id", QString::number(_id)},
					{"subject_id", QString::number(_subject_id)},
					{"first_name", _first_name},
					{"last_name", _last_name},
					{"middle_name", _middle_name},
					{"sex", static_cast<char>(_sex)},
					{"phone", QString::number(_phone)},
					{"email", QJsonObject
											{
												{"address", _email},
												{"name", ""} /// @todo Забытый "name"
											}
					},
					{"country_code", _country_code},
					{"address", _address}
		};
	}

	void ContactCounterparty::parse(const QJsonObject& object)
	{
		_id = object["id"].toString("0").toLongLong();
		_subject_id = object["subject_id"].toString("0").toLongLong();
		_first_name = object["first_name"].toString("");
		_last_name = object["last_name"].toString("");
		_middle_name = object["middle_name"].toString("");
		_sex = static_cast<Sex>(object["sex"].toString("U").at(0).toLatin1());
		_country_code = object["country_code"].toString("");
		const QJsonValue phone(object["phone"]);
		if (phone.isObject()) {
			_phone = phone["e164"].toString("0").toULongLong();
		} else if (phone.isDouble()) {
			_phone = phone.toInt();
		}
		const QJsonValue email(object["email"]);
		if (email.isObject()) {
			 /// @todo Забытый "name"
			_email = email["address"].toString("");
		} else if (email.isString()) {
			_email = email.toString("");
		}
	}

	void ContactCounterparty::setId(Repository::Meta::Essence::Id id)
	{
		_id = id;
	}

	void ContactCounterparty::setSubjectId(Repository::Meta::Subject::Id subject_id)
	{
		_subject_id = subject_id;
	}
	void ContactCounterparty::setFirstName(const QString& value)
	{
		_first_name = value;
	}

	void ContactCounterparty::setLastName(const QString& value)
	{
		_last_name = value;
	}

	void ContactCounterparty::setMiddleName(const QString& value)
	{
		_middle_name = value;
	}

	void ContactCounterparty::setSex(const ContactCounterparty::Sex& sex)
	{
		_sex = sex;
	}

	void ContactCounterparty::setCountryCode(const QString& code)
	{
		_country_code = code;
	}

	void ContactCounterparty::setAddress(const QString& value)
	{
		_address = value;
	}

	void ContactCounterparty::setEmail(const QString& value)
	{
		_email = value;
	}

	void ContactCounterparty::setPhoneNumber(qulonglong number)
	{
		_phone = number;
	}

	const Repository::Meta::Essence::Id& ContactCounterparty::getId()
	{
		return _id;
	}

	const Repository::Meta::Subject::Id& ContactCounterparty::getSubjectId()
	{
		return _subject_id;
	}

	const QString& ContactCounterparty::getFirstName() const
	{
		return _first_name;
	}

	const QString& ContactCounterparty::getMiddleName() const
	{
		return _middle_name;
	}

	const QString& ContactCounterparty::getLastName() const
	{
		return _last_name;
	}

	const ContactCounterparty::Sex& ContactCounterparty::getSex() const
	{
		return _sex;
	}

	const QString& ContactCounterparty::getCountryCode() const
	{
		return _country_code;
	}

	const QString& ContactCounterparty::getAddress() const
	{
		return _address;
	}

	const QString& ContactCounterparty::getEmail() const
	{
		return _email;
	}

	const qulonglong& ContactCounterparty::getPhoneNumber() const
	{
		return _phone;
	}

	QString ContactCounterparty::getPhone() const
	{
		return QString("+%1").arg(_phone);
	}

	bool ContactCounterparty::hasEmail() const
	{
		return !_email.isEmpty();
	}

	bool ContactCounterparty::hasPhone() const
	{
		return _phone > 0;
	}

	QDataStream& operator<<(QDataStream& out, const ContactCounterparty& contact_counterparty)
	{
		out << contact_counterparty._id
			<< contact_counterparty._subject_id
			<< contact_counterparty._first_name
			<< contact_counterparty._last_name
			<< contact_counterparty._middle_name
			<< static_cast<char>(contact_counterparty._sex)
			<< contact_counterparty._phone
			<< contact_counterparty._email
			<< contact_counterparty._country_code
			<< contact_counterparty._address;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, ContactCounterparty& contact_counterparty)
	{
		QChar sex;
		in	>> contact_counterparty._id
			>> contact_counterparty._subject_id
			>> contact_counterparty._first_name
			>> contact_counterparty._last_name
			>> contact_counterparty._middle_name
			>> sex
			>> contact_counterparty._phone
			>> contact_counterparty._email
			>> contact_counterparty._country_code
			>> contact_counterparty._address;
		contact_counterparty._sex = static_cast<ContactCounterparty::Sex>(sex.toLatin1());
		return in;
	}

	QDebug operator<<(QDebug debug, const ContactCounterparty& contact_counterparty)
	{
		debug	<< contact_counterparty._id
				<< contact_counterparty._subject_id
				<< contact_counterparty._first_name
				<< contact_counterparty._last_name
				<< contact_counterparty._middle_name
				<< static_cast<char>(contact_counterparty._sex)
				<< contact_counterparty._phone
				<< contact_counterparty._email
				<< contact_counterparty._country_code
				<< contact_counterparty._address;
		return debug;
	}
} } } }
