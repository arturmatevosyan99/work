/**
 @file Filter.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Filter.hpp"

#include <QCoreApplication>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	Filter::Filter(QObject* parent, Condition condition, const QString& role_name, const QVariant& role_value)
	:
		QObject(parent),

		_role_name(role_name),
		_role_value(role_value),
		_condition(condition)
	{

	}

	Filter::Filter(QObject* parent)
	:
		QObject(parent),

		_role_index(-1),
		_condition(Condition::Equal)
	{

	}

	Filter::Filter(const Filter& origin)
	:
		QObject(origin.parent()),

		_role_index(origin._role_index),
		_role_name(origin._role_name),
		_role_value(origin._role_value),
		_condition(origin._condition)
	{
	}

	bool Filter::operator==(const QVariant& rhs) const
	{
		switch (_condition) {
			case Equal: return _role_value == rhs;
			case NotEqual: return _role_value != rhs;
			case Less: return _role_value > rhs;
			case LessOrEqual: return _role_value <= rhs;
			case Greater: return _role_value < rhs;
			case GreaterOrEqual: return _role_value >= rhs;
		}
		return _condition;
	}

	bool Filter::operator!=(const QVariant& rhs) const
	{
		return !(*this == rhs);
	}

	const QString& Filter::getRoleName() const
	{
		return _role_name;
	}

	const QVariant& Filter::getRoleValue() const
	{
		return _role_value;
	}

	const int& Filter::getRoleIndex() const
	{
		return _role_index;
	}

	const Filter::Condition& Filter::getCondition() const
	{
		return _condition;
	}

	void Filter::setRoleName(const QString& name)
	{
		if (name != _role_name) {
			_role_name = name;
			_role_index = -1;
			emit roleNameChanged();
		}
	}

	void Filter::setRoleValue(const QVariant& value)
	{
		if (value != _role_value) {
			_role_value = value;
			emit roleValueChanged();
		}
	}

	void Filter::setCondition(const Condition& condition)
	{
		if (condition != _condition) {
			_condition = condition;
			emit conditionChanged();
		}
	}

	void Filter::classBegin()
	{

	}

	void Filter::componentComplete()
	{
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::HighEventPriority);
	}
} } } }
