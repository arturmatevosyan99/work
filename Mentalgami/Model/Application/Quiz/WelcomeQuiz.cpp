#include "WelcomeQuiz.hpp"

#include "../../../../Repository/Model/Application/Session.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	WelcomeQuiz::WelcomeQuiz(QObject* parent)
	:
		GenericQuiz(parent),

		_phase(WelcomeQuiz::Phase::SignIn),
		_service("default")
	{
		QObject::connect(&_call, &Net::WelcomeCall::responded, this, &WelcomeQuiz::onResponded);
	}

	WelcomeQuiz::~WelcomeQuiz()
	{

	}

	bool WelcomeQuiz::request(const QString& alias)
	{
		if (!_avatar || !_avatar->getSession()->isRealized()) {
			return false;
		} else if (_avatar->getRealm()->getEndpoint() != _call.getEndpoint()) {
			/// @note Насильно подравняем эндопинты, чтобы это был гарантированно один и тот же rad
			_call.setEndpoint(_avatar->getRealm()->getEndpoint());
		}
		_call.request(alias);
		return true;
	}

	bool WelcomeQuiz::attach(const QVariant& ident)
	{
		if (!_avatar || !_avatar->getSession()->isRealized()) {
			return false;
		}
		const Repository::Meta::Object::Id object_id(ident.canConvert<Core::Meta::Qd>() ? ident.toLongLong() : Core::Meta::Qd::Make(ident).toInt64());
		return _call.getMap().contains(object_id) ? _avatar->getSession()->attach(this, object_id, true) : false;
	}

	QAbstractListModel* WelcomeQuiz::getListModel()
	{
		return &(_call.getModel());
	}

	Repository::Model::Avatar* WelcomeQuiz::getAvatar()
	{
		return _avatar;
	}

	WelcomeQuiz::Phase WelcomeQuiz::getPhase() const
	{
		return _phase;
	}

	const QString& WelcomeQuiz::getService() const
	{
		return _service;
	}

	void WelcomeQuiz::setAvatar(Repository::Model::Avatar* avatar)
	{
		if (_avatar != avatar) {
			_avatar = avatar;
			emit avatarChanged();
		}
	}

	void WelcomeQuiz::setPhase(WelcomeQuiz::Phase phase)
	{
		if (_phase != phase) {
			_phase = phase;
			emit phaseChanged();
		}
	}

	void WelcomeQuiz::setService(const QString& service)
	{
		if (_service != service) {
			_service = service;
			emit serviceChanged();
		}
	}

	void WelcomeQuiz::onResponded(const QString& message)
	{
		if (!message.isEmpty()) {
			qWarning() << "Error:" << message;
			emit rejected(message);
		} else if (_call.getMap().isEmpty()) {
			emit locked();
		} else {
			qDebug() << "Invite list:" << _call.getMap();
			emit invited();
		}
	}

	bool WelcomeQuiz::onHandle()
	{
		return GenericQuiz::onHandle();
	}

	bool  WelcomeQuiz::onOpen()
	{
		return GenericQuiz::onOpen();
	}
} } } }
