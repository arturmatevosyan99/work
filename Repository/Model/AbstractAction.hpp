#ifndef Cadabra_Queen_Repository_Model_AbstractAction_hpp
#define Cadabra_Queen_Repository_Model_AbstractAction_hpp

#include "../../Core/Model/AbstractEvent.hpp"
#include "../Meta/Essence.hpp"

#include <QJsonValue>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {	
	class AbstractAction : public Core::Model::AbstractEvent
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
		explicit AbstractAction();
		AbstractAction(const AbstractAction& action);
		AbstractAction(AbstractAction&& action);
		virtual ~AbstractAction();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		//Meta::RealmTarget _target;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(const QJsonValue& payload);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
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

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
