#ifndef Cadabra_Queen_Repository_Model_Action_SubjectAction_hpp
#define Cadabra_Queen_Repository_Model_Action_SubjectAction_hpp

#include "EssenceAction.hpp"

#include "../../Meta/Subject.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class SubjectAction : public EssenceAction
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		using Incident = Meta::Subject::Operation;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SubjectAction();
		SubjectAction(const SubjectAction& origin);
		SubjectAction(SubjectAction&& origin);
		virtual ~SubjectAction() override = default;
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
		const SubjectAction::Incident& getIncident() const;
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
