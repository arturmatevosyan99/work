/**
 @file Money.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Money.hpp"

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	const QMap<QString, QString> Money::CurrencySignMap = {{"XXX", "x"}, {"RUB", "₽"}, {"USD", "$"}, {"EUR", "€"}, {"GBP", "£"}, {"AST", ""}, {"PMT", "G"}, {"AGT", "H"}, {"CCC", "Ꝣ"}};

	Money::Money()
	:
	  _centum(0),
	  _currency_code("XXX")
	{

	}

	Money::Money(qint64 centum, const QString& currency_code)
	:
		_centum(centum),
		_currency_code(currency_code)
	{

	}

	Money::Money(const QJsonObject& object)
	:
		_centum(object.value("value").toString("0").toLongLong()),
		_currency_code(object.value("currency").toString("XXX"))
	{
		if (object.value("negative").toBool(false)) {
			_centum = -_centum;
		}
	}

	Money::Money(const Money& origin)
	:
		_centum(origin._centum),
		_currency_code(origin._currency_code)
	{

	}

	Money::Money(Money&& origin)
	:
		_centum(std::move(origin._centum)),
		_currency_code(std::move(origin._currency_code))
	{

	}

	Money::~Money()
	{

	}

	Money& Money::operator=(const Money& rhs)
	{
		_centum = rhs._centum;
		_currency_code = rhs._currency_code;
		emit recalculated();

		return *this;
	}

	Money& Money::operator=(Money&& rhs)
	{
		_centum = std::move(rhs._centum);
		_currency_code = std::move(rhs._currency_code);
		emit recalculated();

		return *this;
	}

	Money& Money::operator+=(const Money& rhs)
	{
		if (rhs._currency_code == _currency_code) {
			_centum += rhs._centum;
			emit recalculated();
		}
		return *this;
	}

	Money& Money::operator+=(const qint64& rhs)
	{
		_centum += rhs;
		emit recalculated();

		return *this;
	}

	Money Money::operator+(const Money& rhs) const
	{
		if (rhs._currency_code == _currency_code) {
			return Money(_centum + rhs._centum, rhs._currency_code);
		}
		return Money(_centum, _currency_code);
	}

	Money Money::operator+(const qint64& rhs) const
	{
		return Money(_centum + rhs, _currency_code);
	}

	Money::operator QString() const
	{
		return getPriceTag();
	}

	Money::operator QJsonObject() const
	{
		return QJsonObject {
							{"value", QString::number(qAbs(_centum))},
							{"currency", _currency_code},
							{"negative", _centum < 0}
		};
	}

	void Money::zero()
	{
		if (0 != _centum) {
			_centum = 0;
			emit recalculated();
		}
	}

	Money Money::multiply(double quantity) const
	{
		return Money(qRound64(_centum * quantity), _currency_code);
	}

	Money Money::discount(double percent) const
	{
		if (percent <= 0 || percent > 100) {
			return *this;
		} else if (percent == 100) {
			return Money(0, _currency_code);
		}
		return Money(qRound64(getCost() * percent), _currency_code);
	}

	void Money::setCurrencyCode(const QString& code)
	{
		if (_currency_code != code) {
			_currency_code = code.toUpper();
			emit recalculated();
		}
	}

	const qint64& Money::getCentum() const
	{
		return _centum;
	}

	double Money::getCost() const
	{
		return _centum / 100;
	}

	const QString& Money::getCurrencyCode() const
	{
		return _currency_code;
	}

	const QString& Money::getCurrencySign() const
	{
		const QMap<QString, QString>::ConstIterator s(Money::CurrencySignMap.find(_currency_code));
		return Money::CurrencySignMap.constEnd() == s ? _currency_code : s.value();
	}

	QString Money::getPrice(bool coins, bool currency_sign) const
	{
		return (currency_sign ? QString("%1" + getCurrencySign()) : QString("%1")).arg(coins ? QString::number(_centum) : QString::number(_centum / 100.0));
	}

	QString Money::getPriceTag() const
	{
		return isUndefined() ? "-.-" : getPrice(false, true);
	}

	bool Money::isZero() const
	{
		return _centum == 0 && !isUndefined();
	}

	bool Money::isUndefined() const
	{
		return _currency_code.compare("XXX") == 0;
	}

	QDataStream& operator<<(QDataStream& out, const Money& money)
	{
		out << money._centum << money._currency_code;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, Money& money)
	{
		in >> money._centum >> money._currency_code;
		return in;
	}

	QDebug operator<<(QDebug debug, const Money& money)
	{
		debug << money._centum << money._currency_code;
		return debug;
	}
} } } }
