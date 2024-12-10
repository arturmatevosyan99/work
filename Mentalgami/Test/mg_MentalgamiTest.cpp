#include "../Mentalgami.hpp"

#include "Case/mg_DateCase.hpp"

#include "../../Core/Meta/Scheme/TestScheme.hpp"
#include "../../Core/Test/Case/GeneralCase.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami {
	bool Extension::registerTestCase()
	{
		Core::Meta::Scheme<Core::Meta::TestScheme>::Salt salt(0);
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Mentalgami::Test::DateCase>(salt++, {"Mentalgami", "DateCase", Core::TestVariation::Unit});

		return true;
	}
} } }
