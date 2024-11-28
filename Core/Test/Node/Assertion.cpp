#include "Assertion.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Assertion::Assertion(const QString& description)
	:
		Node(QString("Assertion &%1").arg(reinterpret_cast<quintptr>(this)), description),

		_assert_property_list(this)
	{

	}

	Assertion::~Assertion()
	{

	}

	QQmlListProperty<Assert> Assertion::makeAssertPropertyList()
	{
		return _assert_property_list;
	}

	const QVector<Action*>& Assertion::getActionVector() const
	{
		return _action_vector;
	}

	const Core::Meta::TemplatedPropertyList<Assert>& Assertion::getAssertPropertyList() const
	{
		return _assert_property_list;
	}

	void Assertion::addAction(Action* action)
	{
		_action_vector.append(action);
		emit actionVectorChanged();
	}

	void Assertion::addAssert(Assert* assert)
	{
		_assert_property_list.push_back(assert);
	}

	void Assertion::setActionVector(const QVector<Action*>& actions)
	{
		_action_vector = actions;

		for (int i = 0; i < actions.length(); i++) {
			actions.at(i)->addInputAssertion(this);
		}

		emit actionVectorChanged();
	}
} } } }
