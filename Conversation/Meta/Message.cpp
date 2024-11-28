/**
 @file Message.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Message.hpp"

#include "../../Core/Meta/Qd.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	Message::Message()
	:
		_id(0),
		_reply_id(0),
		_scheme_ident(0),
		_subject_id(0)
	{

	}

	Message::Message(Id id, QUuid uid, Id reply_id, SchemeIdent scheme, Repository::Meta::Subject::Id subject_id, QJsonObject body, QDateTime post_time, QDateTime modification_time)
	:
		_id(id),
		_uid(uid),
		_reply_id(reply_id),
		_scheme_ident(scheme),
		_subject_id(subject_id),
		_body(body),
		_post_time(post_time),
		_modification_time(modification_time)
	{

	}

	Message::Message(const Message& origin)
	:
		_id(origin._id),
		_uid(origin._uid),
		_reply_id(origin._reply_id),
		_scheme_ident(origin._scheme_ident),
		_subject_id(origin._subject_id),
		_body(origin._body),
		_post_time(origin._post_time),
		_modification_time(origin._modification_time)
	{

	}

	Message::Message(Message&& origin)
	:
		_id(std::move(origin._id)),
		_uid(std::move(origin._uid)),
		_reply_id(std::move(origin._reply_id)),
		_scheme_ident(std::move(origin._scheme_ident)),
		_subject_id(std::move(origin._subject_id)),
		_body(std::move(origin._body)),
		_post_time(std::move(origin._post_time)),
		_modification_time(std::move(origin._modification_time))
	{

	}

	Message::~Message()
	{

	}

	Message::operator QJsonObject() const
	{
		return QJsonObject{
							{"id", QString::number(_id)},
							{"uid", _uid.toString(QUuid::StringFormat::WithoutBraces)},
							{"reply_id", QString::number(_reply_id)},
							{"scheme", QString::number(_scheme_ident)},
							{"author_subject_id", QString::number(_subject_id)},
							{"body", _body},
							{"post_timestamp", _post_time.toSecsSinceEpoch()},
							{"modification_timestamp", _modification_time.toSecsSinceEpoch()}
		};
	}

	Message& Message::operator=(const Message& rhs)
	{
		_id = rhs._id;
		_uid = rhs._uid;
		_reply_id = rhs._reply_id;
		_scheme_ident = rhs._scheme_ident;
		_body = rhs._body;
		_subject_id = rhs._subject_id;
		_post_time = rhs._post_time;
		_modification_time = rhs._modification_time;

		return *this;
	}

	Message& Message::operator=(Message&& rhs)
	{
		_id = std::move(rhs._id);
		_uid = std::move(rhs._uid);
		_reply_id = std::move(rhs._reply_id);
		_scheme_ident = std::move(rhs._scheme_ident);
		_body = std::move(rhs._body);
		_post_time = std::move(rhs._post_time);
		_post_time = std::move(rhs._post_time);
		_modification_time = std::move(rhs._modification_time);

		return *this;
	}

	QVariant Message::get(int role) const
	{
		return get(static_cast<Role>(role));
	}

	QVariant Message::get(Role role) const
	{
		switch (role) {
			case Role::Id: return QVariant::fromValue(Core::Meta::Qd(_id));
			case Role::UId: return _uid;
			case Role::ReplyId: return _reply_id;
			case Role::PostTimestamp: return _post_time;
			case Role::ModificationTimestamp: return _modification_time;
			case Role::AuthorSubjectId: return _subject_id;
			case Role::Scheme: return _scheme_ident;
			case Role::Body: return _body;

			default: return QVariant();
		}
	}

	const Cadabra::Queen::Conversation::Meta::Message::Id& Message::getId() const
	{
		return _id;
	}

	const QUuid& Message::getUId() const
	{
		return _uid;
	}

	const Cadabra::Queen::Conversation::Meta::Message::Id& Message::getReplyId() const
	{
		return _reply_id;
	}

	const Cadabra::Queen::Repository::Meta::Subject::Id& Message::getSubjectId() const
	{
		return _subject_id;
	}

	const Cadabra::Queen::Conversation::Meta::Message::SchemeIdent& Message::getSchemeIdent() const
	{
		return _scheme_ident;
	}

	const QJsonObject& Message::getBody() const
	{
		return _body;
	}

	const QDateTime& Message::getPostTime() const
	{
		return _post_time;
	}

	const QDateTime& Message::getModificationTime() const
	{
		return _modification_time;
	}
} } } }
