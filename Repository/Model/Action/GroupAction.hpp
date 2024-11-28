#ifndef Cadabra_Queen_Repository_Model_Action_GroupAction_hpp
#define Cadabra_Queen_Repository_Model_Action_GroupAction_hpp

#include "EssenceAction.hpp"

#include "../../Meta/Group.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class GroupAction : public EssenceAction
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		using Incident = Meta::Group::Operation;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		GroupAction();
		GroupAction(const GroupAction& origin);
		GroupAction(GroupAction&& origin);
		virtual ~GroupAction() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:		
		Incident _incident;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(const QJsonValue& payload) override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual Meta::Essence::Target getTarget() const override;
		const GroupAction::Incident& getIncident() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
