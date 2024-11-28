#include "AbstractRecord.hpp"

#include "AbstractTag.hpp"
#include "Record/Tag.hpp"

#include "../../../Core/Meta/Qd.hpp"
#include "../../../Core/Data/Predicate/LikePredicate.hpp"
#include "../../../Core/Data/Predicate/InEntityPredicate.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	const QMap<Meta::EssenceExpression::OperatorType, Core::Data::ConditionPredicate::Condition> AbstractRecord::ConditionOperatorMap = {
		{Meta::EssenceExpression::OperatorType::Equal, Core::Data::ConditionPredicate::Condition::Equal},
		{Meta::EssenceExpression::OperatorType::Less, Core::Data::ConditionPredicate::Condition::Less},
		{Meta::EssenceExpression::OperatorType::LessOrEqual, Core::Data::ConditionPredicate::Condition::LessOrEqual},
		{Meta::EssenceExpression::OperatorType::Greater, Core::Data::ConditionPredicate::Condition::Greater},
		{Meta::EssenceExpression::OperatorType::GreaterOrEqual, Core::Data::ConditionPredicate::Condition::GreaterOrEqual}
	};

	AbstractRecord::AbstractRecord(const QString& name)
	:
		Core::Data::AbstractEntity("Repository_" + name),

		id("id", Field::Type::Integer, false),
		_live_mode(false),

		_id_compound(new Core::Data::CompoundPredicate(Core::Data::CompoundPredicate::Logic::Or))
	{
		addField(id);

		setPrimaryKey({id});

		_filter_delegate.handle("id", Meta::EssenceExpression::OperandType::LongLong, this, &AbstractRecord::addIdPredicate);

		_filter_delegate.handle("tag", Meta::EssenceExpression::OperandType::String, this, &AbstractRecord::addTagAliasPredicate);
		_filter_delegate.handle("tag", Meta::EssenceExpression::OperandType::LongLong, this, &AbstractRecord::addTagIdPredicate);
	}

	bool AbstractRecord::filter(const Meta::EssenceExpression& expression)
	{
		return _filter_delegate(expression);
	}

	bool AbstractRecord::filter(const QVector<Meta::EssenceToken>& token_vector)
	{
		bool retval(true);
		for (const QVector<Meta::EssenceToken>::value_type& token: token_vector) {
			if (token.active) {
				retval &= filter(token.express());
			}
		}
		return retval;
	}

	bool AbstractRecord::order(int role, Qt::SortOrder order)
	{
		if (Meta::Essence::Role::EssenceId == role) {
			AbstractEntity::addOrder(id, order);
			return true;
		}
		return false;
	}

	bool AbstractRecord::order(const QVector<int>& sort_vector, bool reverse)
	{
		bool retval(true);
		for (const QVector<int>::value_type& role: sort_vector) {
			retval &= order(abs(role), (!reverse ? (role > 0) : (role < 0)) ? Qt::SortOrder::DescendingOrder : Qt::SortOrder::AscendingOrder);
		}
		return retval;
	}

	void AbstractRecord::clear()
	{
		_id_compound->clear();
		if (_bind_tag) {
			_bind_tag->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::Or);
		}
		if (_roaster_tag) {
			_roaster_tag->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::Or);
		}
		resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::And);
		resetOrder();
	}

	void AbstractRecord::setLive(bool onoff)
	{
		_live_mode = onoff;
	}

	QModelIndex AbstractRecord::makeIndex(int row, int column) const
	{
		return createIndex(row, column);
	}

	bool AbstractRecord::addIdPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (!predicate.canConvert<qlonglong>()) {
			return false;
		}
		_id_compound->append(
								QSharedPointer<Core::Data::ConditionPredicate>::create(
																						id,
																						Core::Data::Field(Field::Type::Integer, predicate),
																						ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::Equal)
								)
		);
		return true;
	}

	bool AbstractRecord::addTagAliasPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			return addTagIdPredicate(predicate, operand);
		} else if (!_roaster_tag) {
			_roaster_tag.reset(new Tag());
			_roaster_tag->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::Or);
			_roaster_tag->alias.setGenerated(false);
			_roaster_tag->timestamp.setGenerated(false);
		}
		_roaster_tag->addPredicate(
									QSharedPointer<Core::Data::LikePredicate>::create(
																						_roaster_tag->alias,
																						predicate.toString(),
																						Meta::EssenceExpression::OperatorType::Equal != operand
									)
		);
		return true;
	}

	bool AbstractRecord::addTagIdPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (!predicate.canConvert<qlonglong>()) {
			return false;
		} else if (!_bind_tag) {
			_bind_tag = makeTag();
			_bind_tag->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::Or);
			_bind_tag->tag_id.setGenerated(false);
		}
		_bind_tag->addPredicate(
								QSharedPointer<Core::Data::ConditionPredicate>::create(
																						_bind_tag->tag_id,
																						Core::Data::Field(Field::Type::Integer, predicate.value<qlonglong>()),
																						ConditionOperatorMap.value(operand, Core::Data::ConditionPredicate::Condition::Equal)
								)
		);
		return true;
	}

	void AbstractRecord::onMutated(Meta::EssenceAlterMap map)
	{
		if (!isLiveMode() || map.isEmpty() || !hasRowset()) {
			return ;
		}

		/// @brief Если не очень много измененний, то есть смысл проверить наличие этих элементов в текущем слепке ...
		if (map.count() <= 50) {
			QList<QSqlIndex> index_list;
			for (Meta::EssenceAlterMap::ConstIterator a = map.constBegin(); a != map.constEnd(); ++a) {
				if (a->testFlag(EssenceAlter::InsertAlter)) {
					/// @note Если есть хоть один INSERT, то нельзя однозначно(и дешево) сказать, что он не попал бы в этот срез при тех же условиях фильтрации
					break;
				}
				index_list.append(makeRowsetIndex());
				index_list.last().setValue("id", a.key());
			}
			/// @note ... чтобы вхолостую не переоткрывать
			if (!hasAnyIndex(index_list)) {
				return ;
			}
		}
		reopen();
	}

	bool AbstractRecord::event(QEvent* event)
	{
		switch (event->type()) {
			case QEvent::Type::FocusIn:
				_live_mode = true;
				break;

			case QEvent::Type::FocusOut:
				_live_mode = false;
				break;

			default:
				return Core::Data::AbstractEntity::event(event);
		}
		return true;
	}

	void AbstractRecord::onSelect()
	{
		if (!_id_compound->isEmpty()) {
			addPredicate(_id_compound);
		}

		if (_roaster_tag) {
			if (!_bind_tag) {
				_bind_tag = makeTag();
				_bind_tag->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::Or);
				_bind_tag->tag_id.setGenerated(false);
			}
			_bind_tag->addPredicate(QSharedPointer<Core::Data::InEntityPredicate>::create(_bind_tag->tag_id, _roaster_tag->id));
		}
		if (_bind_tag && !_bind_tag->getCompoundPredicate().isEmpty()) {
			addPredicate(QSharedPointer<Core::Data::InEntityPredicate>::create(id, _bind_tag->target_id));
		}
	}

	void AbstractRecord::onCreate()
	{
		AbstractEntity::onCreate();
	}

	void AbstractRecord::onForeign()
	{
		AbstractEntity::onForeign();
	}

	bool AbstractRecord::isLiveMode() const
	{
		return _live_mode;
	}
} } } }
