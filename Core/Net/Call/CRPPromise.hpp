/**
 @file CRPPromise.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Call_CRPPromise_hpp
#define Cadabra_Queen_Core_Net_Call_CRPPromise_hpp

#include <QDateTime>
#include <QSharedPointer>
#include <QAtomicInteger>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	/**
	 * @brief Promise of CRP result
	 * @todo Хорошо бы сам результат пробросить, а не только признак успеха
	 */
	class CRPPromise
	{
	friend class CRPCall;
	/** @name Classes */
	/** @{ */
	public:
		enum class State : quint8
		{
			Idle = 0,
			Process = 1,
			Success = 2,
			Fault = 3
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_ENUM(State)
		Q_PROPERTY(bool isIdle READ isIdle)
		Q_PROPERTY(bool isBusy READ isBusy)
		Q_PROPERTY(bool isFinished READ isFinished)
		Q_PROPERTY(bool isSuccessful READ isSuccessful)
		Q_PROPERTY(bool isFaulty READ isFaulty)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CRPPromise();
		CRPPromise(CRPPromise&& origin);
		CRPPromise(const CRPPromise& origin);
		~CRPPromise();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QDateTime _datetime;
		QSharedPointer<QAtomicInteger<quint8>> _state;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool resolve();
		bool resolve(bool onoff);
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		CRPPromise& operator=(CRPPromise&& origin);
		CRPPromise& operator=(const CRPPromise& origin);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Q_INVOKABLE Cadabra::Queen::Core::Net::CRPPromise::State fetch() const;
		Q_INVOKABLE int duration() const;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isIdle() const;
		bool isBusy() const;
		bool isFinished() const;
		bool isSuccessful() const;
		bool isFaulty() const;
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Core::Net::CRPPromise)

#endif
