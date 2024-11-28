#include "ObjectPatch.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	ObjectPatch::ObjectPatch()
	:
		AbstractPatch()
	{

	}

	ObjectPatch::~ObjectPatch()
	{

	}

	bool ObjectPatch::map(ObjectMap* target_map)
	{
		return !target_map ? false : AbstractPatch::map(target_map);
	}

	ObjectPatch::Resolution ObjectPatch::stash(Meta::Object::Operation operation, Meta::Object::Id target_id, const LogStamp& logstamp, const QJsonValue& meta)
	{
		if (Meta::Object::Operation::Tag == operation) {
			return AbstractPatch::stash(target_id, logstamp, meta.toArray());
		} else if (Meta::Object::Operation::Touch == operation) {
			if (Meta::Essence::TouchTime::Modification != static_cast<Meta::Essence::TouchTime>(meta.toObject().value("access").toInt(static_cast<int>(Meta::Essence::TouchTime::Modification)))) {
				return Resolution::Ignore;
			}
		}

		Diff& diff(target(target_id, logstamp));
		if (diff.ignore) {
			return Resolution::Ignore;
		}

		switch (operation) {
			case Meta::Object::Operation::Emplacement:
				diff.property_map = meta.toObject();
				break;

			case Meta::Object::Operation::Movement:
				diff.property_map["parent_id"] = meta.toString("0").toLongLong();
				break;

			case Meta::Object::Operation::Refusal:
			case Meta::Object::Operation::Destruction:
				diff.property_map["_"] = true;
				break;

			case Meta::Object::Operation::Picture:
				diff.property_map["picture"] = meta.toString("");
				break;

			case Meta::Object::Operation::Permit:
				diff.property_map["permission"] = meta.toObject({});
				break;

			case Meta::Object::Operation::Renaming:
				diff.property_map["localization"] = false;
				diff.property_map["name"] = meta.toString();
				if (diff.property_map["name"].toString().at(0) == '%') {
					diff.property_map["name"] = diff.property_map.value("name").toString().mid(1);
					diff.property_map["localization"] = true;
				}
				break;

			case Meta::Object::Operation::Flush:
			case Meta::Object::Operation::Touch:
			case Meta::Object::Operation::Link:
			case Meta::Object::Operation::Attribute:
				return Resolution::Accept;

			default:
				return Resolution::Unknown;
		}
		return Resolution::Accept;
	}

	ObjectPatch::Resolution ObjectPatch::stash(const LogStamp& logstamp, const QJsonObject& object, const QJsonValue& meta)
	{
		return stash(
						static_cast<Meta::Object::Operation>(object.value("operation").toInt(static_cast<int>(Meta::Object::Operation::Flush))),
						object.value("object_id").toString("0").toLongLong(),
						logstamp,
						meta
		);
	}
} } } }
