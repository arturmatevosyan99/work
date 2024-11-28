#include "GroupPatch.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	GroupPatch::GroupPatch()
	:
		AbstractPatch()
	{

	}

	GroupPatch::~GroupPatch()
	{

	}

	bool GroupPatch::map(GroupMap* target_map)
	{
		return !target_map ? false : AbstractPatch::map(target_map);
	}

	GroupPatch::Resolution GroupPatch::stash(Meta::Group::Operation operation, Meta::Group::Id target_id, const LogStamp& logstamp, const QJsonValue& meta)
	{
		if (Meta::Group::Operation::Tag == operation) {
			return AbstractPatch::stash(target_id, logstamp, meta.toArray());
		} else if (Meta::Group::Operation::Touch == operation) {
			if (Meta::Essence::TouchTime::Modification != static_cast<Meta::Essence::TouchTime>(meta.toObject().value("access").toInt(static_cast<int>(Meta::Essence::TouchTime::Modification)))) {
				return Resolution::Ignore;
			}
		}

		Diff& diff(target(target_id, logstamp));
		if (diff.ignore) {
			return Resolution::Ignore;
		}

		switch (operation) {
			case Meta::Group::Operation::Emplacement:
				diff.property_map = meta.toObject();
				break;

			case Meta::Group::Operation::Refusal:
			case Meta::Group::Operation::Dissolution:
				diff.property_map["_"] = true;
				break;

			case Meta::Group::Operation::Renaming:
				diff.property_map["name"] = meta.toString();
				break;

			case Meta::Group::Operation::Picture:
				diff.property_map["picture"] = meta.toString("");
				break;

			case Meta::Group::Operation::Description:
				diff.property_map["description"] = meta.toString();
				break;

			case Meta::Group::Operation::Flush:
			case Meta::Group::Operation::Touch:
			case Meta::Group::Operation::Attribute:
				return Resolution::Accept;

			default:
				return Resolution::Unknown;
		}
		return Resolution::Accept;
	}

	GroupPatch::Resolution GroupPatch::stash(const LogStamp& logstamp, const QJsonObject& object, const QJsonValue& meta)
	{
		return stash(
						static_cast<Meta::Group::Operation>(object.value("operation").toInt(static_cast<int>(Meta::Group::Operation::Flush))),
						object.value("group_id").toString("0").toLongLong(),
						logstamp,
						meta
		);
	}
} } } }
