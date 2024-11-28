#include "../DrOblozhko.hpp"

#include "../../Core/Meta/Scheme/TestScheme.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko {
	bool Extension::registerTestCase()
	{
		Core::Meta::Scheme<Core::Meta::TestScheme>::Salt salt(0);
//		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<DrOblozhko::Test::>(salt++, {"DrOblozhko", "", Core::TestVariation::});

		return true;
	}
} } }
