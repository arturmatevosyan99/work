#ifndef Cadabra_Queen_Core_Test_Node_Waiter_SignalWaiter_hpp
#define Cadabra_Queen_Core_Test_Node_Waiter_SignalWaiter_hpp

#include "../AbstractWaiter.hpp"

#include <QJSValue>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class SignalWaiter : public AbstractWaiter
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QJSValue target READ getTarget WRITE setTarget NOTIFY targetChanged)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SignalWaiter(QObject* parent = nullptr);
		virtual ~SignalWaiter() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QJSValue _self;
		QJSValue _slot;
		QJSValue _signal;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QJSValue getTarget();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTarget(QJSValue source_signal);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void targetChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
