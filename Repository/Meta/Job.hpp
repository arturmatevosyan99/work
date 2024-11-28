/**
 @file Job.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_Job_hpp
#define Cadabra_Queen_Repository_Meta_Job_hpp

#include "Essence.hpp"

#include <QString>
namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Job
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Id = JobId;
		using Scheme = qint64;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Job(JobId id);
		Job(const Job& origin);
		Job(Job&& origin);
		virtual ~Job() = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const JobId _id;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const JobId& getId() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator Meta::JobId() const;
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
