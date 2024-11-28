#include "CRPDelay.hpp"

#include "CRPDeferredCall.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CRPDelay::CRPDelay(const QSharedPointer<CRPDelay::Vector>& vector, CRPFuture&& future)
	:
		_vector(vector),
		_future(_vector->append(std::move(future)))
	{

	}

	CRPDelay::CRPDelay(const QSharedPointer<CRPDeferredCall>& call, CRPFuture&& future)
	:
		_vector(new Vector(call.get())),
		_future(_vector->append(std::move(future)))
	{

	}

	CRPDelay::CRPDelay(const QSharedPointer<CRPDeferredCall>& call, const QString& interface, const QString& method, const QJsonObject& payload, qint64 handle)
	:
		_vector(new Vector(call.get())),
		_future(_vector->append({interface, method, payload, handle}))
	{

	}

	CRPDelay::CRPDelay(const QSharedPointer<CRPDeferredCall>& call, const QString& method, const QJsonObject& payload, qint64 handle)
	:
		_vector(new Vector(call.get())),
		_future(_vector->append({method, payload, handle}))
	{

	}

	CRPDelay::CRPDelay(CRPDelay&& origin)
	:
		_vector(std::move(origin._vector)),
		_future(std::move(origin._future))
	{

	}

	CRPDelay&& CRPDelay::detach()
	{
		return std::move(*this);
	}

	CRPDelay&& CRPDelay::detach(std::function<void(const CRPReaction&)>&& callback)
	{
		_future->induct(new CRPFunctor(std::move(callback)));
		return std::move(*this);
	}

	CRPDelay CRPDelay::operator()(CRPFuture&& future)
	{
		return CRPDelay(_vector, std::move(future));
	}

	CRPDelay CRPDelay::operator()(const QString& interface, const QString& method, const QJsonObject& payload, qint64 handle)
	{
		return CRPDelay(_vector, {interface, method, payload, handle});
	}

	CRPDelay CRPDelay::operator()(const QString& method, const QJsonObject& payload, qint64 handle)
	{
		return CRPDelay(_vector, {method, payload, handle});
	}

	CRPFuture* CRPDelay::operator->()
	{
		return _future.get();
	}

	CRPDelay::operator const CRPPromise&() const
	{
		return _future->getPromise();
	}

	CRPDelay::Vector::Vector(CRPDeferredCall* call)
	:
		_call(call)
	{

	}

	CRPDelay::Vector::~Vector()
	{
		if (_call.isNull()) {
			std::for_each
			(
				_container.constBegin(), _container.constEnd(),
				[]
				(const QSharedPointer<Core::Net::CRPFuture>& future)
				{
					future->happen(0, -1, "This future will never come because is dangling around");
				}
			);
		} else {
			_call->act(std::move(_container));
		}
	}

	const QSharedPointer<CRPFuture>& CRPDelay::Vector::append(CRPFuture&& future)
	{
		_container.append(QSharedPointer<CRPFuture>(new CRPFuture(std::move(future))));
		return _container.back();
	}

	CRPDeferredCall* CRPDelay::Vector::getCall() const
	{
		return _call.isNull() ? nullptr : _call.data();
	}
} } } }
