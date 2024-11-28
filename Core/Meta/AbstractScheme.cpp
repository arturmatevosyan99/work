/**
 @file AbstractScheme.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "AbstractScheme.hpp"

#include "../../Qaos/String.hpp"
#include <QRegExp>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	AbstractScheme::AbstractScheme()
	{
	}

	AbstractScheme::AbstractScheme(const QString& extname, const QString& modelname)
	{
		QStringList list(extname.split('.'));
		if (list.size() == 2) {
			_vendor_name = list.first();
			_module_name = list.last();
		} else {
			_vendor_name = "cadabra";
			_module_name = list.first();
		}
		_model_name = modelname;
	}

	AbstractScheme::AbstractScheme(const QString& vendorname, const QString& modulename, const QString& modelname)
	{
		_vendor_name = vendorname;
		_module_name = modulename;
		_model_name = modelname;
	}

	AbstractScheme::AbstractScheme(const AbstractScheme& origin)
	:
		_vendor_name(origin._vendor_name),
		_module_name(origin._module_name),
		_model_name(origin._model_name)
	{
	}

	AbstractScheme::AbstractScheme(AbstractScheme&& origin)
	:
		_vendor_name(std::move(origin._vendor_name)),
		_module_name(std::move(origin._module_name)),
		_model_name(std::move(origin._model_name))
	{
	}

	AbstractScheme::~AbstractScheme()
	{
	}

	AbstractScheme& AbstractScheme::operator=(const AbstractScheme& origin)
	{
		_vendor_name = origin._vendor_name;
		_module_name = origin._module_name;
		_model_name = origin._model_name;

		return *this;
	}

	AbstractScheme& AbstractScheme::operator=(AbstractScheme&& origin)
	{
		_vendor_name = std::move(origin._vendor_name);
		_module_name = std::move(origin._module_name);
		_model_name = std::move(origin._model_name);

		return *this;
	}

	bool AbstractScheme::operator==(const QString& pattern) const
	{
		return QRegExp(
						pattern,
						Qt::CaseSensitivity::CaseInsensitive,
						QRegExp::PatternSyntax::Wildcard
				).exactMatch(QString("%1.%2.%3").arg(_vendor_name, _module_name, _model_name));
	}

	QHash<int, QByteArray> AbstractScheme::roleNames() const
	{
		QHash<int, QByteArray> retval;
		retval[Role::VendorName] = "vendor_name";
		retval[Role::ModuleName] = "module_name";
		retval[Role::ModelName] = "model_name";
		retval[Role::ExtensionURI] = "extension_uri";
		retval[Role::ExtensionHash] = "extension_hash";

		return retval;
	}

	QVariant AbstractScheme::data(int role) const
	{
		switch (role) {
			case Role::VendorName: return getVendorName();
			case Role::ModuleName: return getModuleName();
			case Role::ModelName: return getModelName();
			case Role::ExtensionURI: return getExtensionURI();
			case Role::ExtensionHash: return getExtensionHash();

			default: return QVariant();
		}
	}

	const QString& AbstractScheme::getVendorName() const
	{
		return _vendor_name;
	}

	const QString& AbstractScheme::getModuleName() const
	{
		return _module_name;
	}

	const QString& AbstractScheme::getModelName() const
	{
		return _model_name;
	}

	QString AbstractScheme::getExtensionURI() const
	{
		return QString("%1.%2").arg(_vendor_name, _module_name);
	}

	quint32 AbstractScheme::getExtensionHash() const
	{
		return Qaos::String::MurMur3(getExtensionURI().toLower());
	}
} } } }
