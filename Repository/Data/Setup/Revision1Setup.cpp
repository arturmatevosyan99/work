#include "Revision1Setup.hpp"

#include "../Entity/Sync.hpp"
#include "../Entity/Record/Object.hpp"
#include "../Entity/Record/Group.hpp"
#include "../Entity/Record/Subject.hpp"
#include "../Entity/Record/Tag.hpp"
#include "../Entity/Tag/ObjectTag.hpp"
#include "../Entity/Tag/GroupTag.hpp"
#include "../Entity/Tag/SubjectTag.hpp"

#include "../../../Core/Data/Factory.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	Revision1Setup::Revision1Setup()
	:
		AbstractSetup()
	{

	}

	bool Revision1Setup::go(Core::Data::Provider& data_provider) const
	{
		Data::Object object_data;
		Data::Sync sync_data;
		Data::Group group_data;
		Data::Subject subject_data;

		Data::Tag tag_data;
		Data::ObjectTag object_tag_data;
		Data::GroupTag group_tag_data;
		Data::SubjectTag subject_tag_data;

		Core::Data::Factory data_factory(data_provider.makeFactory());
		return (
				data_factory.create(&sync_data, false)
				&&
				data_factory.create(&object_data, false)
				&&
				data_factory.create(&group_data, false)
				&&
				data_factory.create(&subject_data, false)
				&&
				data_factory.create(&tag_data, false)
				&&
				data_factory.create(&object_tag_data, false)
				&&
				data_factory.create(&group_tag_data, false)
				&&
				data_factory.create(&subject_tag_data, false)
		);
	}
} } } }
