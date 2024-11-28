#include "TestContext.hpp"

#include "../../Skeleton.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace System {
	TestContext::TestContext(QObject* parent)
	:
		QObject(parent),

		_qml_context(_qml_engine.rootContext()),
		_current_case(nullptr)
	{
		QObject::startTimer(16);

		_qml_engine.moveToThread(thread());
		_qml_context.moveToThread(thread());
		_qml_context.setContextObject(this);
		_qml_engine.setIncubationController(&_qml_incubation_controller);

		QObject::connect(&_qml_engine, &QQmlEngine::quit, this, &TestContext::engineQuited);
		QObject::connect(&_qml_engine, &QQmlEngine::exit, this, &TestContext::engineExited);
		QObject::connect(&_qml_engine, &QQmlEngine::warnings, this, &TestContext::engineWarned);
	}

	TestContext::~TestContext()
	{

	}

	void TestContext::clear()
	{
		if (_qml_engine.networkAccessManager()) {
			_qml_engine.networkAccessManager()->clearConnectionCache();
			if (_qml_engine.networkAccessManager()->cache()) {
				_qml_engine.networkAccessManager()->cache()->clear();
			}
		}
		_qml_engine.collectGarbage();
		_qml_engine.clearComponentCache();
	}

	bool TestContext::interrupt(bool onoff)
	{
		if (_qml_engine.isInterrupted() != onoff) {
			_qml_engine.setInterrupted(onoff);
			return true;
		}
		return false;
	}

	QVariant TestContext::getPreferenceOption(const QString& key, const QVariant& default_value) const
	{
		return Core::System::Preference::GlobalInstance().getOption(key, default_value);
	}

	QVariant TestContext::getEnvironmentVariable(const QString& key, const QVariant& default_value) const
	{
		return Environment::GlobalInstance().getVariable(key, default_value);
	}

	Core::Test::AbstractCase* TestContext::getCurrentCase() const
	{
		return _current_case;
	}

	void TestContext::put(Core::Test::AbstractCase* value)
	{
		if (_current_case == value) {
			return ;
		} else if (value) {
			QQmlEngine::setContextForObject(value, &_qml_context);
		}

		_current_case = value;
		emit currentCaseChanged();
	}

	void TestContext::timerEvent(QTimerEvent*)
	{
		_qml_incubation_controller.incubateFor(5);
	}
} } } }
