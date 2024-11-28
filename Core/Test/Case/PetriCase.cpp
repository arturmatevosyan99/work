#include "PetriCase.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	PetrI* PetriCase::Origin(QObject* object)
	{
		Port* port(qobject_cast<Port*>(object));
		return port ? port->getOriginPetrI() : nullptr;
	}

	PetriCase::PetriCase(QObject* parent)
	:
		AbstractCase(parent),

		_petr_i(new PetrI(this, PetriCase::Origin(parent)))
	{
		_petr_i->setAutoFire(!parent);
	}

	PetriCase::~PetriCase()
	{
		if (!_petr_i.isNull()) {
			_petr_i->deleteLater();
		}
	}

	bool PetriCase::run()
	{
		return _petr_i.isNull() ? false : _petr_i->fire();
	}

	bool PetriCase::dial(QQmlIncubator& incubator)
	{
		if (_petr_i) {
			incubator.setInitialProperties(
											{
												{"autoFire", _petr_i->getAutoFire()}
											}
			);
		}
		return true;
	}

	bool PetriCase::dial(const QQmlIncubator& incubator)
	{
		PetrI* origin_petri(nullptr);
		if (!_petr_i.isNull()) {
			if (!_petr_i->isOrigin()) {
				origin_petri = _petr_i->getOrigin();
			}
			_petr_i->deleteLater();
		}
		if ((_petr_i = qobject_cast<PetrI*>(incubator.object())).isNull()) {
			return false;
		}
		_petr_i->resetOrigin(origin_petri);
		QList<Port*>::const_iterator result
		(
			std::find_if_not
			(
				_petr_i->getPortPropertyList().begin(), _petr_i->getPortPropertyList().end(),
				[](Port* port)
				{
					return port->dock();
				}
			)
		);
		return result == _petr_i->getPortPropertyList().end();
	}

	const Context& PetriCase::getContext() const
	{
		return _petr_i.isNull()
								? AbstractCase::getContext()
								: *(_petr_i->getContext())
		;
	}

	Context& PetriCase::getContext()
	{
		return _petr_i.isNull()
								? AbstractCase::getContext()
								: *(_petr_i->getContext())
		;
	}

	const QPointer<PetrI>& PetriCase::getPetrI() const
	{
		return _petr_i;
	}

	void PetriCase::initTestCase()
	{

	}

	void PetriCase::initTestCase_data()
	{

	}

	void PetriCase::cleanupTestCase()
	{

	}

	void PetriCase::init()
	{

	}

	void PetriCase::cleanup()
	{

	}

	bool PetriCase::hasInstance() const
	{
		return !_petr_i.isNull();
	}
} } } }
