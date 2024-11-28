#include "Appointment.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	Appointment::Appointment(QObject* parent)
	:
		QObject(parent),

		_session_id(0),
		_session_handle(0),
		_session_scheme(""),
		_channel_id(0),
		_channel_handle(0),
		_channel_scheme("")
	{
		QObject::connect(&_appointment_call, &Net::AppointmentCall::responded, this, &Appointment::onEnrollSuccess);
		QObject::connect(&_appointment_call, &Net::AppointmentCall::redirected, this, &Appointment::onEnrollRedirect);
		QObject::connect(&_appointment_call, &Net::AppointmentCall::failed, this, &Appointment::onEnrollFailure);

		QObject::connect(&_access_call, &Net::AccessCall::restored, this, &Appointment::onEnrollSuccess);
		QObject::connect(&_access_call, &Net::AccessCall::reassigned, this, &Appointment::onEnrollSuccess);
		QObject::connect(&_access_call, &Net::AccessCall::recovered, this, &Appointment::onAccessRecovery);
		QObject::connect(&_access_call, &Net::AccessCall::prepared, this, &Appointment::onAccessPrepared);
		QObject::connect(&_access_call, &Net::AccessCall::ratified, this, &Appointment::onAccessRatified);
		QObject::connect(&_access_call, &Net::AccessCall::failed, this, &Appointment::onEnrollFailure);
		QObject::connect(&_access_call, &Net::AccessCall::resigned, this, &Appointment::onAccessResign);
	}

	bool Appointment::enroll(QString name)
	{
		if (!_access_call.hasAccount()) {
			return false;
		}
		_access_call.reassign(name);
		emit requested();

		return true;
	}

	bool Appointment::reroll(Cadabra::Queen::Repository::Meta::Session::Id session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle)
	{
		_appointment_call.request(session_id, session_handle);
		emit requested();

		return true;
	}

	bool Appointment::recognize(QString account)
	{
		_access_call.recognize(account.trimmed());
		emit requested();

		return true;
	}

	bool Appointment::recover(QString account)
	{
		_access_call.recover(account.trimmed());
		emit requested();

		return true;
	}

	bool Appointment::restore(QString code)
	{
		if (!_access_call.hasAccount()) {
			return false;
		}
		_access_call.restore(code);
		emit requested();

		return true;
	}

	bool Appointment::ratify(QString code)
	{
		if (!_access_call.hasAccount()) {
			return false;
		}
		_access_call.ratify(code);
		emit requested();

		return true;
	}

	void Appointment::resign(Cadabra::Queen::Repository::Model::Session* session)
	{
		_access_call.resign(session);
	}

	void Appointment::point(Cadabra::Queen::Core::Net::Endpoint* endpoint, QString url)
	{
		if (nullptr == endpoint) {
			return;
		} else if (!url.isEmpty()) {
			endpoint->setURL(QUrl(!url.contains("://") ? "ace://" + url : url));
		}
		_appointment_call.setEndpoint(endpoint);
		_access_call.setEndpoint(endpoint);
	}

	const Cadabra::Queen::Repository::Meta::Session::Id& Appointment::getSessionId() const
	{
		return _session_id;
	}

	const Cadabra::Queen::Repository::Meta::Session::Handle& Appointment::getSessionHandle() const
	{
		return _session_handle;
	}

	const Cadabra::Queen::Repository::Meta::Subject::AvatarScheme& Appointment::getSessionScheme() const
	{
		return _session_scheme;
	}

	const Cadabra::Queen::Repository::Meta::ObjectId& Appointment::getChannelId() const
	{
		return _channel_id;
	}

	const Cadabra::Queen::Repository::Meta::Object::Handle& Appointment::getChannelHandle() const
	{
		return _channel_handle;
	}

	const Cadabra::Queen::Repository::Meta::Object::ApplicationScheme& Appointment::getChannelScheme() const
	{
		return _channel_scheme;
	}

	void Appointment::onEnrollSuccess(Cadabra::Queen::Repository::Meta::Session::Id session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme session_scheme, Cadabra::Queen::Repository::Meta::ObjectId channel_id, Cadabra::Queen::Repository::Meta::Object::Handle channel_handle, Cadabra::Queen::Repository::Meta::Object::ApplicationScheme channel_scheme)
	{
		qDebug()
				<< "Session sceheme:" << session_scheme
				<< "session id:" << session_id
				<< "session handle:" << session_handle
				<< "channel scheme:" << channel_scheme
				<< "channel id:" << channel_id
				<< "channel handle:" << channel_handle
		;

		if (0 == session_id) {
			emit blocked();
			return ;
		}

		_session_scheme = session_scheme;
		_session_id = session_id;
		_session_handle = session_handle;

		_channel_scheme = channel_scheme;
		_channel_id = channel_id;
		_channel_handle = channel_handle;

		/// QFile channel_picture("qrc:///DrOblozhko/Asset/Image/ToolbarIcon1.png");
		/// getRealm()->getHierarchy()->upsertObject(channel_id, 0, channel_scheme, "Смарт Фит", true, "00000000", 0, 0, 0)

		emit issued(_session_id, _session_handle, _session_scheme, _channel_id, _channel_handle, _channel_scheme);
	}

	void Appointment::onEnrollRedirect(QString host, int port, bool secure)
	{
		qDebug() << "Redirecting...";

		_appointment_call.getEndpoint()->setHost(host);
		_appointment_call.getEndpoint()->setPort(port);
		_appointment_call.getEndpoint()->setSecure(secure);

		_appointment_call.request();
	}

	void Appointment::onEnrollFailure(QString error)
	{
		qCritical() << "Failure:" << error;
		emit rejected(error);
	}

	void Appointment::onAccessRecovery(bool success)
	{
		emit locked(success);
	}

	void Appointment::onAccessPrepared(bool success)
	{
		emit arranged(success);
	}

	void Appointment::onAccessRatified(bool success)
	{
		emit verified(success);
	}

	void Appointment::onAccessResign()
	{
		emit reset();
	}

	bool Appointment::isEnrolled() const
	{
		return true == _channel_handle;
	}
} } } }
