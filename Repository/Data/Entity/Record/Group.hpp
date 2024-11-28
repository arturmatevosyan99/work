#ifndef Cadabra_Queen_Repository_Data_Entity_Record_Group_hpp
#define Cadabra_Queen_Repository_Data_Entity_Record_Group_hpp

#include "../AbstractRecord.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Group : public AbstractRecord
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Group();
		virtual ~Group() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field name;
		Field description;
		Field picture;
		Field creation_timestamp;
		Field modification_timestamp;
		Field meta; /// @note local meta, not the meta from ace
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
