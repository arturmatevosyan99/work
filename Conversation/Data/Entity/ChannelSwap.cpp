#include "ChannelSwap.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	ChannelSwap::ChannelSwap(Repository::Meta::ObjectId object_id, const Core::Meta::Qd& scheme_qd)
	:
		ChannelMessage(object_id),

		_scheme_qd(scheme_qd)
	{
		addPredicate(
					QSharedPointer<Core::Data::ConditionPredicate>::create
					(
						scheme,
						Core::Data::Field(Core::Data::Field::Type::Integer, _scheme_qd.toInt64()),
						Core::Data::ConditionPredicate::Condition::Equal
					)
		);
	}

	void ChannelSwap::resetCompoundPredicate(Core::Data::CompoundPredicate::Logic)
	{
		ChannelMessage::resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::And);
		addPredicate(
					QSharedPointer<Core::Data::ConditionPredicate>::create
					(
						scheme,
						Core::Data::Field(Core::Data::Field::Type::Integer, _scheme_qd.toInt64()),
						Core::Data::ConditionPredicate::Condition::Equal
					)
		);
	}

	const Core::Meta::Qd& ChannelSwap::getSchemeQd() const
	{
		return _scheme_qd;
	}
} } } }
