#include "GroupChannel.hpp"

#include "../Event.hpp"
#include "../Message/ProxyMessage.hpp"
#include "../Message/TextMessage.hpp"
#include "../Message/VoiceMessage.hpp"
#include "../Message/PictureMessage.hpp"
#include "../Message/AttachmentMessage.hpp"
#include "../Message/WithdrawalMessage.hpp"
#include "../Accident/MessageTransfer.hpp"
#include "../../Meta/MessageRender.hpp"
#include "../../../Core/Meta/Scheme.hpp"
#include "../../../Core/Data/Factory.hpp"
#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"

#include "../../../Qaos/MakeUnique.hpp"

#include <QJsonDocument>
#include <QJsonArray>
#include <QSet>
#include <QSqlField>
#include <QPair>
#include <QDateTime>
#include <QMimeDatabase>
#include <QCoreApplication>

#include <limits>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	GroupChannel::OffsetCompareFunctor GroupChannel::OffsetComparator(GroupChannel::FeedEnd end)
	{
		return FeedEnd::Head == end ? GroupChannel::GreaterOffset : GroupChannel::LessOffset;
	}

	bool GroupChannel::LessOffset(const Meta::Message::Id& lhs, const Meta::Message::Id& rhs)
	{
		static const std::less<Meta::Message::Id> comparator;
		return comparator(lhs, rhs);
	}

	bool GroupChannel::GreaterOffset(const Meta::Message::Id& lhs, const Meta::Message::Id& rhs)
	{
		static const std::greater<Meta::Message::Id> comparator;
		return comparator(lhs, rhs);
	}

	GroupChannel::GroupChannel(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent),

		_swap_message_id(0),
		_feed_message_id(0),

		_first_swap_message_id(0),
		_last_swap_message_id(0),

		_first_feed_message_id(0),
		_last_feed_message_id(0),

		_last_read_message_id(0),

		_pending_receive_message_id(0),
		_pending_read_message_id(0),

		_last_message(new ProxyMessage(nullptr))
	{
	}

	GroupChannel::~GroupChannel()
	{
	}

	bool GroupChannel::push(Cadabra::Queen::Conversation::Meta::Message message)
	{
		QUuid uid(QUuid::createUuid());

		QSharedPointer<Data::ChannelDraft> draft_data(new Data::ChannelDraft(getDescriptor()->getObjectId()));
		draft_data->uid.setValue(uid);
		draft_data->reply_id.setValue(message.getReplyId() > 0 ? QVariant(message.getReplyId()) : QVariant());
		draft_data->scheme.setValue(message.getSchemeIdent());
		draft_data->timestamp.setValue(QDateTime::currentDateTimeUtc());
		draft_data->body.setValue(QJsonDocument(message.getBody()));

		return push(draft_data, true);
	}

	bool GroupChannel::push(const QString& text)
	{
		if (text.isEmpty()) {
			return false;
		}

		TextMessage message;
		message.setText(text);
		return push(message.exportMeta());
	}

	bool GroupChannel::push(const QUrl& url)
	{
		qDebug() << "Push (local? " << url.isLocalFile() << ") file from:" << url.toString();
		QFile file;
		if (url.scheme().compare("fd") == 0) {
			if (!file.open(url.path().toInt(), QIODevice::ReadOnly, QFileDevice::AutoCloseHandle)) {
				return false;
			}
		} else {
			file.setFileName(url.isLocalFile() ? url.toLocalFile() : url.toString());
			if (!file.open(QIODevice::ReadOnly)) {
				return false;
			}
		}

		QByteArray byte_array(file.readAll());
		QBuffer buffer(&byte_array);
		if (!buffer.open(QIODevice::ReadOnly)) {
			return false;
		}
		return push(&buffer);
	}

	bool GroupChannel::push(QBuffer* attachment, GroupChannel::ContentType content_type)
	{
		if (!attachment || !attachment->seek(0)) {
			return false;
		}

		static const QMimeDatabase mime_database;
		const QMimeType mime_type(mime_database.mimeTypeForData(attachment->data()));

		const QUuid draft_uid(QUuid::createUuid());

		if (
			!getDescriptor()->getRealm()->getHierarchy()->cacheObjectFile
			(
				getDescriptor()->getObjectId(),
				draft_uid.toString(QUuid::StringFormat::WithoutBraces) + ".draft",
				attachment
			)
		) {
			qCritical() << "Could not fetch unique draft id, message will be lost in case of failure";
			return false;
		}

		QSharedPointer<Data::ChannelDraft> draft_data(new Data::ChannelDraft(getDescriptor()->getObjectId()));
		draft_data->uid.setValue(draft_uid);
		draft_data->timestamp.setValue(QDateTime::currentDateTimeUtc());
		if (mime_type.name().startsWith("image/")) {
			content_type = ContentType::ImageBinary;
		} else if (mime_type.name().startsWith("audio/")) {
			content_type = ContentType::AudioBinary;
		} else if (mime_type.name().startsWith("text/")) {
			content_type = ContentType::PlainText;
		}

		switch (content_type) {
			case ContentType::PlainText:
				return push(QString(attachment->data()));

			case ContentType::ImageBinary:
				draft_data->body.setValue(QJsonDocument(PictureMessage::Thumbnail(QImage::fromData(attachment->data())).exportMeta().getBody()));
				draft_data->scheme.setValue(Core::Meta::Scheme<Meta::MessageRender>::Ident<PictureMessage>());
				break;

			case ContentType::AudioBinary:
				draft_data->scheme.setValue(Core::Meta::Scheme<Meta::MessageRender>::Ident<VoiceMessage>());
				break;

			default:
				draft_data->scheme.setValue(Core::Meta::Scheme<Meta::MessageRender>::Ident<AttachmentMessage>());
		}

		const QSharedPointer<Repository::Net::ObjectFile> file(makeFile());
		if (!track(draft_data, file)) {
			return false;
		}

		QObject::connect
		(
			file.get(),
			qOverload<QString>(&Repository::Net::ObjectFile::uploaded),

			this,
			qOverload<QString>(&GroupChannel::onPushed)
		);
		QObject::connect
		(
			file.get(),
			qOverload<QNetworkReply::NetworkError, QString>(&Repository::Net::ObjectFile::faulted),

			this,
			std::bind
			(
				(void(GroupChannel::*)(QNetworkReply::NetworkError, QString, QUuid))&GroupChannel::onPushed,
				this,
				std::placeholders::_1, std::placeholders::_2, draft_uid
			)
		);

		file->upload(attachment, draft_uid.toString(QUuid::StringFormat::WithoutBraces));
		return true;
	}

	bool GroupChannel::repush(const QUuid& draft_uid)
	{
		if (_track_map.contains(draft_uid)) {
			/// @log
			return false;
		}

		Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		QSharedPointer<Data::ChannelDraft> draft_data(new Data::ChannelDraft(getDescriptor()->getObjectId()));
		const Core::Data::Field uid_field(Core::Data::Field::Type::UUID, QVariant::fromValue(draft_uid));
		draft_data->addPredicate
		(
			QSharedPointer<Core::Data::ConditionPredicate>::create
			(
				draft_data->uid,
				uid_field,
				Core::Data::ConditionPredicate::Condition::Equal
			)
		);
		if (!draft_data->open(data_provider, true) || draft_data->isEmpty()) {
			/// @log
			return false;
		}

		if (draft_data->scheme.value() == Core::Meta::Scheme<Meta::MessageRender>::Ident<TextMessage>()) {
			return push(draft_data, false);
		}
		const Repository::Model::Hierarchy* hierarchy(getDescriptor()->getRealm()->getHierarchy());
		const QString draft_file_name(draft_uid.toString(QUuid::StringFormat::WithoutBraces) + ".draft");
		if (!hierarchy->hasObjectFile(getDescriptor()->getObjectId(), draft_file_name)) {
			/// @log
			return false;
		}
		QFile attachment(hierarchy->getObjectFileLocation(getDescriptor()->getObjectId(), draft_file_name).toLocalFile());
		if (!attachment.open(QFile::ReadOnly | QFile::ExistingOnly)) {
			/// @log
			return false;
		}

		const QSharedPointer<Repository::Net::ObjectFile> file(makeFile());
		file->setObserver
		(
			{
				Core::Net::ServiceCall::ObserverFlag::Upload
				|
				Core::Net::ServiceCall::ObserverFlag::Error
				|
				Core::Net::ServiceCall::ObserverFlag::SSLError
				|
				Core::Net::ServiceCall::ObserverFlag::Finish
			}
		);
		QObject::connect
		(
			file.get(),
			&Core::Net::ServiceCall::finished,

			this,
			std::bind
			(
				qOverload<QUuid>(&GroupChannel::onProgressed),
				this,
				draft_uid
			)
		);
		QObject::connect
		(
			file.get(),
			&Core::Net::ServiceCall::faulted,

			this,
			std::bind
			(
				qOverload<QUuid, Core::Net::ServiceCall::FaultFlag>(&GroupChannel::onProgressed),
				this,
				draft_uid, std::placeholders::_1
			)
		);
		QObject::connect
		(
			file.get(),
			qOverload<qint64, qint64>(&Core::Net::ServiceCall::uploaded),

			this,
			std::bind
			(
				qOverload<QUuid, qint64, qint64>(&GroupChannel::onProgressed),
				this,
				draft_uid, std::placeholders::_1, std::placeholders::_2
			)
		);
		_track_map.insert(draft_uid, file);

		QObject::connect
		(
			file.get(),
			qOverload<QString>(&Repository::Net::ObjectFile::uploaded),

			this,
			qOverload<QString>(&GroupChannel::onPushed)
		);
		QObject::connect
		(
			file.get(),
			qOverload<QNetworkReply::NetworkError, QString>(&Repository::Net::ObjectFile::faulted),

			this,
			std::bind
			(
				(void(GroupChannel::*)(QNetworkReply::NetworkError, QString, QUuid))&GroupChannel::onPushed,
				this, std::placeholders::_1, std::placeholders::_2, draft_uid
			)
		);

		file->upload(&attachment, draft_uid.toString(QUuid::StringFormat::WithoutBraces));
		return true;
	}

	void GroupChannel::modify(Cadabra::Queen::Conversation::Meta::Message message)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onModified)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onModified)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Modify",
			{
				{"id", QString::number(message.getId())},
				{"body", message.getBody()}
			}
		);
	}

	void GroupChannel::withdraw(Cadabra::Queen::Core::Meta::Qd id)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onWithdrawn)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onWithdrawn)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Withdraw",
			{
				{"id", id.toString()}
			}
		);
	}

	bool GroupChannel::discard(const QUuid& draft_uid)
	{
		_track_map.remove(draft_uid);

		Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		QSharedPointer<Data::ChannelDraft> draft_data(new Data::ChannelDraft(getDescriptor()->getObjectId()));
		const Core::Data::Field uid_field(Core::Data::Field::Type::UUID, QVariant::fromValue(draft_uid));
		if (
			0 >= data_provider->makeFactory().remove(
													draft_data.get(),
													Qaos::MakeUnique<Core::Data::ConditionPredicate>
													(
														draft_data->uid,
														uid_field,
														Core::Data::ConditionPredicate::Condition::Equal
													).get()
			)
		) {
			return false;
		}

		QSqlRecord record(_message_data->record(nullptr));
		record.setValue(_message_data->id.name(), 0);
		record.setValue(_message_data->uid.name(), draft_uid);
		record.setGenerated(_message_data->author_subject_id.name(), false);
		record.setGenerated(_message_data->scheme.name(), false);
		record.setGenerated(_message_data->body.name(), false);
		record.setGenerated(_message_data->reply_id.name(), false);
		record.setGenerated(_message_data->post_timestamp.name(), false);
		record.setGenerated(_message_data->modification_timestamp.name(), false);
		_message_data->suspend(record);

		Repository::Model::Hierarchy* hierarchy(getDescriptor()->getRealm()->getHierarchy());
		hierarchy->discardObjectFile(getDescriptor()->getObjectId(), draft_uid.toString(QUuid::StringFormat::WithoutBraces).append(".draft"));
		return true;
	}

	void GroupChannel::fetch(GroupChannel::RotationMethod rotation_method, GroupChannel::FeedEnd feed_end)
	{
		const Meta::Message::Id offset_id((FeedEnd::Head == feed_end ? getRotationOffset(rotation_method, feed_end) : -getRotationOffset(rotation_method, feed_end)) + 1);
		QQueue<Meta::Message::Id>& queue(_rotation_queue[rotation_method]);
		if (!queue.isEmpty()) {
			queue.enqueue(offset_id);
			qDebug() << "Fetch(" << static_cast<char>(rotation_method) << ") =" << offset_id << " has been enqueued";
			return ;
		}
		queue.enqueue(offset_id);
		emit rotated(rotation_method, false);
		rotate(rotation_method);
	}

	void GroupChannel::read(QVariant ident, ReadLocation location)
	{
		if (ReadLocation::MessageId == location) {
			commit(ident.toLongLong(), Meta::Message::Incident::View);
			return ;
		}

		const int value(ident.toInt());
		QSqlRecord record(_message_data->record(nullptr));
		if (ReadLocation::FeedOffset == location) {
			record = (value < 0) ? _message_data->record((_message_data->rowCount() - 1) + value) : _message_data->record(value);
		} else if (ReadLocation::FeedIndex == location) {
			/// @todo
			record = _message_data->record(value);
		} else {
			/// @ocd
			return ;
		}

		if (!record.isEmpty()) {
			commit(record.value("id").toLongLong(), Meta::Message::Incident::View);
		}
	}

	bool GroupChannel::swapon(Cadabra::Queen::Core::Meta::Qd scheme)
	{
		if (scheme.isZero() || !hasDescriptor()) {
			return swapoff();
		}
		_swap_message_id = 0;
		_first_swap_message_id = 0;
		_last_swap_message_id = 0;
		_rotation_queue[RotationMethod::SwapRotate].clear();
		_swap_data.reset(new Data::ChannelSwap(getDescriptor()->getObjectId(), scheme));
//		_swap_data->moveToThread(_async_feed ? Skeleton::ThreadInstance("Conversation::ChannelHub") : thread());

		if (!getDescriptor()->getRealm()->getDataProvider()->open(_swap_data.get(), {}, Core::Data::Provider::RowsetType::Tele)) {
			_swap_data.reset();
			return false;
		}
		const QVariant swap_message_id(getMetaValue(QString("first_%1_message_id").arg(scheme.toInt64())));
		if (!swap_message_id.isNull()) {
			_swap_message_id = swap_message_id.toLongLong(0);
			if (getFirstSwapMessageId(true) == 0 && getFirstFeedMessageId() != 0) {
				_rotation_queue[RotationMethod::SwapRotate].append(-getFirstFeedMessageId());
				rotate(RotationMethod::SwapRotate);
			} else {
				getLastSwapMessageId(true);
			}
		} else {
			/// @todo Нужна уточнить отсчеку, поэтому временно ставим 1
			_swap_message_id = 1;
			extremum(scheme.toInt64(), FeedEnd::Tail);
		}

		emit reswapped();
		return true;
	}

	bool GroupChannel::swapoff()
	{
		_swap_message_id = 0;
		_first_swap_message_id = 0;
		_last_swap_message_id = 0;
		_rotation_queue[RotationMethod::SwapRotate].clear();
		if (_swap_data) {
			_swap_data.reset();
			emit reswapped();
		}
		return true;
	}

	Core::Net::CRPPromise GroupChannel::index()
	{
		return Repository::Model::FolderInterface::index();
	}

	void GroupChannel::name(const QString& topic)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onNamed)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onNamed)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Name",
			{
				{"topic", topic}
			}
		);
	}

	bool GroupChannel::push(QSharedPointer<Data::ChannelDraft> draft_data, bool need_track)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		if (!draft_data || (need_track && !track(draft_data, call))) {
			return false;
		}

		const QUuid draft_uid(draft_data->uid.value().toUuid());

		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onPushed)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			std::bind
			(
				(void(GroupChannel::*)(quint8, quint32, QString, QUuid))&GroupChannel::onPushed,
				this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, draft_uid
			)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Push",
			{
				{"uid", draft_uid.toString(QUuid::StringFormat::WithoutBraces)},
				{"reply_id", QString::number(draft_data->reply_id.value().toLongLong())},
				{"scheme", QString::number(draft_data->scheme.value().toLongLong())},
				{"body", draft_data->body.value().toJsonObject()}
			}
		);
		return true;
	}

	void GroupChannel::dump(bool full)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onDumped)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onDumped)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Dump",
			{
				{"full", full}
			}
		);
	}

	void GroupChannel::feed(Cadabra::Queen::Conversation::Meta::Message::Id offset, FeedEnd end, qsizetype limit)
	{
		if (0 == offset) {
			offset = (FeedEnd::Head == end) ? getLastFeedMessageId() + 1 : getFirstFeedMessageId() - 1;
			if (offset <= 0) {
				return ;
			}
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onFed)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onFed)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Feed",
			{
				{"offset", QString::number(offset)},
				{"limit", QString::number(FeedEnd::Head == end ? limit : -limit)}
			}
		);
		qDebug() << "Feed has been requested with offset" << offset << "and" << limit << "limit towards the" << (FeedEnd::Head == end ? "head" : "tail");
	}

	void GroupChannel::swap(Cadabra::Queen::Conversation::Meta::Message::Id offset, FeedEnd end, qsizetype limit)
	{
		if (!_swap_data) {
			return ;
		} else if (0 == offset) {
			offset = (FeedEnd::Head == end) ? getLastFeedMessageId() + 1 : getFirstFeedMessageId() - 1;
			if (offset <= 0) {
				return ;
			}
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onSwapped)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onSwapped)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Feed",
			{
				{"offset", QString::number(offset)},
				{"limit", QString::number(FeedEnd::Head == end ? limit : -limit)},
				{"scheme_filter_list", QJsonArray{_swap_data->getSchemeQd().toString()}}
			}
		);
		qDebug() << "Swap has been requested with offset" << offset << "and" << limit << "limit towards the" << (FeedEnd::Head == end ? "head" : "tail");
	}

	void GroupChannel::extremum(Meta::Message::SchemeIdent scheme, FeedEnd end)
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			std::bind
			(
				(void(GroupChannel::*)(QJsonValue, Meta::Message::SchemeIdent, FeedEnd))
				&GroupChannel::onExtremumed, this, std::placeholders::_1, scheme, end
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			std::bind
			(
				(void(GroupChannel::*)(quint8, quint32, QString, Meta::Message::SchemeIdent, FeedEnd))
				&GroupChannel::onExtremumed, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, scheme, end
			)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Feed",
			{
				{"offset", FeedEnd::Tail == end ? "1" : QString::number(std::numeric_limits<Meta::Message::Id>::max())},
				{"limit", FeedEnd::Tail == end ? "1" : "-1"},
				{"scheme_filter_list", QJsonArray{QString::number(scheme)}}
			}
		);
		qDebug() << "Extremum has been requested towards the" << (FeedEnd::Head == end ? "head" : "tail");
	}

	void GroupChannel::log(Cadabra::Queen::Conversation::Meta::Message::Id offset, FeedEnd end, qsizetype limit)
	{
		if (0 == offset) {
			offset = getLastLocalLogId() + 1;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onLogged)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onLogged)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Log",
			{
				{"offset", QString::number(offset)},
				{"limit", QString::number(FeedEnd::Head == end ? limit : -limit)},
			}
		);
		qDebug() << "Log has been requested with offset" << offset << "and" << limit << "limit towards the" << (FeedEnd::Head == end ? "head" : "tail");
	}

	void GroupChannel::pull(Cadabra::Queen::Conversation::Meta::Message::Id offset, FeedEnd end, qsizetype limit)
	{
		if (0 == offset) {
			offset = getLastLocalLogId() + 1;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onPulled)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onPulled)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Pull",
			{
				{"offset", QString::number(offset)},
				{"limit", QString::number(FeedEnd::Head == end ? limit : -limit)},
			}
		);
		qDebug() << "Pull has been requested with offset" << offset << "and" << limit << "limit towards the" << (FeedEnd::Head == end ? "head" : "tail");
	}

	bool GroupChannel::rotate(RotationMethod method)
	{
		QQueue<Meta::Message::Id>& queue(_rotation_queue[method]);
		while (!queue.isEmpty()) {
			const FeedEnd end(queue.head() > 0 ? FeedEnd::Head : FeedEnd::Tail);
			if (getRotationOffset(method, end) > 0 && !getOffsetCompareVerity(method, end, qAbs(queue.head()))) {
				queue.dequeue();
			} else {
				const Meta::Message::Id offset_id(getRotationOffset(method, end) + ((FeedEnd::Head == end) ? +1 : -1));
				qDebug() << "Last offset(" << static_cast<char>(method) << ") is" << offset_id << "and" << queue.head() << "is requested from the queue";
				switch (method) {
					case RotationMethod::LogRotate:
						log(offset_id, end);
						return true;
					case RotationMethod::PullRotate:
						pull(offset_id, end);
						return true;
					case RotationMethod::FeedRotate:
						feed(offset_id, end);
						return true;
					case RotationMethod::SwapRotate:
						swap(offset_id, end);
						return true;
					default:
						return false;
				}
			}
		}
		emit rotated(method, true);
		qDebug() << "Rotation queue(" << static_cast<char>(method) << ") is empty, nothing to rotate";
		return false;
	}

	bool GroupChannel::enqueue(Meta::Message::Id offset_id, RotationMethod rotation_method)
	{
		QQueue<Meta::Message::Id>& queue(_rotation_queue[rotation_method]);
		if (getOffsetCompareVerity(rotation_method, (offset_id > 0 ? FeedEnd::Head : FeedEnd::Tail), qAbs(offset_id))) {
			if (!queue.isEmpty()) {
				queue.enqueue(offset_id);
				qDebug() << "Offset(" << static_cast<char>(rotation_method) << ") =" << offset_id << " has been enqueued";
				return true;
			}
			queue.enqueue(offset_id);
			emit rotated(rotation_method, false);
		}
		return rotate(rotation_method);
	}

	bool GroupChannel::commit(Meta::Message::Id message_id, Meta::Message::Incident incident)
	{
		switch (incident) {
			case Meta::Message::Incident::Receive:
				if (getLastFeedMessageId() >= message_id || _pending_receive_message_id >= message_id) {
					return false;
				} else {
					_pending_receive_message_id = message_id;
				}
				break;

			case Meta::Message::Incident::View:
				if (_last_read_message_id >= message_id || _pending_read_message_id >= message_id) {
					return false;
				} else {
					_pending_read_message_id = message_id;
				}
				break;

			default:
				return false;
		}

		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),

			this,
			qOverload<QJsonValue>(&GroupChannel::onCommitted)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),

			this,
			qOverload<quint8, quint32, QString>(&GroupChannel::onCommitted)
		);
		call->act
		(
			"Cadabra::Ace::Conversation::Model::ChatInterface",
			"Cadabra::Ace::Conversation::Model::Commit",
			{
				{"message_id", QString::number(message_id)},
				{"incident", static_cast<char>(incident)}
			}
		);
		return true;
	}

	bool GroupChannel::track(QSharedPointer<Data::ChannelDraft> data, QSharedPointer<Core::Net::ServiceCall> call)
	{
		if (
			!_message_data
			||
			!_message_data->hasRowset()
			||
			!getDescriptor()->getRealm()->getDataProvider()->makeFactory().insert(data.get())
		) {
			qWarning() << "Could not save the draft, message will be lost in case of failure";
			return false;
		}

		QSqlRecord record(_message_data->record(nullptr));
		record.setValue(_message_data->id.name(), 0);
		record.setValue(_message_data->author_subject_id.name(), getDescriptor()->getSubjectId());
		record.setValue(_message_data->scheme.name(), data->scheme.value());
		record.setValue(_message_data->body.name(), QJsonDocument(data->body.value().toJsonObject()).toJson(QJsonDocument::JsonFormat::Compact));
		record.setValue(_message_data->uid.name(), data->uid.value());
		record.setValue(_message_data->reply_id.name(), data->reply_id.value());
		record.setValue(_message_data->post_timestamp.name(), data->timestamp.value().toDateTime().toSecsSinceEpoch());
		record.setValue(_message_data->modification_timestamp.name(), QVariant());

		if (!_message_data->prepend(record)) {
			return false;
		}

		call->setObserver
		(
			{
				Core::Net::ServiceCall::ObserverFlag::Upload
				|
				Core::Net::ServiceCall::ObserverFlag::Error
				|
				Core::Net::ServiceCall::ObserverFlag::SSLError
				|
				Core::Net::ServiceCall::ObserverFlag::Finish
			}
		);
		const QUuid uid(data->uid.value().toUuid());
		QObject::connect
		(
			call.get(),
			&Core::Net::ServiceCall::finished,

			this,
			std::bind
			(
				qOverload<QUuid>(&GroupChannel::onProgressed),
				this,
				uid
			)
		);
		QObject::connect
		(
			call.get(),
			&Core::Net::ServiceCall::faulted,

			this,
			std::bind
			(
				qOverload<QUuid, Core::Net::ServiceCall::FaultFlag>(&GroupChannel::onProgressed),
				this,
				uid, std::placeholders::_1
			)
		);
		QObject::connect
		(
			call.get(),
			qOverload<qint64, qint64>(&Core::Net::ServiceCall::uploaded),

			this,
			std::bind
			(
				qOverload<QUuid, qint64, qint64>(&GroupChannel::onProgressed),
				this,
				uid, std::placeholders::_1, std::placeholders::_2
			)
		);
		_track_map.insert(uid, call);
		return true;
	}

	void GroupChannel::preview()
	{
		QSharedPointer<Data::ChannelMessage> message_data;
		do {
			if (hasDescriptor()) {
				message_data.reset(new Data::ChannelMessage(getDescriptor()->getObjectId()));
				message_data->setRowLimit(1);
				message_data->addOrder(message_data->id, Qt::SortOrder::DescendingOrder);
				if (message_data->open(getDescriptor()->getRealm()->getDataProvider()) & !message_data->isEmpty()) {
					break;
				}
			}
			_last_message->import(this, {});
			return ;
		} while (false);

		/// @brief Обновляем последнее сообщение в ленте
		Meta::Message last_message_meta
		(
			message_data->id.value().toLongLong(),
			message_data->uid.value().toUuid(),
			message_data->reply_id.value().toLongLong(),
			message_data->scheme.value().toLongLong(),
			message_data->author_subject_id.value().toLongLong(),
			message_data->body.value().toJsonObject(),
			message_data->post_timestamp.value().toDateTime(),
			message_data->modification_timestamp.value().toDateTime()
		);
		if (
			_last_message->getId() != last_message_meta.getId()
			||
			_last_message->getModificationDateTime() < last_message_meta.getModificationTime()
		) {
			_last_feed_message_id = last_message_meta.getId();
			_last_message->import(this, last_message_meta);
			emit refreshed();
			_assistant_data->dismiss(Meta::Assist::SourceGroup::LastMessage);
			_assistant_data->emplace(_last_message->getAssistList(), Meta::Assist::SourceGroup::LastMessage);
		} else {
			/// @note ErrorMessage instead with [Render error] text?
			qWarning() << "Could not import meta of the last message in the feed during preview";
		}
		teleview(true);
	}

	void GroupChannel::teleview(bool just_me)
	{
		/// @todo Объеденить в один запрос к БД
		/// @todo Сразу добавить ограничения по текущим "ластам" и если не isEmpty, значит точно view

		const Core::Data::Field subject_id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(getDescriptor()->getSubjectId()));

		const QSharedPointer<Data::ChannelMember> member_data(new Data::ChannelMember(getDescriptor()->getObjectId()));
		member_data->silent_mode.setGenerated(false);
		member_data->subject_role.setGenerated(false);
		member_data->last_receive_id.setGenerated(true);
		member_data->last_read_id.setGenerated(true);
		member_data->subject_id.setGenerated(true);

		/// @brief Обновляем свои отсечки прочитанных сообщений
		member_data->addPredicate(QSharedPointer<Core::Data::ConditionPredicate>::create(member_data->subject_id, subject_id_field, Core::Data::ConditionPredicate::Condition::Equal));

		if (member_data->open(getDescriptor()->getRealm()->getDataProvider()) && !member_data->isEmpty()) {
			const Meta::Message::Id cutoff_read_message_id(member_data->last_read_id.isNull() ? 0 : member_data->last_read_id.value().toLongLong());
			if (_last_read_message_id < cutoff_read_message_id) {
				_last_read_message_id = cutoff_read_message_id;
				emit viewed();
			}
		}

		if (just_me) {
			return ;
		}

		member_data->resetCompoundPredicate();
		/// @brief Обновляем чужие отсечки прочитанных и полученных сообщений
		member_data->addPredicate
		(
			QSharedPointer<Core::Data::ConditionPredicate>::create
			(
				member_data->subject_id,
				subject_id_field,
				Core::Data::ConditionPredicate::Condition::NotEqual
			)
		);

		if (member_data->open(getDescriptor()->getRealm()->getDataProvider()) && !member_data->isEmpty()) {
			QMap<Repository::Meta::Subject::Id, QPair<Meta::Message::Id, Meta::Message::Id>> message_delivery_map;
			do {
				QPair<Meta::Message::Id, Meta::Message::Id> current
				(
					_message_delivery_map.value
					(
						member_data->subject_id.value().toLongLong(),
						qMakePair(0, 0)
					)
				);
				QPair<Meta::Message::Id, Meta::Message::Id> update
				{
					member_data->last_read_id.value().toLongLong(),
					member_data->last_receive_id.value().toLongLong()
				};
				if (current.first < update.first || current.second < update.second) {
					message_delivery_map[member_data->subject_id.value().toLongLong()] = update;
				}
			} while (member_data->next());
			if (!message_delivery_map.isEmpty()) {
				message_delivery_map.swap(_message_delivery_map);
				emit televiewed();
				Event event(new MessageDelivery(_message_delivery_map));
				QCoreApplication::sendEvent(getDescriptor(), &event);
			}
		}
	}

	Cadabra::Queen::Core::Data::AbstractEntity* GroupChannel::getSwapEntity() const
	{
		return _swap_data.get();
	}

	Cadabra::Queen::Core::Data::AbstractEntity* GroupChannel::getMessageEntity() const
	{
		return _message_data.get();
	}

	QAbstractListModel* GroupChannel::getSwapModel() const
	{
		return _swap_data.get();
	}

	QAbstractListModel* GroupChannel::getMessageModel() const
	{
		return _message_data.get();
	}

	QAbstractListModel* GroupChannel::getMemberModel() const
	{
		return _member_data.get();
	}

	QAbstractListModel* GroupChannel::getDraftModel() const
	{
		return _draft_data.get();
	}

	QAbstractListModel* GroupChannel::getSyncModel() const
	{
		return _sync_data.get();
	}

	QAbstractListModel* GroupChannel::getTagModel() const
	{
		return _tag_data.get();
	}

	QAbstractListModel* GroupChannel::getIndexModel() const
	{
		return _index_model.get();
	}

	QAbstractListModel* GroupChannel::getAssistantModel() const
	{
		return _assistant_data.get();
	}

	bool GroupChannel::getOffsetCompareVerity(RotationMethod method, FeedEnd end, Meta::Message::Id lhs)
	{
		return std::invoke(GroupChannel::OffsetComparator(end), lhs, getRotationOffset(method, end));
	}

	Meta::Message::Id GroupChannel::getRotationOffset(RotationMethod method, FeedEnd end)
	{
		switch (method) {
			case RotationMethod::LogRotate:
			case RotationMethod::PullRotate:
				return getLocalLogId(end, false);

			case RotationMethod::FeedRotate:
				return getFeedMessageId(end, false);

			case RotationMethod::SwapRotate:
				return getSwapMessageId(end, false);
		}
		return 0;
	}

	Meta::Message::Id GroupChannel::getSwapMessageId(FeedEnd end, bool force)
	{
		static const QMap<FeedEnd, QPair<Qt::SortOrder, Meta::Message::Id GroupChannel::*>> map
		{
			{FeedEnd::Head, {Qt::SortOrder::DescendingOrder, &GroupChannel::_last_swap_message_id}},
			{FeedEnd::Tail, {Qt::SortOrder::AscendingOrder, &GroupChannel::_first_swap_message_id}},
		};

		const QPair<Qt::SortOrder, Meta::Message::Id GroupChannel::*>& pair(map.value(end));
		Meta::Message::Id& message_id(this->*(pair.second));
		if (force || message_id <= 0) {
			const QSharedPointer<Data::ChannelSwap> data(new Data::ChannelSwap(getDescriptor()->getObjectId(), _swap_data->getSchemeQd()));
			data->addOrder(data->id, pair.first);
			data->setRowLimit(1);
			if (data->open(getDescriptor()->getRealm()->getDataProvider()) && !data->isEmpty()) {
				message_id = data->id.value().toLongLong();
				data->close();
			} else {
				message_id = 0;
			}
		}

		return message_id;
	}

	Meta::Message::Id GroupChannel::getFirstSwapMessageId(bool force)
	{
		return getSwapMessageId(FeedEnd::Tail, force);
	}

	Meta::Message::Id GroupChannel::getLastSwapMessageId(bool force)
	{
		return getSwapMessageId(FeedEnd::Head, force);
	}

	Meta::Message::Id GroupChannel::getFeedMessageId(FeedEnd end, bool force)
	{
		static const QMap<FeedEnd, QPair<Qt::SortOrder, Meta::Message::Id GroupChannel::*>> map
		{
			{FeedEnd::Head, {Qt::SortOrder::DescendingOrder, &GroupChannel::_last_feed_message_id}},
			{FeedEnd::Tail, {Qt::SortOrder::AscendingOrder, &GroupChannel::_first_feed_message_id}},
		};

		const QPair<Qt::SortOrder, Meta::Message::Id GroupChannel::*>& pair(map.value(end));
		Meta::Message::Id& message_id(this->*(pair.second));
		if (force || message_id <= 0) {
			const QSharedPointer<Data::ChannelMessage> data(new Data::ChannelMessage(getDescriptor()->getObjectId()));
			data->addPredicate
			(
				QSharedPointer<Core::Data::ConditionPredicate>::create
				(
					data->post_timestamp,
					Core::Data::Field(Core::Data::Field::Type::Timestamp, QDateTime::fromSecsSinceEpoch(0, Qt::UTC)),
					Core::Data::ConditionPredicate::Condition::NotEqual
				)
			);
			data->addOrder(data->id, pair.first);
			data->setRowLimit(1);
			if (data->open(getDescriptor()->getRealm()->getDataProvider()) && !data->isEmpty()) {
				message_id = data->id.value().toLongLong();
				data->close();
			} else {
				message_id = 0;
			}
		}

		return message_id;
	}

	Meta::Message::Id GroupChannel::getFirstFeedMessageId(bool force)
	{
		return getFeedMessageId(FeedEnd::Tail, force);
	}

	Meta::Message::Id GroupChannel::getLastFeedMessageId(bool force)
	{
		return getFeedMessageId(FeedEnd::Head, force);
	}

	Meta::Message::Id GroupChannel::getLocalLogId(FeedEnd end, bool force)
	{
		return (FeedEnd::Head == end) ? getLastLocalLogId(force) : 0;
	}

	Meta::Message::Id GroupChannel::getLastLocalLogId(bool force)
	{
		if (_sync_data && (0 == _last_sync_log_id || force)) {
			if (_sync_data->open(getDescriptor()->getRealm()->getDataProvider()) && !_sync_data->isEmpty()) {
				_last_sync_log_id = _sync_data->log_id.value().toLongLong();
				_sync_data->close();
			}
		}
		return _last_sync_log_id;
	}

	AbstractMessage* GroupChannel::getLastMessage()
	{
		return _last_message->getTarget();
	}

	const MessageDelivery::Map& GroupChannel::getDeliveryMap() const
	{
		return _message_delivery_map;
	}

	bool GroupChannel::onHandle()
	{
		_message_data.reset(new Data::ChannelMessage(getDescriptor()->getObjectId()));
//		_message_data->moveToThread(_async_feed ? Skeleton::ThreadInstance("Conversation::ChannelHub") : thread());
		_message_data->addPredicate
		(
			QSharedPointer<Core::Data::ConditionPredicate>::create
			(
				_message_data->post_timestamp,
				Core::Data::Field(Core::Data::Field::Type::Timestamp, QDateTime::fromSecsSinceEpoch(0, Qt::UTC)),
				Core::Data::ConditionPredicate::Condition::IsNot
			)
		);

		_sync_data.reset(new Data::ChannelSync(getDescriptor()->getObjectId()));
		_sync_data->addOrder(_sync_data->log_id, Qt::SortOrder::DescendingOrder);
		_sync_data->setRowLimit(1);

		_draft_data.reset(new Data::ChannelDraft(getDescriptor()->getObjectId()));
		_draft_data->addOrder(_draft_data->timestamp, Qt::SortOrder::DescendingOrder);

		_member_data.reset(new Data::ChannelMember(getDescriptor()->getObjectId()));
		_tag_data.reset(new Data::ChannelTag(getDescriptor()->getObjectId()));
		_assistant_data.reset(new Data::Assistant(this));
		_index_model.reset(new Repository::Meta::FolderIndex());

		return true;
	}

	bool GroupChannel::onOpen()
	{
		Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		if (data_provider->isExist(_draft_data.get())) {
			Cadabra::Queen::Core::Data::Factory data_factory(data_provider->makeFactory());
			data_factory.remove(
								_draft_data.get(),
								Qaos::MakeUnique<Core::Data::ConditionPredicate>
								(
									_draft_data->timestamp,
									Core::Data::Field(Core::Data::Field::Type::Timestamp, QVariant(QDateTime::currentDateTime().addDays(-3))),
									Core::Data::ConditionPredicate::Condition::LessOrEqual
								).get()
			);
		}

		if (
			!_member_data->open(data_provider)
			||
			!_draft_data->open(data_provider)
			||
			!_tag_data->open(data_provider)
			||
			!_sync_data->open(data_provider)
		) {
			qCritical("Data open error");
			return false;
		}
		bool retval(true);
		_last_sync_log_id = 0;
		_feed_message_id = 0;
		if (_sync_data->isEmpty()) {
			dump(false);
			index();
		} else {
			_feed_message_id = getFirstFeedMessageId(true);
			const QVariant feed_message_id(getMetaValue("first_message_id"));
			if (!feed_message_id.isNull()) {
				_feed_message_id = feed_message_id.toLongLong();
			}
			if (_feed_message_id <= 0 ) {
				dump(false);
				index();
			} else if (!data_provider->open(_message_data.get(), {}, Core::Data::Provider::RowsetType::Tele)) {
				retval = false;
				qCritical("Feed load error");
			} else {
				/// @brief Восстановим аппендикс(неотправленные сообщения)
				while (!_draft_data->isEOF()) {
					QSqlRecord record(_message_data->record(nullptr));
					record.setValue(_message_data->id.name(), 0);
					record.setValue(_message_data->author_subject_id.name(), getDescriptor()->getSubjectId());
					record.setValue(_message_data->scheme.name(), _draft_data->scheme.value());
					record.setValue(_message_data->body.name(), QJsonDocument(_draft_data->body.value().toJsonObject()).toJson(QJsonDocument::JsonFormat::Compact));
					record.setValue(_message_data->uid.name(), _draft_data->uid.value());
					record.setValue(_message_data->reply_id.name(), _draft_data->reply_id.value());
					record.setValue(_message_data->post_timestamp.name(), _draft_data->timestamp.value().toDateTime().toSecsSinceEpoch());
					record.setValue(_message_data->modification_timestamp.name(), QVariant());
					_message_data->append(record);
					_draft_data->next();
				}

				preview();
				teleview();
				_last_sync_log_id = _sync_data->log_id.value().toLongLong();
				_index_model->parse(getMetaValue("index").toJsonValue());
				pull(getLastLocalLogId() + 1);
				emit initialized(false, true);
			}
		}
		_sync_data->close();
		_tag_data->close();
		_draft_data->close();
		_member_data->close();

		return retval;
	}

	bool GroupChannel::onSetup()
	{
		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());

		if (
			!data_factory.create(QScopedPointer<Data::ChannelMember>(new Data::ChannelMember(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.create(QScopedPointer<Data::ChannelMessage>(new Data::ChannelMessage(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.create(QScopedPointer<Data::ChannelDraft>(new Data::ChannelDraft(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.create(QScopedPointer<Data::ChannelTag>(new Data::ChannelTag(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.create(QScopedPointer<Data::ChannelSync>(new Data::ChannelSync(getDescriptor()->getObjectId())).get(), true)
			) {
			qFatal("Error on entities creation");
		}
		return true;
	}

	bool GroupChannel::onSetdown()
	{
		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());

		if (
			!data_factory.drop(QScopedPointer<Data::ChannelMember>(new Data::ChannelMember(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.drop(QScopedPointer<Data::ChannelMessage>(new Data::ChannelMessage(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.drop(QScopedPointer<Data::ChannelDraft>(new Data::ChannelDraft(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.drop(QScopedPointer<Data::ChannelTag>(new Data::ChannelTag(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.drop(QScopedPointer<Data::ChannelSync>(new Data::ChannelSync(getDescriptor()->getObjectId())).get(), true)
			) {
			qFatal("Error on entities drop");
		}
		return true;
	}

	bool GroupChannel::onClose()
	{
		/// @todo clear everything
		_rotation_queue.clear();
		_message_delivery_map.clear();

		return true;
	}

	bool GroupChannel::onTeardown()
	{
		return true;
	}

	void GroupChannel::onDumped(quint8 domain, quint32 code, QString message)
	{
		emit initialized(true, false);
		qCritical() << "[" << domain << code << "]" << "Dump error: " << message;
	}

	void GroupChannel::onExtremumed(quint8 domain, quint32 code, QString message, Meta::Message::SchemeIdent scheme, FeedEnd end)
	{
		qCritical() << "[" << domain << code << "]" << "Extremum error: " << message;
		extremum(scheme, end);
	}

	void GroupChannel::onFed(quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Feed error: " << message;
		rotate(RotationMethod::FeedRotate);
	}

	void GroupChannel::onSwapped(quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Swap error: " << message;
		rotate(RotationMethod::SwapRotate);
	}

	void GroupChannel::onLogged(quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Log error: " << message;
		rotate(RotationMethod::LogRotate);
	}

	void GroupChannel::onPulled(quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Pull error: " << message;
		rotate(RotationMethod::PullRotate);
	}

	void GroupChannel::onPushed(QNetworkReply::NetworkError error, QString message, QUuid uid)
	{
		onPushed(0, static_cast<quint32>(error), message, uid);
	}

	void GroupChannel::onPushed(quint8 domain, quint32 code, QString message, QUuid uid)
	{
		qCritical() << "[" << domain << code << "]" << "Push error: " << message;
		/// @todo resolve exact FaultFlag by code (Network? SSL?)
		onProgressed(uid, Core::Net::ServiceCall::FaultFlag::General);
	}

	void GroupChannel::onModified(quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Modification error: " << message;
	}

	void GroupChannel::onWithdrawn(quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Withdrawn error: " << message;
	}

	void GroupChannel::onCommitted(quint8 domain, quint32 code, QString message)
	{
		_pending_receive_message_id = 0;
		_pending_read_message_id = 0;
		qCritical() << "[" << domain << code << "]" << "Commit error: " << message;
	}

	void GroupChannel::onNamed(quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Name error: " << message;
	}

	void GroupChannel::onDumped(QJsonValue payload)
	{
		Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());

		/// @brief Если делает Dump, то значит это начало всех начал и нужно все сбросить
		if (data_provider->isExist(_message_data.get()) > 0) {
			data_factory.truncate(_message_data.get());
		}
		if (data_provider->isExist(_tag_data.get())) {
			data_factory.truncate(_tag_data.get());
		}
		if (data_provider->isExist(_draft_data.get())) {
			data_factory.truncate(_draft_data.get());
		}
		if (data_provider->isExist(_member_data.get())) {
			data_factory.truncate(_member_data.get());
		}
		if (data_provider->isExist(_sync_data.get())) {
			data_factory.truncate(_sync_data.get());
		}

		_message_data->close();
		_tag_data->close();
		_member_data->close();
		_draft_data->close();
		_sync_data->close();

		const QString topic(payload["topic"].toString());
		const QJsonArray feed(payload["feed"]["array"].toArray());
		const QJsonArray party(payload["party"]["array"].toArray());
		/// @note Теги могли попасть не все в сообщения, так же как и Member в Party
		/// const QJsonArray tag(payload["tag"]["array"].toArray());
		foreach (const QJsonValue& member, party) {
			const Repository::Meta::Subject::Id id(member["subject_id"].toString("0").toLongLong());
			if (id <= 0) {
				continue;
			}
			QSharedPointer<const Repository::Meta::Subject> subject(getDescriptor()->getRealm()->getHierarchy()->getSubjectModel()->upsert(id, "", "", "", "", "", "", 0, 0));
		}

		if (!data_factory.insert(_member_data.get(), party, Core::Data::Factory::InsertionPolicy::Regular)) {
			/// @xxx и как быть?
			qFatal("Member dump error: %s", qPrintable(QJsonDocument(party).toJson()));
		} else if (!data_factory.insert(_message_data.get(), feed, Core::Data::Factory::InsertionPolicy::Regular)) {
			/// @xxx и как быть?
			qFatal("Feed dump error");
		}

		if (!feed.isEmpty()) {
			_first_feed_message_id = feed.last().toObject().value("id").toString("0").toLongLong();
		}
		_feed_message_id = payload["message_min_id"].toString("0").toLongLong();

		if (!data_provider->open(_message_data.get(), {Core::Data::Provider::OpenMode::FetchThrow}, Core::Data::Provider::RowsetType::Tele)) {
			/// @xxx и как быть?
			qFatal("Feed load error");
		}
		setMetaValue("first_message_id", _feed_message_id);
		preview();
		teleview();

		const QSharedPointer<Data::ChannelSync> sync_data(new Data::ChannelSync(getDescriptor()->getObjectId()));
		sync_data->timestamp.setValue(QDateTime::currentDateTimeUtc());
		sync_data->log_id.setValue(payload["log_id"].toString("0").toLongLong());
		if (!data_factory.insert(sync_data.get())) {
			emit initialized(true, false);
			/// @xxx и как быть?
			qFatal("Flush dump error with log_id: %s and timestamp: %s", qPrintable(sync_data->log_id.value().toString()), qPrintable(sync_data->timestamp.value().toString()));
		} else {
			enqueue(getLastLocalLogId(true), RotationMethod::PullRotate);
			emit initialized(true, true);
		}
	}

	void GroupChannel::onFed(QJsonValue payload)
	{
		const QJsonArray array(payload["array"].toArray());
		if (array.isEmpty()) {
			qWarning() << "Cannot use empty feed";
			if (!_rotation_queue[RotationMethod::FeedRotate].isEmpty()) {
				_rotation_queue[RotationMethod::FeedRotate].dequeue();
				rotate(RotationMethod::FeedRotate);
			}
			return ;
		}

		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());
		if (!data_factory.insert(_message_data.get(), array, Core::Data::Factory::InsertionPolicy::Replace)) {
			/// @xxx и как быть?
			qFatal("Feed fetch error");
			return ;
		}

		Meta::Message::Id first_id(array.first().toObject().value("id").toString("0").toLongLong());
		Meta::Message::Id last_id(array.last().toObject().value("id").toString("0").toLongLong());
		if (first_id > last_id) {
			qSwap(first_id, last_id);
		}

		if (getFirstFeedMessageId() > first_id && _feed_message_id > getFirstFeedMessageId(true)) {
			_feed_message_id = getFirstFeedMessageId();
			setMetaValue("first_message_id", _feed_message_id);
		} else if (getLastFeedMessageId() < last_id){
			preview();
			teleview();
		}

		if (_message_data->reopen()) {
			emit fed(first_id, last_id, array.count());
		}
		rotate(RotationMethod::FeedRotate);
	}

	void GroupChannel::onExtremumed(QJsonValue payload, Meta::Message::SchemeIdent scheme, FeedEnd end)
	{
		/// @note Just ignore maximum extremums
		if (FeedEnd::Head == end) {
			return ;
		}

		QJsonArray array(payload["array"].toArray());
		Meta::Message::Id meta_id(0);
		if (!array.isEmpty()) {
			const Meta::Message::Id origin_id(array.first().toObject().value("id").toString("0").toLongLong());
			const QVariant variant_id(getMetaValue(QString("first_%1_message_id").arg(scheme)));
			meta_id = variant_id.isNull() ? std::numeric_limits<Meta::Message::Id>::max() : variant_id.toLongLong();

			if (origin_id != 0 && origin_id < meta_id) {
				meta_id = origin_id;
				setMetaValue(QString("first_%1_message_id").arg(scheme), meta_id);
				if (
					_swap_data
					&&
					_swap_data->getSchemeQd() == scheme
					&&
					getFirstSwapMessageId(false) == 0
					&&
					getFirstFeedMessageId() != 0
				) {
					_rotation_queue[RotationMethod::SwapRotate].append(-getFirstFeedMessageId());
					rotate(RotationMethod::SwapRotate);
				}
			} else {
				return ;
			}
		}
		if (_swap_data && _swap_data->getSchemeQd() == scheme) {
			_swap_message_id = meta_id;
			emit swapped(0, 0, 0);
		}
	}

	void GroupChannel::onSwapped(QJsonValue payload)
	{
		QJsonArray array(payload["array"].toArray());
		if (array.isEmpty()) {
			qWarning() << "Cannot use empty swap";
			if (!_rotation_queue[RotationMethod::SwapRotate].isEmpty()) {
				_rotation_queue[RotationMethod::SwapRotate].dequeue();
				rotate(RotationMethod::SwapRotate);
			}
			return ;
		}

		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());
		if (!data_factory.transact(Core::Data::Factory::TransactionPolicy::AutoRollback)) {
			qWarning() << "Could not begin a tranasction";
			return;
		}

		for (QJsonArray::Iterator s = array.begin(); s != array.end(); ++s) {
			const QJsonObject row(s->toObject());
			const QJsonValue reply_id(row.value("reply_id"));
			QSqlRecord record(_swap_data->record(true));
			if (!reply_id.isNull()) {
				record.setValue("reply_id", Repository::Meta::Essence::ToId(reply_id));
			} else {
				record.setGenerated("reply_id", false);
			}
			record.setValue("id", Repository::Meta::Essence::ToId(row.value("id")));
			record.setValue("author_subject_id", Repository::Meta::Essence::ToId(row.value("author_subject_id")));
			record.setValue("post_timestamp", QDateTime::fromSecsSinceEpoch(0, Qt::UTC));
			record.setValue("modification_timestamp", QDateTime::fromSecsSinceEpoch(row.value("modification_timestamp").toInt(), Qt::UTC));
			record.setValue("uid", QUuid::fromString(row.value("uid").toString("")));
			record.setValue("body", QJsonDocument(row.value("body").toObject()));
			record.setValue("scheme", row.value("scheme").toString("0").toLongLong());

			if (!data_factory.insert(_swap_data.get(), record, Core::Data::Factory::InsertionPolicy::Ignore)) {
				/// @xxx и как быть?
				qFatal("Feed fetch error");
				return ;
			}
		}

		if (!data_factory.commit()) {
			qCritical() << "Commit error";
			return ;
		} else if (getFirstSwapMessageId(true) > getLastSwapMessageId(false)) {
			getLastSwapMessageId(true);
		}

		const QJsonObject first_row(array.first().toObject());
		Meta::Message::Id first_id(first_row.value("id").toString("0").toLongLong());
		const QJsonObject last_row(array.last().toObject());
		Meta::Message::Id last_id(last_row.value("id").toString("0").toLongLong());

		if (_swap_data->reopen()) {
			if (first_id > last_id) {
				qSwap(first_id, last_id);
			}
			emit swapped(first_id, last_id, array.count());
		}
		rotate(RotationMethod::SwapRotate);
	}

	void GroupChannel::onLogged(QJsonValue payload)
	{
		/// @todo Пока не используем накатывание инкрементального лога
	}

	void GroupChannel::onPulled(QJsonValue payload)
	{
		const Meta::Message::Id upper_bound(payload["upper_bound"].toString().toLongLong());
		/// @??? Do we need to check upper_bound with current?

		const Meta::Message::Id lower_bound(payload["lower_bound"].toString().toLongLong());
		qDebug() << "Log has been pulled from" << upper_bound << "up to" << lower_bound;

		const Meta::Message::Id local_log_id(getLastLocalLogId());
		if (upper_bound <= 0 || lower_bound <= 0) {
			qWarning() << "Empty slice has been pulled, just ignore for now";
			if (!_rotation_queue[RotationMethod::PullRotate].isEmpty()) {
				_rotation_queue[RotationMethod::PullRotate].dequeue();
			}
			rotate(RotationMethod::PullRotate);
			return ;
		} else if (local_log_id >= upper_bound) {
			qWarning() << "Cannot use this slice because at least some part of it is already pulled and synchronized";
			rotate(RotationMethod::PullRotate);
			return ;
		} else if (upper_bound - local_log_id > 1) {
			qWarning() << "Cannot use this slice because of the gap between local log";
			rotate(RotationMethod::PullRotate);
			return ;
		}

		qDebug() << "Synchronizing feed of the ChannelMessage " << getDescriptor()->getObjectId() << "...";

		GroupChannel::PullFlags pull_flags;
		Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		Core::Data::Factory data_factory(data_provider->makeFactory());
		if (!data_factory.transact(Core::Data::Factory::TransactionPolicy::AutoRollback)) {
			qWarning() << "Could not begin a tranasction";
			return;
		}
		Core::Data::Rowset::HintTable hint_table;
		QJsonObject roll(payload["roll"].toObject());
		for (QJsonObject::ConstIterator r = roll.constBegin(); r != roll.constEnd(); ++r) {
			QSqlRecord record(_message_data->record(false));
			const Meta::Message::Id message_id(r.key().toLongLong());
			const QJsonObject message_payload(r->toObject());

			if (message_payload.contains("scheme")) {
				Meta::Message::SchemeIdent message_scheme(message_payload.value("scheme").toString("0").toLongLong());
				record.setGenerated("scheme", true);
				record.setValue("scheme", message_scheme == -1 ? Core::Meta::Scheme<Meta::MessageRender>::Ident<WithdrawalMessage>() : message_scheme);
			}
			if (message_payload.contains("modification_timestamp")) {
				record.setGenerated("modification_timestamp", true);
				record.setValue("modification_timestamp", QDateTime::fromSecsSinceEpoch(message_payload.value("modification_timestamp").toInt(), Qt::UTC));
			}
			if (message_payload.contains("body")) {
				record.setGenerated("body", true);
				record.setValue("body", QJsonDocument(message_payload.value("body").toObject()));
			}
			if (message_payload.contains("author_subject_id")) { /// < New message
				if (message_payload.contains("reply_id")) {
					record.setGenerated("reply_id", true);
					record.setValue("reply_id", message_payload.value("reply_id").toString().toLongLong());
				}
				const Repository::Meta::Subject::Id author_subject_id(message_payload.value("author_subject_id").toString().toLongLong());
				const QUuid uid(message_payload.value("uid").toString(""));
				record.setGenerated("id", true);
				record.setValue("id", message_id);
				record.setGenerated("author_subject_id", true);
				record.setValue("author_subject_id", author_subject_id);
				record.setGenerated("post_timestamp", true);
				record.setValue("post_timestamp", QDateTime::fromSecsSinceEpoch(message_payload.value("post_timestamp").toInt(), Qt::UTC));
				record.setGenerated("uid", true);
				record.setValue("uid", uid);
				if (!data_factory.insert(_message_data.get(), record, Core::Data::Factory::InsertionPolicy::Replace)) {
					qCritical() << "Could not insert the message into the feed, data is probably broken";
					return;
				}

				/// @brief Если это наше сообщение, то нужно поискать его в черновиках и удалить, так как оно было успешно доставлено
				if (author_subject_id == getDescriptor()->getSubjectId()) {
					const Core::Data::Field uid_field(Core::Data::Field::Type::UUID, uid);
					/// @note Оно могло быть отправлено и с другого устройства, но при этом того же субъекта
					if (
						!data_factory.remove(
												_draft_data.get(),
												Qaos::MakeUnique<Core::Data::ConditionPredicate>
												(
													_draft_data->uid,
													uid_field,
													Core::Data::ConditionPredicate::Condition::Equal
												).get()
						)
					) {
						qCritical() << "Could not remove the message from the draft, data is probably broken";
						return ;
					} else if (data_factory.getAffectedRowsCount() > 0) {
						QSqlRecord suspended_record(_message_data->record(false));
						suspended_record.setValue("uid", uid);
						suspended_record.setGenerated("uid", true);
						hint_table.insert(Core::Data::Rowset::Mutation::Suspend, suspended_record);
					} else { /// < Новое сообщение от моего же субъекта, но с другой сессии(устройства)
						pull_flags.setFlag(PullCharge::ReceiveMessage);
					}
				} else { /// < Новое сообщение от чужого субъекта(собеседника)
					pull_flags.setFlag(PullCharge::ReceiveMessage);
				}
			} else { /// < Обновление отредактированного сообщения
				if (
					!data_factory.update
					(
						_message_data.get(),
						record,
						Qaos::MakeUnique<Core::Data::ConditionPredicate>
						(
							_message_data->id,
							Core::Data::Field{Core::Data::Field::Type::Integer, message_id},
							Core::Data::ConditionPredicate::Condition::Equal
						).get()
					)
				) {
					qCritical() << "Could not update the message in the feed, data is probably broken";
					return ;
				} else {
					/// @note Сохраняем id для пост-коммитного обновления наблюдателей модели _message_data
					QSqlRecord update_record(_message_data->record(nullptr));
					update_record.setValue("id", message_id);
					update_record.setGenerated("id", true);
					hint_table.insert(Core::Data::Rowset::Mutation::Update, update_record);
				}
			}
		}

		if (qAsConst(hint_table).find(Core::Data::Rowset::Mutation::Suspend) != hint_table.constEnd()) {
			pull_flags.setFlag(PullCharge::PushMessage);
		}

		if (qAsConst(hint_table).find(Core::Data::Rowset::Mutation::Update) != hint_table.constEnd()) {
			pull_flags.setFlag(PullCharge::UpdateMessage);
		}

		/// @brief Обновляем участников канала, их роли, просмотренные/полученные
		Data::ChannelMember member_data(getDescriptor()->getObjectId());
		QJsonObject log(payload["log"].toObject());
		for (QJsonObject::ConstIterator l = log.constBegin(); l != log.constEnd(); ++l) {
			member_data.silent_mode.setGenerated(false);

			const QJsonArray log_payload(l->toArray());
			foreach (const QJsonValue& log_value, log_payload) {
				member_data.subject_id.setGenerated(false);
				member_data.subject_role.setGenerated(false);

				const QJsonObject log_object(log_value.toObject());
				const Meta::Message::Incident incident(static_cast<Meta::Message::Incident>(log_object.value("incident").toInt()));
				if (incident == Meta::Message::Incident::Receive) {
					member_data.last_read_id.setGenerated(false);
					member_data.last_receive_id.setGenerated(true);
					member_data.last_receive_id.setValue(log_object.value("id").toString("0").toLongLong());
				} else if (incident == Meta::Message::Incident::View) {
					member_data.last_receive_id.setGenerated(false);
					member_data.last_read_id.setGenerated(true);
					member_data.last_read_id.setValue(log_object.value("id").toString("0").toLongLong());
				} else {
					continue;
				}

				const Core::Data::Field member_subject_id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(l.key().toLongLong()));
				/// @note Обновляем просмотренные/полученные сообщения
				if (
					data_factory.update(
										&member_data,
										Qaos::MakeUnique<Core::Data::ConditionPredicate>(
											member_data.subject_id,
											member_subject_id_field,
											Core::Data::ConditionPredicate::Condition::Equal
										).get()
					)
				) {
					/// @note ... а если(почему-то?) обновлять нечего - добавим целиком участника канала c известными отсечками получения и
					if (data_factory.getAffectedRowsCount() <= 0) {
						member_data.subject_id.setGenerated(true);
						member_data.subject_id.setValue(member_subject_id_field.value());
						member_data.subject_role.setGenerated(true);
						member_data.subject_role.setValue(static_cast<qint16>(Meta::Channel::Role::Listener));
						if (!data_factory.insert(&member_data, Core::Data::Factory::InsertionPolicy::Replace)) {
							qCritical() << "Could not insert the member to the channel, data is probably broken";
						}
					}
				} else {
					qCritical() << "Could not update the member on the channel, data is probably broken";
				}
			}
		}

		Data::ChannelSync sync_data(getDescriptor()->getObjectId());
		sync_data.timestamp.setValue(QDateTime::currentDateTimeUtc());
		sync_data.log_id.setValue(lower_bound);
		if (!data_factory.insert(&sync_data)) {
			qCritical() << "Could not flush synchronization";
			return ;
		} else if (!data_factory.commit()) {
			qCritical() << "Could not apply and commit all feed modifcations, channel data is probably broken";
			return ;
		} else if (!log.isEmpty()) {
			teleview();
		}

		if (!pull_flags) {
			getLastLocalLogId(true);
		} else if (_message_data->reopen(hint_table)) {
			commit(getLastFeedMessageId(true), Meta::Message::Incident::Receive);
			getLastLocalLogId(true);
			preview();
			emit pulled(upper_bound, lower_bound, pull_flags);
		} else {
			qCritical() << "Could not reopen channel messages";
			return ;
		}
		rotate(RotationMethod::PullRotate);
	}

	void GroupChannel::onPushed(QJsonValue payload)
	{
		if (payload.isObject()) {
			enqueue(payload.toObject().value("log_id").toString("0").toLongLong());
		} else {
			qWarning("Unknown data structure, could not clean the draft");
		}
	}

	void GroupChannel::onPushed(QString file_name)
	{
		/// @note Fetch at least one ahead
		enqueue(getLastLocalLogId() + 1);
	}

	void GroupChannel::onModified(QJsonValue payload)
	{
		if (payload.isObject()) {
			QJsonObject object(payload.toObject());
			enqueue(object["log_id"].toString().toLongLong());
		} else {
			qWarning("Unknown data structure, could not flush");
		}
	}

	void GroupChannel::onWithdrawn(QJsonValue payload)
	{
		if (payload.isObject()) {
			QJsonObject object(payload.toObject());
			enqueue(object["log_id"].toString().toLongLong());
		} else {
			qWarning("Unknown data structure, could not flush");
		}
	}

	void GroupChannel::onCommitted(QJsonValue payload)
	{
		QJsonObject object(payload.toObject());
		const Meta::Message::Id log_id(object["log_id"].toString("0").toLongLong());
		if (0 == log_id) { /// < Already commited, lets fix the local data
			/// @todo Могут быть и другие события у которых нет message_id(e.g. Join, Ban, Kick, Privilege)
			const Meta::Message::Incident incident(static_cast<Meta::Message::Incident>(object.value("incident").toInt(static_cast<qint8>(Meta::Message::Incident::Flush))));
			const Meta::Message::Id message_id(object["message_id"].toString("0").toLongLong());

			Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());
			Data::ChannelMember member_data(getDescriptor()->getObjectId());
			member_data.subject_id.setGenerated(false);
			member_data.subject_role.setGenerated(false);
			member_data.silent_mode.setGenerated(false);

			const Core::Data::Field subject_id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(getDescriptor()->getSubjectId()));
			Core::Data::CompoundPredicate predicate
			(
				{
					QSharedPointer<Core::Data::ConditionPredicate>::create
					(
						member_data.subject_id,
						subject_id_field,
						Core::Data::ConditionPredicate::Condition::Equal
					)
				},
				Core::Data::CompoundPredicate::Logic::And
			);

			const Core::Data::Field message_id_field(Core::Data::Field::Type::Integer, QVariant::fromValue(message_id));
			if (incident == Meta::Message::Incident::View) {
				_pending_read_message_id = 0;
				member_data.last_read_id.setGenerated(true);
				member_data.last_receive_id.setGenerated(false);
				member_data.last_read_id.setValue(message_id);
				predicate.emplace<Core::Data::ConditionPredicate>(member_data.last_read_id, message_id_field, Core::Data::ConditionPredicate::Condition::Less);
			} else if (incident == Meta::Message::Incident::Receive) {
				_pending_receive_message_id = 0;
				member_data.last_read_id.setGenerated(false);
				member_data.last_receive_id.setGenerated(true);
				member_data.last_receive_id.setValue(message_id);
				predicate.emplace<Core::Data::ConditionPredicate>(member_data.last_receive_id, message_id_field, Core::Data::ConditionPredicate::Condition::Less);
			}
			if (!data_factory.update(&member_data, &predicate)) {
				/// @xxx И как быть?
				qWarning("Could not fix the ChannelMember data");
			}
		} else {
			enqueue(log_id);
		}
		teleview(true);
	}

	void GroupChannel::onNamed(QJsonValue payload)
	{

	}

	void GroupChannel::onProgressed(QUuid uid)
	{
		if (_track_map.remove(uid) == 0) {
			return ;
		}

		Event event(new MessageTransfer(uid));
		QCoreApplication::sendEvent(getDescriptor(), &event);
	}

	void GroupChannel::onProgressed(QUuid uid, Core::Net::ServiceCall::FaultFlag flag)
	{
		if (Core::Net::ServiceCall::FaultFlag::None == flag) {
			return ;
		}
		_track_map.remove(uid);

		emit progressed(_track_map.isEmpty() ? 1 : 0);

		Event event(new MessageTransfer(uid, flag));
		QCoreApplication::sendEvent(getDescriptor(), &event);
	}

	void GroupChannel::onProgressed(QUuid uid, qint64 processed_size, qint64 total_size)
	{
		emit progressed(total_size <= 0 ? .0 : (static_cast<double>(processed_size) / static_cast<double>(total_size)));

		Event event(new MessageTransfer(uid, Core::Net::ServiceCall::ObserverFlag::Upload, processed_size, total_size));
		QCoreApplication::sendEvent(getDescriptor(), &event);
	}

	void GroupChannel::onAction(BroadcastAction* action)
	{
		qDebug() << "Broadcast action is fired with log_id" << action->getLogId() << " for descriptor " << getDescriptor()->getObjectId();
		enqueue(action->getLogId());
	}

	void GroupChannel::onIndexed(const Repository::Model::FolderInterface::Map& map)
	{
		if (map.isEmpty()) {
			return ;
		}
//		setMetaValue("index", payload);
		_index_model->remap(map);
		emit indexed();
	}

	void GroupChannel::customEvent(QEvent* event)
	{
		if (event->type() == Repository::Model::AceEvent::Action<BroadcastAction>::RegisterEventType(0)) {
			return onAction(Repository::Model::AceEvent::TypeCast<BroadcastAction>(static_cast<Repository::Model::AceEvent*>(event)));
		}
		return AbstractApplication::customEvent(event);
	}

	bool GroupChannel::hasUnreadMessages() const
	{
		return _last_read_message_id < _last_message->getTarget()->getId();
	}

	bool GroupChannel::hasUnloadedMessages() const
	{
		return _first_feed_message_id > 0 && _feed_message_id != _first_feed_message_id;
	}

	bool GroupChannel::hasUnswappedMessages() const
	{
		return _first_swap_message_id > 0 && _swap_message_id != _first_swap_message_id;
	}

	bool GroupChannel::hasEnqueued(RotationMethod method) const
	{
		QMap<RotationMethod, QQueue<Meta::Message::Id>>::ConstIterator q(_rotation_queue.find(method));
		return (_rotation_queue.constEnd() == q) ? false : q->isEmpty();
	}

	bool GroupChannel::hasEnqueuedLogs() const
	{
		return hasEnqueued(RotationMethod::LogRotate);
	}

	bool GroupChannel::hasEnqueuedPulls() const
	{
		return hasEnqueued(RotationMethod::PullRotate);
	}

	bool GroupChannel::hasEnqueuedFeeds() const
	{
		return hasEnqueued(RotationMethod::FeedRotate);
	}

	bool GroupChannel::hasEnqueuedSwaps() const
	{
		return hasEnqueued(RotationMethod::SwapRotate);
	}

	bool GroupChannel::hasAssistant() const
	{
		return _assistant_data ? _assistant_data->isEmpty() : false;
	}

	bool GroupChannel::isPushing() const
	{
		return !_track_map.isEmpty();
	}

	bool GroupChannel::isInitialized() const
	{
		return _last_sync_log_id > 0;
	}
} } } }
