#include "WaiterCallback.hpp"

#include <QDebug>
#include <QPointer>
#include <QJSEngine>
#include <QJsonValue>
#include <QMetaMethod>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	WaiterCallback::WaiterCallback(QObject* parent)
	:
		QObject(parent)
	{
		QObject::connect(this, &WaiterCallback::done, this, &WaiterCallback::onDone);
	}

	const QString& WaiterCallback::getName() const
	{
		return _name;
	}

	const QJSValue& WaiterCallback::getCallee() const
	{
		return _callee;
	}

	void WaiterCallback::setName(const QString& value)
	{
		if (_name != value) {
			_name = value;
			emit nameChanged();
		}
	}

	void WaiterCallback::setCallee(const QJSValue& callee)
	{
		if (callee.isCallable()) {
			_callee = callee;
			emit calleeChanged();
		}
	}

	void WaiterCallback::setSignalSpy(const QPointer<QObject>& target, const QMetaMethod& signal)
	{
		_signal_parameter_names_list = signal.parameterNames();
		_spy.reset(new WaiterCallback::SignalSpy(target, signal));
	}

	void WaiterCallback::onDone()
	{
		if (_spy && !_spy->isEmpty()) {
			QJSEngine* engine(qjsEngine(parent()));

			if (engine) {
				QJSValue instance(engine->newObject());
				QList<QVariant> first_signal_list(_spy->takeFirst());

				for (int i = 0; i < first_signal_list.length(); i++) {
					instance.setProperty(QString(_signal_parameter_names_list.at(i)), engine->toScriptValue(first_signal_list.at(i)));
				}

				QJSValue result(_callee.callWithInstance(instance));

				if (result.isError()) {
					qWarning() << "An uncaught exception occured at line" << result.property("lineNumber").toInt() << ":" << result.toString();
				}
			}
		}
	}
} } } }
