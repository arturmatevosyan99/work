/**
 @file Scheme.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Scheme.hpp"

#include "AbstractScheme.hpp"

#include <QList>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	SchemeFactory::Table SchemeFactory::Map = {};
	bool SchemeFactory::IsMutable = true;

	void SchemeFactory::Close()
	{
		SchemeFactory::IsMutable = false;
	}

	QObject* SchemeFactory::QInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
	{
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		static SchemeFactory instance;
		SchemeFactory* retval(&instance);

		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	int SchemeFactory::count(QString factory_alias)
	{
		SchemeFactory::Table::ConstIterator f(SchemeFactory::Map.find(factory_alias));
		return (SchemeFactory::Map.constEnd() == f) ? 0 : f->count();
	}

	QVariantList SchemeFactory::list(QString factory_alias, QString filter)
	{
		SchemeFactory::Table::ConstIterator f(SchemeFactory::Map.find(factory_alias));
		QVariantList retval;
		if (filter.isEmpty() || filter.compare("*") == 0) {
			for (int count = f->count(), i = 0; i < count; ++i) {
				retval.append(f->index(i));
			}
		} else {
			/// @not implemented yet
		}
		return retval;
	}

	QVariant SchemeFactory::load(QString factory_alias, QVariant scheme_id)
	{
		Core::Meta::Qd qd(scheme_id);
		if (qd.isZero()) {
			return QVariant();
		}
		SchemeFactory::Table::ConstIterator f(SchemeFactory::Map.find(factory_alias));
		return (SchemeFactory::Map.constEnd() == f) ? QVariant() : f->key(qd);
	}

	QVariant SchemeFactory::load(QString factory_alias, QString extension_alias, QString model_alias)
	{
		if (extension_alias.isEmpty() || model_alias.isEmpty()) {
			return QVariant();
		}
		const int meta_id(QMetaType::type(QString("Cadabra::Queen::%1::Model::%2*").arg(extension_alias, model_alias).toLocal8Bit()));
		if (QMetaType::UnknownType == meta_id) {
			return QVariant();
		}
		const QMetaType meta_type(meta_id);
		if (!meta_type.isValid()) {
			return QVariant();
		}
		SchemeFactory::Table::ConstIterator f(SchemeFactory::Map.find(factory_alias));
		return (SchemeFactory::Map.constEnd() == f) ? QVariant() : f->type(meta_type);
	}

	QVariant SchemeFactory::ident(QString factory_alias, QString extension_alias, QString model_alias)
	{
		if (extension_alias.isEmpty() || model_alias.isEmpty()) {
			return QVariant();
		}
		const int meta_id(QMetaType::type(QString("Cadabra::Queen::%1::Model::%2*").arg(extension_alias, model_alias).toLocal8Bit()));
		if (QMetaType::UnknownType == meta_id) {
			return QVariant();
		}
		const QMetaType meta_type(meta_id);
		if (!meta_type.isValid()) {
			return QVariant();
		}
		SchemeFactory::Table::ConstIterator f(SchemeFactory::Map.find(factory_alias));
		return (SchemeFactory::Map.constEnd() == f) ? QVariant() : QVariant::fromValue(Qd(f->ident(meta_type)));
	}

	SchemeList::SchemeList(const QString& factory_alias, QObject* parent)
	:
		QAbstractListModel(parent),

		_iterator(SchemeFactory::Map.find(factory_alias))
	{

	}

	QHash<int, QByteArray> SchemeList::roleNames() const
	{
		return (SchemeFactory::Map.constEnd() == _iterator) ? QHash<int, QByteArray>{} : _iterator->role();
	}

	void SchemeList::load()
	{
	}

	void SchemeList::refresh(bool force)
	{
		if (!force) {
			return ;
		}
		beginResetModel();
		load();
		endResetModel();
	}

	QString SchemeList::getFactoryAlias() const
	{
		return SchemeFactory::Map.constEnd() == _iterator ? "" : _iterator.key();
	}

	int SchemeList::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : SchemeFactory::Map.constEnd() == _iterator ? 0 : _iterator->count();
	}

	QVariant SchemeList::data(const QModelIndex& index, int role) const
	{
		return (!index.isValid() || SchemeFactory::Map.constEnd() == _iterator || index.row() >= _iterator->count()) ? QVariant() : _iterator->data(index, role);
	}

	SchemeModel::SchemeModel(QObject* parent)
	:
		SortFilterProxyModel(parent)
	{
		QObject::connect(this, &QSortFilterProxyModel::modelReset, this, &SchemeModel::factoryReset);

		QObject::connect(&_filter_list, &AbstractPropertyList::cleared, this, &SchemeModel::onFilterListCleared);
		QObject::connect(&_filter_list, &AbstractPropertyList::expanded, this, &SchemeModel::onFilterListExpanded);
		QObject::connect(&_filter_list, &AbstractPropertyList::updated, this, &SchemeModel::onFilterListUpdated);

		QObject::connect(&_order_list, &AbstractPropertyList::cleared, this, &SchemeModel::onOrderListCleared);
		QObject::connect(&_order_list, &AbstractPropertyList::expanded, this, &SchemeModel::onOrderListExpanded);
		QObject::connect(&_order_list, &AbstractPropertyList::updated, this, &SchemeModel::onOrderListUpdated);

		setDynamicSortFilter(true);
	}

	void SchemeModel::filter()
	{
		invalidateFilter();
	}

	bool SchemeModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
	{
		if (!sourceModel() || _filter_list.isEmpty()) {
			return QSortFilterProxyModel::filterAcceptsColumn(source_row, source_parent);
		}

		for (Filter* const& filter : static_cast<const QList<Filter*>&>(_filter_list)) {
			if (filter->getRoleIndex() == -1) {
				continue;
			} else if (*filter != sourceModel()->data(sourceModel()->index(source_row, 0, source_parent), filter->getRoleIndex())) {
				return false;
			}
		}
		return true;
	}

	bool SchemeModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
	{
		if (!sourceModel() || _order_list.length() <= 1) {
			return QSortFilterProxyModel::lessThan(source_left, source_right);
		}

		for (Order* const& order : static_cast<const QList<Order*>&>(_order_list)) {
			if (order->getRoleIndex() == -1) {
				continue;
			} else if (sourceModel()->data(source_left, order->getRoleIndex()) >= sourceModel()->data(source_right, order->getRoleIndex())) {
				return false;
			}
		}
		return true;
	}

	void SchemeModel::validate(Filter* filter)
	{
		if (filter) {
			filter->_role_index = getRoleHashKey(filter->getRoleName());
		}
	}

	void SchemeModel::validate(Order* order)
	{
		if (order) {
			order->_role_index = getRoleHashKey(order->getRoleName());
		}
	}

	QHash<int, QByteArray> SchemeModel::roleNames() const
	{
		QHash<int, QByteArray> retval(SortFilterProxyModel::roleNames());
		if (_scheme_list) {
			retval.insert(_scheme_list->roleNames());
		}
		return retval;
	}

	QString SchemeModel::getFactoryAlias() const
	{
		return !_scheme_list ? "" : _scheme_list->getFactoryAlias();
	}

	QQmlListProperty<Filter> SchemeModel::getFilterList()
	{
		return _filter_list;
	}

	QQmlListProperty<Order> SchemeModel::getOrderList()
	{
		return _order_list;
	}

	void SchemeModel::addFilter(Filter* filter)
	{
		_filter_list.push_back(filter);
		onFilterListExpanded();
	}

	void SchemeModel::addOrder(Order* order)
	{
		_order_list.push_back(order);
		onOrderListExpanded();
	}

	bool SchemeModel::setFactoryList(SchemeList* list)
	{
		if (list && (!_scheme_list || _scheme_list->getFactoryAlias() != list->getFactoryAlias())) {
			_scheme_list.reset(list);
			_scheme_list->refresh();
			SortFilterProxyModel::validate();
			for (Filter* f : static_cast<const QList<Filter*>&>(qAsConst(_filter_list))) {
				validate(f);
			}
			for (Order* o : static_cast<const QList<Order*>&>(qAsConst(_order_list))) {
				validate(o);
			}
			if (!_order_list.isEmpty()) {
				setSortRole(_order_list.first()->_role_index);
				sort(0, _order_list.first()->_role_order);
			}
			setSourceModel(_scheme_list.get());
			return true;
		}
		return false;
	}

	bool SchemeModel::setFactoryAlias(const QString& alias)
	{
		if (!_scheme_list || _scheme_list->getFactoryAlias() != alias) {
			_scheme_list.reset(new SchemeList(alias));
			SortFilterProxyModel::validate();
			for (Filter* f : static_cast<const QList<Filter*>&>(qAsConst(_filter_list))) {
				validate(f);
			}
			for (Order* o : static_cast<const QList<Order*>&>(qAsConst(_order_list))) {
				validate(o);
			}
			if (!_order_list.isEmpty()) {
				setSortRole(_order_list.first()->_role_index);
				sort(0, _order_list.first()->_role_order);
			}
			setSourceModel(_scheme_list.get());
			return true;
		}
		return false;
	}

	void SchemeModel::onFilterListCleared()
	{
		invalidateFilter();
	}

	void SchemeModel::onFilterListExpanded()
	{
		validate(_filter_list.last());
		invalidateFilter();
	}

	void SchemeModel::onFilterListUpdated()
	{
		invalidateFilter();
	}

	void SchemeModel::onOrderListCleared()
	{
		invalidate();
	}

	void SchemeModel::onOrderListExpanded()
	{
		validate(_order_list.last());
		invalidate();
	}

	void SchemeModel::onOrderListUpdated()
	{
		invalidate();
	}
} } } }
