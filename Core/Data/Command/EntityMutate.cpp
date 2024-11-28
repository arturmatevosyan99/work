#include "EntityMutate.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	EntityMutate::EntityMutate()
	:
		EntityCommand(nullptr)
	{

	}

	EntityMutate::EntityMutate(QSharedPointer<AbstractEntity> entity, const Rowset::MutationList& list)
	:
		EntityCommand(entity),

		_list(list)
	{

	}

	EntityMutate::EntityMutate(QSharedPointer<AbstractEntity> entity, const QSqlRecord& record, Rowset::Mutation operation)
	:
		EntityCommand(entity),

		_list({qMakePair(operation, record)})
	{

	}

	EntityMutate::EntityMutate(const EntityMutate& origin)
	:
		EntityCommand(origin),

		_list(origin._list)
	{

	}

	EntityMutate::EntityMutate(EntityMutate&& origin)
	:
		EntityCommand(origin),

		_list(std::move(origin._list))
	{

	}

	EntityMutate::~EntityMutate()
	{

	}

	const Rowset::MutationList& EntityMutate::getList() const
	{
		return _list;
	}
} } } }
