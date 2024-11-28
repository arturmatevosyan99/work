#ifndef Cadabra_Queen_Conversation_Model_Accident_MessageTransfer_hpp
#define Cadabra_Queen_Conversation_Model_Accident_MessageTransfer_hpp

#include "../AbstractAccident.hpp"

#include "../../../Core/Net/ServiceCall.hpp"

#include <QUuid>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class MessageTransfer : public AbstractAccident
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
		MessageTransfer();
		MessageTransfer(const QUuid& uid);
		MessageTransfer(const QUuid& uid, Core::Net::ServiceCall::FaultFlag flag);
		MessageTransfer(const QUuid& uid, Core::Net::ServiceCall::ObserverFlag flag, qint64 processed_size, qint64 total_size);
		MessageTransfer(const MessageTransfer& origin);
		MessageTransfer(MessageTransfer&& origin);
		virtual ~MessageTransfer() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QUuid _uid;
		Core::Net::ServiceCall::ObserverFlag _flag;
		qint64 _processed_size;
		qint64 _total_size;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QUuid& getUId() const;
		const Core::Net::ServiceCall::ObserverFlag& getFlag() const;
		const qint64& getProcessedSize() const;
		const qint64& getTotalSize() const;

		int getProgress() const;
		double getProgressValue() const;
		int getProgressPercent() const;
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
		bool isFaulty() const;
		bool isLast() const;
	/** @} */
	};
} } } }

#endif
