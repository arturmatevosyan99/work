#include "Tag.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	Tag::Tag(QObject* parent)
	:
		QObject(parent)
	{
	}

	Tag::Tag(QObject* parent, QMap<Meta::Tag::Id, QObject*>::ConstIterator iterator)
	:
		QObject(parent),
		_iterator(iterator)
	{
		QObject::connect(_iterator.value(), &QObject::objectNameChanged, this, &Tag::aliasChanged);
	}

	Tag::Tag(const Tag& origin)
	:
		QObject(origin.parent()),
		_iterator(origin._iterator)
	{
		QObject::connect(_iterator.value(), &QObject::objectNameChanged, this, &Tag::aliasChanged);
	}

	Tag::Tag(Tag&& origin)
	:
		QObject(origin.parent()),
		_iterator(std::move(origin._iterator))
	{
		QObject::connect(_iterator.value(), &QObject::objectNameChanged, this, &Tag::aliasChanged);
		QObject::disconnect(_iterator.value(), &QObject::objectNameChanged, &origin, &Tag::aliasChanged);
	}

	Tag::~Tag()
	{
		QObject::disconnect(_iterator.value(), &QObject::objectNameChanged, this, &Tag::aliasChanged);
	}

	Tag& Tag::operator=(const Tag& rhs)
	{
		setParent(rhs.parent());
		_iterator = rhs._iterator;
		QObject::connect(_iterator.value(), &QObject::objectNameChanged, this, &Tag::aliasChanged);

		emit aliasChanged();

		return *this;
	}

	Tag& Tag::operator=(Tag&& rhs)
	{
		setParent(rhs.parent());
		_iterator = rhs._iterator;
		QObject::connect(_iterator.value(), &QObject::objectNameChanged, this, &Tag::aliasChanged);
		QObject::disconnect(_iterator.value(), &QObject::objectNameChanged, &rhs, &Tag::aliasChanged);

		emit aliasChanged();

		return *this;
	}

	const Meta::Tag::Id& Tag::getId() const
	{
		return _iterator.key();
	}

	Core::Meta::Qd Tag::getQd() const
	{
		return _iterator.key();
	}

	QString Tag::getAlias() const
	{
		return _iterator.value()->objectName();
	}
} } } }
