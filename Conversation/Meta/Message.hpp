/**
 @file Message.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Conversation_Meta_Message_hpp
#define Cadabra_Queen_Conversation_Meta_Message_hpp

#include "../../Repository/Meta/Subject.hpp"

#include <QObject>
#include <QSharedPointer>
#include <QJsonObject>
#include <QDateTime>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	/**
	 * @brief Message meta information
	 */
	class Message
	{
	/** @name Aliases */
	/** @{ */
	public:
		/**
		 * @brief Channel(Chat) event
		 */
		enum class Incident : qint8
		{
			Unknown = '0',
			Flush = 'F', ///< Internal synchonization event and nothing important for anybody else
			Push = 'P', ///< Message is pushed
			Update = 'U', ///< Message is modified
			Rerender = 'R', ///< Message is replaced
			Withdraw = 'W', ///< Message is erased
			Mute = 'M', ///< Channel is muted by member
			Topic = 'T', ///< Topic is changed
			Kick = 'K', ///< Member is kicked
			Ban = 'B', ///< Member is banned
			Join = 'J', ///< Member has joined
			Leave = 'L', ///< Member has left
			Role = 'R', ///< Member role is changed(down or up-graded)
			Tag = 't', ///< Message is tagged
			Receive = 'r', ///< Message is received
			View = 'v' ///< Message is viewed
		};

		/**
		 * @brief Field role in a data model
		 */
		enum class Role : int
		{
			Id = Qt::UserRole + 1, ///< Unique id in the feed
			UId, ///< Unique id in the draft
			ReplyId, ///< Original message id in the feed
			PostTimestamp, ///< Creation timestamp
			ModificationTimestamp, ///< Last modification timestamp
			AuthorSubjectId, ///< Subject id of author in the Realm
			Scheme, ///< Message type (`-8034428181974876159` means Text message)
			Body ///< Message payload
		};

		/**
		 * @brief The Link
		 */
		enum Link
		{
			File = 'F',
			Picture = 'P',
			Audio = 'A',
			Voice = 'V',
			Video = 'V',
			Hyper = 'H' /// < special params: ~weight(order), ~title(caption) ~key(click index) ~state=(disabled, enabled=default, switchedon, switchedoff)
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_ENUM(Incident)
		Q_ENUM(Role)
		/**
		 * @brief Message unique id in the feed
		 */
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Message::Id id READ getId)
		/**
		 * @brief Message unique id in the draft
		 */
		Q_PROPERTY(QUuid uid READ getUId)
		/**
		 * @brief Original message id in the feed
		 */
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Message::Id replyId READ getReplyId)
		/**
		 * @brief Message type to use appropriate renderer
		 */
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Message::SchemeIdent scheme READ getSchemeIdent)
		/**
		 * @brief Subject id
		 */
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Subject::Id subjectId READ getSubjectId)
		/**
		 * @brief Message payload
		 */
		Q_PROPERTY(QJsonValue body READ getBody)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Id = qint64;
		using Scheme = Repository::Meta::Essence::Scheme;
		using SchemeIdent = Repository::Meta::Essence::SchemeIdent;
	/** @} */

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
		Message();
		Message(Id id, QUuid uid, Id reply_id, SchemeIdent scheme, Repository::Meta::Subject::Id subject_id, QJsonObject body, QDateTime post_time = QDateTime(), QDateTime modification_time = QDateTime());
		Message(const Message& origin);
		Message(Message&& origin);
		~Message();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Id _id;
		QUuid _uid;
		Id _reply_id;
		SchemeIdent _scheme_ident;
		Repository::Meta::Subject::Id _subject_id;
		QJsonObject _body;
		QDateTime _post_time;
		QDateTime _modification_time;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator QJsonObject() const;
		Message& operator=(const Message& rhs);
		Message& operator=(Message&& rhs);
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QVariant get(int role) const;
		QVariant get(Role role) const;
		const Cadabra::Queen::Conversation::Meta::Message::Id& getId() const;
		const QUuid& getUId() const;
		const Cadabra::Queen::Conversation::Meta::Message::Id& getReplyId() const;
		const Cadabra::Queen::Repository::Meta::Subject::Id& getSubjectId() const;
		const Cadabra::Queen::Conversation::Meta::Message::SchemeIdent& getSchemeIdent() const;
		const QJsonObject& getBody() const;
		const QDateTime& getPostTime() const;
		const QDateTime& getModificationTime() const;
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
