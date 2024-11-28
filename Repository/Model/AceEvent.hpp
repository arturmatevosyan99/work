#ifndef Cadabra_Queen_Repository_Model_AceEvent_hpp
#define Cadabra_Queen_Repository_Model_AceEvent_hpp

#include "../../Core/Model/Event.hpp"

#include "Realm.hpp"
#include "AbstractAction.hpp"
#include "../Meta/Object.hpp"

#include <QEvent>
#include <QMetaType>
#include <QSharedPointer>
#include <QJsonValue>
#include <type_traits>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class AceEvent : public Core::Model::Event<AbstractAction>
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit AceEvent(const QString& name, Meta::RealmTarget target);
		AceEvent(const AceEvent& origin);
		virtual ~AceEvent();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const Meta::RealmTarget _target;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool import(const QJsonValue& payload);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Meta::RealmTarget& getTarget() const;
	/** @} */
	};
} } } }

#endif
