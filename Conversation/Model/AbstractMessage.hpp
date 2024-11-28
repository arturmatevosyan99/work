#ifndef Cadabra_Queen_Conversation_Model_AbstractMessage_hpp
#define Cadabra_Queen_Conversation_Model_AbstractMessage_hpp

#include "../Meta/Message.hpp"
#include "../Meta/Channel.hpp"
#include "../Meta/Assist.hpp"
#include "Accident/MessageDelivery.hpp"

#include "../../Repository/Meta/Subject.hpp"
#include "../../Repository/Model/AbstractDescriptor.hpp"

#include <QPointer>
#include <QUuid>
#include <QJsonValue>
#include <QClipboard>
#include <QFlags>
#include <QSet>
#include <QMap>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class GroupChannel;

	class AbstractMessage : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	public:
		enum Content
		{
			Empty	  = 0b00000000,
			Text	  = 0b00000001,
			Hyperlink = 0b00000010,
			Picture	  = 0b00000100,
			Video	  = 0b00001000,
			Audio	  = 0b00010000,
			File	  = 0b00100000,
			Document  = 0b01000000, /// < PDF, Handbook
			Applet	  = 0b10000000 /// < Artefact
		};
		Q_DECLARE_FLAGS(ContentType, Content)
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAG(ContentType)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::AbstractDescriptor* channelDescriptor READ getChannelDescriptor CONSTANT)
		Q_PROPERTY(QUuid uid READ getUId NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd selfQd READ getSelfQd NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Message::Id selfId READ getSelfId NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Message::Id replyId READ getReplyId NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Message::Scheme scheme READ getScheme NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd schemeQd READ getSchemeQd NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Subject::Id authorSubjectId READ getAuthorSubjectId NOTIFY imported)
		Q_PROPERTY(QDateTime postDateTime READ getPostDateTime NOTIFY imported)
		Q_PROPERTY(QDateTime modificationDateTime READ getModificationDateTime NOTIFY imported)
		Q_PROPERTY(qint64 actualTimestamp READ getTimestamp NOTIFY imported)
		Q_PROPERTY(QDateTime actualDateTime READ getDateTime NOTIFY imported)
		Q_PROPERTY(QString authorSubjectName READ getAuthorSubjectName NOTIFY imported)
		Q_PROPERTY(int progress READ getProgress NOTIFY progressed)
		Q_PROPERTY(QString previewText READ getPreviewText NOTIFY imported)
		Q_PROPERTY(QString preview READ getPreview NOTIFY imported)
		Q_PROPERTY(ContentType contentType READ getContentType NOTIFY imported)
		Q_PROPERTY(bool hasChannelDescriptor READ hasChannelDescriptor)
		Q_PROPERTY(bool hasClipboard READ hasClipboard CONSTANT)
		Q_PROPERTY(bool isImported READ isImported NOTIFY imported)
		Q_PROPERTY(bool isEditable READ isEditable NOTIFY imported)
		Q_PROPERTY(bool isErasable READ isErasable NOTIFY imported)
		Q_PROPERTY(bool isGlobal READ isGlobal NOTIFY progressed)
		Q_PROPERTY(bool isLocal READ isLocal NOTIFY progressed)
		Q_PROPERTY(bool isFaulty READ isFaulty NOTIFY progressed)
		Q_PROPERTY(bool isReceived READ isReceived NOTIFY delivered)
		Q_PROPERTY(bool isRead READ isRead NOTIFY delivered)
		Q_PROPERTY(bool inProgress READ inProgress NOTIFY progressed)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using MentionSet = QSet<Repository::Meta::Subject::Id>;
		using LinkMap = QMap<QUrl, Meta::Message::Link>;
		using AssistList = QList<Meta::Assist>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractMessage(QObject* parent = nullptr);
		virtual ~AbstractMessage() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QPointer<Repository::Model::AbstractDescriptor> _channel_descriptor;
		Meta::Message::Id _id;
		QUuid _uid;
		Meta::Message::Id _reply_id;
		Repository::Meta::SubjectId _subject_id;
		QDateTime _post_time;
		QDateTime _modification_time;
		int _progress;
		bool _is_imported;
		bool _is_received;
		bool _is_read;

		MentionSet _mention_set;
		LinkMap _link_map;
		AssistList _assist_list;

	protected:
		QFlags<Content> _content;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, const Cadabra::Queen::Conversation::Meta::Message& data);
		Q_INVOKABLE bool import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, Cadabra::Queen::Conversation::Meta::Message::Id id, QUuid uid, Cadabra::Queen::Conversation::Meta::Message::Id reply_id, Cadabra::Queen::Repository::Meta::Subject::Id subject_id, const QJsonObject& body, const QDateTime& post_time, const QDateTime& modification_time);
		Q_INVOKABLE Cadabra::Queen::Conversation::Meta::Message exportMeta() const;
		Q_INVOKABLE bool clip() const;

	protected:
		virtual QJsonObject exportBody() const = 0;
		virtual bool importBody(const QJsonObject& body) = 0;
		virtual bool clipBody() const;

	private:
		void delivery(const MessageDelivery::Map& map);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Repository::Model::AbstractDescriptor* getChannelDescriptor() const;
		Meta::Channel::Role getChannelRole() const;
		Meta::Message::Id getId() const;
		QUuid getUId() const;
		Core::Meta::Qd getSelfQd() const;
		Meta::Message::Id getSelfId() const;
		Meta::Message::Id getReplyId() const;
		Meta::Message::SchemeIdent getSchemeIdent() const;
		Core::Meta::Qd getSchemeQd() const;
		Meta::Message::SchemeIdent getScheme() const;
		Repository::Meta::Subject::Id getAuthorSubjectId() const;
		QDateTime getActualDateTime() const;
		QDateTime getPostDateTime() const;
		QDateTime getModificationDateTime() const;
		QString getAuthorSubjectName() const;
		QString getPreview() const;
		QDateTime getDateTime() const;
		qint64 getTimestamp() const;
		int getProgress() const;
		ContentType getContentType() const;
		virtual QString getPreviewText() const;
		const MentionSet& getMentionSet() const;
		const LinkMap& getLinkMap() const;
		virtual const AssistList& getAssistList() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void imported();
		void modified();
		void progressed();
		void delivered();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool eventFilter(QObject* object, QEvent* event) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasChannelDescriptor() const;
		bool hasAssistant() const;
		virtual bool hasClipboard() const;
		virtual bool isErasable() const;
		virtual bool isEditable() const;
		bool isImported() const;
		bool isGlobal() const;
		bool isLocal() const;
		bool isFaulty() const;
		bool isReceived() const;
		bool isRead() const;
		bool inProgress() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Conversation::Model::AbstractMessage::ContentType)
Q_DECLARE_METATYPE(Cadabra::Queen::Conversation::Model::AbstractMessage::ContentType)

#endif
