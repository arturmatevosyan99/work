#include "AbstractShocker.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	AbstractShocker::AbstractShocker(QObject* parent)
	:
		QObject(parent)
	{
	}

	bool AbstractShocker::turn(bool onoff)
	{
		if (onoff) {
			if (!_onoff.tryLock()) {
				return false;
			} else if (!turnOn()) {
				_onoff.unlock();
				return false;
			}
			return true;
		} else if (!turnOff()) {
			return false;
		}
		_onoff.unlock();
		return true;
	}
} } } }
