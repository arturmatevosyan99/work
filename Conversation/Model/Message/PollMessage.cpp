#include "PollMessage.hpp"

#include <QJsonObject>
#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	PollMessage::PollMessage(QObject* parent)
	:
		AbstractMessage(parent),

		_sync_timer_id(0)
	{

	}

	QString PollMessage::opt(int index) const
	{
		return (index < 0 || index >= _option_map.size()) ? "" : (_option_map.constBegin() + index).key();
	}

	QString PollMessage::describe(int index) const
	{
		return (index < 0 || index >= _option_map.size()) ? "" : (_option_map.constBegin() + index).value();
	}

	bool PollMessage::check(const QString& option) const
	{
		if (_exit_set.contains(option)) {
			return true;
		}

		QPair<VoteMap::ConstIterator, VoteMap::ConstIterator> range(_vote_map.equal_range(getChannelDescriptor()->getSubjectId()));
		while (range.first != range.second) {
			if (range.first->compare(option) == 0) {
				return true;
			}
			++range.first;
		}

		return false;
	}

	bool PollMessage::vote(const QString& option, bool auto_sync)
	{
		if (!_option_map.contains(option)) {
			return false;
		}

		if (isEligible() && (!isAnswered() || (isAnswered() && isEditableMode()))) {
			if (!isMultiplyMode() && !_exit_set.isEmpty()) {
				return false;
			}
			_exit_set.insert(option);
			return auto_sync ? sync() : true;
		}
		return false;
	}

	bool PollMessage::sync(bool timer)
	{
		if (!_exit_set.empty()) {
			_is_sync = true;
			if (_sync_timer_id > 0) {
				killTimer(_sync_timer_id);
				_sync_timer_id = 0;
			}
			if (timer) {
				_sync_timer_id = startTimer(20000);
			}
			emit voted();
			emit modified();
			return true;
		}
		return false;
	}

	QJsonObject PollMessage::exportBody() const
	{
		QJsonObject exit;
		if (!_exit_set.isEmpty()) {
			exit.insert("subject_id", QString::number(getChannelDescriptor()->getSubjectId()));
			exit.insert("option_array", QJsonArray::fromStringList(_exit_set.values()));
		}

		QJsonArray vote_array;
		for (VoteMap::ConstIterator a = _vote_map.constBegin(); a != _vote_map.constEnd(); ++a) {
			vote_array.push_back(
									QJsonObject(
												{
													{"subject_id", QString::number(a.key())},
													{"option", a.value()}
												}
									)
			);
		}

		QJsonObject option_map;
		for (OptionMap::ConstIterator o = _option_map.constBegin(); o != _option_map.constEnd(); ++o) {
			option_map.insert(o.key(), o.value());
		}

		QJsonArray eligible_set;
		for (const Repository::Meta::Subject::Id subject_id : _eligible_set) {
			eligible_set.push_back(QString::number(subject_id));
		}

		QJsonArray rule_vector;
		for (const QPair<ElectionRule, QSet<QString>>& rule : _rule_vector) {
			rule_vector.push_back(
									QJsonObject(
												{
													{"election_rule", static_cast<char>(rule.first)},
													{"option_array", QJsonArray::fromStringList(rule.second.values())}
												}
									)
			);
		}

		return QJsonObject({
							{"question", _question},
							{"editable_mode", _editable_mode},
							{"display_mode", static_cast<char>(_display_mode)},
							{"option_map", option_map},
							{"eligible_array", eligible_set},
							{"rule_array", rule_vector},
							{"vote_array", vote_array},
							{"exit", exit}
		});
	}

	bool PollMessage::importBody(const QJsonObject& body)
	{
		if (_sync_timer_id > 0) {
			killTimer(_sync_timer_id);
			_sync_timer_id = 0;
		}

		_exit_set.clear();
		_vote_map.clear();
		_option_map.clear();
		_eligible_set.clear();
		_rule_vector.clear();

		_question = body.value("question").toString("?");
		_editable_mode = body.value("editable_mode").toBool(false);
		_display_mode = static_cast<DisplayMode>(body.value("display_mode").toString("M").at(0).toLatin1());

		QJsonArray vote_map(body.value("vote_array").toArray());
		for (QJsonArray::const_iterator a = vote_map.constBegin(); a != vote_map.constEnd(); ++a) {
			if (a->isObject()) {
				const QJsonObject answer(a->toObject());
				_vote_map.insert(
									answer.value("subject_id").toString("0").toLongLong(),
									answer.value("option").toString()
				);
			}
		}

		QJsonObject option_map(body.value("option_map").toObject());
		for (QJsonObject::const_iterator o=option_map.constBegin(); o != option_map.constEnd(); ++o) {
			_option_map.insert(o.key(), o.value().toString(""));
		}

		QJsonArray eligible_array(body.value("eligible_array").toArray());
		for (QJsonArray::const_iterator e = eligible_array.constBegin(); e != eligible_array.constEnd(); ++e) {
			_eligible_set.insert(e->toString("0").toLongLong());
		}

		QJsonArray rule_array(body.value("rule_array").toArray());
		for (QJsonArray::const_iterator r = rule_array.constBegin(); r != rule_array.constEnd(); ++r) {
			if (r->isObject()) {
				const QJsonObject rule(r->toObject());
				const QJsonArray option_array(rule.value("option_array").toArray());
				QSet<QString> option_set;
				for (QJsonArray::const_iterator ro = option_array.constBegin(); ro != option_array.constEnd(); ++ro) {
					const QString rule_option(ro->toString(""));
					if (_option_map.contains(rule_option)) {
						option_set.insert(rule_option);
					}
				}
				_rule_vector.push_back(
										qMakePair(
													static_cast<ElectionRule>(rule.value("election_rule").toString("N").at(0).toLatin1()),
													option_set
										)
				);
			}
		}

		if (_is_sync || !_vote_map.empty()) {
			_is_sync = false;
			emit voted();
		}

		return true;
	}

	QString PollMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Poll"));
	}

	const QString& PollMessage::getQuestion() const
	{
		return _question;
	}

	PollMessage::DisplayMode PollMessage::getDisplayMode() const
	{
		return _display_mode;
	}

	int PollMessage::getOptionCount() const
	{
		return _option_map.size();
	}

	const PollMessage::OptionMap& PollMessage::getOptionMap() const
	{
		return _option_map;
	}

	const PollMessage::VoteMap& PollMessage::getVoteMap() const
	{
		return _vote_map;
	}

	const PollMessage::RuleVector& PollMessage::getRuleVector() const
	{
		return _rule_vector;
	}

	const QSet<Repository::Meta::Subject::Id>& PollMessage::getEligibleSet() const
	{
		return _eligible_set;
	}

	void PollMessage::timerEvent(QTimerEvent* event)
	{
		if (event->timerId() == _sync_timer_id) {
			killTimer(_sync_timer_id);
			_sync_timer_id = 0;
			if (_is_sync) {
				_is_sync = false;
				emit voted();
			}
		}
	}

	bool PollMessage::isEligible() const
	{
		return _eligible_set.empty() || _eligible_set.contains(getChannelDescriptor()->getSubjectId());
	}

	bool PollMessage::isAnswered() const
	{
		return hasChannelDescriptor() ? _vote_map.contains(getChannelDescriptor()->getSubjectId()) : false;
	}

	bool PollMessage::isVotable() const
	{
		if (_is_sync || !isEligible()) {
			return false;
		}
		return isEditableMode() || !isAnswered();
	}

	bool PollMessage::isEditableMode() const
	{
		return _editable_mode;
	}

	bool PollMessage::isMultiplyMode() const
	{
		for (const QPair<PollMessage::ElectionRule, QSet<QString>>& rule : _rule_vector) {
			if (rule.first == PollMessage::ElectionRule::SomeExcept || rule.first == PollMessage::ElectionRule::SomeOf || rule.first == PollMessage::ElectionRule::AllOf) {
				return true;
			}
		}
		return false;
	}
} } } }
