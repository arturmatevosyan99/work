#include "AbstractCursor.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	AbstractCursor::AbstractCursor(QObject* parent)
	:
		Core::Net::AbstractResource(parent)
	{
		/// @brief Открывает удаленный Application::ObjectCursor(Domain) и получает обновления(в том числе и изменения) непосредственно от туда
	}

	AbstractCursor::~AbstractCursor()
	{

	}

	bool AbstractCursor::page(bool fetch_all)
	{
		return AbstractResource::page(fetch_all);
	}

	bool AbstractCursor::filter(const Meta::EssenceExpression& expression)
	{
		return _filter_delegate(expression);
	}

	bool AbstractCursor::filter(const QVector<Meta::EssenceToken>& token_vector)
	{
		bool retval(true);
		for (const QVector<Meta::EssenceToken>::value_type& token: token_vector) {
			if (token.active) {
				retval &= filter(token.express());
			}
		}
		return retval;
	}

	bool AbstractCursor::order(const QVector<int>& role_vector, bool reverse)
	{
		return AbstractResource::order(role_vector, reverse);
	}

	bool AbstractCursor::order(int role, Qt::SortOrder order)
	{
		return AbstractResource::order(role, order);
	}

	void AbstractCursor::clear()
	{
		return AbstractResource::clear();
	}

	QModelIndex AbstractCursor::index(int row, int column, const QModelIndex& parent) const
	{
		return createIndex(row, column);
	}

	QModelIndex AbstractCursor::parent(const QModelIndex& child) const
	{
		return QModelIndex();
	}

	int AbstractCursor::rowCount(const QModelIndex& parent) const
	{
		return 0;
	}

	int AbstractCursor::columnCount(const QModelIndex& parent) const
	{
		return 0;
	}

	QVariant AbstractCursor::data(const QModelIndex& index, int role) const
	{
		return QVariant();
	}

	Qt::ItemFlags AbstractCursor::flags(const QModelIndex& index) const
	{
		return {};
	}

	QHash<int, QByteArray> AbstractCursor::roleNames() const
	{
		return {};
	}
} } } }
