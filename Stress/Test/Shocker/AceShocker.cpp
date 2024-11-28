#include "AceShocker.hpp"

#include "../../../Skeleton.hpp"

#include <QtConcurrent>
#include <QElapsedTimer>
#include <QScopedPointer>

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	AceShocker::AceShocker(QObject* parent)
	:
		NetShocker(parent),

		_signal(Signal::Kill),
		_state_atom(State::Idle),
		_timeout(-1)
	{

	}

	AceShocker::~AceShocker()
	{
		if (!_signal_call.isNull()) {
			delete _signal_call;
		}
	}

	bool AceShocker::turnOn()
	{
		if (!_endpoint || !_signal_call.isNull() || getState() > State::Idle) {
			return false;
		}

		_signal_call = new Net::SignalCall(_endpoint);
		_signal_call->call(static_cast<int>(_signal));
		_state_atom = State::EndpointMaintenanced;

		return true;
	}

	bool AceShocker::turnOff()
	{
		if (_signal_call.isNull() || !_signal_call->isIdle()) {
			return false;
		}

		QAtomicInt& state_atom(_state_atom);
		QMetaObject::Connection connection_1
		(
			QObject::connect
			(
				_endpoint,
				&Core::Net::Endpoint::outdated,

				[&state_atom](Core::Net::Endpoint::Side side)
				{
					state_atom.storeRelease(State::StartpointOutdated);
					qWarning() << (Core::Net::Endpoint::Side::Client == side ? "Queen is outdated" : "Ace is outdated");
				}
			)
		);
		QMetaObject::Connection connection_2
		(
			QObject::connect
			(
				_endpoint,
				&Core::Net::Endpoint::maintenanced,

				[&state_atom]()
				{
					state_atom.storeRelease(State::EndpointMaintenanced);
					qWarning() << "Ace is under maintenance";
				}
			)
		);
		QMetaObject::Connection connection_3
		(
			QObject::connect
			(
				_endpoint,
				&Core::Net::Endpoint::reached,

				[&state_atom]()
				{
					state_atom.storeRelease(State::EndpointReached);
					qWarning() << "Ace is reached";
				}
			)
		);

		QElapsedTimer timer;
		timer.start();
		qWarning("Waiting for ace...");
		if (_timeout > 0) while(State::EndpointMaintenanced == getState()) {
			_endpoint->reach();
			if (timer.elapsed() > (_timeout * 1000)) {
				QObject::disconnect(connection_1);
				QObject::disconnect(connection_2);
				QObject::disconnect(connection_3);
				qWarning("Timeout has expired after %lld milliseconds, giving up...", timer.elapsed());
				_state_atom = EndpointTimeouted;
				return false;
			}
			Queen::Skeleton::processEvents();
		} else while(State::EndpointMaintenanced == getState()) {
			_endpoint->reach();
			Queen::Skeleton::processEvents();
		}
		QObject::disconnect(connection_1);
		QObject::disconnect(connection_2);
		QObject::disconnect(connection_3);
		qWarning("... waited for %lld milliseconds", timer.elapsed());
		emit stateChanged();
		return State::EndpointReached == getState();
	}

	AceShocker::State AceShocker::getState() const
	{
		return static_cast<State>(_state_atom.loadAcquire());
	}
} } } }
