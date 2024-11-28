/**
 @file Qd.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Qd.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	bool Qd::IsValid(const QVariant& v)
	{
		if (v.isValid()) {
			bool ok(false);
			switch (v.type()) {
				case QVariant::Type::String:
					v.toString().toLongLong(&ok);
					break;

				default:
					v.toLongLong(&ok);
			}
			if (ok) {
				return true;
			}
		}
		return false;
	}

	Qd Qd::Make(const QVariant& v)
	{
		return Qd(v);
	}

	Qd::Qd()
	:
		value(0)
	{

	}

	Qd::Qd(qint64 v)
	:
		value(v)
	{

	}

	Qd::Qd(const Qd& origin)
	:
		value(origin.value)
	{

	}

	Qd::Qd(Qd&& origin)
	:
		value(std::move(origin.value))
	{

	}

	Qd::Qd(const QVariant& v)
	:
		value(0)
	{
		if (!v.isValid()) {
			return ;
		}
		bool ok(false);
		switch (v.type()) {
			case QVariant::Type::String:
				value = v.toString().toLongLong(&ok);
				break;

			case QVariant::UserType:
				if (v.canConvert<Meta::Qd>()) {
					value = v.value<Meta::Qd>();
					ok = true;
				}
				break;

			default:
				value = v.toLongLong(&ok);
		}
		if (!ok) {
			value = 0;
		}
	}

	void Qd::set(qint64 v)
	{
		value = v;
	}

	qint64 Qd::get() const
	{
		return value;
	}

	Qd::operator qint64() const
	{
		return value;
	}

	Qd::operator QVariant() const
	{
		return QVariant(value);
	}

	bool Qd::operator<(const qint64& rhs) const
	{
		return value < rhs;
	}

	bool Qd::operator>(const qint64& rhs) const
	{
		return value > rhs;
	}

	bool Qd::operator==(const qint64& rhs) const
	{
		return value == rhs;
	}

	bool Qd::operator<(const Qd& rhs) const
	{
		return value < rhs.value;
	}

	bool Qd::operator>(const Qd& rhs) const
	{
		return value > rhs.value;
	}

	bool Qd::operator==(const Qd& rhs) const
	{
		return value == rhs.value;
	}

	Qd& Qd::operator=(const Qd& rhs)
	{
		value = rhs.value;
		return *this;
	}

	qint64 Qd::toInt64() const noexcept
	{
		return value;
	}

	QString Qd::toString() const noexcept
	{
		return QString::number(value);
	}

	QVariant Qd::toVariant() const noexcept
	{
		return QVariant(value);
	}

	bool Qd::isZero() const
	{
		return value == 0;
	}

	bool Qd::isAny(const QList<Qd>& list) const
	{
		const qint64& v(value);
		return std::any_of(list.begin(), list.end(), [&v](const Qd& t){ return v == t.value; });
	}

	bool Qd::isAny(std::initializer_list<qint64> list) const
	{
		const qint64& v(value);
		return std::any_of(list.begin(), list.end(), [&v](const qint64& t){ return v == t; });
	}

	QDataStream& operator<<(QDataStream& out, const Qd& id)
	{
		out << id.value;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, Qd& id)
	{
		in >> id.value;
		return in;
	}

	QDebug operator<<(QDebug debug, const Qd& id)
	{
		debug.nospace() << id.value;
		return debug;
	}
} } } }
