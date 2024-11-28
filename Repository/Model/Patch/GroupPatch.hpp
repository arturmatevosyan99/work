#ifndef Cadabra_Queen_Repository_Model_Patch_GroupPatch_hpp
#define Cadabra_Queen_Repository_Model_Patch_GroupPatch_hpp

#include "../AbstractPatch.hpp"

#include "../Map/GroupMap.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class GroupPatch : public AbstractPatch
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
		GroupPatch();
		virtual ~GroupPatch() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool map(GroupMap* target_map);
		Resolution stash(Meta::Group::Operation operation, Meta::Group::Id target_id, const LogStamp& logstamp, const QJsonValue& meta);

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
