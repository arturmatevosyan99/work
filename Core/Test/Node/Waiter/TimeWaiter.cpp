#include "TimeWaiter.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	TimeWaiter::TimeWaiter(QObject* parent)
	:
		AbstractWaiter(parent),

		_id(0),
		_interval(0)
	{
		QObject::connect(this, &AbstractWaiter::set, this, &TimeWaiter::onSet);
	}

	int TimeWaiter::getSecoundCount()
	{
		return _interval / 1000;
	}

	int TimeWaiter::getMillisecondCount()
	{
		return _interval;
	}

	void TimeWaiter::setSecondCount(int value)
	{
		_interval = value * 1000;
		emit timerChanged();
	}

	void TimeWaiter::setMillisecondCount(int value)
	{
		_interval = value;
		emit timerChanged();
	}

	void TimeWaiter::onSet(bool onoff)
	{
		if (_id > 0) {
			QObject::killTimer(_id);
			_id = 0;
		}

		if (onoff) {
			_id = QObject::startTimer(_interval);
		}
	}

	void TimeWaiter::timerEvent(QTimerEvent* event)
	{
		if (event->timerId() == _id) {
			_id = 0;
			emit done();
		}
	}
} } } }
