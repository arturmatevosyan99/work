/**
 @file EssenceToken.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "EssenceToken.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	EssenceToken::EssenceToken(QObject* parent)
	:
		QObject(parent),
		active(true),
		type(Type::Invalid)
	{

	}

	EssenceToken::EssenceToken(const EssenceToken& origin)
	:
		active(origin.active),
		type(origin.type),
		value(origin.value)
	{

	}

	EssenceToken::EssenceToken(EssenceToken&& origin)
	:
		active(std::move(origin.active)),
		type(std::move(origin.type)),
		value(std::move(origin.value))
	{

	}

	bool EssenceToken::parse(const QVariant& v)
	{
		return parse(v.toString());
	}

	bool EssenceToken::parse(const QString& input)
	{
		type = Type::Invalid;
		value = input;
		if (input.length() < 1) {
			value.clear();
			return false;
		} else if (value.startsWith("§")) {
			value.replace("§", "$");
		}

		char c(value.at(0).toLatin1());
		switch (c) {
			case '$':
				type = Type::Object;
				break;

			case '@':
				type = Type::Subject;
				break;

			case '%':
				type = Type::Group;
				break;

			case '#':
				type = Type::Tag;
				break;

			case ':':
				type = Type::Attribute;
				break;

			case '/':
				type = Type::Path;
				break;

			case '~':
				type = Type::TimeFrame;
				break;

			case '<':
				type = Type::BeforeDate;
				break;

			case '>':
				type = Type::AfterDate;
				break;

			case '(':
				type = Type::Interface;
				break;

			case ')':
				type = Type::Method;
				break;

			case '?':
				type = Type::Any;
				break;
		}

		if (Type::Invalid == type) {
			type = Type::Any;
		} else if (value.length() > 1) {
			value.remove(0, 1);
		} else {
			type = Type::Invalid;
			value.clear();
			return false;
		}
		emit changed();
		return true;
	}

	EssenceExpression EssenceToken::express() const
	{
		switch (type) {
			case Type::Tag: return EssenceExpression("tag", value);
			case Type::Path: return EssenceExpression("path", value);
			case Type::Object: return EssenceExpression("object", value);
			case Type::Group: return EssenceExpression("group", value);
			case Type::Subject: return EssenceExpression("subject", value);
			case Type::TimeFrame: return EssenceExpression("timeframe", value.front() == '-' ? value : "-" + value, EssenceExpression::OperatorType::GreaterOrEqual);
			case Type::AfterDate: return EssenceExpression("date", value, EssenceExpression::OperatorType::GreaterOrEqual);
			case Type::BeforeDate: return EssenceExpression("date", value, EssenceExpression::OperatorType::LessOrEqual);
			case Type::Interface: return EssenceExpression("interface", value);
			case Type::Method: return EssenceExpression("method", value);
			case Type::Any: return EssenceExpression("essence", value);
			case Type::Attribute: return EssenceExpression(value);
		}

		return EssenceExpression();
	}

	EssenceToken::operator qint64() const
	{
		return value.toLongLong();
	}

	EssenceToken::operator QVariant() const
	{
		return QVariant(value);
	}

	bool EssenceToken::operator<(const EssenceToken& rhs) const
	{
		return value < rhs.value;
	}

	bool EssenceToken::operator>(const EssenceToken& rhs) const
	{
		return value > rhs.value;
	}

	bool EssenceToken::operator==(const EssenceToken& rhs) const
	{
		return value == rhs.value;
	}

	EssenceToken& EssenceToken::operator=(const EssenceToken& rhs)
	{
		type = rhs.type;
		value = rhs.value;

		return *this;
	}

	QString EssenceToken::getSign() const
	{
		return QChar(static_cast<char>(type));
	}

	QString EssenceToken::dump() const noexcept
	{
		return QString("%1%2").arg(static_cast<char>(type)).arg(value);
	}

	bool EssenceToken::isInvalid() const
	{
		return type == EssenceToken::Type::Invalid || value.isEmpty();
	}

	QDebug operator<<(QDebug debug, const EssenceToken& token)
	{
		debug.nospace() << static_cast<char>(token.type) << token.value;

		return debug;
	}
} } } }
