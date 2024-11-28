#include "Assert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Assert::Assert(Action* target_action, const QString& description)
	:
		QObject(nullptr)
	{
		_target_action = target_action;
		_description = description;
	}

	Assert::~Assert()
	{

	}

	void Assert::beginRun()
	{
		QMetaObject::invokeMethod(this, "actionPrepared", Qt::BlockingQueuedConnection);
	}

	void Assert::endRun()
	{
		QMetaObject::invokeMethod(this, "actionCompleted", Qt::BlockingQueuedConnection);
	}

	Action* Assert::getTargetAction() const
	{
		return _target_action;
	}

	const QString& Assert::getDescription() const
	{
		return _description;
	}

	void Assert::setTargetAction(Action* target_action)
	{
		_target_action = target_action;
		emit targetActionChanged();
	}

	void Assert::setDescription(const QString& description)
	{
		if (description != _description) {
			_description = description;
			emit descriptionChanged();
		}
	}
} } } }
