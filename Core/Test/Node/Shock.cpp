#include "Shock.hpp"

#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Shock::Shock(const QString& description)
	:
		Action(description),

		_shocker(nullptr)
	{
		QObject::connect(this, &Action::acted, this, &Shock::onActed);
	}

	Shock::~Shock()
	{

	}

	AbstractShocker* Shock::getShocker() const
	{
		return _shocker;
	}

	void Shock::setShocker(AbstractShocker* value)
	{
		if (value != _shocker) {
			_shocker = value;
			emit shockerChanged();
		}
	}

	void Shock::onActed()
	{
		if (!_shocker) {
			qWarning() << "Shocker is missed!";
		} else if (!_shocker->turn(_onoff)) {
			qWarning() << "Shocker could not be turn" << (_onoff ? "on" : "off");
		} else {
			qDebug() << "Shocker was turn" << (_onoff ? "on" : "off");
		}
	}
} } } }
