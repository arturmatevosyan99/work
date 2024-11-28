#ifndef Cadabra_Queen_Stress_Test_Shocker_AceShocker_hpp
#define Cadabra_Queen_Stress_Test_Shocker_AceShocker_hpp

#include "../../../Stress/Test/Shocker/NetShocker.hpp"

#include "../../Net/Call/SignalCall.hpp"

#include <QPointer>
#include <QAtomicInt>

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	class AceShocker : public NetShocker
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Signal
		{
			OutOfMemory = 0, /// < SIGOOM: Чрезмерное выделение памяти, которое закончится SIGKILL
			Kill = 9, /// < SIGKILL
			SegmentationViolation = 11, /// < SIGSEGV: обращение к памяти через невалидный указатель
			Abort = 6, /// < SIGABRT: вызов abort
			FloatingPointException = 8 /// < SIGFPE: деление на ноль
		};

		enum State
		{
			Idle = 0,
			EndpointMaintenanced = 1,
			StartpointOutdated = 2,
			EndpointOutdated = 3,
			EndpointReached = 4,
			EndpointTimeouted = 5
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Signal)
		Q_ENUM(State)
		Q_PROPERTY(Signal signal MEMBER _signal)
		Q_PROPERTY(State state READ getState NOTIFY stateChanged)
		Q_PROPERTY(int timeout MEMBER _timeout)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AceShocker(QObject* parent = nullptr);
		virtual ~AceShocker() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Signal _signal;
		QPointer<Net::SignalCall> _signal_call;
		QAtomicInt _state_atom;
		int _timeout;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual bool turnOn() override;
		virtual bool turnOff() override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		State getState() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void stateChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
