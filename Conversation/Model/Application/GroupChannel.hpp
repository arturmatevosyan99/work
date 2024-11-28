#ifndef Cadabra_Queen_Conversation_Model_Application_GroupChannel_hpp
#define Cadabra_Queen_Conversation_Model_Application_GroupChannel_hpp

#include "../../../Repository/Model/AbstractApplication.hpp"

#include "../../../Conversation/Model/Application/Interface/ChatInterface.hpp"
#include "../../../Repository/Model/Application/Interface/FolderInterface.hpp"

#include "../AbstractMessage.hpp"
#include "../Message/ProxyMessage.hpp"
#include "../Accident/MessageDelivery.hpp"
#include "../Action/BroadcastAction.hpp"
#include "../Action/SyncAction.hpp"

#include "../../Meta/Channel.hpp"
#include "../../Meta/Message.hpp"

#include "../../Data/Assistant.hpp"
#include "../../Data/Entity/ChannelSwap.hpp"
#include "../../Data/Entity/ChannelMessage.hpp"
#include "../../Data/Entity/ChannelMember.hpp"
#include "../../Data/Entity/ChannelDraft.hpp"
#include "../../Data/Entity/ChannelTag.hpp"
#include "../../Data/Entity/ChannelSync.hpp"

#include "../../../Core/System/AudioRecorder.hpp"
#include "../../../Core/System/ImageThumbnail.hpp"
#include "../../../Repository/Meta/FolderIndex.hpp"

