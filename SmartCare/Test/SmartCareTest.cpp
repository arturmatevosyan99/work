#include "../SmartCare.hpp"

#include "Case/DateCase.hpp"

#include "../../Core/Meta/Scheme/TestScheme.hpp"
#include "../../Core/Test/Case/GeneralCase.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare {
	bool Extension::registerTestCase()
	{
		Core::Meta::Scheme<Core::Meta::TestScheme>::Salt salt(0);
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<SmartCare::Test::DateCase>(salt++, {"SmartCare", "DateCase", Core::TestVariation::Unit});

		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"SmartCare", "MarkdownViewTest", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});

		return true;
	}
} } }
