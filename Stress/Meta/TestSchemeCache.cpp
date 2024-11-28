/**
 @file TestSchemeCache.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "TestSchemeCache.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Meta {
	TestSchemeCache::TestSchemeCache(QObject* parent)
	:
		Core::Meta::SchemeList("Core.Meta.TestScheme", parent)
	{
	}

	void TestSchemeCache::refresh(bool force)
	{
		Q_UNUSED(force)

		_cache.clear();
		return SchemeList::refresh(true);
	}

	void TestSchemeCache::append(TestSchemeCache::Cache&& vector)
	{
		vector.reserve(_cache.count() + vector.count());
		while (!vector.isEmpty()) {
			_cache += vector.takeFirst();
		}
	}

	void TestSchemeCache::append(const QString& extension, const QString& module, const QString& model, Core::TestVariation variation, Core::TestDialect dialect, Core::TestFactory factory)
	{
		_cache.push_back({extension, module, model, variation, dialect, factory});
	}

	int TestSchemeCache::getCacheSize() const
	{
		return _cache.count();
	}

	int TestSchemeCache::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _cache.count();
	}

	QVariant TestSchemeCache::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid() || _cache.count() <= index.row())  {
			return QVariant();
		} else if (Qt::UserRole == role) {
			return QVariant::fromValue(_cache.at(index.row()));
		}
		const QVariant retval(_cache.at(index.row()).data(role));
		return retval.isValid() ? retval : Core::Meta::SchemeList::data(index, role);
	}
} } } }
