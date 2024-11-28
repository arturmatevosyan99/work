/**
 @file RenderScheme.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "RenderScheme.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	RenderScheme::RenderScheme()
	:
		_path(),
		_qml_filename(),
		_version_major(0),
		_version_minor(0)
	{

	}
	RenderScheme::RenderScheme(const QString& path, int version_major, int version_minor, const QString& extname, const QString& modelname, const QString& qmlfilename)
	:
		Core::Meta::AbstractScheme(extname, modelname),

		_path(path),
		_qml_filename(qmlfilename),
		_version_major(version_major),
		_version_minor(version_minor)
	{

	}

	RenderScheme::RenderScheme(const RenderScheme& origin)
	:
		Core::Meta::AbstractScheme(origin.getExtensionURI(), origin.getModelName()),

		_path(origin._path),
		_qml_filename(origin._qml_filename),
		_version_major(origin._version_major),
		_version_minor(origin._version_minor)
	{

	}

	RenderScheme::RenderScheme(RenderScheme&& origin)
	:
		Core::Meta::AbstractScheme(origin.getExtensionURI(), origin.getModelName()),

		_path(std::move(origin._path)),
		_qml_filename(std::move(origin._qml_filename)),
		_version_major(std::move(origin._version_major)),
		_version_minor(std::move(origin._version_minor))
	{

	}

	RenderScheme& RenderScheme::operator=(const RenderScheme& origin)
	{
		_qml_filename = origin._qml_filename;
		_path = origin._path;
		_version_major = origin._version_major;
		_version_minor = origin._version_minor;
		AbstractScheme::operator=(origin);

		return *this;
	}

	RenderScheme& RenderScheme::operator=(RenderScheme&& origin)
	{
		_qml_filename = std::move(origin._qml_filename);
		_path = std::move(origin._path);
		_version_major = std::move(origin._version_major);
		_version_minor = std::move(origin._version_minor);
		AbstractScheme::operator=(origin);

		return *this;
	}

	QUrl RenderScheme::makeURL(const QString& suffix) const
	{
		return QUrl(_qml_filename.contains("/") ? QString("qrc:/%1.qml").arg(_qml_filename) : QString("qrc:/%1/%2/%3%4.qml").arg(getModuleName(), _path, _qml_filename, suffix));
	}

	QHash<int, QByteArray> RenderScheme::roleNames() const
	{
		QHash<int, QByteArray> retval(AbstractScheme::roleNames());
		retval[Role::Path] = "path";
		retval[Role::Filename] = "filename";
		retval[Role::URL] = "url";
		retval[Role::MajorVersion] = "major_version";
		retval[Role::MinorVersion] = "minor_version";

		return retval;
	}

	QVariant RenderScheme::data(int role) const
	{
		switch (role) {
			case Role::Path: return getPath();
			case Role::Filename: return getName();
			case Role::URL: return makeURL();
			case Role::MajorVersion: return _version_major;
			case Role::MinorVersion: return _version_minor;

			default: return AbstractScheme::data(role);
		}
	}

	const QString& RenderScheme::getPath() const
	{
		return _path;
	}

	const QString& RenderScheme::getName() const
	{
		return _qml_filename;
	}
} } } }
