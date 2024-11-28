/**
 @file JobSpecification.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_JobSpecification_hpp
#define Cadabra_Queen_Repository_Meta_JobSpecification_hpp

#include "../../Core/Meta/AbstractScheme.hpp"

#include "../Model/AbstractJob.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class JobSpecification : public Core::Meta::AbstractScheme
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using _ = Model::AbstractJob;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		JobSpecification();
		JobSpecification(const QString& extname, const QString& modelname);
		JobSpecification(const JobSpecification& origin);
		JobSpecification(JobSpecification&& origin);
		virtual ~JobSpecification() override = default;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
		JobSpecification& operator=(const JobSpecification& origin);
		JobSpecification& operator=(JobSpecification&& origin);
	/** @} */
		
	/** @name Properties */
	/** @{ */
	public:
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	private:
		Q_INVOKABLE virtual bool load(const Cadabra::Queen::Core::Meta::Qd& qd) override;
	/** @} */
		
	/** @name Converters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Formatters */
	/** @{ */
	public:
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

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::JobSpecification)

#endif
