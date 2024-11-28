#include "CustomRiddle.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	CustomRiddle::CustomRiddle(const QString& text, const QString& descriptor)
	:
		AbstractRiddle(text, descriptor)
	{

	}

	QJsonObject CustomRiddle::exportBody() const
	{
		return QJsonObject();
	}

	bool CustomRiddle::importBody(const QJsonObject& body)
	{
		return true;
	}

	QVariant CustomRiddle::exportAnswer() const
	{
		return QVariant();
	}

	bool CustomRiddle::importAnswer(const QVariant& answer)
	{
		return true;
	}

	QVariant CustomRiddle::makeDefault() const
	{
		return QVariant();
	}

	bool CustomRiddle::startBusyness(int length)
	{
		if (length <= 0 || isBusynessInProcess()) {
			return false;
		}

		_busyness_plan.fill(true, length);
		AbstractRiddle::setBusy(true);

		return true;
	}

	bool CustomRiddle::completeBusynessPhase(int phase_number)
	{
		if (phase_number < 0 || _busyness_plan.count() <= phase_number) {
			return false;
		}

		_busyness_plan.setBit(phase_number, false);
		AbstractRiddle::setBusy(isBusynessInProcess());

		return true;
	}

	void CustomRiddle::stopBusyness()
	{
		_busyness_plan.clear();
		AbstractRiddle::setBusy(false);
	}

	QString CustomRiddle::getType() const
	{
		return QString("Custom");
	}

	void CustomRiddle::setBusy(bool onoff)
	{
		if (onoff) {
			startBusyness();
		} else {
			stopBusyness();
		}
	}

	bool CustomRiddle::isBusy() const
	{
		return _busyness_plan.isNull() ? AbstractRiddle::isBusy() : isBusynessInProcess();
	}

	bool CustomRiddle::isBusynessInProcess() const
	{
		return _busyness_plan.count(true) > 0;
	}
} } } }
