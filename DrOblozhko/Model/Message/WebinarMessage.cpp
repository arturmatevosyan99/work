#include "WebinarMessage.hpp"

#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"

#include <QtGlobal>
#include <QDesktopServices>
#include <QTimeZone>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	WebinarMessage::WebinarMessage(QObject* parent)
	:
		Conversation::Model::AppletMessage(parent, -8663136378210484204),

		_active(false)
	{

	}

	void WebinarMessage::fetch()
	{
		if (!isImported()) {
			return ;
		}
		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&WebinarMessage::onFetched));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&WebinarMessage::onFetched));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Fetch",
					{
					}
		);
	}

	void WebinarMessage::sign()
	{
		if (!isImported()) {
			return ;
		}
		activate(false);

		QSharedPointer<Core::Net::CRPCall> call(getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->makeCall(getId()));
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&WebinarMessage::onSigned));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&WebinarMessage::onSigned));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Sign",
					{
					}
		);
	}

	QJsonObject WebinarMessage::exportBody() const
	{
		QJsonObject retval(AppletMessage::exportBody());
		retval["host_name"] = _host_name;
		retval["broadcast_time_point"] = _broadcast_time_point.toSecsSinceEpoch();

		return retval;
	}

	bool WebinarMessage::importBody(const QJsonObject& body)
	{
		if (!AppletMessage::importBody(body)) {
			return false;
		}

		_host_name = body["host_name"].toString();
		_broadcast_time_point = QDateTime::fromSecsSinceEpoch(body["broadcast_time_point"].toInt(0), Qt::UTC);
		activate(true);

		return true;
	}

	void WebinarMessage::activate(bool onoff)
	{
		if (onoff != _active) {
			_active = onoff;
			emit activated(_active);
		}
	}

	QString WebinarMessage::getHostName() const
	{
		return _host_name;
	}

	QDateTime WebinarMessage::getBroadcastTimePoint() const
	{
		return _broadcast_time_point;
	}

	void WebinarMessage::onFetched(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void WebinarMessage::onFetched(QJsonValue payload)
	{
		activate(true);
		if (!payload.isString()) {
			emit denied();
			return ;
		}
		emit accessed(QUrl(payload.toString()));
	}

	void WebinarMessage::onSigned(quint8 domain, quint32 code, QString message)
	{
		activate(true);
		emit denied();
	}

	void WebinarMessage::onSigned(QJsonValue payload)
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

	bool WebinarMessage::isActive() const
	{
		return _active;
	}

	bool WebinarMessage::isFetched() const
	{
		return _active;
	}
} } } }
