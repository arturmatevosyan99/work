#include "AuthorizationRiddle.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	AuthorizationRiddle::AuthorizationRiddle(const QString& text, const QString& descriptor)
	:
		CustomRiddle(text, descriptor),

		_welcome_quiz(nullptr)
	{

	}

	bool AuthorizationRiddle::signIn(const QString& account)
	{
		if (isBusy() || !_welcome_quiz) {
			return false;
		}

		const WelcomeQuiz::Phase phase(_welcome_quiz->getPhase());
		if (phase != WelcomeQuiz::Phase::SignIn) {
			qDebug() << "Wrong phase:" <<  static_cast<qint8>(phase);
			return false;
		}

		startBusyness();
		_welcome_quiz->getAvatar()->getSession()->signInOTP(account, "");
		return true;
	}

	bool AuthorizationRiddle::signUp(const QString& account)
	{
		if (isBusy() || !_welcome_quiz) {
			return false;
		}

		const WelcomeQuiz::Phase phase(_welcome_quiz->getPhase());
		if (phase != WelcomeQuiz::Phase::SignIn) {
			qDebug() << "Wrong phase:" <<  static_cast<qint8>(phase);
			return false;
		}

		startBusyness();
		_welcome_quiz->setPhase(WelcomeQuiz::Phase::SignUp);
		_welcome_quiz->getAvatar()->getSession()->signUpOTP(account, "", _welcome_quiz->getAvatar()->getAlias());
		return true;
	}

	bool AuthorizationRiddle::verify(const QString& account, const QString& code)
	{
		if (isBusy() || !_welcome_quiz) {
			return false;
		}

		const WelcomeQuiz::Phase phase(_welcome_quiz->getPhase());
		switch (phase) {
			case WelcomeQuiz::Phase::CodeIn:
				startBusyness(5);
				_welcome_quiz->getAvatar()->getSession()->signInOTP(account, code);
				break;

			case WelcomeQuiz::Phase::CodeUp:
				startBusyness(5);
				_welcome_quiz->getAvatar()->getSession()->signUpOTP(account, code, _welcome_quiz->getAvatar()->getAlias());
				break;

			default:
				qDebug() << "Wrong phase:" <<  static_cast<qint8>(phase);
				return false;
		}
		return true;
	}

	bool AuthorizationRiddle::activate()
	{
		if (!_welcome_quiz) {
			return false;
		} else if (_welcome_quiz->getPhase() == WelcomeQuiz::Phase::SignUp) {
			_welcome_quiz->setPhase(WelcomeQuiz::Phase::CodeUp);
			emit accounted(true);
		} else if (_welcome_quiz->getPhase() == WelcomeQuiz::Phase::SignIn) {
			_welcome_quiz->setPhase(WelcomeQuiz::Phase::CodeIn);
			emit accounted(true);
		} else {
			qWarning() << "Unexpected state:", _welcome_quiz->getPhase();
			return false;
		}
		return true;
	}

	WelcomeQuiz* AuthorizationRiddle::getWelcomeQuiz()
	{
		return _welcome_quiz;
	}

	void AuthorizationRiddle::setWelcomeQuiz(WelcomeQuiz* welcome_quiz)
	{
		if (_welcome_quiz != welcome_quiz) {
			if (_welcome_quiz) {
				_welcome_quiz->disconnect();
			}
			_welcome_quiz = welcome_quiz;
			if (_welcome_quiz && _welcome_quiz->getAvatar() && _welcome_quiz->getAvatar()->getSession()) {
				QObject::connect
				(
					_welcome_quiz->getAvatar()->getSession(),
					&Repository::Model::Session::faulted,
					this,
					&AuthorizationRiddle::onFaulted
				);
				QObject::connect
				(
					_welcome_quiz->getAvatar()->getSession(),
					&Repository::Model::Session::registered,
					this,
					&AuthorizationRiddle::onRegistered
				);
				QObject::connect
				(
					_welcome_quiz->getAvatar()->getSession(),
					&Repository::Model::Session::declined,
					this,
					&AuthorizationRiddle::onDeclined
				);
				QObject::connect
				(
					_welcome_quiz->getAvatar()->getSession(),
					&Repository::Model::Session::authorized,
					this,
					&AuthorizationRiddle::onAuthorized
				);
			}
			emit welcomeQuizChanged();
		}
	}

	void AuthorizationRiddle::onFaulted(Repository::Model::Session::AuthorizationPhase phase, Repository::Model::Session::AuthorizationReject reject, QString message)
	{
		if (
			Repository::Model::Session::AuthorizationPhase::Authentication == phase
			&&
			Repository::Model::Session::AuthorizationReject::AuthenticationError == reject
		) {
			emit accounted(false);
		} else if (!_welcome_quiz) {
			qWarning("Fault with rejection code %d in phase %d cannot be processed becasue WelcomeQuiz is missed", reject, phase);
		} else if (Repository::Model::Session::AuthorizationReject::NetworkError == reject) {
			emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Network error or service is unavailable"));
		} else switch (phase) {
			case Repository::Model::Session::AuthorizationPhase::Verification:
				emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Incorrect verification code"));
				break;

			case Repository::Model::Session::AuthorizationPhase::Authentication:
				_welcome_quiz->setPhase(WelcomeQuiz::Phase::SignIn);
				emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("SignIn error: ").append(message));
				break;

			case Repository::Model::Session::AuthorizationPhase::Registration:
				emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("SignUp error: ").append(message));
				_welcome_quiz->setPhase(WelcomeQuiz::Phase::SignIn);
				break;

			default:
				qWarning("Unexpected fault(%d, %d): %s", phase, reject, qUtf8Printable(message));
		}
		stopBusyness();
	}

	void AuthorizationRiddle::onRegistered(int id)
	{
		if (!_welcome_quiz) {
			qWarning("WelcomeQuiz is missed during registration processing");
		} else if (id == 0) {
			if (_welcome_quiz->getPhase() == WelcomeQuiz::Phase::CodeUp) {
				emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Verification code is incorrect, please try again"));
			} else if (
						_welcome_quiz->getPhase() == WelcomeQuiz::Phase::SignUp
						||
						_welcome_quiz->getPhase() == WelcomeQuiz::Phase::SignIn
				) {
				if (!activate()) {
					emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Unexpected registration state"));
				}
			} else {
				_welcome_quiz->setPhase(WelcomeQuiz::Phase::SignIn);
				emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Unexpected registration error"));
			}
		} else if (!activate()) {
			emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Unexpected registration state"));
		}
		stopBusyness();
	}

	void AuthorizationRiddle::onDeclined()
	{
		stopBusyness();
	}

	void AuthorizationRiddle::onAuthorized()
	{
		if (!_welcome_quiz) {
			qWarning("WelcomeQuiz is missed during authorization processing");
		} else if (
					_welcome_quiz->getPhase() == WelcomeQuiz::Phase::CodeIn
					||
					_welcome_quiz->getPhase() == WelcomeQuiz::Phase::CodeUp
			) {
			if (
				_welcome_quiz->getAvatar()->getSession()->getPoolId() == 0
				||
				_welcome_quiz->getAvatar()->getSession()->getPoolHandle().isZero()
				) {
				emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Verification code is incorrect, please try again"));
			} else if (_welcome_quiz->request(_welcome_quiz->getService())) {
				_welcome_quiz->setPhase(WelcomeQuiz::Phase::OnBoard);
				completeBusynessPhase(Authorization);
				return ;
			} else {
				_welcome_quiz->setPhase(WelcomeQuiz::Phase::SignIn);
				emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Unexpected welcome error"));
			}
		} else {
			emit _welcome_quiz->exceptionThrew(Core::ApplicationMessage::WarningMessage, tr("Unexpected authorization state"));
		}
		stopBusyness();
	}
} } } }
