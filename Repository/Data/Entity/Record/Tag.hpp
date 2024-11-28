#ifndef Cadabra_Queen_Repository_Data_Entity_Record_Tag_hpp
#define Cadabra_Queen_Repository_Data_Entity_Record_Tag_hpp

#include "../AbstractRecord.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Tag : public AbstractRecord
	{
	/** @name Constructors */
	/** @{ */
	public:
		Tag();
		virtual ~Tag() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field timestamp;
		Field alias;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	protected:
		virtual QSharedPointer<AbstractTag> makeTag() const override;
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
