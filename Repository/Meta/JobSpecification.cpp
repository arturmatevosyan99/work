/**
 @file JobSpecification.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "JobSpecification.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	JobSpecification::JobSpecification()
	:
		Core::Meta::AbstractScheme()
	{

	}

	JobSpecification::JobSpecification(const QString& extname, const QString& modelname)
	:
		Core::Meta::AbstractScheme(extname, modelname)
	{

	}

	JobSpecification::JobSpecification(const JobSpecification& origin)
	:
		Core::Meta::AbstractScheme(origin)
	{

	}

	JobSpecification::JobSpecification(JobSpecification&& origin)
	:
		Core::Meta::AbstractScheme(origin)
	{

	}

	JobSpecification& JobSpecification::operator=(const JobSpecification& origin)
	{
		AbstractScheme::operator=(origin);
		return *this;
	}

	JobSpecification& JobSpecification::operator=(JobSpecification&& origin)
	{		
		AbstractScheme::operator=(origin);
		return *this;
	}

	bool JobSpecification::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{		
		Core::Meta::Scheme<JobSpecification>::Iterator i(Core::Meta::Scheme<JobSpecification>::Iterate(qd));
		if (!Core::Meta::Scheme<JobSpecification>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}
} } } }
