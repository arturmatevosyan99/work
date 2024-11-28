#include "../Conversation.hpp"

#include "../../Core/Meta/Scheme/TestScheme.hpp"
#include "../../Core/Test/Case/GeneralCase.hpp"

namespace Cadabra { namespace Queen { namespace Conversation {
	bool Extension::registerTestCase()
	{
		Core::Meta::Scheme<Core::Meta::TestScheme>::Salt salt(0);
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Conversation", "GroupChannelTest", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});

		return true;
	}
} } }
