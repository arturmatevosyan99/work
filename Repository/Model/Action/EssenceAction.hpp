#ifndef Cadabra_Queen_Repository_Model_Action_EssenceAction_hpp
#define Cadabra_Queen_Repository_Model_Action_EssenceAction_hpp

#include "../AbstractAction.hpp"

#include <QDateTime>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class EssenceAction : public Repository::Model::AbstractAction
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		EssenceAction();
		EssenceAction(const EssenceAction& origin);
		EssenceAction(EssenceAction&& origin);
		virtual ~EssenceAction() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _acestamp;
		QString _timestamp;
		Meta::Essence::Id _target_id;
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
		const QString& getAceStamp() const;
		const QString& getTimeStamp() const;
		const Meta::Essence::Id& getTargetId() const;
		virtual Meta::Essence::Target getTarget() const = 0;
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
