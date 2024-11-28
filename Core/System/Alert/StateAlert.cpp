/**
 @file StateAlert.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "StateAlert.hpp"

#include "../Alert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	StateAlert::StateAlert(QObject* parent)
	:
		AbstractAlert(parent),

		_source(nullptr),
		_timer_id(0),
		_timer_interval(0)
	{

	}

	StateAlert::StateAlert(QObject* parent, const QString& name, const QString& title, const QString& description, QObject* source, AbstractAlert::Level level)
	:
		AbstractAlert(parent, name, title, level),

		_description(description),
		_source(source)
	{

	}

	bool StateAlert::warn()
	{
		return Alert::Instance()->enqueue(this);
	}

	bool StateAlert::discard()
	{
		return Alert::Instance()->dequeue(this);
	}

	void StateAlert::stopTimer()
	{
		if (_timer_id > 0) {
			killTimer(_timer_id);
			_timer_id = 0;
		}
	}

	void StateAlert::startTimer(int value)
	{
		if (_timer_interval < 1) {
			stopTimer();
			_timer_interval = 0;
		} else if (_timer_id < 1) {
			_timer_interval = value;
			_timer_id = QObject::startTimer(_timer_interval);
		}
	}

	void StateAlert::progressTimer(int value)
	{
		_timer_interval += value;
		if (_timer_interval < 1) {
			stopTimer();
		} else if (_timer_id < 1) {
			_timer_id = QObject::startTimer(_timer_interval);
		}
	}

	const QString& StateAlert::getDescription() const
	{
		return _description;
	}

	QObject* StateAlert::getSource() const
	{
		return _source;
	}

	void StateAlert::setDescription(const QString& value)
	{
		if (value != _description) {
			_description = value;
			emit descriptionChanged();
		}
	}

	void StateAlert::setTimerInterval(int value)
	{
		_timer_interval = value > 0 ? value : 0;
	}

	void StateAlert::timerEvent(QTimerEvent* event)
	{
		if (event->timerId() != _timer_id) {
			return ;
		}
		killTimer(_timer_id);
		_timer_id = 0;
		emit timerExpired();
	}
} } } }
