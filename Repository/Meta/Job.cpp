/**
 @file Job.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Job.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	Job::Job(JobId id)
	:
		_id(id)
	{

	}

	Job::Job(const Job& origin)
	:
		_id(origin._id)
	{

	}

	Job::Job(Job&& origin)
	:
		_id(std::move(origin._id))
	{

	}

	const JobId& Job::getId() const
	{
		return _id;
	}

	Job::operator Meta::JobId() const
	{
		return _id;
	}
} } } }
