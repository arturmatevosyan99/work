/**
 @file AbstractJob.cpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#include "AbstractJob.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AbstractJob::AbstractJob(const Meta::Job::Scheme scheme, QObject* parent)
	:
		QObject(parent),

		_scheme(scheme),
		_id(-1),
		_state(JobState::IdleJobState),
		_session(nullptr),
		_error()
	{
	}

	AbstractJob::~AbstractJob()
	{

	}

	void AbstractJob::plan(Session* session)
	{
		if (JobState::IdleJobState != _state) {
			return ;
		} else if (nullptr == session) {
		   _state = JobState::FaultJobState;
		   _error = "Unexpected plan error";
		} else {
			_id = 0;
			_session = session;
			_state = JobState::PlanJobState;
		}
		emit stateChanged();
	}


	bool AbstractJob::probe()
	{
		if (nullptr == _session) {
			return false;
		}

		return true;
		/// @todo Уточнить у ace состояние
		/// сохранять _session при plan(Session* != nullptr)
	}

	void AbstractJob::fault(const QString& message)
	{
		if (_state != JobState::FaultJobState) {
			_state = JobState::FaultJobState;
			_error = message;
			emit stateChanged();
		}
	}

	void AbstractJob::complete(bool yesno)
	{
		if (JobState::ProgressJobState != _state) {
			return ;
		} else if (!yesno) {
			_error = "Unexpected completion error";
		   _state = JobState::FaultJobState;
		} else {
			_error.clear();
			_state = JobState::CompleteJobState;
		}
		emit stateChanged();
	}

	QJsonObject AbstractJob::makeEnvironment() const
	{
		return {};
	}

	const Meta::Job::Scheme& AbstractJob::getScheme() const
	{
		return _scheme;
	}

	const JobState& AbstractJob::getState() const
	{
		return _state;
	}

	const QString& AbstractJob::getError() const
	{
		return _error;
	}
} } } }
