/**
 @file Duration.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Duration.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	/// @todo tr
	const QMap<QString, Duration::Span> Duration::Vocabulary = {
																	{"second", Span::Second},
																	{"seconds", Span::Second},
																	{"s", Span::Second},
																	{"с", Span::Second},
																	{"секунда", Span::Second},
																	{"секунд", Span::Second},
																	{"секунды", Span::Second},

																	{"minute", Span::Minute},
																	{"minutes", Span::Minute},
																	{"M", Span::Minute},
																	{"м", Span::Minute},
																	{"минута", Span::Minute},
																	{"минут", Span::Minute},
																	{"минуты", Span::Minute},

																	{"hour", Span::Hour},
																	{"hours", Span::Hour},
																	{"h", Span::Hour},
																	{"ч", Span::Hour},
																	{"час", Span::Hour},
																	{"часов", Span::Hour},
																	{"часа", Span::Hour},

																	{"day", Span::Day},
																	{"days", Span::Day},
																	{"d", Span::Day},
																	{"д", Span::Day},
																	{"дня", Span::Day},
																	{"день", Span::Day},
																	{"дней", Span::Day},

																	{"month", Span::Month},
																	{"m", Span::Month},
																	{"м", Span::Month},
																	{"месяц", Span::Month},
																	{"месяца", Span::Month},
																	{"месяцев", Span::Month},

																	{"years", Span::Year},
																	{"year", Span::Year},
																	{"y", Span::Year},
																	{"г", Span::Year},
																	{"год", Span::Year},
																	{"года", Span::Year},
																	{"лет", Span::Year}
	};

	Duration::Duration()
	{

	}

	Duration::Duration(const QString& value)
	{
		parse(value);
	}

	Duration::Duration(const Duration& origin)
	:
		_map(origin._map)
	{

	}

	Duration::Duration(Duration&& origin)
	:
		_map(std::move(origin._map))
	{

	}

	Duration::Duration(const QVariant& v)
	{
		parse(v.toString());
	}

	bool Duration::parse(const QString& value)
	{
		if (value.isEmpty()) {
			return false;
		}
		int length_offset(-1);
		while (value.length() - 1 > length_offset) {
			if (value.at(++length_offset).isDigit()) {
				continue;
			} else if (0 == length_offset && (value.at(0).toLatin1() == '-' || value.at(0).toLatin1() == '+')) {
				continue;
			} else {
				if (0 == length_offset || (1 == length_offset && (value.at(0).toLatin1() == '-' || value.at(0).toLatin1() == '+'))) {
					return false;
				}
				break;
			}
		}

		QMap<QString, Duration::Span>::ConstIterator d(Duration::Vocabulary.find(value.mid(length_offset).trimmed()));
		if (Duration::Vocabulary.constEnd() == d) {
			return false;
		}

		add(d.value(), value.left(length_offset).trimmed().toInt());
		return true;
	}

	void Duration::add(Duration::Span span, int value)
	{
		QMap<Duration::Span, int>::Iterator m(_map.find(span));
		if (_map.constEnd() != m) {
			m.value() = m.value() + value;
		} else {
			_map[span] = value;
		}
	}

	Duration::operator QString() const
	{
		return toString();
	}

	Duration::operator QVariant() const
	{
		return toVariant();
	}

	bool Duration::operator<(const Duration& rhs) const
	{
		/// @todo convert everyting to secs
		return false;
	}

	bool Duration::operator>(const Duration& rhs) const
	{
		/// @todo convert everyting to secs
		return false;
	}

	bool Duration::operator==(const Duration& rhs) const
	{
		/// @todo convert everyting to secs
		return false;
	}

	Duration& Duration::operator=(const Duration& rhs)
	{
		_map = rhs._map;
		return *this;
	}

	QString Duration::toString() const noexcept
	{
		return "'" + toStringList().join("', '") + "'";
	}

	QStringList Duration::toStringList() const noexcept
	{
		QStringList retval;
		for (QMap<Duration::Span, int>::ConstIterator d = _map.constBegin(); d != _map.constEnd(); ++d) {
			if (d.value() == 0) {
				continue;
			}
			switch (d.key()) {
				case Span::Second:
					retval.append("seconds");
					break;

				case Span::Minute:
					retval.append("minutes");
					break;

				case Span::Hour:
					retval.append("hours");
					break;

				case Span::Day:
					retval.append("days");
					break;

				case Span::Month:
					retval.append("month");
					break;

				case Span::Year:
					retval.append("years");
					break;

				default:
					continue;
			}

			if (d.value() > 0) {
				retval.last().prepend(QString("+%1 ").arg(d.value()));
			} else {
				retval.last().prepend(QString("%1 ").arg(d.value()));
			}
		}
		return retval;
	}

	QVariant Duration::toVariant() const noexcept
	{
		return QVariant(Duration(*this));
	}

	bool Duration::isZero() const
	{
		return _map.isEmpty();
	}

	QDataStream& operator<<(QDataStream& out, const Duration& duration)
	{
		out << duration._map;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, Duration& duration)
	{
		in >> duration._map;
		return in;
	}

	QDebug operator<<(QDebug debug, const Duration& duration)
	{
		debug.nospace() << duration.toStringList().join(", ");
		return debug;
	}
} } } }
