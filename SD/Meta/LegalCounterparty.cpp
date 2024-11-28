/**
 @file LegalCounterparty.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "LegalCounterparty.hpp"

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	LegalCounterparty::LegalCounterparty()
	:
		_type(Type::UndefinedPerson)
	{

	}

	LegalCounterparty::LegalCounterparty(const LegalCounterparty& origin)
	:
		_id(origin._id),
		_type(origin._type),
		_name(origin._name),
		_comment(origin._comment),
		_post_code(origin._post_code),
		_country_code(origin._country_code),
		_address1(origin._address1),
		_address2(origin._address2),
		_website(origin._website),
		_phone(origin._phone),
		_email(origin._email)
	{

	}

	LegalCounterparty::LegalCounterparty(LegalCounterparty&& origin)
	:
		_id(std::move(origin._id)),
		_type(std::move(origin._type)),
		_name(std::move(origin._name)),
		_comment(std::move(origin._comment)),
		_post_code(std::move(origin._post_code)),
		_country_code(std::move(origin._country_code)),
		_address1(std::move(origin._address1)),
		_address2(std::move(origin._address2)),
		_phone(std::move(origin._phone)),
		_email(std::move(origin._email))
	{

	}

	LegalCounterparty::~LegalCounterparty()
	{

	}

	LegalCounterparty& LegalCounterparty::operator=(const LegalCounterparty& rhs)
	{
		_id = rhs._id;
		_name = rhs._name;
		_type = rhs._type;
		_comment = rhs._comment;
		_post_code = rhs._post_code;
		_country_code = rhs._country_code;
		_website = rhs._website;
		_address1 = rhs._address1;
		_address2 = rhs._address2;
		_phone = rhs._phone;
		_email = rhs._email;

		return *this;
	}

	LegalCounterparty& LegalCounterparty::operator=(LegalCounterparty&& rhs)
	{
		_id = std::move(rhs._id);
		_name = std::move(rhs._name);
		_type = std::move(rhs._type);
		_comment = std::move(rhs._comment);
		_post_code =std::move(rhs._post_code);
		_country_code =std::move(rhs._country_code);
		_address1 = std::move(rhs._address1);
		_address2 = std::move(rhs._address2);
		_website = std::move(rhs._website);
		_email = std::move(rhs._email);
		_phone = std::move(rhs._phone);

		return *this;
	}

	LegalCounterparty::operator QJsonObject() const
	{
		return {
				{"id", QString::number(_id)},
				{"name", _name},
				{"type", static_cast<char>(_type)},
				{"comment", _comment},
				{"country_code", _country_code},
				{"post_code", _post_code},
				{"address1", _address1},
				{"address2", _address2},
				{"website", _website},
				{"phone", QJsonObject
										{
											{"e164", QString::number(_phone)}
										}
				},
				{"email", QJsonObject
										{
											{"address", _email},
											{"name", ""} /// @todo Забытый "name"
										}
				}
		};
	}

	void LegalCounterparty::parse(const QJsonObject& object)
	{
		_id = object["id"].toString("0").toLongLong();
		_name = object["name"].toString("");
		_type = static_cast<LegalCounterparty::Type>(object["type"].toString("p").at(0).toLatin1());
		_comment = object["comment"].toString("");
		_post_code = object["post_code"].toString("");
		_country_code = object["country_code"].toString("");
		_address1 = object["address1"].toString("");
		_address2 = object["address2"].toString("");
		_website = object["website"].toString("");
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

	void LegalCounterparty::setId(Repository::Meta::Essence::Id id)
	{
		_id = id;
	}

	void LegalCounterparty::setType(LegalCounterparty::Type value)
	{
		_type = value;
	}

	void LegalCounterparty::setName(const QString& value)
	{
		_name = value;
	}

	void LegalCounterparty::setComment(const QString& value)
	{
		_comment = value;
	}

	void LegalCounterparty::setPostCode(const QString& value)
	{
		_post_code = value;
	}

	void LegalCounterparty::setCountryCode(const QString& value)
	{
		_country_code = value;
	}

	void LegalCounterparty::setAddress1(const QString& value)
	{
		_address1 = value;
	}

	void LegalCounterparty::setAddress2(const QString& value)
	{
		_address2 = value;
	}

	void LegalCounterparty::setEmail(const QString& value)
	{
		_email = value;
	}

	void LegalCounterparty::setPhoneNumber(qulonglong value)
	{
		_phone = value;
	}

	const Repository::Meta::Essence::Id& LegalCounterparty::getId() const
	{
		return _id;
	}

	const LegalCounterparty::Type& LegalCounterparty::getType() const
	{
		return _type;
	}

	const QString& LegalCounterparty::getName() const
	{
		return _name;
	}

	const QString& LegalCounterparty::getComment() const
	{
		return _comment;
	}

	const QString& LegalCounterparty::getPostCode() const
	{
		return _post_code;
	}

	const QString& LegalCounterparty::getCountryCode() const
	{
		return _country_code;
	}

	const QString& LegalCounterparty::getAddress1() const
	{
		return _address1;
	}

	const QString& LegalCounterparty::getAddress2() const
	{
		return _address2;
	}

	const QString& LegalCounterparty::getEmail() const
	{
		return _email;
	}

	const qulonglong& LegalCounterparty::getPhoneNumber() const
	{
		return _phone;
	}

	QString LegalCounterparty::getPhone() const
	{
		return QString("+%1").arg(_phone);
	}

	bool LegalCounterparty::hasEmail() const
	{
		return !_email.isEmpty();
	}

	bool LegalCounterparty::hasPhone() const
	{
		return _phone > 0;
	}

	QDataStream& operator<<(QDataStream& out, const LegalCounterparty& legal_counterparty)
	{
		out << static_cast<char>(legal_counterparty._type) << legal_counterparty._name << legal_counterparty._country_code << legal_counterparty._address1 << legal_counterparty._address2 << legal_counterparty._email << legal_counterparty._phone;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, LegalCounterparty& legal_counterparty)
	{
		QChar type;
		in >> type >> legal_counterparty._name >> legal_counterparty._country_code >> legal_counterparty._address1 >> legal_counterparty._address2 >> legal_counterparty._email >> legal_counterparty._phone;
		legal_counterparty._type = static_cast<LegalCounterparty::Type>(type.toLatin1());
		return in;
	}

	QDebug operator<<(QDebug debug, const LegalCounterparty& legal_counterparty)
	{
		debug << legal_counterparty._type << legal_counterparty._name << legal_counterparty._country_code << legal_counterparty._address1 << legal_counterparty._address2 << legal_counterparty._email << legal_counterparty._phone;
		return debug;
	}
} } } }
