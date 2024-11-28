/**
 @file PaymentPreference.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "PaymentPreference.hpp"

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	PaymentPreference::PaymentPreference()
	:
	  _type(Type::Terminal),
	  _alias("default")
	{

	}

	PaymentPreference::PaymentPreference(PaymentPreference::Type type, const QString& alias)
	:
		_type(type),
		_alias(alias)
	{

	}

	PaymentPreference::PaymentPreference(const QString& type, const QString& alias)
	:
		_type(type.isEmpty() ? Type::Terminal : static_cast<Type>(type.at(0).toLatin1())),
		_alias(alias)
	{

	}

	PaymentPreference::PaymentPreference(const char& type, const QString& alias)
	:
		_type(static_cast<Type>(type)),
		_alias(alias)
	{

	}

	PaymentPreference::PaymentPreference(const PaymentPreference& origin)
	:
		_type(origin._type),
		_alias(origin._alias)
	{

	}

	PaymentPreference::PaymentPreference(PaymentPreference&& origin)
	:
		_type(std::move(origin._type)),
		_alias(std::move(origin._alias))
	{

	}

	PaymentPreference& PaymentPreference::operator=(const PaymentPreference& rhs)
	{
		_type = rhs._type;
		_alias = rhs._alias;

		return *this;
	}

	PaymentPreference& PaymentPreference::operator=(PaymentPreference&& rhs)
	{
		_type = std::move(rhs._type);
		_alias = std::move(rhs._alias);

		return *this;
	}

	PaymentPreference::operator QJsonObject() const
	{
		return {
				{"type", static_cast<char>(_type)},
				{"alias", _alias}
		};
	}

	const QString& PaymentPreference::getAlias() const
	{
		return _alias;
	}

	const PaymentPreference::Type& PaymentPreference::getType() const
	{
		return _type;
	}

	QDataStream& operator<<(QDataStream& out, const PaymentPreference& payment_preference)
	{
		out << static_cast<char>(payment_preference._type) << payment_preference._alias;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, PaymentPreference& payment_preference)
	{
		QChar type_char;
		in >> type_char >> payment_preference._alias;
		payment_preference._type = static_cast<PaymentPreference::Type>(type_char.toLatin1());

		return in;
	}

	QDebug operator<<(QDebug debug, const PaymentPreference& payment_preference)
	{
		debug << static_cast<char>(payment_preference._type) << ":" << payment_preference._alias;
		return debug;
	}
} } } }
