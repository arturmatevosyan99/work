#include "Object.hpp"

#include "../Tag/ObjectTag.hpp"

#include "../../../Meta/Object.hpp"
#include "../../../Meta/ApplicationRender.hpp"
#include "../../../../Core/Meta/Duration.hpp"
#include "../../../../Core/Meta/Scheme.hpp"
#include "../../../../Core/Data/Predicate/LikePredicate.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Object::Object()
	:
		AbstractRecord("Object"),

		parent_id("parent_id", Field::Type::Integer, true),
		application_scheme("application_scheme", Field::Type::Integer, true),
		name("name", Field::Type::Text, true),
		favourite("favourite", Field::Type::Boolean, QVariant::fromValue(false)),
		localization("localization", Field::Type::Boolean, QVariant::fromValue(false)),
		picture("picture", Field::Type::Text, true),
		permission("permission", Field::Type::Integer, true),
		creation_timestamp("creation_timestamp", Field::Type::DateTime, true),
		modification_timestamp("modification_timestamp", Field::Type::DateTime, true),
		access_timestamp("access_timestamp", Field::Type::DateTime, true),
		meta("meta", Field::Type::Document, true),

		_application_compound(new Core::Data::CompoundPredicate(Core::Data::CompoundPredicate::Logic::Or)),
		_parent_id_compound(new Core::Data::CompoundPredicate(Core::Data::CompoundPredicate::Logic::Or)),
		_timeframe_compound(new Core::Data::CompoundPredicate(Core::Data::CompoundPredicate::Logic::And))
	{
		addField(parent_id);
		addField(application_scheme);
		addField(name);
		addField(favourite);
		addField(localization);
		addField(picture);
		addField(permission);
		addField(creation_timestamp);
		addField(modification_timestamp);
		addField(access_timestamp);
		addField(meta);

		_filter_delegate.handle("application", Meta::EssenceExpression::OperandType::String, this, &Object::addApplicationPredicate);
		_filter_delegate.handle("application", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addApplicationPredicate);
		_filter_delegate.handle("application_alias", Meta::EssenceExpression::OperandType::String, this, &Object::addApplicationPredicate);
		_filter_delegate.handle("application_ident", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addApplicationPredicate);

		_filter_delegate.handle("object", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addIdentPredicate);
		_filter_delegate.handle("object", Meta::EssenceExpression::OperandType::String, this, &Object::addIdentPredicate);
		_filter_delegate.handle("object_id", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addIdentPredicate);
		_filter_delegate.handle("object_name", Meta::EssenceExpression::OperandType::String, this, &Object::addIdentPredicate);

		_filter_delegate.handle("object_path", Meta::EssenceExpression::OperandType::String, this, &Object::addPathPredicate);
		_filter_delegate.handle("object_path", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addParentIdPredicate);

		_filter_delegate.handle("path", Meta::EssenceExpression::OperandType::String, this, &Object::addPathPredicate);
		_filter_delegate.handle("path", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addParentIdPredicate);
		_filter_delegate.handle("parent", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addPathPredicate);
		_filter_delegate.handle("parent_id", Meta::EssenceExpression::OperandType::LongLong, this, &Object::addParentIdPredicate);

		_filter_delegate.handle("date", Meta::EssenceExpression::OperandType::String, this, &Object::addTimeframePredicate);
		_filter_delegate.handle("timeframe", Meta::EssenceExpression::OperandType::String, this, &Object::addTimeframePredicate);
		_filter_delegate.handle("name", Meta::EssenceExpression::OperandType::String, this, &Object::addIdentPredicate);

/*
		_filter_delegate.handle("tag", Meta::EssenceExpression::OperandType::String, &Object::addTagFilter);
		_filter_delegate.handle("tag", Meta::EssenceExpression::OperandType::LongLong, &Object::addTagFilter);
		_filter_delegate.handle("tag_alias", Meta::EssenceExpression::OperandType::String, &Object::addTagFilter);
		_filter_delegate.handle("tag_id", Meta::EssenceExpression::OperandType::LongLong, &Object::addTagFilter);
*/
	}

	bool Object::order(int role, Qt::SortOrder order)
	{
		switch (role) {
			case Meta::Object::Role::Name:
				AbstractEntity::addOrder(name, order);
				return true;

			case Meta::Object::Role::ParentId:
				AbstractEntity::addOrder(parent_id, order);
				return true;

			case Meta::Object::Role::ModificationDateTime:
				AbstractEntity::addOrder(modification_timestamp, order);
				return true;

			case Meta::Object::Role::AccessDateTime:
				AbstractEntity::addOrder(modification_timestamp, order);
				return true;

			case Meta::Object::Role::CreationDateTime:
				AbstractEntity::addOrder(creation_timestamp, order);
				return true;

			case Meta::Object::Role::Favourite:
				AbstractEntity::addOrder(favourite, order);
				return true;
		}
		return AbstractRecord::order(role, order);
	}

	void Object::clear()
	{
		_application_compound->clear();
		_parent_id_compound->clear();
		_timeframe_compound->clear();

		AbstractRecord::clear();
	}

	QSharedPointer<AbstractTag> Object::makeTag() const
	{
		return QSharedPointer<ObjectTag>::create();
	}

	bool Object::addIdentPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			_id_compound->append(
									QSharedPointer<Core::Data::ConditionPredicate>::create(
																							id,
																							Core::Data::Field(Field::Type::Integer, predicate),
																							ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::Is)
									)
			);
		} else {
			QString pattern(predicate.toString().replace("*", "%"));
			if (pattern.isEmpty()) {
				return false;
			} else if (!pattern.contains("%")) {
				pattern.append("%").prepend("%");
			}
			_id_compound->append(
									QSharedPointer<Core::Data::LikePredicate>::create(
																						name,
																						pattern,
																						Meta::EssenceExpression::OperatorType::Equal != operand
									)
			);
		}
		return true;
	}

	bool Object::addPathPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			_parent_id_compound->append(
										QSharedPointer<Core::Data::ConditionPredicate>::create(
																								 parent_id,
																								 Core::Data::Field(Field::Type::Integer, predicate),
																								 ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::Equal)
										)
			);
			return true;
		}
		/// @todo support /object_name1/object_name2/object_name3
		/// @brief Нужно получать по пути конечный object_id и фильтровать как по parent_id
		/// @brief Имеет смысл только если накладывать весь путь за один раз
		return false;
	}

	bool Object::addParentIdPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (!Core::Meta::Qd::IsValid(predicate)) {
			return false;
		}
		_parent_id_compound->append(
									QSharedPointer<Core::Data::ConditionPredicate>::create(
																							 parent_id,
																							 Core::Data::Field(Field::Type::Integer, predicate),
																							 ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::Equal)
									)
		);
		return true;
	}

	bool Object::addApplicationPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		QList<Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Iterator> list;
		if (static_cast<QMetaType::Type>(predicate.type()) == QMetaType::LongLong) {
			list.push_back(Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Iterate(Core::Meta::Qd(predicate)));
		} else if (predicate.canConvert<QString>()) {
			list = Core::Meta::Scheme<Repository::Meta::ApplicationRender>::List(predicate.toString());
		}

		for (const Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Iterator& i : qAsConst(list)) {
			if (Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Exists(i)) {
				_application_compound->append(
												QSharedPointer<Core::Data::ConditionPredicate>::create(
																										application_scheme,
																										Core::Data::Field(Field::Type::Integer, i.key()),
																										ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::Is)
												)
				);
			}
		}
		return true;
	}

	bool Object::addTimeframePredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (!predicate.canConvert<QString>()) {
			return false;
		}

		const QString value(predicate.toString());
		if (value.count("-") > 1) { /// < date('1980-12-01')
			QDateTime date_time(QDateTime::fromString(value, Qt::DateFormat::ISODate));
			if (!date_time.isValid()) {
				return false;
			}
			_timeframe_compound->append(
										QSharedPointer<Core::Data::ConditionPredicate>::create(
																								modification_timestamp,
																								Core::Data::Field(Field::Type::DateTime, QVariant(date_time)),
																								ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::GreaterOrEqual)
										)
			);
			return true;
		} else { /// < date('now','start of month','+1 month','-1 day')
			Core::Meta::Duration duration(predicate);
			if (duration.isZero()) {
				return false;
			}
			_timeframe_compound->append(
										QSharedPointer<Core::Data::ConditionPredicate>::create(
																								modification_timestamp,
																								Core::Data::Field(Field::Type::Duration, QVariant::fromValue(duration)),
																								ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::GreaterOrEqual)
										)
			);
			return true;
		}
		return false;
	}

	void Object::onSelect()
	{
		if (!_application_compound->isEmpty()) {
			addPredicate(_application_compound);
		}
		if (!_parent_id_compound->isEmpty()) {
			addPredicate(_parent_id_compound);
		}
		if (!_timeframe_compound->isEmpty()) {
			addPredicate(_timeframe_compound);
		}
		AbstractRecord::onSelect();
	}

	void Object::onCreate()
	{
		AbstractRecord::onCreate();
	}

	void Object::onForeign()
	{
		AbstractRecord::onForeign();
	}
} } } }
