#include "InteractiveApplicationMessage.hpp"

#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	InteractiveApplicationMessage::InteractiveApplicationMessage(QObject* parent)
	:
		Conversation::Model::AppletMessage(parent, -8663136378210484212),

		_active(false)
	{

	}

	InteractiveApplicationMessage::~InteractiveApplicationMessage()
	{

	}

	void InteractiveApplicationMessage::fetch()
	{
		if (!isImported()) {
			return ;
		}

		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&InteractiveApplicationMessage::onFetched));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&InteractiveApplicationMessage::onFetched));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Fetch",
					{
						{"name", QString("%1.interactiveapplication").arg(_index)}
					}
		);
	}

	void InteractiveApplicationMessage::sign()
	{
		if (!isImported()) {
			return ;
		}

		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&InteractiveApplicationMessage::onSigned));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&InteractiveApplicationMessage::onSigned));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Sign",
					{
						{"name", QString("%1.interactiveapplication").arg(_index)}
					}
		);
	}

	QJsonObject InteractiveApplicationMessage::exportBody() const
	{
		QJsonObject retval(AppletMessage::exportBody());
		retval["review"] = _local_review  ? true : _review;
		retval["subject_id"] = QString::number(_subject_id);
		retval["index"] = _index;

		return retval;
	}

	bool InteractiveApplicationMessage::importBody(const QJsonObject& body)
	{
		if (!AppletMessage::importBody(body)) {
			return false;
		}

		_review = body["review"].toBool();
		_local_review = false;
		_subject_id = body["subject_id"].toString("0").toLongLong();
		_index = body["index"].toInt(0);
		activate(true);

		return true;
	}

	void InteractiveApplicationMessage::activate(bool onoff)
	{
		if (onoff != _active) {
			_active = onoff;
			emit activated(_active);
		}
	}

	Repository::Meta::Subject::Id InteractiveApplicationMessage::getSubjectId() const
	{
		return _subject_id;
	}

	int InteractiveApplicationMessage::getIndex() const
	{
		return _index;
	}

	void InteractiveApplicationMessage::setReview(bool review)
	{
		if (_local_review < review && getChannelDescriptor()->getSubjectId() == _subject_id) {
			_local_review = review;
			emit modified();
		}
	}

	void InteractiveApplicationMessage::onFetched(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void InteractiveApplicationMessage::onFetched(QJsonValue payload)
	{		
		activate(true);
		if (!payload.isString()) {
			emit denied();
			return ;
		}
		emit accessed(QUrl(payload.toString()));
	}

	void InteractiveApplicationMessage::onSigned(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void InteractiveApplicationMessage::onSigned(QJsonValue payload)
	{
		activate(true);
		if (!payload.isObject()) {
			emit denied();
			return ;
		}
		/// @todo cache : resource_locator, life_time, token_map
		const QJsonObject object(payload.toObject());
		emit accessed(object.value("resource_locator").toString(""));
	}

	bool InteractiveApplicationMessage::isActive() const
	{
		return _active;
	}

	bool InteractiveApplicationMessage::isFetched() const
	{
		return _active;
	}

	bool InteractiveApplicationMessage::isReviewed() const
	{
		return _local_review > 0 ? _local_review : _review;
	}
} } } }
