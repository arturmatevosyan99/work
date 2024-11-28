/**
 @file CRPFuture.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Call_CRPFuture_hpp
#define Cadabra_Queen_Core_Net_Call_CRPFuture_hpp

#include "CRPPromise.hpp"
#include "CRPInterface.hpp"
#include "CRPReactor.hpp"
#include "CRPReaction.hpp"

#include <QJsonObject>
#include <QJsonValue>
#include <functional>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CRPFuture
	{
	friend class CRPDeferredCall;

	/** @name Constructors */
	/** @{ */
	public:
		CRPFuture();
		CRPFuture(const QString& interface, const QString& method, const QJsonObject& payload, qint64 handle = 0);
		CRPFuture(const QString& method, const QJsonObject& payload, qint64 handle = 0);
		CRPFuture(CRPFuture&& origin);
		~CRPFuture();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		CRPPromise _promise;
		const QJsonObject _action;
		CRPReactor* _reactor;
		std::function<void(const CRPReaction&)> _handler_callback;
		std::function<void(QJsonValue)> _reaction_callback;
		std::function<void(quint8, quint32, QString)> _exception_callback;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		bool progress();

	public:
		CRPFuture& happen(QJsonValue payload);
		CRPFuture& happen(quint8 domain, quint32 code, QString message);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const CRPPromise& getPromise() const;
		const QJsonObject& getAction() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		bool induct(CRPReactor* reactor);

		CRPFuture& react(std::function<void(const CRPReaction&)>&& callback);
		CRPFuture& handle(std::function<void(QJsonValue)>&& callback);
		CRPFuture& except(std::function<void(quint8, quint32, QString)>&& callback);
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator const CRPPromise&() const;
	/** @} */
	};
} } } }

#endif
