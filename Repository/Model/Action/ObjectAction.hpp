#ifndef Cadabra_Queen_Repository_Model_Action_ObjectAction_hpp
#define Cadabra_Queen_Repository_Model_Action_ObjectAction_hpp

#include "EssenceAction.hpp"

#include "../../Meta/Object.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class ObjectAction : public EssenceAction
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		using Incident = Meta::Object::Operation;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectAction();
		ObjectAction(const ObjectAction& origin);
		ObjectAction(ObjectAction&& origin);
		virtual ~ObjectAction() override = default;
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
		const ObjectAction::Incident& getIncident() const;
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
