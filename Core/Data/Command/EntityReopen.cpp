#include "EntityReopen.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	EntityReopen::EntityReopen()
	:
		EntityCommand(nullptr)
	{

	}

	EntityReopen::EntityReopen(QSharedPointer<AbstractEntity> entity, const Rowset::HintTable& hint_table)
	:
		EntityCommand(entity),

		_hint_table(hint_table)
	{

	}

	EntityReopen::EntityReopen(const EntityReopen& origin)
	:
		EntityCommand(origin),

		_hint_table(origin._hint_table)
	{

	}

	EntityReopen::EntityReopen(EntityReopen&& origin)
	:
		EntityCommand(origin),

		_hint_table(origin._hint_table)
	{

	}

	EntityReopen::~EntityReopen()
	{

	}

	const Rowset::HintTable& EntityReopen::getHintTable() const
	{
		return _hint_table;
	}
} } } }
