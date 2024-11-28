/**
 @file EssenceExpression.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "EssenceExpression.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	QPair<QStringRef, QStringRef> EssenceExpression::Split(const QStringRef& input, EssenceExpression::OperatorType& op)
	{
		/// @bug Учесть то, что один из операторов может быть внутри '... < ...'
		int position(-1);
		if ((position = input.indexOf(">=")) > 0) {
			op = OperatorType::GreaterOrEqual;
			return qMakePair(input.left(position).trimmed(), input.mid(position + 2).trimmed());
		} else if ((position = input.indexOf("<=")) > 0) {
			op = OperatorType::LessOrEqual;
			return qMakePair(input.left(position).trimmed(), input.mid(position + 2).trimmed());
		} else if ((position = input.indexOf("<>")) > 0) {
			op = OperatorType::NotEqual;
			return qMakePair(input.left(position).trimmed(), input.mid(position + 2).trimmed());
		} else if ((position = input.indexOf(">")) > 0) {
			op = OperatorType::Greater;
			return qMakePair(input.left(position).trimmed(), input.mid(position + 1).trimmed());
		} else if ((position = input.indexOf("<")) > 0) {
			op = OperatorType::Less;
			return qMakePair(input.left(position).trimmed(), input.mid(position + 1).trimmed());
		} else if ((position = input.indexOf("=")) > 0) {
			op = OperatorType::Equal;
			return qMakePair(input.left(position).trimmed(), input.mid(position + 1).trimmed());
		}
		op = OperatorType::None;
		return qMakePair(input, input);
	}

	QVariant EssenceExpression::Parse(const QStringRef& input)
	{
		EssenceExpression::OperatorType op;
		const QPair<QStringRef, QStringRef> pair(EssenceExpression::Split(input.trimmed(), op));

		QVariant retval;
		if (EssenceExpression::OperatorType::None != op) {
			retval.setValue(EssenceExpression(pair.first, pair.second, op));
		} else if (input.length() >= 2 && input.front().toLatin1() == '\'' && input.back().toLatin1() == '\'') { /// < string literal
			retval = input.mid(1, input.length() - 2).toString();
		} else if (input.compare(QLatin1String("null"), Qt::CaseInsensitive) == 0) { /// < null literal
			retval = QVariant::fromValue(nullptr);
		} else if (input.compare(QLatin1String("true"), Qt::CaseInsensitive) == 0) { /// < bool literal
			retval = true;
		} else if (input.compare(QLatin1String("false"), Qt::CaseInsensitive) == 0) { /// < bool literal
			retval = false;
		} else if (input.length() >= 1) { /// < numeric literal
			if (input.front().isNumber() || ((input.length() > 1) && input.at(1).isNumber() && (input.front().toLatin1() == '-' || input.front().toLatin1() == '.'))) {
				bool ok(false);
				retval = input.toInt(&ok);
				if (!ok) {
					if (input.contains(".")) {
						retval = input.toDouble(&ok);
					} else {
						retval = input.toLongLong(&ok);
					}
				}
			} else if (input.front().isLetter() || input.front().toLatin1() == '_') {
				retval = input.toUtf8().toLower(); /// < variable
			}
		} else {
			retval.clear();
		}
		return retval;
	}

	EssenceExpression::OperandType EssenceExpression::DetectOperandType(const QVariant& operand)
	{
		if (!operand.isValid()) {
			return OperandType::Invalid;
		} else if (operand.isNull()) {
			return OperandType::Null;
		}

		const QMetaType::Type type(static_cast<QMetaType::Type>(operand.type()));
		switch (type) {
			case QMetaType::Int: return OperandType::Int;
			case QMetaType::LongLong: return OperandType::LongLong;
			case QMetaType::Double: return OperandType::Double;
			case QMetaType::QByteArray: return OperandType::Variable;
			case QMetaType::QString: return OperandType::String;
			case QMetaType::Bool: return OperandType::Boolean;
			case QMetaType::User: return operand.canConvert<EssenceExpression>() ? OperandType::Expression : OperandType::Invalid;
			default: return OperandType::Invalid;
		}
	}

	EssenceExpression::EssenceExpression()
	:
		_operator(OperatorType::None)
	{

	}

	EssenceExpression::EssenceExpression(const QString& left, const QVariant& right, EssenceExpression::OperatorType op)
	:
		_operator(op),
		_left(left.toUtf8().toLower()),
		_right(right)
	{

	}

	EssenceExpression::EssenceExpression(const QStringRef& left, const QStringRef& right, EssenceExpression::OperatorType op)
	:
		_operator(op),
		_left(EssenceExpression::Parse(left)),
		_right(EssenceExpression::Parse(right))
	{

	}

	EssenceExpression::EssenceExpression(const QString& input)
	{
		QString expression(input.trimmed());
		if (expression.isEmpty()) {
			_left.clear(); /// < UnknownType
			_right.clear(); /// < UnknownType
			_operator = OperatorType::None;
			return ;
		}

		const QPair<QStringRef, QStringRef> pair(EssenceExpression::Split(QStringRef(&expression), _operator));
		if (EssenceExpression::OperatorType::None != _operator) {
			_left = EssenceExpression::Parse(pair.first);
			_right = EssenceExpression::Parse(pair.second);
		} else {
			_left = EssenceExpression::Parse(pair.first);
			_right.clear();
		}
	}

	EssenceExpression::EssenceExpression(const EssenceExpression& origin)
	:
		_operator(origin._operator),
		_left(origin._left),
		_right(origin._right)
	{

	}

	EssenceExpression::EssenceExpression(EssenceExpression&& origin)
	:
		_operator(std::move(origin._operator)),
		_left(std::move(origin._left)),
		_right(std::move(origin._right))
	{

	}

	EssenceExpression::operator QVariant() const
	{
		return toVariant();
	}

	EssenceExpression& EssenceExpression::operator=(const EssenceExpression& rhs)
	{
		_operator = rhs._operator;
		_left = rhs._left;
		_right = rhs._right;

		return *this;
	}

	const EssenceExpression::OperatorType& EssenceExpression::getOperatorType() const
	{
		return _operator;
	}

	const QVariant& EssenceExpression::getLeftOperand() const
	{
		return _left;
	}

	const QVariant& EssenceExpression::getRightOperand() const
	{
		return _right;
	}

	EssenceExpression::OperandType EssenceExpression::getLeftOperandType() const
	{
		return EssenceExpression::DetectOperandType(_left);
	}

	EssenceExpression::OperandType EssenceExpression::getRightOperandType() const
	{
		return EssenceExpression::DetectOperandType(_right);
	}

	QVariant EssenceExpression::toVariant() const noexcept
	{
		return QVariant(*this);
	}

	QDataStream& operator<<(QDataStream& out, const EssenceExpression& token)
	{
		out << token.getLeftOperand() << static_cast<char>(token.getOperatorType()) << token.getRightOperand();
		return out;
	}

	QDataStream& operator>>(QDataStream& in, EssenceExpression& expression)
	{
		qint8 op;
		in >> expression._left >> op >> expression._right;
		expression._operator = static_cast<EssenceExpression::OperatorType>(op);
		return in;
	}

	QDebug operator<<(QDebug debug, const EssenceExpression& expression)
	{
		debug.nospace() << expression.getLeftOperand() << static_cast<char>(expression.getOperatorType()) << expression.getRightOperand();
		return debug;
	}
} } } }
