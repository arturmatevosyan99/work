#include "SubjectPatch.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	SubjectPatch::SubjectPatch()
	:
		AbstractPatch()
	{

	}

	SubjectPatch::~SubjectPatch()
	{

	}

	bool SubjectPatch::map(SubjectMap* target_map)
	{
		return !target_map ? false : AbstractPatch::map(target_map);
	}

	SubjectPatch::Resolution SubjectPatch::stash(Meta::Subject::Operation operation, Meta::Subject::Id target_id, const LogStamp& logstamp, const QJsonValue& meta)
	{
		if (Meta::Subject::Operation::Tag == operation) {
			return AbstractPatch::stash(target_id, logstamp, meta.toArray());
		} else if (Meta::Subject::Operation::Touch == operation) {
			if (Meta::Essence::TouchTime::Modification != static_cast<Meta::Essence::TouchTime>(meta.toObject().value("access").toInt(static_cast<int>(Meta::Essence::TouchTime::Modification)))) {
				return Resolution::Ignore;
			}
		}

		Diff& diff(target(target_id, logstamp));
		if (diff.ignore) {
			return Resolution::Ignore;
		}
		switch (operation) {
			case Meta::Subject::Operation::Emplacement:
				diff.property_map = meta.toObject();
				break;

			case Meta::Subject::Operation::Refusal:
			case Meta::Subject::Operation::Cremation:
				diff.property_map["_"] = true;
				break;

			case Meta::Subject::Operation::Description:
				diff.property_map["description"] = meta.toString("");
				break;

			case Meta::Subject::Operation::Picture:
				diff.property_map["picture"] = meta.toString("");
				break;

			case Meta::Subject::Operation::Timeshift:
				diff.property_map["timezone"] = meta.toString("UTC");
				break;

			case Meta::Subject::Operation::Renaming:
				diff.property_map["name"] = meta.toString("");
				break;

			case Meta::Subject::Operation::Localization:
				diff.property_map["locale"] = meta.toString("en_US");
				break;

			case Meta::Subject::Operation::Flush:
			case Meta::Subject::Operation::Touch:
			case Meta::Subject::Operation::Attribute:
				return Resolution::Accept;

			default:
				return Resolution::Unknown;
		}
		return Resolution::Accept;
	}

	SubjectPatch::Resolution SubjectPatch::stash(const LogStamp& logstamp, const QJsonObject& object, const QJsonValue& meta)
	{
		return stash(
						static_cast<Meta::Subject::Operation>(object.value("operation").toInt(static_cast<int>(Meta::Subject::Operation::Flush))),
						object.value("subject_id").toString("0").toLongLong(),
						logstamp,
						meta
		);
	}
} } } }