#include <QSharedPointer>
#include <QAbstractListModel>
#include <QMimeType>
#include <QSet>
#include <QUuid>
#include <QMultiMap>
#include <QQueue>
#include <QHash>
#include <QIODevice>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	/**
	 * @brief Frontend application model for instant messaging
	 * @author Daniil A Megrabyan (danil@megrabyan.pro)
	 *
	 * @todo Избавиться от _index_model в пользу Avatar::pickUpProfile -> index
	 *
	 * Implements common chat API(Conversation::Model::ChatInterface). Each channel is also a virtual folder(Repository::Model::FolderInterface)
	 * and may consits of any children objects to provide additional functionality according to access level of a user(eg. its role) and channel purpose.
	 * Objects are regular realm objects and can be opened with compatible application model and view if GUI needs to be supplied
	 * When model is successfully opened("linked" with backend application model in the session pool) for the very first time
	 * ::dump() is called automatically. Eventually, initialized signal is emited and model is ready to use: fetch messages from the feed and push new ones.
	 * To push a message use appropriate push-method, to load a page with new events or unloaded messages to keep front-end data model uptodated
	 * ::fetch() can be called where rotation method has to be specified explicitly(see RotationMethod for the details). Despite this,
	 * fetch(pull) is enqueued automatically when back-end model requires for a broadcast action on a live network stream between back/front-models.
	 *
	 * @include /Conversation/Test/Integration/GroupChannelTest.qml
	 */
	class GroupChannel : public Repository::Model::AbstractApplication, public ChatInterface, public Repository::Model::FolderInterface
	{
	/** @name Classes */
	/** @{ */
	public:
		/**
		 * @brief Fetch mode to synchronise with backend application model
		 */
		enum class RotationMethod
		{
			PullRotate = 'P', ///< Fetch a combined roll with messages and events
			LogRotate = 'L', ///< [not in use] Fetch a pure log page with all kind of events(messages, joins, bans, kicks, ...) on a channel
			FeedRotate = 'F', ///< Fetch a page with messages of all types
			SwapRotate = 'S' ///< Fetch a page with specific messages for an active swap which are missed because have not been loaded before
		};

		/**
		 * @brief Cutoff type to validate the last message that was read by the user
		 */
		enum class ReadLocation
		{
			MessageId, ///< Unique message id
			FeedIndex, ///< Row index in the active list model wich is based on a data(bms) model of the feed
			FeedOffset ///< Relative position regarding the tail(if value is negative) or the head(if value is positive)
		};

		/**
		 * @brief Two ends of the feed
		 */
		enum class FeedEnd : char
		{
			Tail = '<', ///< Start (First message ever)
			Head = '>' ///< Finish (The most recent message)
		};

		/**
		 * @brief Content type of a message
		 */
		enum class ContentType : char
		{
			PlainText = 't',
			RichText = 'r',
			MarkdownText = 'm',
			ImageBinary = 'I',
			AudioBinary = 'A',
			RegularBinary = 'B'
		};

		/**
		 * @brief What has happend on a channel and mentioned in the "pull"
		 */
		enum PullCharge
		{
			Log = 0b00, ///< Basic(not really important for now) events
			PushMessage = 0b01, ///< User message was pushed
			ModifyMessage = 0b10, ///< Legacy (need to be removed in the upcoming release)
			ReceiveMessage = 0b100, ///< New message(s) was/were received
			UpdateMessage = 0b1000 ///< Message(s) was/were updated
		};
		Q_DECLARE_FLAGS(PullFlags, PullCharge)
	/** @} */

	/** @name Comparator */
	/** @{ */
	public:
		using OffsetCompareFunctor = bool (*)(const Meta::Message::Id& lhs, const Meta::Message::Id& rhs);
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static OffsetCompareFunctor OffsetComparator(FeedEnd end);
		static bool LessOffset(const Meta::Message::Id& lhs, const Meta::Message::Id& rhs);
		static bool GreaterOffset(const Meta::Message::Id& lhs, const Meta::Message::Id& rhs);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(Cadabra::Queen::Conversation::Model::ChatInterface Cadabra::Queen::Repository::Model::FolderInterface)
		Q_FLAG(PullFlags)
		Q_ENUM(RotationMethod)
		Q_ENUM(ReadLocation)
		Q_ENUM(FeedEnd)
		Q_ENUM(ContentType)
		/**
		 * @brief Local data(bms) model to cache messages just of specific kind/schema which are monotonously loaded on demand
		 */
		Q_PROPERTY(Cadabra::Queen::Core::Data::AbstractEntity* swapEntity READ getSwapEntity NOTIFY reswapped)
		/**
		 * @brief Local data(bms) model to cache a continuous feed of all messages which are monotonously loaded on demand
		 */
		Q_PROPERTY(Cadabra::Queen::Core::Data::AbstractEntity* messageEntity READ getMessageEntity NOTIFY opened)
		/**
		 * @brief List model(via data model) with messages of an active swap(just specific messages from the feed)
		 */
		Q_PROPERTY(QAbstractListModel* swapModel READ getSwapModel NOTIFY reswapped)
		/**
		 * @brief List model(via data model) with all messages from the feed
		 */
		Q_PROPERTY(QAbstractListModel* messageModel READ getMessageModel NOTIFY opened)
		/**
		 * @brief List model(via data model) with members
		 */
		Q_PROPERTY(QAbstractListModel* memberModel READ getMemberModel NOTIFY opened)
		/**
		 * @brief List model(via data model) with unsent messages
		 */
		Q_PROPERTY(QAbstractListModel* draftModel READ getDraftModel NOTIFY opened)
		/**
		 * @brief List model(via data model) with tags mentioned in the chat
		 */
		Q_PROPERTY(QAbstractListModel* tagModel READ getTagModel NOTIFY opened)
		/**
		 * @brief List model with loaded child objects are available for the user and can be opened with an appropriate application model
		 * @see Repository::Meta::FolderIndex
		 */
		Q_PROPERTY(QAbstractListModel* indexModel READ getIndexModel NOTIFY opened)
		/**
		 * @brief List model with assistants from all source groups and for all target layouts
		 * @see Conversation::Meta::Assist
		 */
		Q_PROPERTY(QAbstractListModel* assistantModel READ getAssistantModel NOTIFY opened)
		/**
		 * @brief The lastest available message in the feed
		 */
		Q_PROPERTY(Cadabra::Queen::Conversation::Model::AbstractMessage* lastMessage READ getLastMessage NOTIFY refreshed)
		/**
		 * @brief New messages in the feed have not been read yet
		 */
		Q_PROPERTY(bool hasUnreadMessages READ hasUnreadMessages NOTIFY viewed)
		/**
		 * @brief Not all messages in a feed history have been fetched and available
		 */
		Q_PROPERTY(bool hasUnloadedMessages READ hasUnloadedMessages NOTIFY refreshed)
		/**
		 * @brief Not all messages in a swap history have been fetched and available
		 */
		Q_PROPERTY(bool hasUnswappedMessages READ hasUnswappedMessages NOTIFY swapped)
		/**
		 * @brief Has application model a plan in a queue to fetch some log pages?
		 */
		Q_PROPERTY(bool hasEnqueuedLogs READ hasEnqueuedLogs NOTIFY rotated)
		/**
		 * @brief Has application model a plan in a queue to fetch some pull pages(a combined roll with messages and events)?
		 */
		Q_PROPERTY(bool hasEnqueuedPulls READ hasEnqueuedPulls NOTIFY rotated)
		/**
		 * @brief Has application model a plan in a queue to fetch some feed messages?
		 */
		Q_PROPERTY(bool hasEnqueuedFeeds READ hasEnqueuedFeeds NOTIFY rotated)
		/**
		 * @brief Has application model a plan in a queue to fetch some swap messages?
		 */
		Q_PROPERTY(bool hasEnqueuedSwaps READ hasEnqueuedSwaps NOTIFY rotated)
		/**
		 * @brief Is application model pushing(sending) a message right now?
		 */
		Q_PROPERTY(bool isPushing READ isPushing NOTIFY progressed)
		/**
		 * @brief Nothig was synced from the backend
		 */
		Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY initialized)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		GroupChannel(QObject* parent = nullptr);
		virtual ~GroupChannel() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Meta::Message::Id _swap_message_id;
		Meta::Message::Id _feed_message_id;

		Meta::Message::Id _first_swap_message_id;
		Meta::Message::Id _last_swap_message_id;

		Meta::Message::Id _first_feed_message_id;
		Meta::Message::Id _last_feed_message_id;

		Meta::Message::Id _last_read_message_id;

		Meta::Message::Id _pending_receive_message_id;
		Meta::Message::Id _pending_read_message_id;

		MessageDelivery::Map _message_delivery_map;

		Meta::Message::Id _last_sync_log_id;

		QMap<RotationMethod, QQueue<Meta::Message::Id>> _rotation_queue;

		QSharedPointer<Model::ProxyMessage> _last_message;

		QSharedPointer<Data::ChannelSwap> _swap_data;
		QSharedPointer<Data::ChannelMessage> _message_data;
		QSharedPointer<Data::ChannelMember> _member_data;
		QSharedPointer<Data::ChannelDraft> _draft_data;
		QSharedPointer<Data::ChannelTag> _tag_data;
		QSharedPointer<Data::ChannelSync> _sync_data;

		QMap<QUuid, QSharedPointer<Core::Net::ServiceCall>> _track_map;
		QSharedPointer<Data::Assistant> _assistant_data;

		QSharedPointer<Repository::Meta::FolderIndex> _index_model;
	/** @} */

	/** @name Qrocedures */
	/** @{ */
	public:
		/**
		 * @brief Push a custom message where meta prepared somewhere outside by your own
		 * Message meta information guid
		 * @param message_meta
		 * @return success
		 */
		Q_INVOKABLE bool push(Cadabra::Queen::Conversation::Meta::Message meta);
		/**
		 * @brief Push a text message
		 * Just wrapper around ::push(meta)
		 * @param text
		 * @return success
		 */
		Q_INVOKABLE bool push(const QString& text);
		/**
		 * @brief Push a file by its URL on local(queen) system
		 * @param url If starts from fd:// the host part of a URI is considering as a file descriptor
		 * @return success
		 */
		Q_INVOKABLE bool push(const QUrl& url);
		/**
		 * @brief Push a binary sequence from a buffer
		 * @param attachment (picture, voice, ...)
		 * @param content_type
		 * @return success
		 */
		Q_INVOKABLE bool push(QBuffer* attachment, Cadabra::Queen::Conversation::Model::GroupChannel::ContentType content_type = ContentType::RegularBinary);
		/**
		 * @brief Push a message from a draft again
		 * Repeat network transfer in case of a network error
		 * @param draft_uid
		 * @return success
		 */
		Q_INVOKABLE bool repush(const QUuid& draft_uid);
		/**
		 * @brief Apply message modifications
		 * @param message
		 */
		Q_INVOKABLE void modify(Cadabra::Queen::Conversation::Meta::Message meta);
		/**
		 * @brief Erase a message in the feed
		 * @param id
		 */
		Q_INVOKABLE void withdraw(Cadabra::Queen::Core::Meta::Qd id);
		/**
		 * @brief Remove a message from the draft and stop current transfering(if there is one)
		 * @param draft_uid
		 * @return success
		 */
		Q_INVOKABLE bool discard(const QUuid& draft_uid);
		/**
		 * @brief Request data
		 * @param rotation_method
		 * @param feed_end
		 */
		Q_INVOKABLE void fetch(Cadabra::Queen::Conversation::Model::GroupChannel::RotationMethod rotation_method, Cadabra::Queen::Conversation::Model::GroupChannel::FeedEnd feed_end);
		/**
		 * @brief Confirm(Commit) the last read messages
		 * @param ident
		 * @param location
		 */
		Q_INVOKABLE void read(QVariant ident, Cadabra::Queen::Conversation::Model::GroupChannel::ReadLocation location);
		/**
		 * @brief Turn swap on
		 * @note Start a partial load messages with particular scheme (eg: all video lessons)
		 * @param scheme
		 * @return success
		 */
		Q_INVOKABLE bool swapon(Cadabra::Queen::Core::Meta::Qd scheme);
		/**
		 * @brief Turn swap off
		 * @return success
		 */
		Q_INVOKABLE bool swapoff();
		/**
		 * @brief Load child objects (think like GroupChannel is a folder with files)
		 * @note Result index may vary depending on the access level
		 */
		Q_INVOKABLE Cadabra::Queen::Core::Net::CRPPromise index();
		/**
		 * @brief Rename the channel
		 * @note Actually, this boils down to rename a corresponding realm object
		 * @param topic
		 */
		Q_INVOKABLE void name(const QString& topic);
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		bool push(QSharedPointer<Data::ChannelDraft> draft, bool need_track = true);

		void dump(bool full = true);
		void feed(Meta::Message::Id offset = 0, FeedEnd end = FeedEnd::Head, qsizetype limit = 50);
		void swap(Meta::Message::Id offset, FeedEnd end, qsizetype limit = 50);
		void extremum(Meta::Message::SchemeIdent scheme, FeedEnd end);
		void log(Meta::Message::Id offset = 0, FeedEnd end = FeedEnd::Head, qsizetype limit = 50);
		void pull(Meta::Message::Id offset = 0, FeedEnd end = FeedEnd::Head, qsizetype limit = 100);
		bool rotate(RotationMethod method = RotationMethod::PullRotate);
		bool enqueue(Meta::Message::Id offset_id, RotationMethod rotation_method = RotationMethod::PullRotate);
		bool commit(Meta::Message::Id message_id, Meta::Message::Incident incident);
		bool track(QSharedPointer<Data::ChannelDraft> data, QSharedPointer<Core::Net::ServiceCall> call);
		void preview();
		void teleview(bool just_me = false);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Core::Data::AbstractEntity* getSwapEntity() const;
		Cadabra::Queen::Core::Data::AbstractEntity* getMessageEntity() const;

		QAbstractListModel* getSwapModel() const;
		QAbstractListModel* getMessageModel() const;
		QAbstractListModel* getMemberModel() const;
		QAbstractListModel* getDraftModel() const;
		QAbstractListModel* getSyncModel() const;
		QAbstractListModel* getTagModel() const;
		QAbstractListModel* getIndexModel() const;
		QAbstractListModel* getAssistantModel() const;

		bool getOffsetCompareVerity(RotationMethod method, FeedEnd end, Meta::Message::Id lhs);
		Meta::Message::Id getRotationOffset(RotationMethod method, FeedEnd end);

		Meta::Message::Id getSwapMessageId(FeedEnd end, bool force);
		Meta::Message::Id getFirstSwapMessageId(bool force = false);
		Meta::Message::Id getLastSwapMessageId(bool force = false);

		Meta::Message::Id getFeedMessageId(FeedEnd end, bool force);
		Meta::Message::Id getFirstFeedMessageId(bool force = false);
		Meta::Message::Id getLastFeedMessageId(bool force = false);

		Meta::Message::Id getLocalLogId(FeedEnd end, bool force);
		Meta::Message::Id getLastLocalLogId(bool force = false);

		const MessageDelivery::Map& getDeliveryMap() const;

		AbstractMessage* getLastMessage();
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		/**
		 * @brief Signal "Ready for use"
		 * @note Local cache are initialized and ready to use and further incremental sync
		 */
		void initialized(bool dump, bool success);
		/**
		 * @brief Signal "Last message has been loaded"
		 * @note In the general sense actually means that feed was updated by sync process
		 */
		void refreshed();
		/**
		 * @brief Signal "Swap partition is changed"
		 * @note Another scheme was specified (eg. when media tab is changed by user)
		 */
		void reswapped();
		/**
		 * @brief Signal "New messages were read by the user"
		 */
		void viewed();
		/**
		 * @brief Signal "New messages were read by others"
		 */
		void televiewed();
		/**
		 * @brief Signal "Page with some messages for the feed were fetched and internally cached"
		 * @param upper_message_bound
		 * @param lower_message_bound
		 * @param count
		 */
		void fed(Cadabra::Queen::Conversation::Meta::Message::Id upper_message_bound, Cadabra::Queen::Conversation::Meta::Message::Id lower_message_bound, int count);
		/**
		 * @brief Signal "Page with some messages for the swap was loaded and internally cached"
		 * @param upper_message_bound First message id on a page
		 * @param lower_message_bound Last message id on a page
		 * @param count
		 */
		void swapped(Cadabra::Queen::Conversation::Meta::Message::Id upper_message_bound, Cadabra::Queen::Conversation::Meta::Message::Id lower_message_bound, int count);
		/**
		 * @brief Signal "Page with some messages and events were fetched, internally cached and applied to the feed"
		 * @param upper_log_bound
		 * @param lower_log_bound
		 * @param flags
		 */
		void pulled(Cadabra::Queen::Conversation::Meta::Message::Id upper_log_bound, Cadabra::Queen::Conversation::Meta::Message::Id lower_log_bound, Cadabra::Queen::Conversation::Model::GroupChannel::PullFlags flags);
		/**
		 * @brief Signal "Data(a fetch) was requested by specific method or rotation queue is empty because all requests were done"
		 * @param method
		 * @param empty
		 */
		void rotated(Cadabra::Queen::Conversation::Model::GroupChannel::RotationMethod method, bool empty);
		/**
		 * @brief Signal "Child objects were indexed"
		 */
		void indexed();
		/**
		 * @brief Signal "Assistant has been switched on or off"
		 * @deprecated
		 * @param uid
		 * @param onoff
		 */
		void assisted(QUuid uid, bool onoff);
		/**
		 * @brief Signal "Channel was renamed" (New topic is applied)
		 */
		void named();
		/**
		 * @brief Signal "Push/Upload progress was changed"
		 * @param value
		 */
		void progressed(double value);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		/**
		 * @brief Slot "Dump error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onDumped(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Bound determinating error"
		 * @param domain
		 * @param code
		 * @param message
		 * @param scheme
		 * @param end
		 */
		void onExtremumed(quint8 domain, quint32 code, QString message, Meta::Message::SchemeIdent scheme, FeedEnd end);
		/**
		 * @brief Slot "Feed fetch error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onFed(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Swap fetch error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onSwapped(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Log fetch error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onLogged(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Pull fetch error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onPulled(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Message push network error"
		 * @param error
		 * @param message
		 * @param uid
		 */
		void onPushed(QNetworkReply::NetworkError error, QString message, QUuid uid);
		/**
		 * @brief Slot "Message push error"
		 * @param domain
		 * @param code
		 * @param message
		 * @param uid
		 */
		void onPushed(quint8 domain, quint32 code, QString message, QUuid uid);
		/**
		 * @brief Slot "Message modification error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onModified(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Message erase error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onWithdrawn(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Commit error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onCommitted(quint8 domain, quint32 code, QString message);
		/**
		 * @brief Slot "Change topic/name error"
		 * @param domain
		 * @param code
		 * @param message
		 */
		void onNamed(quint8 domain, quint32 code, QString message);

		/**
		 * @brief Slot "First ever initial dump was received"
		 * @param payload
		 */
		void onDumped(QJsonValue payload);
		/**
		 * @brief Slot "Last or first(top or bottom bound) specific scheme message id in the feed is determinated"
		 * @note Usefull to limit swapping when first picture(e.g) message is known and further swapping does not make sense any more
		 * @param payload
		 * @param scheme
		 * @param end
		 */
		void onExtremumed(QJsonValue payload, Meta::Message::SchemeIdent scheme, FeedEnd end);
		/**
		 * @brief Slot "Feed page was fetched"
		 * @param payload
		 */
		void onFed(QJsonValue payload);
		/**
		 * @brief Slot "Swap page was fetched"
		 * @param payload
		 */
		void onSwapped(QJsonValue payload);
		/**
		 * @brief Slot "Log page was fetched"
		 * @param payload
		 */
		void onLogged(QJsonValue payload);
		/**
		 * @brief Slot "Pull was fetched"
		 * @param payload
		 */
		void onPulled(QJsonValue payload);
		/**
		 * @brief Slot "Message was pushed"
		 * @param payload
		 */
		void onPushed(QJsonValue payload);
		/**
		 * @brief Slot "File was uploaded"
		 * @param file_name
		 */
		void onPushed(QString file_name);
		/**
		 * @brief Slot "Message was modified/edited"
		 * @param payload
		 */
		void onModified(QJsonValue payload);
		/**
		 * @brief Slot "Message was erased"
		 * @param payload
		 */
		void onWithdrawn(QJsonValue payload);
		/**
		 * @brief Slot "Read/Receive was commited"
		 * @param payload
		 */
		void onCommitted(QJsonValue payload);
		/**
		 * @brief Slot "Channel/Object was renamed"
		 * @param payload
		 */
		void onNamed(QJsonValue payload);
		/**
		 * @brief Slot "Data(file, message itself) transferring last event"
		 * @param uid
		 */
		void onProgressed(QUuid uid);
		/**
		 * @brief Slot "Data(file, message itself) transferring error event"
		 * @param uid
		 * @param flag
		 */
		void onProgressed(QUuid uid, Core::Net::ServiceCall::FaultFlag flag);
		/**
		 * @brief Slot "Data(file, message itself) transferring upload event"
		 * @param uid
		 * @param receive_size
		 * @param total_size
		 */
		void onProgressed(QUuid uid, qint64 processed_size, qint64 total_size);
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
		void onAction(BroadcastAction* action);

		virtual void onIndexed(const Repository::Model::FolderInterface::Map& map) override;

	protected:
		virtual void customEvent(QEvent* event) override;
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
		virtual bool onSetdown() override;
		virtual bool onClose() override;
		virtual bool onTeardown() override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasUnreadMessages() const;
		bool hasUnloadedMessages() const;
		bool hasUnswappedMessages() const;
		bool hasEnqueued(RotationMethod method) const;
		bool hasEnqueuedLogs() const;
		bool hasEnqueuedPulls() const;
		bool hasEnqueuedFeeds() const;
		bool hasEnqueuedSwaps() const;
		bool hasAssistant() const;
		bool isPushing() const;
		bool isInitialized() const;
	/** @} */
	};

	inline uint qHash(const std::pair<Meta::Message::Id, Meta::Message::Id>& pair)
	{
		return ::qHash(pair.first, 0) ^ ::qHash(pair.second, 0);
	}
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Conversation::Model::GroupChannel::PullFlags)
Q_DECLARE_METATYPE(Cadabra::Queen::Conversation::Model::GroupChannel::PullFlags)

#endif
