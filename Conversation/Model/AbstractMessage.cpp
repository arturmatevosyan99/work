#include "AbstractMessage.hpp"

#include "Event.hpp"
#include "Application/GroupChannel.hpp"
#include "Accident/MessageTransfer.hpp"
#include "Accident/MessageDelivery.hpp"
#include "../Meta/MessageRender.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	AbstractMessage::AbstractMessage(QObject* parent)
	:
		QObject(parent),

		_channel_descriptor(nullptr),
		_id(0),
		_reply_id(0),
		_subject_id(0),
		_progress(0),
		_is_imported(false),
		_is_received(false),
		_is_read(false)
	{

	}

	AbstractMessage::~AbstractMessage()
	{
		if (!_channel_descriptor.isNull() && _is_imported) {
			_channel_descriptor->removeEventFilter(this);
		}
	}

	bool AbstractMessage::import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, const Cadabra::Queen::Conversation::Meta::Message& message)
	{
		return message.getSchemeIdent() != getSchemeIdent()
		? false
		: import
		(
			channel_application,
			message.getId(),
			message.getUId(),
			message.getReplyId(),
			message.getSubjectId(),
			message.getBody(),
			message.getPostTime(),
			message.getModificationTime()
		);
	}

	bool AbstractMessage::import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, Cadabra::Queen::Conversation::Meta::Message::Id id, QUuid uid, Cadabra::Queen::Conversation::Meta::Message::Id reply_id, Cadabra::Queen::Repository::Meta::Subject::Id subject_id, const QJsonObject& body, const QDateTime& post_time, const QDateTime& modification_time)
	{
		_channel_descriptor = !channel_application ? nullptr : channel_application->getDescriptor();
		_id = id;
		_uid = uid;
		_reply_id = reply_id;
		_subject_id = subject_id;
		_post_time = post_time;
		_modification_time = modification_time;
		_content = QFlags<Content>();
		_assist_list.clear();
		_is_received = false;
		_is_read = false;

		if (!_channel_descriptor || !importBody(body)) {
			_is_imported = false;
			return false;
		} else {
			delivery(channel_application->getDeliveryMap());
		}

		QJsonObject spec_object;
		QJsonObject::ConstIterator s(body.find("$"));
		if (s != body.constEnd() && s->isObject()) {
			spec_object = s->toObject();

			QJsonObject::ConstIterator m(spec_object.find("mentions"));
			if (m != spec_object.constEnd() && m->isArray()) {
				QJsonArray mention_array(m->toArray());
				for (QJsonArray::ConstIterator mm = mention_array.constBegin(); mm != mention_array.constEnd(); ++mm) {
					_mention_set.insert(Repository::Meta::Essence::ToId(*m, true));
				}
			}
			QJsonObject::ConstIterator l(spec_object.find("links"));
			if (l != spec_object.constEnd() && l->isObject()) {
				QJsonObject link_object(l->toObject());
				for (QJsonObject::ConstIterator ll = link_object.constBegin(); ll != link_object.constEnd(); ++ll) {
					if (ll->isDouble()) {
						_link_map.insert(ll.key(), static_cast<Meta::Message::Link>(ll->toInt(0)));
					}
				}
			}
		} else if ((s = body.find("$assistant")) != body.constEnd() && s->isObject()) { /// < backward compatibility
			spec_object = s->toObject();
		}

		const QJsonObject::ConstIterator a(spec_object.find("assists"));
		if (spec_object.constEnd() != a && a->isArray()) {
			const QJsonArray assist_array(a->toArray());
			for (QJsonArray::ConstIterator aa = assist_array.constBegin(); aa != assist_array.constEnd(); ++aa) {
				if (aa->isObject()) {
					Meta::Assist meta(aa->toObject());
					if (
						meta.isValid()
						&&
						(
							meta.getAffectedSubjectIdSet().empty()
							||
							meta.getAffectedSubjectIdSet().contains(_channel_descriptor->getSubjectId())
						)
					) {
						_assist_list.push_back(meta);
					}
				}
			}
		}

		if (_is_imported) {
			_channel_descriptor->removeEventFilter(this);
		} else {
			_is_imported = true;
		}

		if (_id > 0) {
			_progress = 100;
			emit imported();
			emit progressed();
		} else if (nullptr != _channel_descriptor) {
			_channel_descriptor->installEventFilter(this);
			emit imported();
		}

		return true;
	}

	Cadabra::Queen::Conversation::Meta::Message AbstractMessage::exportMeta() const
	{
		return Meta::Message
		(
			_id,
			_uid,
			_reply_id,
			getSchemeIdent(),
			_subject_id,
			exportBody(),
			_post_time,
			_modification_time
		);
	}

	bool AbstractMessage::clip() const
	{
		return hasClipboard() ? clipBody() : false;
	}

	bool AbstractMessage::clipBody() const
	{
		return false;
	}

	void AbstractMessage::delivery(const MessageDelivery::Map& map)
	{
		bool is_received(_is_received), is_read(_is_read);
		for (MessageDelivery::Map::ConstIterator d = map.constBegin(); d != map.constEnd(); ++d) {
			if (d.key() != _subject_id) {
				if (!is_read) {
					is_received = is_received || (_id <= d.value().first);
					is_read = (_id <= d.value().second);
				} else if (!is_received) {
					is_received = (_id <= d.value().first);
				} else {
					break;
				}
			}
		}
		if (_is_received != is_received || _is_read != is_read) {
			_is_received = is_received;
			_is_read = is_read;
			emit delivered();
		}
	}

	Cadabra::Queen::Repository::Model::AbstractDescriptor* AbstractMessage::getChannelDescriptor() const
	{
		return _channel_descriptor;
	}

	Meta::Channel::Role AbstractMessage::getChannelRole() const
	{
		return Meta::Channel::Role::Listener;
	}

	Cadabra::Queen::Conversation::Meta::Message::Id AbstractMessage::getId() const
	{
		return _id;
	}

	QUuid AbstractMessage::getUId() const
	{
		return _uid;
	}

	Cadabra::Queen::Core::Meta::Qd AbstractMessage::getSelfQd() const
	{
		return _id;
	}

	Meta::Message::Id AbstractMessage::getSelfId() const
	{
		return _id;
	}

	Meta::Message::Id AbstractMessage::getReplyId() const
	{
		return  _reply_id;
	}

	Meta::Message::SchemeIdent AbstractMessage::getSchemeIdent() const
	{
		const int type_id(QMetaType::type(QString("*").prepend(this->metaObject()->className()).toLatin1().data()));
		return QMetaType::UnknownType == type_id ? 0 : Core::Meta::Scheme<Meta::MessageRender>::Ident(QMetaType(type_id));
	}

	Core::Meta::Qd AbstractMessage::getSchemeQd() const
	{
		return getSchemeIdent();
	}

	Meta::Message::SchemeIdent AbstractMessage::getScheme() const
	{
		return getSchemeIdent();
	}

	Repository::Meta::Subject::Id AbstractMessage::getAuthorSubjectId() const
	{
		return _subject_id;
	}

	QDateTime AbstractMessage::getActualDateTime() const
	{
		return (_post_time > _modification_time) ?  _post_time : _modification_time;
	}

	QDateTime AbstractMessage::getPostDateTime() const
	{
		return _post_time;
	}

	QDateTime AbstractMessage::getModificationDateTime() const
	{
		return _modification_time;
	}

	QString AbstractMessage::getAuthorSubjectName() const
	{
		QSharedPointer<const Repository::Meta::Subject> subject(_channel_descriptor ? _channel_descriptor->getRealm()->getHierarchy()->getSubjectModel()->getSubject(_subject_id) : nullptr);
		return subject ? subject->getName() : "";
	}

	QString AbstractMessage::getPreview() const
	{
		return !_is_imported ? "..." : QString("@%1: %2").arg(getAuthorSubjectName(), getPreviewText());
	}

	QDateTime AbstractMessage::getDateTime() const
	{
		return !_post_time.isValid() ? QDateTime() : _modification_time.isValid() ? _modification_time > _post_time ? _modification_time : _post_time : _post_time;
	}

	qint64 AbstractMessage::getTimestamp() const
	{
		return !_post_time.isValid() ? 0 : (_modification_time.isValid() && _modification_time > _post_time) ? _modification_time.toSecsSinceEpoch() : _post_time.toSecsSinceEpoch();
	}

	int AbstractMessage::getProgress() const
	{
		return _progress;
	}

	QString AbstractMessage::getPreviewText() const
	{
		return "...";
	}

	const AbstractMessage::MentionSet& AbstractMessage::getMentionSet() const
	{
		return _mention_set;
	}

	const AbstractMessage::LinkMap& AbstractMessage::getLinkMap() const
	{
		return _link_map;
	}

	const AbstractMessage::AssistList& AbstractMessage::getAssistList() const
	{
		return _assist_list;
	}

	AbstractMessage::ContentType AbstractMessage::getContentType() const
	{
		return _content;
	}

	bool AbstractMessage::eventFilter(QObject* object, QEvent* event)
	{
		if (object == _channel_descriptor) {
			if (event->type() == Event::Action<MessageTransfer>::RegisterEventType()) {
				MessageTransfer* accident(Event::TypeCast<MessageTransfer>(event));
				if (accident && accident->getUId() == _uid) {
					_progress = accident->getProgress();
					emit progressed();
					if (accident->isLast()) {
						_channel_descriptor->removeEventFilter(this);
					}
				}
			} else if (event->type() == Event::Action<MessageDelivery>::RegisterEventType() && (!_is_received || !_is_read)) {
				delivery(Event::TypeCast<MessageDelivery>(event)->getMap());
			}
		}
		return false;
	}

	bool AbstractMessage::hasChannelDescriptor() const
	{
		return nullptr != _channel_descriptor;
	}

	bool AbstractMessage::hasAssistant() const
	{
		return !_assist_list.empty();
	}

	bool AbstractMessage::hasClipboard() const
	{
		return false;
	}

	bool AbstractMessage::isErasable() const
	{
		return _is_imported && _channel_descriptor->getSubjectId() == _subject_id;
	}

	bool AbstractMessage::isEditable() const
	{
		return false;
	}

	bool AbstractMessage::isImported() const
	{
		return _is_imported;
	}

	bool AbstractMessage::isGlobal() const
	{
		return _id > 0;
	}

	bool AbstractMessage::isLocal() const
	{
		return _id == 0;
	}

	bool AbstractMessage::isFaulty() const
	{
		return _progress < 0;
	}

	bool AbstractMessage::isReceived() const
	{
		return _is_received;
	}

	bool AbstractMessage::isRead() const
	{
		return _is_read;
	}

	bool AbstractMessage::inProgress() const
	{
		return _progress > 0 && _progress < 100;
	}
} } } }
