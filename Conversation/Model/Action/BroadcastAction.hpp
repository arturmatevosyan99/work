#ifndef Cadabra_Queen_Conversation_Model_Action_BroadcastAction_hpp
#define Cadabra_Queen_Conversation_Model_Action_BroadcastAction_hpp

#include "../../../Repository/Model/AbstractAction.hpp"

#include "../../Meta/Message.hpp"

#include <QSharedPointer>
#include <QJsonValue>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class BroadcastAction : public Repository::Model::AbstractAction
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Incident : char
		{
			Unknown = 0,
			Push = 'P',
			Withdraw = 'W', /// @note withdraw = delete и на самом деле, это ведь всего лишь rererndering в 0?
			Receive = 'R',
			View = 'V',
			Update = 'U',
			Tag = 'T',
			Remessage = 'r'
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		BroadcastAction();
		BroadcastAction(const BroadcastAction& origin);
		BroadcastAction(BroadcastAction&& origin);
		virtual ~BroadcastAction() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Meta::Message::Id _log_id;
		Meta::Message::Id _message_id;
		Repository::Meta::Subject::Id _broadcaster_subject_id;
		int _timestamp;
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
		const Meta::Message::Id& getLogId() const;
		const Meta::Message::Id& getMessageId() const;
		const Repository::Meta::Subject::Id& getBroadcasterSubjectId() const;
		const BroadcastAction::Incident& getIncident() const;
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
