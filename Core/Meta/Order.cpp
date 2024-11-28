/**
 @file Order.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Order.hpp"

#include <QCoreApplication>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	Order::Order(QObject* parent)
	:
		QObject(parent),

		_role_index(-1),
		_role_order(Qt::SortOrder::AscendingOrder)
	{

	}

	Order::Order(QObject* parent, const QString& name, Qt::SortOrder order)
	:
		QObject(parent),

		_role_index(-1),
		_role_name(name),
		_role_order(order)
	{

	}

	Order::Order(const Order& origin)
	:
		QObject(origin.parent()),

		_role_index(origin._role_index),
		_role_name(origin._role_name),
		_role_order(origin._role_order)
	{
	}

	const QString& Order::getRoleName() const
	{
		return _role_name;
	}

	const Qt::SortOrder& Order::getRoleOrder() const
	{
		return _role_order;
	}

	const int& Order::getRoleIndex() const
	{
		return _role_index;
	}

	void Order::setRoleName(const QString& name)
	{
		if (name != _role_name) {
			_role_name = name;
			_role_index = -1;
			emit roleNameChanged();
		}
	}

	void Order::setRoleOrder(Qt::SortOrder order)
	{
		if (order != _role_order) {
			_role_order = order;
			emit roleOrderChanged();
		}
	}

	void Order::classBegin()
	{

	}

	void Order::componentComplete()
	{
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::HighEventPriority);
	}
} } } }
