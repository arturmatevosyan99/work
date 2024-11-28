/**
 @file FilesSystemTestDirectory.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "FileSystemTestDirectory.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Meta {
	FileSystemTestDirectory::FileSystemTestDirectory(const QUrl& path, QObject* parent)
	:
		TestSchemeCache(parent),

		_path(path)
	{
	}

	void FileSystemTestDirectory::load()
	{
		if (_path.scheme() != "file") {
			return ;
		}
		const QDir base_directory(_path.path());
		const QFileInfoList directory_list(base_directory.entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot | QDir::NoSymLinks, QDir::Name));
		for (const QFileInfo& directory_info : directory_list) {
			if (!directory_info.isReadable()) {
				continue;
			}
			QDir directory(directory_info.filePath());
			const QFileInfoList test_list(directory.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot | QDir::NoSymLinks, QDir::Name));
			for (const QFileInfo& test_info : test_list) {
				if (!test_info.isReadable()) {
					continue;
				}
				const QString filename(test_info.fileName());
				const QString basename(directory_info.baseName());

				Core::TestVariation variation(Core::TestVariation::Custom);
				if (basename == "Integration") {
					variation = Core::TestVariation::Integration;
				} else if (basename == "Unit") {
					variation = Core::TestVariation::Unit;
				} else if (basename == "Death") {
					variation = Core::TestVariation::Death;
				} else if (basename == "Proof") {
					variation = Core::TestVariation::Proof;
				} else if (basename == "Development") {
					variation = Core::TestVariation::Development;
				}

				Core::TestFactory factory(Core::TestFactory::General);
				Core::TestDialect dialect(Core::TestDialect::None);
				if (filename.endsWith(".qml")) {
					factory = Core::TestFactory::General;
					dialect = Core::TestDialect::QML;
				} else if (filename.endsWith(".pqml")) {
					factory = Core::TestFactory::PetrI;
					dialect = Core::TestDialect::QML;
				} else if (filename.endsWith(".gml")) {
					factory = Core::TestFactory::General;
					dialect = Core::TestDialect::GraphML;
				} else if (filename.endsWith(".pgml")) {
					factory = Core::TestFactory::PetrI;
					dialect = Core::TestDialect::GraphML;
				} else if (filename.endsWith(".ghk")) {
					factory = Core::TestFactory::General;
					dialect = Core::TestDialect::Gherkin;
				} else if (filename.endsWith(".pghk")) {
					factory = Core::TestFactory::PetrI;
					dialect = Core::TestDialect::Gherkin;
				} else if (filename.endsWith(".py")) {
					factory = Core::TestFactory::General;
					dialect = Core::TestDialect::Python;
				} else if (filename.endsWith(".ppy")) {
					factory = Core::TestFactory::PetrI;
					dialect = Core::TestDialect::Python;
				} else {
					/// @log
				}
				append(base_directory.dirName(), "file://" + directory.absolutePath(), filename, variation, dialect, factory);
			}
		}
	}

	QString FileSystemTestDirectory::getFactoryAlias() const
	{
		return _path.toString();
	}
} } } }
