#ifndef Cadabra_Queen_Repository_Data_Entity_Record_TaggedObject_hpp
#define Cadabra_Queen_Repository_Data_Entity_Record_TaggedObject_hpp

#include "Object.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class TaggedObject : public Object
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TaggedObject();
		virtual ~TaggedObject() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field tag_counter;
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
