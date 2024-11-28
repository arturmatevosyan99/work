/**
 @file Counterparty.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Counterparty.hpp"

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	QSharedPointer<Counterparty> Counterparty::Parse(const QJsonObject& object)
	{
		QSharedPointer<Counterparty> retval(new Counterparty());
		retval->parse(object);
		return retval;
	}

	Counterparty::Counterparty()
	{

	}

	Counterparty::Counterparty(const Counterparty& origin)
	:
		_legal(origin._legal)
	{

	}

	Counterparty::Counterparty(Counterparty&& origin)
	:
		_legal(std::move(origin._legal))
	{

	}

	Counterparty::~Counterparty()
	{

	}

	Counterparty::operator QJsonObject() const
	{
		QJsonArray contact_array;
		for (const ContactCounterparty& contact : _contact_vector) {
			contact_array.push_back(static_cast<QJsonObject>(contact));
		}
		return {
					{"legal", static_cast<QJsonObject>(_legal)},
					{"contacts", contact_array}
		};
	}

	void Counterparty::parse(const QJsonObject& object)
	{
		_legal.parse(object["legal"].toObject());
		QJsonArray contact_array(object.value("contacts").toArray());
		for (const QJsonValue& contact : contact_array) {
			_contact_vector.push_back({});
			_contact_vector.back().parse(contact.toObject());
		}
		emit parsed();
	}

	const LegalCounterparty& Counterparty::getLegal() const
	{
		return _legal;
	}

	const QVector<ContactCounterparty>& Counterparty::getContactList() const
	{
		return _contact_vector;
	}

	Counterparty& Counterparty::operator=(const Counterparty& rhs)
	{
		_legal = rhs._legal;

		return *this;
	}

	Counterparty& Counterparty::operator=(Counterparty&& rhs)
	{
		_legal = std::move(rhs._legal);

		return *this;
	}

	QDataStream& operator<<(QDataStream& out, const Counterparty& counterparty)
	{
		out << counterparty._legal;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, Counterparty& counterparty)
	{
		in >> counterparty._legal;
		return in;
	}

	QDebug operator<<(QDebug debug, const Counterparty& counterparty)
	{
		debug << counterparty._legal;
		return debug;
	}
} } } }
