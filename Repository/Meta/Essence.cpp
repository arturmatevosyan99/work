/**
 @file Essence.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Essence.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	Essence::Id Essence::ToId(const QString& value, bool exception_safe)
	{
		if (!exception_safe) {
			return static_cast<Essence::Id>(value.toLongLong());
		}
		try {
			return static_cast<Essence::Id>(value.toLongLong());
		} catch (const std::exception&) {
			return 0;
		}
	}

	Essence::Id Essence::ToId(const QJsonValue& value, bool exception_safe)
	{
		if (!exception_safe) {
			return Essence::ToId(value.toString());
		}
		try {
			return Essence::ToId(value.toString(""));
		} catch (const std::exception&) {
			return 0;
		}
	}

	Handle Handle::fromUInt64(const quint64& value)
	{
		return Handle(value);
	}

	Handle Handle::fromString(const QString& value)
	{
		return Handle(value.toULongLong());
	}

	Handle Handle::fromJson(const QJsonValue& value)
	{
		return Handle(value.toString().toULongLong());
	}

	Handle::Handle()
	:
		_value(0)
	{

	}

	Handle::Handle(const Handle& origin)
	:
		_value(origin._value)
	{

	}

	Handle::Handle(Handle&& origin)
	:
		_value(std::move(origin._value))
	{

	}

	Handle::Handle(const quint64& value)
	:
		_value(value)
	{

	}

	Handle::Handle(const QString& value)
	:
		_value(value.toULongLong())
	{

	}

	Handle::Handle(quint64&& value)
	:
		_value(std::move(value))
	{

	}

	void Handle::setValue(const QString& value)
	{
		_value = value.toULongLong();
	}

	QString Handle::getValue() const
	{
		return QString::number(_value);
	}

	bool Handle::increment(const Handle& delta)
	{
		_value += delta._value;
		return true;
	}

	bool Handle::decrement(const Handle& delta)
	{
		if (_value < delta._value) {
			return false;
		}

		_value -= delta._value;
		return true;
	}

	Handle& Handle::ment(const Handle& delta, bool positive)
	{
		if (positive) {
			increment(delta);
		} else {
			decrement(delta);
		}
		return *this;
	}

	Handle& Handle::operator=(const Handle& rhs)
	{
		_value = rhs._value;
		return *this;
	}

	Handle& Handle::operator=(const quint64& rhs)
	{
		_value = rhs;
		return *this;
	}

	Handle& Handle::operator=(Handle&& rhs)
	{
		_value = std::move(rhs._value);
		return *this;
	}

	Handle& Handle::operator=(quint64&& rhs)
	{
		_value = std::move(rhs);
		return *this;
	}

	Handle& Handle::operator-=(const Handle& rhs)
	{
		_value -= rhs._value;
		return *this;
	}

	Handle& Handle::operator+=(const Handle& rhs)
	{
		_value += rhs._value;
		return *this;
	}

	Handle Handle::operator-(const Handle& rhs)
	{
		Handle retval(rhs._value);
		retval._value -= rhs._value;
		return retval;
	}

	Handle Handle::operator+(const Handle& rhs)
	{
		Handle retval(rhs._value);
		retval._value += rhs._value;
		return retval;
	}

	bool Handle::operator==(const quint64& rhs) const
	{
		return rhs == _value;
	}

	bool Handle::operator==(const int& rhs) const
	{
		return static_cast<quint64>(rhs) == _value;
	}

	bool Handle::operator!() const
	{
		return 0 == _value;
	}

	Handle::operator bool() const
	{
		return 0 != _value;
	}

	Handle::operator QString() const
	{
		return QString::number(_value);
	}

	bool Handle::isZero() const
	{
		return 0 == _value;
	}

	QDataStream& operator<<(QDataStream& out, const Handle& handle)
	{		
		out << handle._value;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, Handle& handle)
	{
		in >> handle._value;
		return in;
	}

	QDebug operator<<(QDebug debug, const Handle& handle)
	{
		QDebugStateSaver saver(debug);
		debug.nospace() << handle._value;

		return debug;
	}

	Cache::Cache()
	:
		_id(0),
		_handle(0),
		_scheme("0")
	{

	}

	Cache::Cache(const Cache& origin)
	:
		_id(origin._id),
		_handle(origin._handle),
		_scheme(origin._scheme)
	{

	}

	Cache::Cache(Meta::ObjectId id, Meta::Essence::Handle handle, Meta::Essence::Scheme scheme)
	:
		_id(id),
		_handle(handle),
		_scheme(scheme)
	{

	}

	Cache::Cache(Cache&& origin)
	:
		_id(std::move(origin._id)),
		_handle(std::move(origin._handle)),
		_scheme(std::move(origin._scheme))
	{

	}

	void Cache::setId(const Meta::Essence::Id& value)
	{
		_id = value;
	}

	void Cache::setHandle(const Meta::Essence::Handle& value)
	{
		_handle = value;
	}

	void Cache::setScheme(const Essence::Scheme& value)
	{
		_scheme = value;
	}

	Meta::Essence::Id Cache::getId() const
	{
		return _id;
	}

	Meta::Essence::Handle Cache::getHandle() const
	{
		return _handle;
	}

	Essence::Scheme Cache::getScheme() const
	{
		return _scheme;
	}

	Cache& Cache::operator=(const Cache& rhs)
	{
		_id = rhs._id;
		_handle = rhs._handle;
		_scheme = rhs._scheme;

		return *this;
	}

	Cache& Cache::operator=(const Meta::Essence::Id& rhs)
	{
		_id = rhs;

		return *this;
	}

	Cache& Cache::operator=(const Meta::Essence::Handle& rhs)
	{
		_handle = rhs;

		return *this;
	}

	Cache& Cache::operator=(const Meta::Essence::Scheme& rhs)
	{
		_scheme = rhs;

		return *this;
	}

	Cache& Cache::operator=(Cache&& rhs)
	{
		_id = std::move(rhs._id);
		_handle = std::move(rhs._handle);
		_scheme = std::move(rhs._scheme);

		return *this;
	}

	Cache& Cache::operator=(Meta::Essence::Id&& rhs)
	{
		_id = std::move(rhs);

		return *this;
	}

	Cache& Cache::operator=(Meta::Essence::Handle&& rhs)
	{
		_handle = std::move(rhs);

		return *this;
	}

	Cache& Cache::operator=(Meta::Essence::Scheme&& rhs)
	{
		_scheme = std::move(rhs);

		return *this;
	}

	bool Cache::operator!() const
	{
		return !_id || !_handle;
	}

	Cache::operator Meta::Essence::Id() const
	{
		return _id;
	}

	Cache::operator Meta::Essence::Handle() const
	{
		return _handle;
	}

	Cache::operator Meta::Essence::Scheme() const
	{
		return _scheme;
	}

	Cache::operator bool() const
	{
		return _id && _handle;
	}

	QDataStream& operator<<(QDataStream& out, const Cache& cache)
	{
		out << cache._id << cache._handle << cache._scheme;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, Cache& cache)
	{
		in >> cache._id >> cache._handle >> cache._scheme;
		return in;
	}

	QDebug operator<<(QDebug debug, const Cache& cache)
	{
		QDebugStateSaver saver(debug);
		debug.nospace() << cache._scheme << ":" << cache._id<< ":" << cache._handle;

		return debug;
	}

	RealmTarget::RealmTarget()
	:
		target(Essence::Target::Zilch),
		id(0)
	{

	}

	RealmTarget::RealmTarget(Meta::Essence::Target target, Meta::Essence::Id id)
	:
		target(target),
		id(id)
	{

	}

	RealmTarget::RealmTarget(const RealmTarget& origin)
	:
		target(origin.target),
		id(origin.id)
	{

	}

	RealmTarget::RealmTarget(RealmTarget&& origin)
	:
		target(origin.target),
		id(origin.id)
	{
		//origin.target = 0;
		origin.id = 0;
	}

	RealmTarget& RealmTarget::operator=(const RealmTarget& origin)
	{
		target = origin.target;
		id = origin.id;

		return *this;
	}

	const Meta::Essence::Target& RealmTarget::geTarget() const
	{
		return target;
	}

	const Meta::Essence::Id& RealmTarget::getId() const
	{
		return id;
	}
} } } }
