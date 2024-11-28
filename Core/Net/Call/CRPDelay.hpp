/**
 @file CRPDelay.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Call_CRPDelay_hpp
#define Cadabra_Queen_Core_Net_Call_CRPDelay_hpp

#include "CRPFuture.hpp"
#include "CRPReactor.hpp"
#include "CRPDeferredCall.hpp"

#include <QPointer>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CRPDelay
	{
	/** @name Classes */
	/** @{ */
	private:
		struct Vector
		{
		public:
			Vector(CRPDeferredCall* call);
			~Vector();

		private:
			const QPointer<CRPDeferredCall> _call;
			QVector<QSharedPointer<CRPFuture>> _container;

		public:
			const QSharedPointer<CRPFuture>& append(CRPFuture&& future);

		public:
			CRPDeferredCall* getCall() const;
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	private:
		CRPDelay(const QSharedPointer<CRPDelay::Vector>& trigger, CRPFuture&& future);

	public:
		CRPDelay(const QSharedPointer<CRPDeferredCall>& call, CRPFuture&& future);
		CRPDelay(const QSharedPointer<CRPDeferredCall>& call, const QString& interface, const QString& method, const QJsonObject& payload, qint64 handle = 0);
		CRPDelay(const QSharedPointer<CRPDeferredCall>& call, const QString& method, const QJsonObject& payload, qint64 handle = 0);
		CRPDelay(CRPDelay&& origin);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QSharedPointer<Vector> _vector;
		const QSharedPointer<CRPFuture>& _future;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		CRPDelay&& detach();
		CRPDelay&& detach(std::function<void(const CRPReaction&)>&& callback);

		template <typename I>
		CRPDelay&& detach(I* object, typename CRPInteractor<I>::Method method)
		{
			if (object) {
				_future->induct(new CRPInteractor<I>(object, method));
			}
			return std::move(*this);
		}
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		CRPDelay operator()(CRPFuture&& future);
		CRPDelay operator()(const QString& interface, const QString& method, const QJsonObject& payload, qint64 handle);
		CRPDelay operator()(const QString& method, const QJsonObject& payload, qint64 handle);
		CRPFuture* operator->();
		operator const CRPPromise&() const;
	/** @} */
	};
} } } }

#endif
