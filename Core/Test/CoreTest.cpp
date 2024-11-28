#include "../Core.hpp"

#include "../Meta/Scheme.hpp"
#include "../Meta/Scheme/TestScheme.hpp"

#include <QtTest/QtTest>

namespace Cadabra { namespace Queen { namespace Core {
	bool Extension::registerTestCase()
	{
		qRegisterMetaType<Meta::TestScheme>("Cadabra::Queen::Core::Meta::TestScheme");
		Meta::SchemeFactory::Bind<Meta::TestScheme>("Core.Meta.TestScheme");

		return true;
	}
} } }
