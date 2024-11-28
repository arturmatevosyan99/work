#include "EntityOpen.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	EntityOpen::EntityOpen()
	:
		EntityCommand(nullptr),

		_provider(nullptr),
		_mode_set({}),
		_rowset_type(Provider::RowsetType::Regular)
	{

	}

	EntityOpen::EntityOpen(QSharedPointer<AbstractEntity> entity, Provider* provider, QFlags<Provider::OpenMode> mode_set, Provider::RowsetType rowset_type)
	:
		EntityCommand(entity),

		_provider(provider),
		_mode_set(mode_set),
		_rowset_type(rowset_type)
	{

	}

	EntityOpen::EntityOpen(const EntityOpen& origin)
	:
		EntityCommand(origin),

		_provider(origin._provider),
		_mode_set(origin._mode_set),
		_rowset_type(origin._rowset_type)
	{

	}

	EntityOpen::EntityOpen(EntityOpen&& origin)
	:
		EntityCommand(origin),

		_provider(origin._provider),
		_mode_set(origin._mode_set),
		_rowset_type(origin._rowset_type)
	{

	}

	EntityOpen::~EntityOpen()
	{

	}

	const QFlags<Provider::OpenMode>& EntityOpen::getModeSet() const
	{
		return _mode_set;
	}


	const Provider::RowsetType& EntityOpen::getRowsetType() const
	{
		return _rowset_type;
	}

	Provider* EntityOpen::getProvider() const
	{
		return _provider;
	}
} } } }
