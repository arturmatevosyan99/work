#ifndef Cadabra_Queen_Mentalgami_Model_Question_AuthorizationRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_AuthorizationRiddle_hpp

#include "mg_CustomRiddle.hpp"

#include "../../Model/Application/Quiz/mg_WelcomeQuiz.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class AuthorizationRiddle : public CustomRiddle
	{
	/** @name Classes */
	/** @{ */
	public:
		enum BusynessPhase
		{
			Authorization,
			Invitation,
			QuizOpening,
			RiddleAsking,
			Completion
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(BusynessPhase)
		Q_PROPERTY(Cadabra::Queen::Mentalgami::Model::WelcomeQuiz* welcomeQuiz READ getWelcomeQuiz WRITE setWelcomeQuiz NOTIFY welcomeQuizChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AuthorizationRiddle(const QString& text = QString(), const QString& descriptor = QString());
		virtual ~AuthorizationRiddle() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		WelcomeQuiz* _welcome_quiz;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool signIn(const QString& account);
		Q_INVOKABLE bool signUp(const QString& account);
		Q_INVOKABLE bool verify(const QString& account, const QString& code);
		bool activate();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		WelcomeQuiz* getWelcomeQuiz();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setWelcomeQuiz(WelcomeQuiz* welcome_quiz);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void welcomeQuizChanged();
		void accounted(bool onoff);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onFaulted(Cadabra::Queen::Repository::Model::Session::AuthorizationPhase phase, Cadabra::Queen::Repository::Model::Session::AuthorizationReject reject, QString message);
		void onRegistered(int id);
		void onDeclined();
		void onAuthorized();
	/** @} */
	};
} } } }

#endif
