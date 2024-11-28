#include "AbstractResource.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	AbstractResource::AbstractResource(QObject* parent)
	:
		QAbstractItemModel(parent)
	{
		/// @brief Открывает удаленный Application::ObjectCursor(Domain) и получает обновления(в том числе и изменения) непосредственно от туда
	}

	AbstractResource::~AbstractResource()
	{

	}

	bool AbstractResource::page(bool fetch_all)
	{
		return false;
	}

	bool AbstractResource::order(const QVector<int>& role_vector, bool reverse)
	{
		return false;
	}

	bool AbstractResource::order(int role, Qt::SortOrder order)
	{
		return false;
	}

	void AbstractResource::clear()
	{

	}

	QModelIndex AbstractResource::index(int row, int column, const QModelIndex& parent) const
	{
		return createIndex(row, column);
	}

	QModelIndex AbstractResource::parent(const QModelIndex& child) const
	{
		return QModelIndex();
	}

	int AbstractResource::rowCount(const QModelIndex& parent) const
	{
		return 0;
	}

	int AbstractResource::columnCount(const QModelIndex& parent) const
	{
		return 0;
	}

	QVariant AbstractResource::data(const QModelIndex& index, int role) const
	{
		return QVariant();
	}

	Qt::ItemFlags AbstractResource::flags(const QModelIndex& index) const
	{
		return {};
	}

	QHash<int, QByteArray> AbstractResource::roleNames() const
	{
		return {};
	}
} } } }
