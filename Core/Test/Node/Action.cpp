#include "Action.hpp"

#include <QDebug>
#include <QMutexLocker>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QJSEngine>
#include <QJSValueIterator>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	AbstractWaiter* Action::Item(QQmlListProperty<AbstractWaiter>* list, int index)
	{
		Action* action(reinterpret_cast<Action*>(list->object));
		return action && action->_wait_list.length() > index ? action->_wait_list.at(index) : nullptr;
	}

	int Action::Size(QQmlListProperty<AbstractWaiter>* list)
	{
		Action* action(reinterpret_cast<Action*>(list->object));
		return action ? action->_wait_list.length() : 0;
	}

	void Action::Clear(QQmlListProperty<AbstractWaiter>* list)
	{
		Action* action(reinterpret_cast<Action*>(list->object));
		if (action) {
			qDeleteAll(action->_wait_list);
			action->_wait_list.clear();
		}
	}

	void Action::Append(QQmlListProperty<AbstractWaiter>* list, AbstractWaiter* item)
	{
		Action* action(reinterpret_cast<Action*>(list->object));
		if (action) {
			action->_wait_list.append(item);
			if (!QObject::connect(action->_wait_list.last(), &AbstractWaiter::done, action, &Action::awake, Qt::DirectConnection)) {
				qWarning("Action: Unable to make the connection with waiter");
			}
		}
	}

	Action::Action(const QString& description)
	:
		Node(QString("Action &%1").arg(reinterpret_cast<quintptr>(this)), description),

		_wait_timeout(0),
		_condition(true)
	{

	}

	Action::~Action()
	{

	}

	void Action::run()
	{
		if (!_wait_atom.testAndSetRelaxed(false, true)) {
			/// @log
			return ;
		}

		QMutexLocker locker(&_wait_mutex);
		for (AbstractWaiter*& waiter : _wait_list) {
			emit waiter->set(true);
		}
		QMetaObject::invokeMethod(this, "acted", Qt::BlockingQueuedConnection);
		if (_wait_atom.loadRelaxed() && !_wait_list.isEmpty()) {
			bool deadline(false);
			if (_wait_timeout > 0) {
				deadline = _wait_codition.wait(&_wait_mutex, _wait_timeout);
			} else {
				deadline = _wait_codition.wait(&_wait_mutex, QDeadlineTimer(QDeadlineTimer::Forever));
			}
			qDebug() << (deadline ? "At long last a wake-up from" : "No wake-up from") << &_wait_codition;
		} else if (!_wait_list.isEmpty()) {
			qDebug() << "Meanwhile everything was accomplished by and no reason to wait a wake-up from" << &_wait_codition;
		}
		for (AbstractWaiter*& waiter : _wait_list) {
			emit waiter->set(false);
		}
		_wait_atom.storeRelaxed(false);
	}

	QQmlListProperty<AbstractWaiter> Action::makeWaitPropertyList()
	{
		return QQmlListProperty<AbstractWaiter>(this, nullptr, &Action::Append, &Action::Size, &Action::Item, &Action::Clear);
	}

	quint32 Action::getWaitTimeout() const
	{
		return _wait_timeout;
	}

	const QVector<Assertion*>& Action::getAssertionVector() const
	{
		return _assertion_vector;
	}

	const QVector<Assertion*>& Action::getInputAssertionVector() const
	{
		return _input_assertion_vector;
	}

	bool Action::getCondition() const
	{
		return _condition;
	}

	void Action::setWaitTimeout(quint32 value)
	{
		if (value >= 0 && value != _wait_timeout) {
			_wait_timeout = value;
			emit waitTimeoutChanged();
		}
	}

	void Action::addAssertion(Assertion* assertion)
	{
		_assertion_vector.append(assertion);
		emit assertionVectorChanged();
	}

	void Action::addInputAssertion(Assertion* input_assertion)
	{
		_input_assertion_vector.append(input_assertion);
	}

	void Action::setAssertionVector(const QVector<Assertion*>& value)
	{
		_assertion_vector = value;
		emit assertionVectorChanged();
	}

	void Action::setInputAssertionVector(const QVector<Assertion*>& value)
	{
		_input_assertion_vector = value;
	}

	void Action::setCondition(bool condition)
	{
		if (_condition != condition) {
			_condition = condition;
			emit conditionChanged();
		}
	}

	void Action::awake()
	{
		_wait_atom.storeRelaxed(false);
		qDebug() << "Caught signal from a waiter, waking up" << &_wait_codition << "...";
		_wait_codition.wakeAll();
	}
} } } }
