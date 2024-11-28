#include "CRPReaction.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CRPReaction::CRPReaction()
	:
		_domain(0),
		_code(0),
		_value(QJsonValue::Type::Undefined)
	{

	}

	CRPReaction::CRPReaction(CRPReaction&& orig)
	:
		_domain(std::move(orig._domain)),
		_code(std::move(orig._code)),
		_message(std::move(orig._message)),
		_value(std::move(orig._value))
	{

	}

	CRPReaction::CRPReaction(const CRPReaction& orig)
	:
		_domain(orig._domain),
		_code(orig._code),
		_message(orig._message),
		_value(orig._value)
	{

	}

	CRPReaction::CRPReaction(quint8 domain, quint32 code, const QString& message)
	:
		_domain(domain),
		_code(code),
		_message(message)
	{

	}

	CRPReaction::CRPReaction(QJsonValue&& value)
	:
		_domain(0),
		_code(0),
		_value(std::move(value))
	{

	}

	CRPReaction::CRPReaction(const QJsonValue& value)
	:
		_domain(0),
		_code(0),
		_value(value)
	{

	}

	CRPReaction::operator bool() const
	{
		return _domain == 0 && _code == 0 && !_value.isUndefined();
	}

	bool CRPReaction::operator!() const
	{
		return _domain > 0 || _code > 0 || _value.isUndefined();
	}

	QJsonValue& CRPReaction::operator*()
	{
		return _value;
	}

	const QJsonValue& CRPReaction::operator*() const
	{
		return _value;
	}

	void CRPReaction::log(const QString& prefix) const
	{
		if (_domain > 0 || _code > 0) {
			qWarning() << prefix << "CRP call error" << _code << "in domain" << _domain << "with message: " << _message;
		}
	}

	const quint8& CRPReaction::getDomain() const
	{
		return _domain;
	}

	const quint32& CRPReaction::getCode() const
	{
		return _code;
	}

	const QString& CRPReaction::getMessage() const
	{
		return _message;
	}

	bool CRPReaction::isFaulty() const
	{
		return _domain > 0 || _code > 0;
	}

	bool CRPReaction::isUndefined() const
	{
		return _value.isUndefined();
	}
} } } }
