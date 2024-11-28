/**
 @file SetupDependency.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "SetupDependency.hpp"

#include "../../Qaos/String.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	SetupDependency::SetupDependency()
	:
		Core::Meta::AbstractScheme()
	{

	}

	SetupDependency::SetupDependency(const QString& extname, const QString& modelname, const SetupDependency::Map& map)
	:
		Core::Meta::AbstractScheme(extname, modelname),

		_map(map)
	{

	}

	SetupDependency::SetupDependency(const SetupDependency& origin)
	:
		Core::Meta::AbstractScheme(origin),

		_map(origin._map)
	{

	}

	SetupDependency::SetupDependency(SetupDependency&& origin)
	:
		Core::Meta::AbstractScheme(origin),

		_map(std::move(origin._map))
	{

	}

	SetupDependency& SetupDependency::operator=(const SetupDependency& origin)
	{
		AbstractScheme::operator=(origin);
		_map = origin._map;

		return *this;
	}

	SetupDependency& SetupDependency::operator=(SetupDependency&& origin)
	{		
		AbstractScheme::operator=(origin);
		_map = std::move(origin._map);

		return *this;
	}

	bool SetupDependency::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{
		Core::Meta::Scheme<SetupDependency>::Iterator i(Core::Meta::Scheme<SetupDependency>::Iterate(qd));
		if (!Core::Meta::Scheme<SetupDependency>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}

	QSet<Core::Meta::Scheme<SetupDependency>::Id> SetupDependency::makeSchemeSet() const
	{
		QSet<Core::Meta::Scheme<SetupDependency>::Id> retval;
		const QString extension_pattern(getVendorName() + ".");
		for (Map::ConstIterator m = _map.constBegin(); m != _map.constEnd(); ++m) {
			retval.insert(
							Core::Meta::Scheme<SetupDependency>::Season
							(
								Qaos::String::MurMur3((extension_pattern + m.key().first).toLower()),
								m.value()
							)
			);
		}
		return retval;
	}

	const SetupDependency::Map& SetupDependency::getMap() const
	{
		return _map;
	}

	QHash<int, QByteArray> SetupDependency::roleNames() const
	{
		QHash<int, QByteArray> retval(AbstractScheme::roleNames());
		retval[Role::Set] = "set";

		return retval;
	}

	QVariant SetupDependency::data(int role) const
	{
		switch (role) {
			case Role::Set: return QVariant::fromValue(makeSchemeSet());

			default: return AbstractScheme::data(role);
		}
	}

} } } }
