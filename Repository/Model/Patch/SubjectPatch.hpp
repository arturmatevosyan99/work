#ifndef Cadabra_Queen_Repository_Model_Patch_SubjectPatch_hpp
#define Cadabra_Queen_Repository_Model_Patch_SubjectPatch_hpp

#include "../AbstractPatch.hpp"

#include "../Map/SubjectMap.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class SubjectPatch : public AbstractPatch
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
		SubjectPatch();
		virtual ~SubjectPatch() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool map(SubjectMap* target_map);
		Resolution stash(Meta::Subject::Operation operation, Meta::Subject::Id target_id, const LogStamp& logstamp, const QJsonValue& meta);

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
