#include "MapModel.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	MapModel::Down::Down(MapModel* origin)
	:
		map_model(origin)
	{
		if (map_model) {
			map_model->beginResetModel();
		}
	}

	MapModel::Down::Down(Down&& origin)
	:
		map_model(std::move(origin.map_model))
	{
		origin.map_model = nullptr;
	}

	MapModel::Down::~Down()
	{
		if (map_model) {
			map_model->endResetModel();
		}
	}

	void MapModel::Down::detach()
	{
		map_model = nullptr;
	}

	MapModel::MapModel(QObject* parent)
	:
		QAbstractListModel(parent)
	{

	}

	int MapModel::rowCount(const QModelIndex& parent) const
	{
		return 0;
	}

	int MapModel::columnCount(const QModelIndex& parent) const
	{
		return 2;
	}

	QVariant MapModel::data(const QModelIndex& index, int role) const
	{
		return QVariant();
	}
} } } }
