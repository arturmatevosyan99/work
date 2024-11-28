#ifndef Cadabra_Queen_Conversation_Model_Action_SyncAction_hpp
#define Cadabra_Queen_Conversation_Model_Action_SyncAction_hpp

#include "../../../Repository/Model/AbstractAction.hpp"

#include "BroadcastAction.hpp"
#include "../../Meta/Message.hpp"

#include <QJsonArray>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class SyncAction : public Repository::Model::AbstractAction
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const Meta::Message::Id ZeroId;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SyncAction();
		SyncAction(const SyncAction& origin);
		SyncAction(SyncAction&& origin);
		virtual ~SyncAction() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
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

	/** @name Setters */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
