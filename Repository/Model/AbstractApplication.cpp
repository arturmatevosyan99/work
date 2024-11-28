/**
 @file AbstractApplication.cpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#include "AbstractApplication.hpp"

#include "AceEvent.hpp"
#include "Action/HandleAction.hpp"
#include "Descriptor/SessionDescriptor.hpp"
#include "../Meta/ApplicationRender.hpp"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AbstractApplication::AbstractApplication(QObject* parent, bool upstream)
	:
		QObject(parent),

		_descriptor(nullptr),
		_ack(0),
		_upstream_onoff(upstream),
		_upstream_pinger(new QTimer(this)),
		_lost_pinger(0)
	{
/*
		QList<QSslCertificate> cert = QSslCertificate::fromPath(QLatin1String("server-certificate.pem"));
		QSslError ssl_error(QSslError::SelfSignedCertificate, cert.at(0));
		QList<QSslError> ssl_error_list;
		ssl_error_list.append(ssl_error);
		_session_socket.ignoreSslErrors(ssl_error_list);
*/
		QObject::connect(this, &AbstractApplication::exceptionThrew, this, &AbstractApplication::onExceptionCaught);

		QObject::connect(&_upstream_socket, &QWebSocket::connected, this, &AbstractApplication::onUpstreamConnect);
		QObject::connect(&_upstream_socket, &QWebSocket::disconnected, this, &AbstractApplication::onUpstreamDisconnect);
		QObject::connect(&_upstream_socket, &QWebSocket::stateChanged, this, &AbstractApplication::onUpstreamState);
		QObject::connect(&_upstream_socket, qOverload<QAbstractSocket::SocketError>(&QWebSocket::error), this, &AbstractApplication::onUpstreamError);
		QObject::connect(&_upstream_socket, &QWebSocket::pong, this, &AbstractApplication::onPong);
		QObject::connect(&_upstream_socket, &QWebSocket::textMessageReceived, this, qOverload<const QString&>(&AbstractApplication::onUpstreamData));
		QObject::connect(&_upstream_socket, &QWebSocket::binaryMessageReceived, this, qOverload<const QByteArray&>(&AbstractApplication::onUpstreamData));

		QObject::connect(_upstream_pinger, &QTimer::timeout, this, &AbstractApplication::onPingTime);
	}

	AbstractApplication::~AbstractApplication()
	{
		if (hasDescriptor()) {
			/// @destruct
		}
		close();
	}

	bool AbstractApplication::open(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor)
	{
		if (_descriptor || !descriptor) {
			emit unopened();
			return false;
		}

		_descriptor = descriptor;
		if (!onHandle()) {
			_descriptor = nullptr;
			qCritical() << "Handle application error";
			emit unopened();
			return false;
		}

		if (_descriptor->hasRealm() && !_descriptor->getRealm()->getHierarchy()->getObjectModel()->option(_descriptor->getObjectId(), "_setup").toBool()) {
			if (!onSetup()) {
				_descriptor = nullptr;
				qCritical() << "Setup application error";
				emit unopened();
				return false;
			} else {
				_descriptor->getRealm()->getHierarchy()->getObjectModel()->opt(_descriptor->getObjectId(), "_setup", true);
			}
		}

		if (!onOpen()) {
			_descriptor = nullptr;
			qCritical() << "Open application error";
			emit unopened();
			return false;
		}
		_descriptor->getRealm()->getHierarchy()->getObjectModel()->access(_descriptor->getObjectId());

		_descriptor->installEventFilter(this);
		QObject::connect(_descriptor, &AbstractDescriptor::destroyed, this, &AbstractApplication::onDisposed);
		if (_upstream_onoff) {
			upStream(true);
		}

		emit opened();
		emit descriptorChanged();
		return true;
	}

	bool AbstractApplication::clean(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor)
	{
		if (_descriptor || !descriptor) {
			return false;
		}

		_descriptor = descriptor;
		if (!onHandle()) {
			_descriptor = nullptr;
			qCritical() << "Handle application error";
			return false;
		}

		bool retval(true);
		if (_descriptor->hasRealm() && _descriptor->getRealm()->getHierarchy()->getObjectModel()->option(_descriptor->getObjectId(), "_setup").toBool()) {
			if (!onSetdown()) {
				retval = false;
				qCritical() << "Setdown application error";

			} else {
				_descriptor->getRealm()->getHierarchy()->getObjectModel()->opt(_descriptor->getObjectId(), "_setup", false);
			}
		}

		if (!onTeardown()) {
			qCritical() << "Teardown application error";
		}
		_descriptor = nullptr;
		return retval;
	}

	bool AbstractApplication::reopen()
	{
		if (!_descriptor) {
			return false;
		}

		if (!onReopen()) {
			qCritical() << "Reopen application error";
			return false;
		}

		if (_upstream_onoff) {
			upStream(true);
		}

		emit reopened();
		return true;
	}

	bool AbstractApplication::close()
	{
		if (!_descriptor) {
			return false;
		}

		_descriptor->removeEventFilter(this);
		QObject::disconnect
		(
			_descriptor,
			&AbstractDescriptor::destroyed,
			this,
			&AbstractApplication::onDisposed
		);

		if (!onTeardown()) {
			qCritical() << "Teardown application error";
		}
		_descriptor = nullptr;
		if (_upstream_socket.isValid()) {
			_upstream_socket.close();
		}

		if (!onClose()) {
			qWarning() << "Close application error";
			return false;
		}
		emit closed();
		emit descriptorChanged();
		return true;
	}

	void AbstractApplication::upStream(bool onoff)
	{
		_upstream_onoff = onoff;
		if (_descriptor && _descriptor->hasRealm() && _upstream_onoff && !_upstream_socket.isValid()) {
			/// @refactor as ObjectStream
			QUrl url;
			url.setScheme(_descriptor->getRealm()->getEndpoint()->isSecured() ? "wss" : "ws");
			url.setHost(_descriptor->getRealm()->getEndpoint()->getHost());
			url.setPort(_descriptor->getRealm()->getEndpoint()->getPort());
			url.setPath("/repository/object/channel" + _descriptor->getMoniker());
//			url.setPath("/repository/object/upstream" + _descriptor->getMoniker());

			QNetworkRequest request(url);
			request.setHeader(
								QNetworkRequest::UserAgentHeader,
								QString("cadabra.%1/%2").arg
								(
									QCoreApplication::applicationName(),
									QCoreApplication::applicationVersion()
								)
			);
			request.setRawHeader(
									QByteArray("Sec-WebSocket-Protocol"),
									QByteArray("CRP")
			);
			_upstream_socket.setProxy(_descriptor->getRealm()->getEndpoint()->getProxy());
			_upstream_socket.open(request);
		} else if (!_upstream_onoff && _upstream_socket.isValid()) {
			_upstream_socket.close();
		}
	}

	Core::System::StateAlert* AbstractApplication::setState(const QString& name, const QString& title, const QString& description, Core::System::StateAlert::Level level)
	{
		Core::System::StateAlert* state(findChild<Core::System::StateAlert*>(name));
		if (!state) {
			QScopedPointer<Core::System::StateAlert> state(new Core::System::StateAlert(this, name, title, description, this, level));
			return Core::System::Alert::Instance()->enqueue(state.get()) ? state.take() : nullptr;
		}
		if (!title.isEmpty()) {
			state->setTitle(title);
			if (!description.isEmpty()) {
				state->setDescription(description);
			}
			state->setLevel(level);
			return state;
		}
		if (!description.isEmpty()) {
			state->setDescription(description);
		}
		return state;
	}

	bool AbstractApplication::resetState(const QString& name)
	{
		Core::System::StateAlert* state(findChild<Core::System::StateAlert*>(name));
		return state && Core::System::Alert::Instance()->dequeue(state);
	}

	void AbstractApplication::dequeue()
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),
			this,
			qOverload<QJsonValue>(&AbstractApplication::onDequeued)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),
			this,
			qOverload<quint8, quint32, QString>(&AbstractApplication::onDequeued)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::EventQueueInterface",
					"Cadabra::Ace::Repository::Model::Dequeue",
					{
						{"ack", QString::number(_ack)}
					}
		);
	}

	void AbstractApplication::acknowledge()
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect
		(
			call.get(),
			qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),
			this,
			qOverload<QJsonValue>(&AbstractApplication::onAcknowledge)
		);
		QObject::connect
		(
			call.get(),
			qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),
			this,
			qOverload<quint8, quint32, QString>(&AbstractApplication::onAcknowledge)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::EventQueueInterface",
					"Cadabra::Ace::Repository::Model::Acknowledge",
					{
						{"ack", QString::number(_ack)}
					}
		);
	}

	AbstractDescriptor* AbstractApplication::getDescriptor() const
	{
		return _descriptor;
	}

	Meta::Object::ApplicationSchemeIdent AbstractApplication::getSchemeIdent() const
	{
		const int type_id(QMetaType::type(QString("*").prepend(this->metaObject()->className()).toLatin1().data()));
		return QMetaType::UnknownType == type_id ? 0 : Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Ident(QMetaType(type_id));
	}

	Meta::Object::ApplicationScheme AbstractApplication::getScheme() const
	{
		return QString::number(getSchemeIdent());
	}

	Session* AbstractApplication::getSession() const
	{
		return nullptr != _descriptor ? _descriptor->getRealm()->isInitialized() ? _descriptor->getRealm()->getSessionDescriptor()->getSession() : nullptr : nullptr;
	}

	QVariant AbstractApplication::getMetaValue(const QString& key) const
	{
		if (key.at(0) == '_' || !hasDescriptor()) {
			return QVariant();
		}
		return _descriptor->getRealm()->getHierarchy()->getObjectModel()->option(_descriptor->getObjectId(), key);
	}

	QUrl AbstractApplication::getFileEndpoint(const QString& file_name) const
	{
		Core::Net::Endpoint* endpoint(nullptr);
		if (hasDescriptor()) {
			endpoint = getDescriptor()->getRealm()->getEndpoint();
		}
		if (!endpoint) {
			return QUrl();
		}

		QUrl retval;
		retval.setScheme(endpoint->isSecured() ? "https" : "http");
		retval.setHost(endpoint->getHost());
		retval.setPort(endpoint->getPort());
		retval.setPath("/repository/object/file");

		QUrlQuery query(getDescriptor()->makeMoniker());
		if (!file_name.isEmpty()) {
			query.addQueryItem("file_name", file_name);
		}
		retval.setQuery(query);
		return retval;
	}

	void AbstractApplication::resetPingInterval(int timeout)
	{
		_upstream_pinger->setInterval(timeout);
	}

	bool AbstractApplication::setMetaValue(const QString& key, const QVariant& value)
	{
		if (key.at(0) == '_' || !hasDescriptor()) {
			return false;
		}
		return _descriptor->getRealm()->getHierarchy()->getObjectModel()->opt(_descriptor->getObjectId(), key, value);
	}

	void AbstractApplication::onDequeued(quint8 domain, quint32 code, QString message)
	{

	}

	void AbstractApplication::onDequeued(QJsonValue event)
	{
		_ack = event["ack"].toVariant().toULongLong();
		/// @note Пока очередь пуста
		if (0 == _ack) {
			return ;
		}

		onQueueEvent(event);
		dequeue(); /// @note more?
	}

	void AbstractApplication::onAcknowledge(quint8 domain, quint32 code, QString message)
	{

	}

	void AbstractApplication::onAcknowledge(QJsonValue payload)
	{
		_ack = 0;
	}

	bool AbstractApplication::onHandle()
	{
		return true;
	}

	bool AbstractApplication::onOpen()
	{
		return true;
	}

	bool AbstractApplication::onReopen()
	{
		return true;
	}

	bool AbstractApplication::onClose()
	{
		return true;
	}

	bool AbstractApplication::onTeardown()
	{
		return true;
	}

	bool AbstractApplication::onSetup()
	{
		return true;
	}

	bool AbstractApplication::onSetdown()
	{
		return true;
	}

	void AbstractApplication::onCall(bool start)
	{
		emit callmapChanged(isEmptyCallMap());
	}

	void AbstractApplication::onPingTime()
	{
		if (_upstream_socket.isValid()) {
			if (_lost_pinger.fetchAndAddRelease(1) <= 3) {
				/// @??? После signout продолжаются попытки
				_upstream_socket.ping();
			} else {
				qWarning("Too many pings are lost, probably this upstream is obsolete or detached from proxy application");
				_upstream_socket.close();
			}
		} else {
			qWarning("Could not ping invalid upstream");
		}
	}

	void AbstractApplication::onDisposed()
	{
		close();
	}

	void AbstractApplication::onSignal(quint16 signal, QDataStream& stream)
	{

	}

	void AbstractApplication::onQueueEvent(const QJsonValue& body)
	{
		qDebug() << "Queue event:" << body;
		AceEvent event
		(
			body["name"].toString(),
			Meta::RealmTarget
			(
				static_cast<Meta::Essence::Target>(body["target"]["target"].toInt(0)),
				body["target"]["id"].toVariant().toLongLong()
			)
		);
		if (event.import(body["payload"])) {
			/// @brief local dispatch (check target with descriptor?)
			QCoreApplication::sendEvent(this, &event);
		}
	}

	void AbstractApplication::onExceptionCaught(Core::ApplicationMessage level, QString message)
	{
		switch (level) {
			case Core::ApplicationMessage::FatalMessage:
				qFatal("%s", qUtf8Printable(message));
				break;

			case Core::ApplicationMessage::CriticalMessage:
				qCritical() << message;
				break;

			case Core::ApplicationMessage::WarningMessage:
				qWarning() << message;
				break;

			case Core::ApplicationMessage::InfoMessage:
				qInfo() << message;
				break;

			case Core::ApplicationMessage::DebugMessage:
				qDebug() << message;
				break;
		}
	}

	bool AbstractApplication::eventFilter(QObject* object, QEvent* event)
	{
		if (object != _descriptor) {
			return false;
		} else if (event->type() == Repository::Model::AceEvent::Action<HandleAction>::RegisterEventType(0)) {
			HandleAction* handle_action(Repository::Model::AceEvent::TypeCast<HandleAction>(static_cast<Repository::Model::AceEvent*>(event)));
			switch (handle_action->getCause()) {
				case HandleAction::Cause::Invalidation:
					close();
					break;
				case HandleAction::Cause::Revalidation:
					reopen();
					break;

				default:
					return false;
			}
		} else {
			customEvent(event);
		}
		return false;
	}

	void AbstractApplication::customEvent(QEvent* event)
	{
		return QObject::customEvent(event);
	}

	void AbstractApplication::onUpstreamData(const QString& message)
	{
		QJsonParseError error;
		const QJsonDocument document(QJsonDocument::fromJson(message.toUtf8(), &error));
		if (error.error != QJsonParseError::NoError || !document.isObject()) {
			qWarning("Upstream data malformed: %s", qUtf8Printable(error.errorString()));
			return ;
		}
		/// @todo по сохраненному uid можно определить что вызывали и какой callback дернуть обратно, а если нет, то через общую калитку onActionChannel(action, document);
	}

	void AbstractApplication::onUpstreamData(const QByteArray& message)
	{
		QDataStream stream(message);
		quint16 signal;
		stream >> signal;

		switch (signal) {
			case 0:	return dequeue();
			default: return onSignal(signal, stream);
		}
	}

	void AbstractApplication::onUpstreamConnect()
	{
		qDebug("Upstream connected");
		if (_upstream_pinger->interval() > 0) {
			_lost_pinger = 0;
			_upstream_pinger->start();
			QObject::connect
			(
				_descriptor->getRealm()->getEndpoint(),
				&Core::Net::Endpoint::proxyChanged,
				this,
				&AbstractApplication::onUpstreamChange
			);
		}
		emit upstreamed(true);
	}

	void AbstractApplication::onUpstreamDisconnect()
	{
		qDebug("Upstream disconnected");
		if (_upstream_pinger->isActive()) {
			_lost_pinger = 0;
			_upstream_pinger->stop();
		}
		emit upstreamed(false);
		if (_upstream_onoff) {
			upStream(true);
		} else {
			QObject::disconnect
			(
				nullptr,
				&Core::Net::Endpoint::proxyChanged,
				this,
				&AbstractApplication::onUpstreamChange
			);
		}
	}

	void AbstractApplication::onUpstreamState(QAbstractSocket::SocketState state)
	{
		qDebug("Upstream state is changed: %d", state);
	}

	void AbstractApplication::onUpstreamError(const QAbstractSocket::SocketError& error)
	{
		qWarning("Upstream error: %d", error);
	}

	void AbstractApplication::onUpstreamChange()
	{
		qDebug("Upstream changed");
		_upstream_socket.close();
	}

	void AbstractApplication::onPong(quint64 roundtrip, const QByteArray& payload)
	{
		qDebug("Got a pong after %llu ms, current lost counter is %d", roundtrip, _lost_pinger.fetchAndSubRelaxed(1) - 1);
		const QByteArray moniker(getDescriptor()->getMoniker().toLatin1());
		if (payload != moniker) {
			if (payload == "140285") {
				qInfo("Ping(%s) is conditionally equal with Pong(%s)", moniker.data(), payload.data());
			} else {
				qWarning("Ping(%s) does not compatible with Pong(%s)", moniker.data(), payload.data());
				upStream(false);
			}
		}
	}

	bool AbstractApplication::hasDescriptor() const
	{
		return (nullptr != _descriptor);
	}

	bool AbstractApplication::hasUpstream() const
	{
		return _upstream_socket.isValid();
	}

	bool AbstractApplication::isUpstream() const
	{
		return _upstream_onoff;
	}

	bool AbstractApplication::isIdle() const
	{
		return isEmptyCallMap();
	}
} } } }
