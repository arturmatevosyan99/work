#ifndef Cadabra_Queen_Repository_Model_Patch_ObjectPatch_hpp
#define Cadabra_Queen_Repository_Model_Patch_ObjectPatch_hpp

#include "../AbstractPatch.hpp"

#include "../Map/ObjectMap.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class ObjectPatch : public AbstractPatch
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectPatch();
		virtual ~ObjectPatch() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool map(ObjectMap* target_map);
		Resolution stash(Meta::Object::Operation operation, Meta::Object::Id target_id, const LogStamp& logstamp, const QJsonValue& meta);

	protected:
		virtual Resolution stash(const LogStamp& logstamp, const QJsonObject& object, const QJsonValue& meta) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
