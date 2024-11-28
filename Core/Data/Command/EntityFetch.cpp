#include "EntityFetch.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	EntityFetch::EntityFetch()
	:
		EntityCommand(nullptr),

		_row_index(0)
	{

	}

	EntityFetch::EntityFetch(QSharedPointer<AbstractEntity> entity, int row_index)
	:
		EntityCommand(entity),

		_row_index(row_index)
	{

	}

	EntityFetch::EntityFetch(const EntityFetch& origin)
	:
		EntityCommand(origin),

		_row_index(origin._row_index)
	{

	}

	EntityFetch::EntityFetch(EntityFetch&& origin)
	:
		EntityCommand(origin),

		_row_index(origin._row_index)
	{

	}

	EntityFetch::~EntityFetch()
	{

	}

	const int& EntityFetch::getRowIndex() const
	{
		return _row_index;
	}
} } } }
