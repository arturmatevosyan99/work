#include "CRPFuture.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CRPFuture::CRPFuture()
	:
		_reactor(nullptr)
	{

	}

	CRPFuture::CRPFuture(const QString& interface, const QString& method, const QJsonObject& payload, qint64 handle)
	:
		_action(
				{
					{"moniker", handle > 0 ? QString::number(handle) : ""},
					{
						"reflector",
						QJsonObject
						{
							{"interface", interface},
							{"method", method}
						},
					},
					{"payload", payload}
				}
		),
		_reactor(nullptr)
	{

	}

	CRPFuture::CRPFuture(const QString& method, const QJsonObject& payload, qint64 handle)
	:
		_action(
				{
					{"moniker", handle > 0 ? QString::number(handle) : ""},
					{"reflector", method},
					{"payload", payload}
				}
		),
		_reactor(nullptr)
	{

	}

	CRPFuture::CRPFuture(CRPFuture&& origin)
	:
		_promise(std::move(origin._promise)),
		_action(std::move(origin._action)),
		_reactor(std::move(origin._reactor)),
		_handler_callback(std::move(origin._handler_callback)),
		_reaction_callback(std::move(origin._reaction_callback)),
		_exception_callback(std::move(origin._exception_callback))
	{

	}

	CRPFuture::~CRPFuture()
	{
		_promise.resolve(false);
		if (_reactor) {
			_reactor->deleteLater();
		}
	}

	bool CRPFuture::progress()
	{
		return _promise.resolve();
	}

	CRPFuture& CRPFuture::happen(QJsonValue payload)
	{
		if (!_promise.resolve(true)) {
			return *this;
		}
		if (_reactor) {
			_reactor->callback({payload});
		}
		if (_reaction_callback) {
			_reaction_callback(payload);
		} else if (_handler_callback) {
			_handler_callback({payload});
		}
		return *this;
	}

	CRPFuture& CRPFuture::happen(quint8 domain, quint32 code, QString message)
	{
		if (!_promise.resolve(false)) {
			return *this;
		}
		if (_reactor) {
			_reactor->callback({domain, code, message});
		}
		if (_exception_callback) {
			_exception_callback(domain, code, message);
		} else if (_handler_callback) {
			_handler_callback({domain, code, message});
		}
		return *this;
	}

	const CRPPromise& CRPFuture::getPromise() const
	{
		return _promise;
	}

	const QJsonObject& CRPFuture::getAction() const
	{
		return _action;
	}

	bool CRPFuture::induct(CRPReactor* reactor)
	{
		if (!reactor) {
			return false;
		} else if (_reactor) {
			reactor->deleteLater();
			return false;
		}
		_reactor = reactor;
		return true;
	}

	CRPFuture& CRPFuture::react(std::function<void(const CRPReaction&)>&& callback)
	{
		_handler_callback = callback;
		return *this;
	}

	CRPFuture& CRPFuture::handle(std::function<void(QJsonValue)>&& callback)
	{
		_reaction_callback = callback;
		return *this;
	}

	CRPFuture& CRPFuture::except(std::function<void(quint8, quint32, QString)>&& callback)
	{
		_exception_callback = callback;
		return *this;
	}

	CRPFuture::operator const CRPPromise&() const
	{
		return _promise;
	}
} } } }
