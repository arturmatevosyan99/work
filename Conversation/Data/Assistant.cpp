#include "Assistant.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	Assistant::Assistant(QObject* parent)
	:
		QAbstractListModel(parent)
	{

	}

	void Assistant::emplace(const Meta::Assist& assist, SourceGroup group)
	{
		beginInsertRows(QModelIndex(), 0, 0);
		_list.prepend(assist);
		if (Meta::Assist::SourceGroup::Default != group) {
			_list.front().setSourceGroup(group);
		}
		endInsertRows();
	}

	void Assistant::emplace(const QList<Meta::Assist>& assist_list, SourceGroup group)
	{
		if (assist_list.isEmpty()) {
			return;
		}
		beginInsertRows(QModelIndex(), 0, assist_list.count() - 1);
		for (const Meta::Assist& assist : assist_list) {
			_list.prepend(assist);
			if (Meta::Assist::SourceGroup::Default != group) {
				_list.front().setSourceGroup(group);
			}
		}
		endInsertRows();
	}

	void Assistant::dismiss(const Meta::Assist& target)
	{
		dismiss(target.getUId());
	}

	void Assistant::dismiss(const QUuid& target_uid)
	{
		for (int row = 0; row < _list.count(); ++row) {
			if (_list.at(row).getUId() == target_uid) {
				beginRemoveRows(QModelIndex(), row, row);
				_list.removeAt(row);
				--row;
				endRemoveRows();
			}
		}
	}

	void Assistant::dismiss(SourceGroup source)
	{
		for (int row = 0; row < _list.count(); ++row) {
			if (_list.at(row).getSourceGroup() == source) {
				beginRemoveRows(QModelIndex(), row, row);
				_list.removeAt(row);
				--row;
				endRemoveRows();
			}
		}
	}

	void Assistant::dismiss(TargetLayout layout)
	{
		for (int row = 0; row < _list.count(); ++row) {
			if (_list.at(row).getTargetLayout() == layout) {
				beginRemoveRows(QModelIndex(), row, row);
				_list.removeAt(row);
				--row;
				endRemoveRows();
			}
		}
	}

	int Assistant::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _list.size();
	}

	QVariant Assistant::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}

		switch (role) {
			case Role::UId: return _list.at(index.row()).getUId();
			case Role::Scheme: return QVariant::fromValue(_list.at(index.row()).makeSchemeQd());
			case Role::Body: return _list.at(index.row()).getBody();
			case Role::AffectedSubjectQdList: return _list.at(index.row()).makeAffectedSubjectQdVariantList();
			case Role::Layout: return QVariant::fromValue(_list.at(index.row()).getTargetLayout());
			case Role::Source: return QVariant::fromValue(_list.at(index.row()).getSourceGroup());

			default: return QVariant();
		}
	}

	QHash<int, QByteArray> Assistant::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractListModel::roleNames());
		retval[Role::UId] = "uid";
		retval[Role::Scheme] = "scheme";
		retval[Role::Body] = "body";
		retval[Role::AffectedSubjectQdList] = "affected_subject_qd_list";
		retval[Role::Layout] = "layout";
		retval[Role::Source] = "source";

		return retval;
	}

	bool Assistant::isEmpty() const
	{
		return _list.isEmpty();
	}
} } } }
