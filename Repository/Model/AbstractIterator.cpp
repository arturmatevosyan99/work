#include "AbstractIterator.hpp"

#include <QDateTime>
#include <QDate>
#include <QTime>

#include "../Meta/Object.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AbstractIterator::AbstractIterator(AbstractMap* source_model_map)
	:
		QSortFilterProxyModel(nullptr),

		_source_model_map(source_model_map)
	{
		setDynamicSortFilter(true);
		setSourceModel(source_model_map);

		_filter_delegate.handle("id", Meta::EssenceExpression::OperandType::LongLong, this, &AbstractIterator::addIdFilter);
		_filter_delegate.handle("tag", Meta::EssenceExpression::OperandType::String, this, &AbstractIterator::addTagFilter);
		_filter_delegate.handle("tag", Meta::EssenceExpression::OperandType::LongLong, this, &AbstractIterator::addTagFilter);
	}

	AbstractIterator::~AbstractIterator()
	{

	}

	bool AbstractIterator::filter(const Meta::EssenceExpression& expression)
	{
		return _filter_delegate(expression);
	}

	bool AbstractIterator::filter(const QVector<Meta::EssenceToken>& token_vector)
	{
		bool retval(true);
		for (const QVector<Meta::EssenceToken>::value_type& token: token_vector) {
			if (!token.active) {
				continue;
			} else if (token.type == Meta::EssenceToken::Type::Any) {
				_filter_any_list.push_back(qMakePair(Meta::EssenceExpression::OperatorType::Equal, token));
			} else {
				retval &= filter(token.express());
			}
		}
		return retval;
	}

	bool AbstractIterator::order(int role, Qt::SortOrder order)
	{
		if (!sourceModel() || !sourceModel()->roleNames().contains(role)) {
			return false;
		}

		_sort_list.push_back(qMakePair(role, order));
		return true;
	}

	bool AbstractIterator::order(const QVector<int>& sort_vector, bool reverse)
	{
		bool retval(true);
		for (const QVector<int>::value_type& role: sort_vector) {
			retval &= order(abs(role), (!reverse ? (role > 0) : (role < 0)) ? Qt::SortOrder::DescendingOrder : Qt::SortOrder::AscendingOrder);
		}
		return retval;
	}

	void AbstractIterator::clear()
	{
		_sort_list.clear();
		_filter_role_map.clear();
		_filter_any_list.clear();
		_filter_tag_list.clear();
	}

	bool AbstractIterator::validate()
	{
		if (!sourceModel()) {
			return false;
		}

		emit beginResetModel();
		if (!_sort_list.isEmpty()) {
			setSortCaseSensitivity(Qt::CaseInsensitive);
			setSortRole(_sort_list.front().first);
			sort(0, _sort_list.front().second);
		}
		emit endResetModel();
		return true;
	}

	void AbstractIterator::setSourceModel(QAbstractItemModel* source_model)
	{
		QSortFilterProxyModel::setSourceModel(source_model);
	}

	bool AbstractIterator::addRoleFilter(int role, const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (!sourceModel() || !sourceModel()->roleNames().contains(role)) {
			return false;
		}
		_filter_role_map[role].push_back(qMakePair(operand, predicate));
		return true;
	}

	bool AbstractIterator::addIdFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		_filter_role_map[Meta::Essence::Role::EssenceId].push_back(qMakePair(operand, predicate));
		return true;
	}

	bool AbstractIterator::addTagFilter(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand)
	{
		if (Core::Meta::Qd::IsValid(predicate)) {
			_filter_tag_list.push_back(qMakePair(operand, predicate));
		} else {
			QSet<Meta::Essence::Id> tag_set(_source_model_map->getRoaster()->filterSet(predicate.toString()));
			for (const Meta::Essence::Id& id : tag_set) {
				_filter_tag_list.push_back(qMakePair(operand, QVariant::fromValue(id)));
			}
		}
		return true;
	}

	QModelIndex AbstractIterator::index(int row, int column, const QModelIndex& parent) const
	{
		return QSortFilterProxyModel::index(row, column, parent);
	}

	bool AbstractIterator::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
	{
		/// @note Фильтруем по тегам если что-то задано
		if (!_filter_tag_list.empty()) {
			const QSet<Meta::Essence::Id>& tag_set(_source_model_map->tagSet(_source_model_map->index(source_row, 0)));
			for (const FilterCondition& condition : _filter_tag_list) {
				switch (condition.first) {
					case Meta::EssenceExpression::OperatorType::Equal:
						if (!tag_set.contains(condition.second.toLongLong())) {
							return false;
						}
						break;

					case Meta::EssenceExpression::OperatorType::NotEqual:
						if (tag_set.contains(condition.second.toLongLong())) {
							return false;
						}
						break;

					/// @brief Other cases are meaningless
					default:
						return false;
				}
			}
		}

		/// @brief Фильтры общего назначения (Any)
		for (const FilterCondition& condition : _filter_any_list) {
			if (!filterAcceptsRow(condition, source_row, source_parent)) {
				return false;
			}
		}

		/// @brief Фильтры на роли
		for (FilterRoleMap::ConstIterator f = _filter_role_map.constBegin(); f != _filter_role_map.constEnd(); ++f) {
			const QVariant value(_source_model_map->data(_source_model_map->index(source_row, 0), f.key()));
			const QMetaType::Type value_type(static_cast<QMetaType::Type>(value.type()));
			bool role_accept(f.value().empty());
			for (const FilterCondition& condition : f.value()) {
				switch (value_type) {
					case QMetaType::QString:
						if (condition.first == Meta::EssenceExpression::OperatorType::Equal) {
							role_accept = QRegExp(condition.second.toString(), Qt::CaseSensitivity::CaseInsensitive, QRegExp::PatternSyntax::Wildcard).exactMatch(value.toString());
						} else if (condition.first == Meta::EssenceExpression::OperatorType::NotEqual) {
							role_accept = !QRegExp(condition.second.toString(), Qt::CaseSensitivity::CaseInsensitive, QRegExp::PatternSyntax::Wildcard).exactMatch(value.toString());
						} else {
							role_accept = false;
						}
						break;

					default:
						if (condition.first == Meta::EssenceExpression::OperatorType::Equal) {
							role_accept = (condition.second == value);
						} else if (condition.first == Meta::EssenceExpression::OperatorType::NotEqual) {
							role_accept = (condition.second != value);
						} else if (condition.first == Meta::EssenceExpression::OperatorType::LessOrEqual) {
							role_accept = (condition.second <= value);
						} else if (condition.first == Meta::EssenceExpression::OperatorType::Less) {
							role_accept = (condition.second < value);
						} else if (condition.first == Meta::EssenceExpression::OperatorType::GreaterOrEqual) {
							role_accept = (condition.second >= value);
						} else if (condition.first == Meta::EssenceExpression::OperatorType::Greater) {
							role_accept = (condition.second > value);
						} else {
							role_accept = false;
						}
				}
				/// @note Если хотя бы один из фильтров удовлетворяет - переходим к следующей роли
				if (role_accept) {
					break;
				}
			}
			/// @note Если ни один из фильтров (OR) роли не удовлетворяет - значит не принимаем строку целиком
			if (!role_accept) {
				return false;
			}
		}
		return true;
	}

	bool AbstractIterator::filterAcceptsRow(const FilterCondition& filter, int source_row, const QModelIndex& source_parent) const
	{
		return true;
	}

	bool AbstractIterator::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
	{
		if (_sort_list.length() <= 1) {
			return QSortFilterProxyModel::lessThan(source_left, source_right);
		}
		for (const QPair<int, Qt::SortOrder>& sort : _sort_list) {
			QVariant left(_source_model_map->data(source_left, sort.first));
			QVariant right(_source_model_map->data(source_right, sort.first));
			if (Qt::SortOrder::AscendingOrder == sort.second) {
				left.swap(right);
			}
			switch (left.userType()) {
				case QVariant::Invalid:
					return (right.type() == QVariant::Invalid);

				case QVariant::Int: {
					const int l(left.toInt()), r(right.toInt());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::UInt: {
					const uint l(left.toUInt()), r(right.toUInt());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::LongLong: {
					const qlonglong l(left.toLongLong()), r(right.toLongLong());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::ULongLong: {
					const qulonglong l(left.toULongLong()), r(right.toULongLong());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QMetaType::Float: {
					const float l(left.toFloat()), r(right.toFloat());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::Double: {
					const double l(left.toDouble()), r(right.toDouble());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::Char: {
					const QChar l(left.toChar()), r(right.toChar());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::Date: {
					const QDate l(left.toDate()), r(right.toDate());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::Time: {
					const QTime l(left.toTime()), r(right.toTime());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::DateTime: {
					const QDateTime l(left.toDateTime()), r(right.toDateTime());
					if (l == r) {
						continue;
					}
					return l < r;
				}

				case QVariant::String:
				default: {
					const QString l(left.toString()), r(right.toString());
					if (l == r) {
						continue;
					}
					return l.localeAwareCompare(r) < 0;
				}
			}
		}
		return false;
	}
} } } }
