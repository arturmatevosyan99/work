#include "SignalWaiter.hpp"

#include <QJSEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	SignalWaiter::SignalWaiter(QObject* parent)
	:
		AbstractWaiter(parent)
	{

	}

	QJSValue SignalWaiter::getTarget()
	{
		return _signal;
	}

	void SignalWaiter::setTarget(QJSValue source_signal)
	{
		if (!parent() || !source_signal.isCallable() || !source_signal.isObject() || !source_signal.hasProperty("connect")) {
			return ;
		} else if (!_signal.isNull() && _signal.isObject() && _signal.hasProperty("disconnect")) {
			QJSValue disconnect_method(_signal.property("disconnect"));
			disconnect_method.callWithInstance(_signal, {_slot});
		} else if (_self.isUndefined() || _slot.isUndefined()) {
			_self = qjsEngine(parent())->newQObject(qobject_cast<AbstractWaiter*>(this));
			_slot = _self.property("done");
		}

		_signal = source_signal;

		QJSValue connect_method(source_signal.property("connect"));
		connect_method.callWithInstance(source_signal, {_slot});
		emit targetChanged();
	}
} } } }
