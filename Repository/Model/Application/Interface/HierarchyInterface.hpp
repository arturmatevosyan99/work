#ifndef Cadabra_Queen_Repository_Model_Application_Interface_HierarchyInterface_hpp
#define Cadabra_Queen_Repository_Model_Application_Interface_HierarchyInterface_hpp

#include "../AbstractInterface.hpp"

#include "../../../Meta/Object.hpp"

#include <QPair>
#include <QString>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class HierarchyInterface : public virtual Repository::Model::AbstractInterface
	{
	/** @name Statics */
	/** @{ */
	public:
		static const quint16 LogLimit;
		static const quint16 PullLimit;
	/** @} */

	/** @name Aliases */
	/** @{ */
	protected:
		using LogStamp = QPair<QString, QString>;
		using Map = QMap<Meta::Object::Id, QSharedPointer<const Meta::Object>>;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const LogStamp NilStamp;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		HierarchyInterface() = default;
		virtual ~HierarchyInterface() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Core::Net::CRPDelay log(Meta::Essence::Target target, const LogStamp& log_stamp, quint16 limit = HierarchyInterface::LogLimit);
		Core::Net::CRPDelay flush(Meta::Essence::Target target);
		Core::Net::CRPDelay pull(Meta::Essence::Target target, Meta::Essence::Id offset_id = 9, quint16 limit = HierarchyInterface::PullLimit);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
/*
		virtual void onLogged(const Map& map);
*/

	private:
		void onLogged(const Core::Net::CRPReaction& reaction);
		void onFlushed(const Core::Net::CRPReaction& reaction);
	/** @} */
	};
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::Repository::Model::HierarchyInterface, "cadabra.queen.repository.model.hierarchy/1.0");

#endif
