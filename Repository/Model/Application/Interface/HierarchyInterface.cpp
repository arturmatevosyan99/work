#include "HierarchyInterface.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	const quint16 HierarchyInterface::LogLimit = 500;
	const quint16 HierarchyInterface::PullLimit = 500;
	const HierarchyInterface::LogStamp HierarchyInterface::NilStamp = {QString("0000x0000000000000000"), QString("0000-00-00 00:00:00.000000")};

	Core::Net::CRPDelay HierarchyInterface::log(Meta::Essence::Target target, const LogStamp& log_stamp, quint16 limit)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::HierarchyInterface",
			"Cadabra::Ace::Repository::Model::RealmLog",
			{
				{"target", static_cast<char>(target)},
				{"acestamp", log_stamp.first},
				{"timestamp", log_stamp.second},
				{"limit", QString::number(limit)}
			}
		).detach(this, &HierarchyInterface::onLogged);
	}

	Core::Net::CRPDelay HierarchyInterface::flush(Meta::Essence::Target target)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::HierarchyInterface",
			"Cadabra::Ace::Repository::Model::RealmFlush",
			{
				{"target", static_cast<char>(target)}
			}
		).detach(this, &HierarchyInterface::onFlushed);
	}

	Core::Net::CRPDelay HierarchyInterface::pull(Meta::Essence::Target target, Meta::Essence::Id offset_id, quint16 limit)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::HierarchyInterface",
			"Cadabra::Ace::Repository::Model::RealmPull",
			{
				{"target", static_cast<char>(target)},
				{"offset", QString::number(offset_id)},
				{"limit", QString::number(limit)},
			}
		).detach(this, &HierarchyInterface::onFlushed);
	}

	void HierarchyInterface::onLogged(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			/// @log
			reaction.log("HierarchyInterface::onLogged");
			return ;
		} else if (!(*reaction).isObject()) {
			qWarning("Unknown data structure, could not log");
			return ;
		}
		// onLogged(Meta::Object::Parse(*reaction));
	}

	void HierarchyInterface::onFlushed(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			/// @log
			reaction.log("HierarchyInterface::onFlushed");
			return ;
		} else if (!(*reaction).isObject()) {
			qWarning("Unknown data structure, could not flush");
			return ;
		}
		// onLogged(Meta::Object::Parse(*reaction));
	}
} } } }
