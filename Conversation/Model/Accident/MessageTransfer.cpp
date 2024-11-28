#include "MessageTransfer.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	MessageTransfer::MessageTransfer()
	:
		AbstractAccident(),

		_flag(Core::Net::ServiceCall::ObserverFlag::Finish),
		_processed_size(0),
		_total_size(0)
	{

	}

	MessageTransfer::MessageTransfer(const QUuid& uid)
	:
		AbstractAccident(),

		_uid(uid),
		_flag(Core::Net::ServiceCall::ObserverFlag::Finish),
		_processed_size(1),
		_total_size(1)
	{

	}

	MessageTransfer::MessageTransfer(const QUuid& uid, Core::Net::ServiceCall::FaultFlag flag)
	:
		AbstractAccident(),

		_uid(uid),
		_flag(static_cast<Core::Net::ServiceCall::ObserverFlag>(flag)),
		_processed_size(0),
		_total_size(0)
	{

	}

	MessageTransfer::MessageTransfer(const QUuid& uid, Core::Net::ServiceCall::ObserverFlag flag, qint64 processed_size, qint64 total_size)
	:
		AbstractAccident(),

		_uid(uid),
		_flag(flag),
		_processed_size(processed_size),
		_total_size(total_size)
	{

	}

	MessageTransfer::MessageTransfer(const MessageTransfer& origin)
	:
		AbstractAccident(origin),

		_uid(origin._uid),
		_flag(origin._flag),
		_processed_size(origin._processed_size),
		_total_size(origin._total_size)
	{

	}

	MessageTransfer::MessageTransfer(MessageTransfer&& origin)
	:
		AbstractAccident(std::move(origin)),

		_uid(std::move(origin._uid)),
		_flag(std::move(origin._flag)),
		_processed_size(std::move(origin._processed_size)),
		_total_size(std::move(origin._total_size))
	{
		_uid = QUuid();
		_flag = Core::Net::ServiceCall::ObserverFlag::Finish;
		_processed_size = 0;
		_total_size = 0;
	}

	MessageTransfer::~MessageTransfer()
	{

	}

	const QUuid& MessageTransfer::getUId() const
	{
		return _uid;
	}

	const Core::Net::ServiceCall::ObserverFlag& MessageTransfer::getFlag() const
	{
		return _flag;
	}

	const qint64& MessageTransfer::getProcessedSize() const
	{
		return _processed_size;
	}

	const qint64& MessageTransfer::getTotalSize() const
	{
		return _total_size;
	}

	int MessageTransfer::getProgress() const
	{
		switch (_flag) {
			case Core::Net::ServiceCall::ObserverFlag::Finish:
				return 100;

			case Core::Net::ServiceCall::ObserverFlag::Error:
				return -1;

			case Core::Net::ServiceCall::ObserverFlag::SSLError:
				return -2;

			case Core::Net::ServiceCall::ObserverFlag::Download:
			case Core::Net::ServiceCall::ObserverFlag::Upload:
				return getProgressPercent();

			default:
				return 0;
		}
	}

	double MessageTransfer::getProgressValue() const
	{
		return _total_size <= 0 ? .0 : (static_cast<double>(_processed_size) / static_cast<double>(_total_size));
	}

	int MessageTransfer::getProgressPercent() const
	{
		return getProgressValue() * 100;
	}

	bool MessageTransfer::isFaulty() const
	{
		return Core::Net::ServiceCall::ObserverFlag::Error == _flag || Core::Net::ServiceCall::ObserverFlag::SSLError == _flag;
	}

	bool MessageTransfer::isLast() const
	{
		return isFaulty() || Core::Net::ServiceCall::ObserverFlag::Finish == _flag;
	}
} } } }
