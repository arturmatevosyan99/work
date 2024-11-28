#ifndef Cadabra_Queen_Repository_Repository_hpp
#define Cadabra_Queen_Repository_Repository_hpp

#include "../Bone.hpp"

#include "../Core/Meta/Scheme.hpp"
#include "../Core/System/NotificationCenter.hpp"

#include <QMap>

namespace Cadabra { namespace Queen { namespace Repository {
	class Extension : public Queen::Bone
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
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
		Extension();
		virtual ~Extension() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */	
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool registerMetaObject() override;
		virtual bool registerMetaType() override;
		virtual bool registerMetaTypeConverter() override;
		virtual bool registerMetaTypeStreamOperator() override;
		virtual bool registerQMLType() override;
		virtual bool registerAceAction() override;
		virtual bool registerAceScheme() override;
		virtual bool registerQMLSingletonType() override;
		virtual bool tweakRootContext(QQmlContext& context) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onAttach(Skeleton& skeleton) override;
		virtual void onDetach(Skeleton& skeleton) override;
		virtual void onPreDisclose(Cupboard& cupboard) override;
		virtual void onPostDisclose(Cupboard& cupboard) override;

		void onTokenRegistered(QString token);
	/** @} */

#ifdef QT_TESTLIB_LIB
	/** @name Procedures */
	/** @{ */
	public:
		virtual bool registerTestCase() override;
	/** @} */
#endif
	};
} } }

namespace Cadabra { namespace Queen { namespace Repository { Q_NAMESPACE
	enum EssenceIncident : int
	{
		AlterIncident = 0x01
	};
	Q_DECLARE_FLAGS(EssenceIncidentFlags, EssenceIncident)
	Q_FLAG_NS(EssenceIncidentFlags)

	enum EssenceAlter : int
	{
		InsertAlter = 0x01,
		UpdateAlter = 0x02,
		DeleteAlter = 0x40000000
	};
	Q_DECLARE_FLAGS(EssenceAlterFlags, EssenceAlter)
	Q_FLAG_NS(EssenceAlterFlags)

	enum class JobState : char
	{
		IdleJobState = 'I',
		PlanJobState = 'p',
		ProgressJobState = 'P',
		CompleteJobState = 'D',
		FaultJobState = 'F'
	};
	Q_ENUM_NS(JobState)

	enum class ProxyIndex : int
	{
		MapIterator = 0x01,
		DataRecord = 0x02,
		NetCursor = 0x04,
		Custom = 0x08
	};
	Q_DECLARE_FLAGS(ProxyIndexFlags, ProxyIndex)
	Q_FLAG_NS(ProxyIndexFlags)
} } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Repository::EssenceIncidentFlags)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::EssenceIncidentFlags)

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Repository::EssenceAlterFlags)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::EssenceAlterFlags)

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Repository::ProxyIndexFlags)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::ProxyIndexFlags)

#endif
