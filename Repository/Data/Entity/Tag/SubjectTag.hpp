#ifndef Cadabra_Queen_Repository_Data_Entity_Tag_SubjectTag_hpp
#define Cadabra_Queen_Repository_Data_Entity_Tag_SubjectTag_hpp

#include "../AbstractTag.hpp"

#include "../Record/Subject.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class SubjectTag : public AbstractTag
	{
	/** @name Constructors */
	/** @{ */
	public:
		SubjectTag();
		virtual ~SubjectTag() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Core::Data::ForeignEntity<Subject> target;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual void onSelect() override;
		virtual void onCreate() override;
		virtual void onForeign() override;
	/** @} */
	};
} } } }

#endif
