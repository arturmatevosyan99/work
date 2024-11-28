#ifndef Cadabra_Queen_Core_Test_Node_Waiter_TimeWaiter_hpp
#define Cadabra_Queen_Core_Test_Node_Waiter_TimeWaiter_hpp

#include "../AbstractWaiter.hpp"

#include <QTimerEvent>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class TimeWaiter : public AbstractWaiter
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(int seconds READ getSecoundCount WRITE setSecondCount NOTIFY timerChanged)
		Q_PROPERTY(int milliseconds READ getMillisecondCount WRITE setMillisecondCount NOTIFY timerChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TimeWaiter(QObject* parent = nullptr);
		virtual ~TimeWaiter() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		int _id;
		int _interval;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		int getSecoundCount();
		int getMillisecondCount();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setSecondCount(int value);
		void setMillisecondCount(int value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void timerChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onSet(bool onoff);
	/** @} */

	/** @name Slots */
	/** @{ */
	protected:
		virtual void timerEvent(QTimerEvent* event) override;
	/** @} */
	};
} } } }

#endif
