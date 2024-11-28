/**
 @file SetupScheme.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "SetupScheme.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	SetupScheme::SetupScheme()
	:
		Core::Meta::AbstractScheme()
	{

	}

	SetupScheme::SetupScheme(const QString& extname, const QString& modelname, const QMap<QString, qulonglong>& dependency_map)
	:
		Core::Meta::AbstractScheme(extname, modelname),

		_dependency_map(dependency_map)
	{

	}

	SetupScheme::SetupScheme(const SetupScheme& origin)
	:
		Core::Meta::AbstractScheme(origin),

		_dependency_map(origin._dependency_map)
	{

	}

	SetupScheme::SetupScheme(SetupScheme&& origin)
	:
		Core::Meta::AbstractScheme(origin),

		_dependency_map(std::move(origin._dependency_map))
	{

	}

	SetupScheme& SetupScheme::operator=(const SetupScheme& origin)
	{
		AbstractScheme::operator=(origin);
		_dependency_map = origin._dependency_map;

		return *this;
	}

	SetupScheme& SetupScheme::operator=(SetupScheme&& origin)
	{		
		AbstractScheme::operator=(origin);
		_dependency_map = std::move(origin._dependency_map);

		return *this;
	}

	bool SetupScheme::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{		
		Core::Meta::Scheme<SetupScheme>::Iterator i(Core::Meta::Scheme<SetupScheme>::Iterate(qd));
		if (!Core::Meta::Scheme<SetupScheme>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}
} } } }
