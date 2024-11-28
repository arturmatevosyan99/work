#include "VideoLessonMessage.hpp"

#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	VideoLessonMessage::VideoLessonMessage(QObject* parent)
	:
		Conversation::Model::AppletMessage(parent, -8663136378210484214),

		_active(false)
	{

	}

	void VideoLessonMessage::fetch()
	{
		if (!isImported()) {
			return ;
		}
		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&VideoLessonMessage::onFetched));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&VideoLessonMessage::onFetched));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Fetch",
					{
						{"name", QString("%1.videolesson").arg(_index)}
					}
		);
	}

	void VideoLessonMessage::sign()
	{
		if (!isImported()) {
			return ;
		}
		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&VideoLessonMessage::onSigned));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&VideoLessonMessage::onSigned));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Sign",
					{
						{"name", QString("%1.videolesson").arg(_index)}
					}
		);
	}

	QJsonObject VideoLessonMessage::exportBody() const
	{
		QJsonObject retval(AppletMessage::exportBody());
		retval["progress"] = _local_progress > 0 ? _local_progress : _progress;
		retval["subject_id"] = QString::number(_subject_id);
		retval["index"] = _index;

		return retval;
	}

	bool VideoLessonMessage::importBody(const QJsonObject& body)
	{
		if (!AppletMessage::importBody(body)) {
			return false;
		}

		_progress = body["progress"].toDouble(0);
		_local_progress = 0;
		_subject_id = body["subject_id"].toString("0").toLongLong();
		_index = body["index"].toInt(0);
		activate(true);

		return true;
	}

	void VideoLessonMessage::activate(bool onoff)
	{
		if (onoff != _active) {
			_active = onoff;
			emit activated(_active);
		}
	}

	double VideoLessonMessage::getProgress() const
	{
		return _local_progress > 0 ? _local_progress : _progress;
	}

	Repository::Meta::Subject::Id VideoLessonMessage::getSubjectId() const
	{
		return _subject_id;
	}

	int VideoLessonMessage::getIndex() const
	{
		return _index;
	}

	void VideoLessonMessage::setProgress(double progress)
	{
		if (_progress < progress && getChannelDescriptor()->getSubjectId() == _subject_id) {
			_local_progress = progress;
			emit modified();
		}
	}

	void VideoLessonMessage::onFetched(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void VideoLessonMessage::onFetched(QJsonValue payload)
	{
		activate(true);
		if (!payload.isString()) {
			emit denied();
			return ;
		}
		emit accessed(QUrl(payload.toString()));
	}

	void VideoLessonMessage::onSigned(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void VideoLessonMessage::onSigned(QJsonValue payload)
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

	bool VideoLessonMessage::isActive() const
	{
		return _active;
	}

	bool VideoLessonMessage::isFetched() const
	{
		return _active;
	}
} } } }
