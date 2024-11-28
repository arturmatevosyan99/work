#include "ObjectIterator.hpp"

#include "../../Meta/ApplicationRender.hpp"
#include "../../../Core/Meta/Scheme.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	ObjectIterator::ObjectIterator(ObjectMap* parent)
	:
		AbstractIterator(parent)
	{
		_filter_delegate.handle("application", Meta::EssenceExpression::OperandType::LongLong, this, &ObjectIterator::addApplicationFilter);
		_filter_delegate.handle("application", Meta::EssenceExpression::OperandType::String, this, &ObjectIterator::addApplicationFilter);
		_filter_delegate.handle("application_ident", Meta::EssenceExpression::OperandType::LongLong, this, &ObjectIterator::addApplicationFilter);
		_filter_delegate.handle("application_alias", Meta::EssenceExpression::OperandType::String, this, &ObjectIterator::addApplicationFilter);

		_filter_delegate.handle("object", Meta::EssenceExpression::OperandType::LongLong, this, &AbstractIterator::addIdFilter);
		_filter_delegate.handle("object", Meta::EssenceExpression::OperandType::String, this, &ObjectIterator::addObjectFilter);
		_filter_delegate.handle("object_id", Meta::EssenceExpression::OperandType::LongLong, this, &AbstractIterator::addIdFilter);
		_filter_delegate.handle("object_name", Meta::EssenceExpression::OperandType::String, this, &ObjectIterator::addObjectFilter);
		_filter_delegate.handle("object_path", Meta::EssenceExpression::OperandType::String, this, &ObjectIterator::addPathFilter);
		_filter_delegate.handle("object_path", Meta::EssenceExpression::OperandType::LongLong, this, &ObjectIterator::addParentIdFilter);

		_filter_delegate.handle("path", Meta::EssenceExpression::OperandType::String, this, &ObjectIterator::addPathFilter);
		_filter_delegate.handle("path", Meta::EssenceExpression::OperandType::LongLong, this, &ObjectIterator::addParentIdFilter);
		_filter_delegate.handle("parent", Meta::EssenceExpression::OperandType::LongLong, this, &ObjectIterator::addPathFilter);
		_filter_delegate.handle("parent_id", Meta::EssenceExpression::OperandType::LongLong, this, &ObjectIterator::addParentIdFilter);

		_filter_delegate.handle("name", Meta::EssenceExpression::OperandType::String, this, &ObjectIterator::addObjectFilter);
	}

	ObjectIterator::~ObjectIterator()
	{

	}

	bool ObjectIterator::addApplicationFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			return addRoleFilter(Meta::Object::Role::ApplicationSchema, predicate, operator_type);
		} else if (predicate.canConvert<QString>()) {
			QList<Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Iterator> list(Core::Meta::Scheme<Repository::Meta::ApplicationRender>::List(predicate.toString()));
			for (const Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Iterator& i : list) {
				if (!addRoleFilter(Meta::Object::Role::ApplicationSchema, QString::number(i.key()), operator_type)) {
					return false;
				}
			}
			return !list.isEmpty();
		}
		return false;
	}

	bool ObjectIterator::addObjectFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			return addIdFilter(predicate, operator_type);
		}
		return addRoleFilter(Meta::Object::Role::Name, operator_type == Meta::EssenceExpression::OperatorType::Equal ? QString("*%1*").arg(predicate.toString()) : predicate, operator_type);
	}

	bool ObjectIterator::addPathFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			return addRoleFilter(Meta::Object::Role::ParentId, predicate, operator_type);
		}
		/// @todo support /object_name1/object_name2/object_name3
		/// @brief Нужно получать по пути конечный object_id и фильтровать как по parent_id
		/// @brief Имеет смысл только если накладывать весь путь за один раз
		return false;
	}

	bool ObjectIterator::addParentIdFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operator_type)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			return addRoleFilter(Meta::Object::Role::ParentId, predicate, operator_type);
		}
		return false;
	}

	bool ObjectIterator::filterAcceptsRow(const FilterCondition& filter, int source_row, const QModelIndex& source_parent) const
	{
		/// @brief Фильтры общего назначения (Any)
		return true;
	}
} } } }
