#include "CheckList.hpp"

#include <QDebug>

#include "../Meta/TestSchemeCache/FileSystemTestDirectory.hpp"
#include "../Meta/TestSchemeCache/WebDAVTestDirectory.hpp"

#include <numeric>

namespace Cadabra { namespace Queen { namespace Stress { namespace Model {
	CheckList::CheckList(QObject* parent)
	:
		QAbstractListModel(parent),

		_selection_list(this)
	{

	}

	CheckList::~CheckList()
	{
		qDeleteAll(_model_list);
	}

	bool CheckList::append(QUrl url)
	{
		QScopedPointer<Core::Meta::SchemeList> list;
		if (url.scheme().startsWith("webdav")) {
			url.setScheme(url.scheme().back() == 's' ? "https" : "http");
			list.reset(new Meta::WebDAVTestDirectory(url));
		} else if (url.scheme().startsWith("http")) {
			list.reset(new Meta::WebDAVTestDirectory(url));
		} else if (url.scheme().startsWith("file")) {
			list.reset(new Meta::FileSystemTestDirectory(url));
		} else if (url.scheme().startsWith("queen") || url.path().startsWith("Core.Meta.TestScheme")) {
			/// @todo support queen://extension/variation или ://Core.Meta.TestScheme/extension/variation
			QScopedPointer<Core::Meta::SchemeModel> scheme_model(new Core::Meta::SchemeModel());
			scheme_model->addOrder(new Core::Meta::Order(scheme_model.get(), "module_display_name", Qt::SortOrder::AscendingOrder));
			if (!scheme_model->setFactoryAlias("Core.Meta.TestScheme")) {
				return false;
			}
			return append(std::move(scheme_model));
		} else {
			return false;
		}

		return append(list.get()) ? !!list.take() : false;
	}

	bool CheckList::append(Core::Meta::SchemeList* list)
	{
		if (!list) {
			return false;
		}
		QScopedPointer<Core::Meta::SchemeModel> model(new Core::Meta::SchemeModel());
		if (!model->setFactoryList(list)) {
			return false;
		}
		return append(model.get()) ? !!model.take() : false;
	}

	bool CheckList::append(Core::Meta::SchemeModel* model)
	{
		if (!model) {
			return false;
		}
		for (const QString& filter : qAsConst(_filter_list)) {
			const QStringList path_list(filter.split("/"));
			if (path_list.size() == 3) {
				if (path_list.at(0) != "*") {
					model->addFilter(new Core::Meta::Filter(model, Core::Meta::Filter::Equal, "module_name", path_list.at(0)));
				}
				if (path_list.at(1) != "*") {
					model->addFilter(new Core::Meta::Filter(model, Core::Meta::Filter::Equal, "variation", static_cast<int>(Stress::Extension::VariationMap.value(path_list.at(1)))));
				}
				if (path_list.at(2) != "*") {
					model->addFilter(new Core::Meta::Filter(model, Core::Meta::Filter::Equal, "model_name", path_list.at(2)));
				}
			}
		}
		const int model_row_count(model->rowCount());
		/// @xxx Может быть уже добавлено, по какой-то причине
		QQmlListReference role_model_list_reference(model, "roleList"), order_model_list_reference(model, "orderList");
		if (!role_model_list_reference.canAppend() || !order_model_list_reference.canAppend()) {
			return false;
		}
		role_model_list_reference.append(new Core::Meta::DynamicRole(model, "status_state", QString("ready")));
		order_model_list_reference.append(new Core::Meta::Order(model, "module_name", Qt::SortOrder::AscendingOrder));

		if (model_row_count > 0) {
			int offset(rowCount());
			beginInsertRows(QModelIndex(), offset, offset + model->rowCount() - 1);
			_model_list.append(model);
			model->setParent(this);
			endInsertRows();
		} else {
			_model_list.append(model);
			model->setParent(this);
		}

		QObject::connect
		(
			model,
			&QAbstractItemModel::rowsInserted,

			this,
			std::bind
			(
				(void(CheckList::*)(int, const QModelIndex&, int, int)) &CheckList::rowsInserted,
				this, _model_list.size() - 1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			)
		);

		/// @todo removed, moved, reset, layout changed

		return true;
	}

	bool CheckList::append(QScopedPointer<Core::Meta::SchemeModel>&& model)
	{
		return append(model.get()) ? !!model.take() : false;
	}

	bool CheckList::remove(const QUrl& url)
	{
		int offset(0);
		const QString raw_url(url.toString());
		for (int m = 0; m < _model_list.count(); ++m) {
			Core::Meta::SchemeModel* model(_model_list.at(m));
			if (model->getFactoryAlias() == raw_url) {
				beginRemoveRows(QModelIndex(), offset, model->rowCount() - 1);
				_model_list.removeAt(m);
				endRemoveRows();
				model->deleteLater();
				return true;
			}
			offset += model->rowCount();
		}
		return false;
	}

