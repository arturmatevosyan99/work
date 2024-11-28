#include "HandBookMessage.hpp"

#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	HandBookMessage::HandBookMessage(QObject* parent)
	:
		Conversation::Model::AppletMessage(parent, -8663136378210484213),

		_active(false),
		_file("XXXXXX.pdf")
	{
		QObject::connect(&_file, &Core::Net::FileCall::loaded, this, qOverload<>(&HandBookMessage::onLoaded));
		QObject::connect(&_file, &Core::Net::FileCall::faulted, this, qOverload<QString>(&HandBookMessage::onLoaded));
	}

	HandBookMessage::~HandBookMessage()
	{

	}

	void HandBookMessage::fetch()
	{
		if (!isImported()) {
			return ;
		}
		if (_file.getFile().isOpen()) {
			emit accessed(_file.makeURL());
		}

		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&HandBookMessage::onFetched));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&HandBookMessage::onFetched));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Fetch",
					{
						{"name", QString("%1.handbook").arg(_index)}
					}
		);
	}

	void HandBookMessage::sign()
	{
		if (!isImported()) {
			return ;
		}
		if (_file.getFile().isOpen()) {
			emit accessed(_file.makeURL());
		}

		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&HandBookMessage::onSigned));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&HandBookMessage::onSigned));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Sign",
					{
						{"name", QString("%1.handbook").arg(_index)}
					}
		);
	}

	QJsonObject HandBookMessage::exportBody() const
	{
		QJsonObject retval(AppletMessage::exportBody());
		retval["review"] = _local_review  ? true : _review;
		retval["subject_id"] = QString::number(_subject_id);
		retval["index"] = _index;

		return retval;
	}

	bool HandBookMessage::importBody(const QJsonObject& body)
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

	void HandBookMessage::activate(bool onoff)
	{
		if (onoff != _active) {
			_active = onoff;
			emit activated(_active);
		}
	}

	Repository::Meta::Subject::Id HandBookMessage::getSubjectId() const
	{
		return _subject_id;
	}

	int HandBookMessage::getIndex() const
	{
		return _index;
	}

	void HandBookMessage::setReview(bool review)
	{
		if (_local_review < review && getChannelDescriptor()->getSubjectId() == _subject_id) {
			_local_review = review;
			emit modified();
		}
	}

	void HandBookMessage::onFetched(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void HandBookMessage::onFetched(QJsonValue payload)
	{
		if (!payload.isString()) {
			emit denied();
			return ;
		}
		_file.load(QUrl(payload.toString()));
	}

	void HandBookMessage::onSigned(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void HandBookMessage::onSigned(QJsonValue payload)
	{
		activate(true);
		if (!payload.isObject()) {
			emit denied();
			return ;
		}
		/// @todo cache : resource_locator, life_time, token_map
		const QJsonObject object(payload.toObject());
		_file.load(QUrl(object.value("resource_locator").toString("")));
	}

	void HandBookMessage::onLoaded(QString error)
	{
		activate(true);
		emit denied();
	}

	void HandBookMessage::onLoaded()
	{
		activate(true);
		emit accessed(_file.makeURL());
	}

	bool HandBookMessage::isActive() const
	{
		return _active;
	}

	bool HandBookMessage::isFetched() const
	{
		return _active;
	}

	bool HandBookMessage::isReviewed() const
	{
		return _local_review > 0 ? _local_review : _review;
	}
} } } }
