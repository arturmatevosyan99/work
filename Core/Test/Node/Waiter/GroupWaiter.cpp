#include "GroupWaiter.hpp"

#include <QMutexLocker>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	AbstractWaiter* GroupWaiter::Item(QQmlListProperty<AbstractWaiter>* list, int index)
	{
		GroupWaiter* group(reinterpret_cast<GroupWaiter*>(list->object));
		return group && group->_waiter_list.length() > index ? group->_waiter_list.at(index) : nullptr;
	}

	int GroupWaiter::Size(QQmlListProperty<AbstractWaiter>* list)
	{
		GroupWaiter* group(reinterpret_cast<GroupWaiter*>(list->object));
		return group ? group->_waiter_list.length() : 0;
	}

	void GroupWaiter::Clear(QQmlListProperty<AbstractWaiter>* list)
	{
		GroupWaiter* group(reinterpret_cast<GroupWaiter*>(list->object));
		if (group) {
			qDeleteAll(group->_waiter_list);
			group->_waiter_list.clear();
		}
	}

	void GroupWaiter::Append(QQmlListProperty<AbstractWaiter>* list, AbstractWaiter* item)
	{
		GroupWaiter* group(reinterpret_cast<GroupWaiter*>(list->object));
		if (group) {
			group->_waiter_list.append(item);
			QObject::connect(group->_waiter_list.last(), &AbstractWaiter::done, group, &GroupWaiter::onPartiallyDone);
			QObject::connect(group, &GroupWaiter::set, group->_waiter_list.last(), &AbstractWaiter::set);
		}
	}

	GroupWaiter::GroupWaiter(QObject* parent)
	:
		AbstractWaiter(parent)
	{

	}

	void GroupWaiter::reset(bool signal)
	{
		if (signal) {
			emit done();
		}
		_idle_set.clear();
	}

	QQmlListProperty<AbstractWaiter> GroupWaiter::makePropertyList()
	{
		return QQmlListProperty<AbstractWaiter>
		(
			this,
			nullptr,
			&GroupWaiter::Append,
			&GroupWaiter::Size,
			&GroupWaiter::Item,
			&GroupWaiter::Clear
		);
	}

	void GroupWaiter::onPartiallyDone()
	{
		QMutexLocker locker(&_mutex);
		AbstractWaiter* waiter(qobject_cast<AbstractWaiter*>(sender()));
		if (!waiter) {
			return ;
		}
		_idle_set.insert(waiter);
		if (_idle_set.size() == _waiter_list.length()) {
			emit done();
			_idle_set.clear();
		}
	}
} } } }
