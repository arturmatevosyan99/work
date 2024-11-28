#include "CRPPromise.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CRPPromise::CRPPromise()
	:
		_datetime(QDateTime::currentDateTime()),
		_state(new QAtomicInteger<quint8>(0))
	{

	}

	CRPPromise::CRPPromise(CRPPromise&& origin)
	:
		_datetime(std::move(origin._datetime)),
		_state(std::move(origin._state))
	{

	}

	CRPPromise::CRPPromise(const CRPPromise& origin)
	:
		_datetime(origin._datetime),
		_state(origin._state)
	{

	}

	CRPPromise::~CRPPromise()
	{

	}

	bool CRPPromise::resolve()
	{
		return !_state ? false : _state->testAndSetRelaxed(0, 1);
	}

	bool CRPPromise::resolve(bool onoff)
	{
		return !_state ? false : _state->testAndSetRelaxed(1, onoff ? 2 : 3) || _state->testAndSetRelaxed(0, onoff ? 2 : 3);
	}

	CRPPromise& CRPPromise::operator=(CRPPromise&& origin)
	{
		_datetime = origin._datetime;
		_state = origin._state;
		return *this;
	}

	CRPPromise& CRPPromise::operator=(const CRPPromise& origin)
	{
		_datetime = std::move(origin._datetime);
		_state = std::move(origin._state);
		return *this;
	}

	CRPPromise::State CRPPromise::fetch() const
	{
		const quint8 retval(!_state ? 0 : _state->loadRelaxed());
		return (retval < 3) ? static_cast<State>(retval) : State::Fault;
	}

	int CRPPromise::duration() const
	{
		return fetch() > State::Process ? (QDateTime::currentDateTime().toSecsSinceEpoch() - _datetime.toSecsSinceEpoch()) : -1;
	}

	bool CRPPromise::isIdle() const
	{
		return fetch() == State::Idle;
	}

	bool CRPPromise::isBusy() const
	{
		return fetch() == State::Process;
	}

	bool CRPPromise::isFinished() const
	{
		const State retval(fetch());
		return State::Success == retval || State::Fault == retval;
	}

	bool CRPPromise::isSuccessful() const
	{
		return fetch() == State::Success;
	}

	bool CRPPromise::isFaulty() const
	{
		return fetch() == State::Fault;
	}
} } } }
