#include "TagSet.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	TagSet::TagSet()
	:
		_dummy(new QObject(nullptr))
	{
	}

	TagSet::TagSet(const TagSet& origin)
	:
		_dummy(origin._dummy),
		_set(origin._set)
	{
	}

	TagSet::TagSet(TagSet&& origin)
	:
		_dummy(std::move(origin._dummy)),
		_set(std::move(origin._set))
	{
		origin._set.clear();
	}

	TagSet::~TagSet()
	{
	}

	QStringList TagSet::toStringList() const noexcept
	{
		QStringList retval;
		for (Cadabra::Queen::Repository::Meta::Tag* tag : _set) {
			retval.append("#" + tag->getAlias());
		}
		return retval;
	}

	QString TagSet::toString() const noexcept
	{
		return toStringList().join("\n");
	}

	int TagSet::getCount() const
	{
		return _set.count();
	}

	QSet<Cadabra::Queen::Repository::Meta::Tag*> TagSet::getSet() const
	{
		return _set;
	}

	Cadabra::Queen::Repository::Meta::Tag* TagSet::getTag(int index) const
	{
		return (getCount() >= index) ? nullptr : *(_set.begin() + index);
	}

	Cadabra::Queen::Core::Meta::Qd TagSet::getQd(int index) const
	{
		return (getCount() >= index) ? Core::Meta::Qd(0) : (*(_set.begin() + index))->getQd();
	}

	QString TagSet::getAlias(int index) const
	{
		return (getCount() >= index) ? "" : (*(_set.begin() + index))->getAlias();
	}

	void TagSet::add(Meta::Tag* tag)
	{
		tag->setParent(_dummy.get());
		_set.insert(tag);
	}
} } } }
