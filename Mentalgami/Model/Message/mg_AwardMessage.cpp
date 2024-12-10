#include "mg_AwardMessage.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	AwardMessage::AwardMessage(QObject* parent)
	:
		Conversation::Model::CommandMessage(parent) /// < 588829656820482052
	{

	}

	AwardMessage::~AwardMessage()
	{

	}

	Repository::Meta::Subject::Id AwardMessage::getSubjectId() const
	{
		return !getMentionSet().isEmpty() ? *(getMentionSet().constBegin()) : Repository::Meta::Subject::DummyId;
	}

	Core::Meta::Qd AwardMessage::getSubjectQd() const
	{
		return !getMentionSet().isEmpty() ? *(getMentionSet().constBegin()) : Repository::Meta::Subject::DummyId;
	}

	const QString& AwardMessage::getCRN() const
	{
		return getOutput();
	}

	const QString& AwardMessage::getMotive() const
	{
		return getAction();
	}
} } } }
