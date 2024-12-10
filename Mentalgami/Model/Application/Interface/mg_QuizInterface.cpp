#include "mg_QuizInterface.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	Core::Net::CRPDelay QuizInterface::name()
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Mentalgami::Model::QuizInterface",
			"Cadabra::Ace::Mentalgami::Model::Name",
			{
			}
		).detach(this, &QuizInterface::onName);
	}

	Core::Net::CRPDelay QuizInterface::ask(int limit, int offset)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Mentalgami::Model::QuizInterface",
			"Cadabra::Ace::Mentalgami::Model::Ask",
			{
				{"limit", limit},
				{"offset", offset}
			}
		).detach(this, &QuizInterface::onAsk);
	}

	Core::Net::CRPDelay QuizInterface::answer(AbstractRiddle* riddle)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Mentalgami::Model::QuizInterface",
			"Cadabra::Ace::Mentalgami::Model::Answer",
			{
				riddle->dump(false)
			}
		).detach(this, &QuizInterface::onAnswer);
	}

	void QuizInterface::onName(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return onNamed(reaction.getCode(), reaction.getDomain(), reaction.getMessage());
		} else if (!(*reaction).isString()) {
			return onNamed(0, 5, "Payload is not a string");
		}
		onNamed((*reaction).toString());
	}
	void QuizInterface::onNamed(quint8 domain, quint32 code, const QString& message)
	{

	}
	void QuizInterface::onNamed(const QString& retval)
	{

	}

	void QuizInterface::onAsk(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return onAsked(reaction.getCode(), reaction.getDomain(), reaction.getMessage());
		} else if (!(*reaction).isArray()) {
			return onAsked(0, 5, "Payload is not an array");
		}
		onAsked(*reaction);
	}
	void QuizInterface::onAsked(quint8 domain, quint32 code, const QString& message)
	{

	}
	void QuizInterface::onAsked(const QJsonValue& retval)
	{

	}

	void QuizInterface::onAnswer(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return onAnswered(reaction.getCode(), reaction.getDomain(), reaction.getMessage());
		} else if (!(*reaction).isArray()) {
			return onAnswered(0, 5, "Payload is not an array");
		}
		onAnswered(*reaction);
	}
	void QuizInterface::onAnswered(quint8 domain, quint32 code, const QString& message)
	{

	}
	void QuizInterface::onAnswered(const QJsonValue& riddles)
	{

	}
} } } }
