#include "Revision2Setup.hpp"

#include "../Entity/Session.hpp"

#include "../../../Core/Data/Factory.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Revision2Setup::Revision2Setup()
	:
		AbstractSetup()
	{

	}

	bool Revision2Setup::go(Core::Data::Provider& data_provider) const
	{
		Data::Session session_data;

		Core::Data::Factory data_factory(data_provider.makeFactory());
		return (
				data_factory.create(&session_data, false)
		);
	}
} } } }
