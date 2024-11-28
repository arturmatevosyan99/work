/**
 @file WebDAVTestDirectory.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "WebDAVTestDirectory.hpp"

#include "../../../Qaos/DAV/WebConnection.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Meta {
	WebDAVTestDirectory::WebDAVTestDirectory(const QUrl& url, QObject* parent)
	:
		TestSchemeCache(parent),

		_connection(new Qaos::DAV::WebConnection(url))
	{

	}

	void WebDAVTestDirectory::load()
	{
		collect("Unit/");
		collect("Death/");
		collect("Development/");
		collect("Integration/");
		collect("Custom/");
	}

	bool WebDAVTestDirectory::collect(const QString& path)
	{
		QScopedPointer<Qaos::DAV::WebCollection> collection(new Qaos::DAV::WebCollection(_connection.get(), false));
		if (!collection->fetch(path)) {
			return false;
		}
		QObject::connect
		(
			collection.get(),
			&Qaos::DAV::WebCollection::finished,
			std::bind
			(
				&WebDAVTestDirectory::onSuccess,
				this,
				collection.get()
			)
		);
		QObject::connect
		(
			collection.get(),
			&Qaos::DAV::WebCollection::failed,
			std::bind
			(
				&WebDAVTestDirectory::onFault,
				this,
				collection.get()
			)
		);
		return collection.take();
	}

	void WebDAVTestDirectory::onFault(Qaos::DAV::WebCollection* const collection)
	{
		collection->deleteLater();
	}

	void WebDAVTestDirectory::onSuccess(Qaos::DAV::WebCollection* const collection)
	{
		const QScopedPointer<Qaos::DAV::WebCollection> safe_collection(collection);
		if (collection->isEmpty()) {
			return ;
		}

		TestSchemeCache::Cache scheme_cache;
		for (const Qaos::DAV::WebItem& item : collection->getItemList()) {
			if (item.isDirectory()) {
				continue;
			}
			Core::TestVariation variation(Core::TestVariation::Custom);
			if (item.getPath().startsWith("Integration/")) {
				variation = Core::TestVariation::Integration;
			} else if (item.getPath().startsWith("Unit/")) {
				variation = Core::TestVariation::Unit;
			} else if (item.getPath().startsWith("Death/")) {
				variation = Core::TestVariation::Death;
			} else if (item.getPath().startsWith("Development/")) {
				variation = Core::TestVariation::Development;
			}

			Core::TestFactory factory(Core::TestFactory::General);
			Core::TestDialect dialect(Core::TestDialect::None);
			if (item.getName().endsWith(".qml")) {
				factory = Core::TestFactory::General;
				dialect = Core::TestDialect::QML;
			} else if (item.getName().endsWith(".pqml")) {
				factory = Core::TestFactory::PetrI;
				dialect = Core::TestDialect::QML;
			} else if (item.getName().endsWith(".gml")) {
				factory = Core::TestFactory::General;
				dialect = Core::TestDialect::GraphML;
			} else if (item.getName().endsWith(".pgml")) {
				factory = Core::TestFactory::PetrI;
				dialect = Core::TestDialect::GraphML;
			} else if (item.getName().endsWith(".ghk")) {
				factory = Core::TestFactory::General;
				dialect = Core::TestDialect::Gherkin;
			} else if (item.getName().endsWith(".pghk")) {
				factory = Core::TestFactory::PetrI;
				dialect = Core::TestDialect::Gherkin;
			} else if (item.getName().endsWith(".py")) {
				factory = Core::TestFactory::General;
				dialect = Core::TestDialect::Python;
			} else if (item.getName().endsWith(".ppy")) {
				factory = Core::TestFactory::PetrI;
				dialect = Core::TestDialect::Python;
			} else {
				qWarning() << "Unknown test: " << item.getPath() << item.getName();
			}
			scheme_cache.append
			(
				{
					_connection->getBaseURL().path().section('/', -1, -1, QString::SectionSkipEmpty),
					_connection->getBaseURL().toString() + collection->getPath().chopped(1),
					item.getName(),
					variation,
					dialect,
					factory
				}
			);
		}
		beginInsertRows(QModelIndex(), getCacheSize(), getCacheSize() + scheme_cache.count() - 1);
		append(std::move(scheme_cache));
		endInsertRows();
	}

	QString WebDAVTestDirectory::getFactoryAlias() const
	{
		return _connection->getBaseURL().toString();
	}
} } } }
