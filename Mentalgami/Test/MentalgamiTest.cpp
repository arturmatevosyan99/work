#include "../Mentalgami.hpp"

#include "Case/DateCase.hpp"

#include "../../Core/Meta/Scheme/TestScheme.hpp"
#include "../../Core/Test/Case/GeneralCase.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami {
	bool Extension::registerTestCase()
	{
		Core::Meta::Scheme<Core::Meta::TestScheme>::Salt salt(0);
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Mentalgami::Test::DateCase>(salt++, {"Mentalgami", "DateCase", Core::TestVariation::Unit});
        Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Mentalgami", "SmartBookTest", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});
		return true;
	}
} } }