	void CheckList::filter(const QStringList& filter_list)
	{
		if (filter_list.isEmpty()) {
			return ;
		}
		_filter_list += filter_list;
		beginResetModel();
		for (int m = 0; m < _model_list.count(); ++m) {
			Core::Meta::SchemeModel* scheme_model(_model_list.at(m));
			for (const QString& filter : qAsConst(_filter_list)) {
				const QStringList path_list(filter.split("/"));
				if (path_list.size() == 3) {
					if (path_list.at(0) != "*") {
						scheme_model->addFilter(new Core::Meta::Filter(scheme_model, Core::Meta::Filter::Equal, "module_name", path_list.at(0)));
					}
					if (path_list.at(1) != "*") {
						scheme_model->addFilter(new Core::Meta::Filter(scheme_model, Core::Meta::Filter::Equal, "variation", static_cast<int>(Stress::Extension::VariationMap.value(path_list.at(1)))));
					}
					if (path_list.at(2) != "*") {
						scheme_model->addFilter(new Core::Meta::Filter(scheme_model, Core::Meta::Filter::Equal, "model_name", path_list.at(2)));
					}
				}
			}
		}
		endResetModel();
	}

	Core::Meta::SchemeModel* CheckList::getFirst() const
	{
		return _model_list.isEmpty() ? nullptr : _model_list.first();
	}

	Core::Meta::SchemeModel* CheckList::getLast() const
	{
		return _model_list.isEmpty() ? nullptr : _model_list.last();
	}

	Core::Meta::SchemeModel* CheckList::get(int index) const
	{
		return _model_list.count() > index ? _model_list.at(index) : nullptr;
	}

	int CheckList::rowCount(const QModelIndex& parent) const
	{
		return std::accumulate
		(
			_model_list.constBegin(), _model_list.constEnd(),
			0,
			[](int a, Core::Meta::SchemeModel* b)
			{
				return a + b->rowCount();
			}
		);
	}

	QVariant CheckList::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}
		int offset(0);
		for (Core::Meta::SchemeModel* model : _model_list) {
			const int row_count(model->rowCount());
			if ((offset += row_count) > index.row()) {
				if (Role::SchemeModel == role) {
					return QVariant::fromValue(model);
				} else if (Role::FactoryAlias == role) {
					return model->getFactoryAlias();
				} else if (Role::StatusState == role) {
					role = model->roleNames().key("status_state");
				} else if (Role::CheckState == role) {
					return _selection_list.isSelected(index);
				}
				return model->data(model->index(index.row() - (offset - row_count), 0), role);
			}
		}
		return QVariant{};
	}

	QHash<int, QByteArray> CheckList::roleNames() const
	{
		QHash<int, QByteArray> retval(Core::Meta::SchemeList("Core.Meta.TestScheme").roleNames());
		retval.insert
		(
			{
				{Role::SchemeModel, "scheme_model"},
				{Role::FactoryAlias, "factory_alias"},
				{Role::CheckState, "check_state"},
				{Role::StatusState, "status_state"}
			}
		);
		return retval;
	}

	bool CheckList::setData(const QModelIndex& index, const QVariant& value, int role)
	{
		if (!index.isValid()) {
			return false;
		}
		int offset(0);
		int model_role(role);
		for (Core::Meta::SchemeModel* model : qAsConst(_model_list)) {
			const int row_count(model->rowCount());
			if ((offset += row_count) > index.row()) {
				do {
					if (Role::CheckState == role) {
						_selection_list.select(index, value.toBool() ? QItemSelectionModel::SelectionFlag::Select : QItemSelectionModel::SelectionFlag::Deselect);
						break;
					} else if (Role::StatusState == role) {
						model_role = model->roleNames().key("status_state");
					}
					if (!model->setData(model->index(index.row() - (offset - row_count), 0), value, model_role)) {
						return false;
					}
				} while (false);
				emit dataChanged(index, index, {role});
				return true;
			}
		}
		return false;
	}

	void CheckList::setCheckState(const QModelIndex& index, bool value)
	{
		_selection_list.select(index, value ? QItemSelectionModel::SelectionFlag::Select : QItemSelectionModel::SelectionFlag::Deselect);
	}

	void CheckList::setStatusState(const QModelIndex& index, QString value)
	{
		setData(index, value, Role::StatusState);
	}

	void CheckList::rowsInserted(int list_index, const QModelIndex& parent_index, int start, int end)
	{
		int offset(0);
		for (int l = 0; l < list_index; ++l) {
			offset += _model_list.at(l)->rowCount();
		}
		beginInsertRows(QModelIndex{}, offset + start, offset + end);
		endInsertRows();
	}
} } } }
