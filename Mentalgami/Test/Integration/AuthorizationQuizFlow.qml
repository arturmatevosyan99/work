import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.System 0.1

import Repository.Model 0.1
import Mentalgami.Model 0.1

import Stress.Global 0.1
import Stress.System 0.1

import "qrc:/Conversation/View/Application" as ConversationApplication
import "qrc:/Mentalgami/View/Application" as MentalgamiApplication

/**
 * @brief Регистрация пациентов
 * @todo Восстанавливать доступ к уже существующим аккаунтам
 * @todo восстановление доступа к ранее зарегистрированному аккаунту
 * @todo авторизация через пролонгацию уже существующей сессии
 */
PetrI {
	id: petrI
/*
	property var context: QtObject {

		property Avatar patient: Avatar {
			Component.onCompleted: {
				pointEnd(getEnvironmentVariable("ACE_ENDPOINT", "http://127.0.0.1:8080"));
			}
			readonly property string suffix: ( new Date().toLocaleString(Qt.locale(), "hh.mm.ss_dd.MM.yyyy") )
			readonly property string account: "patient_CAP-50" + "_" + suffix + "_" + "@cadabra.local"
			property string code: "999"
			property int channelFeedId
			session.skeletonName: "eureka"
			realm.objectName: "ladybug" + "_" + suffix
			realm.synchronizationMode: Realm.Lazy

			property ConversationApplication.ChannelFeed channelFeed: ConversationApplication.ChannelFeed {

			}

			property MentalgamiApplication.AuthorizationQuiz authorizationQuiz: MentalgamiApplication.AuthorizationQuiz{
				avatar: patient
			}
		}

		property Avatar supervisor: Avatar {
			Component.onCompleted: {
				pointEnd(getEnvironmentVariable("ACE_ENDPOINT", "http://127.0.0.1:8080"));
			}
		}
	}

	property Component tokenComponent: Component {
		Token {
			//property alias context: petrI.context
		}
	}

	Component.onCompleted: {
		console.log(petrI + ".autoFire:", petrI.autoFire);
		if (!petrI.autoFire) {
			return ;
		}
		petrI.tokens.push
		(
			petrI.tokenComponent.createObject
			(
				petrI,
				{
					"description": "",
					"positions": [petrI.preAssertion]
				}
			)
		);
	}
*/

	preAssertion: Assertion {
		id: initialAssertion

		actions: [realizeSessionAction]

		Assert {
			description: "Check authorization stage and all essentials before start"

			targetAction: realizeSessionAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient);
				console.assert(!!petrI.context.patient.realm);
				console.assert(!!petrI.context.patient.session);
				console.log("Current authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
			}
		}
	}

	preActions: [
		Action {
			id: realizeSessionAction
			description: "Link Session with local Realm.Lite"

			assertions: [realizeSessionAssertion]

			onActed: {
				petrI.context.patient.session.realize(petrI.context.patient.realm);
				console.log("Current authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
			}

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.session : null

				function onRealized()
				{
					console.log("Session was realized!");
				}
			}

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.session : null

				function onFaulted()
				{
					console.error("Session unexpected fault");
				}
			}

			TimeWaiter {
				seconds: 10

				onDone: {
					console.error("Couldn't wait for the Session to be linked with Realm.Lite(realized)");
				}
			}
		}
	]

	Assertion {
		id: realizeSessionAssertion

		actions: [welcomeRiddleLogAction]

		Assert {
			description: "Has the Realm.Lite been linked with the Session?"

			targetAction: realizeSessionAction

			onActionCompleted: {
				console.assert(petrI.context.patient.session.isRealized);
			}
		}

		Assert {
			description: "Is Quiz ready to start authorization process?"

			targetAction: welcomeRiddleLogAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.SignIn) {
					console.error("Unexpected authorization stage:", petrI.context.patient.authorizationQuiz.model.phase);
				}
			}
		}
	}

	Action {
		id: welcomeRiddleLogAction
		description: "Log first and welcome question from the Riddle"

		assertions: [welcomeRiddleQuestionAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: welcomeRiddleQuestionAssertion

		actions: [welcomeRiddleGoToAction]

		Assert {
			description: "Has the question been logged?"

			targetAction: welcomeRiddleLogAction

			onActionCompleted: {
				console.assert(petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion().length > 0);
			}
		}

		Assert {
			description: "Is asked question just a welcome message and we are able to skip one?"

			targetAction: welcomeRiddleGoToAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.SignIn) {
					console.error("Unexpected authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
				} else if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("No any available questions ahead");
				}
			}
		}
	}

	Action {
		id: welcomeRiddleGoToAction
		description: "Go to the next Riddle"

		assertions: [accountRiddleIndexAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				/// @todo console.log("Riddle changed from a to b")
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Could not change riddle for a long time");
			}
		}
	}

	Assertion {
		id: accountRiddleIndexAssertion

		actions: [accountRiddleLogAction]

		Assert {
			description: "Has the Riddle been switched?"

			targetAction: welcomeRiddleGoToAction

			onActionCompleted: {
				/// @todo Проверить, что мы действительно перешли к следующему вопросу (например, через индекс текущего Riddle)
				console.assert(true);
			}
		}

		Assert {
			description: "Are we in the appropriate phase?"

			targetAction: accountRiddleLogAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.SignIn) {
					console.error("Unexpected authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
				}
			}
		}
	}

	Action {
		id: accountRiddleLogAction
		description: "Log a question from the Riddle"

		assertions: [accountRiddleQuestionAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: accountRiddleQuestionAssertion

		actions: [accountRiddleAnswerAction]

		Assert {
			description: "Has the question been logged?"

			targetAction: accountRiddleLogAction

			onActionCompleted: {
				console.assert(petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion().length > 0);
			}
		}

		Assert {
			description: "Is asked question needs an answer before continue?"

			targetAction: accountRiddleAnswerAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.SignIn) {
					console.error("Unexpected authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
				} else if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("Moving on to the next question without an answer is unexpected");
				}
			}
		}
	}

	Action {
		id: accountRiddleAnswerAction
		description: "Pass email address or phone number as an account"

		assertions: [accountRiddleAnswerAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.enterAnswer(petrI.context.patient.account);
		}
	}

	Assertion {
		id: accountRiddleAnswerAssertion
		actions: [accountRiddleGoToAction]

		Assert {
			description: "Has the answer been accepted by the Riddle?"

			targetAction: accountRiddleAnswerAction

			onActionCompleted: {
				/// @todo Проверить, что ответ реально сохранился внутри Риддла
				console.assert(true);
			}
		}

		Assert {
			description: "Are we allowed go to the next question and activate next Riddle?"

			targetAction: accountRiddleGoToAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.SignIn) {
					console.error("Unexpected authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
				} else if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("No any available questions ahead");
				}
			}
		}
	}

	Action {
		id: accountRiddleGoToAction
		description: "Go to the next the Riddle"

		assertions: [codeRiddleIndexAssertion]

		onActed: {
			/// @note ... приведет к SignIn или Alert с опцией зарегистрировать несуществующий аккаунт
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		GroupWaiter {
			id: signInGroupWaiter

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.authorizationQuiz.controller.active.model : null

				function onAccounted(onoff: bool)
				{
					console.log("onAccounted:" + onoff);
					if (onoff) { /// < Аккаунт переведен в режим восстановления
						/// @note Тогда нет смысла ждать еще один сигнал об Alert
						signInGroupWaiter.reset(true);
					} else { /// < Аккаунт не существует и нужна регистрация, поэтому подождем Alert с опцией
						console.info("Need to wait for an Alert with a sign up option");
						/// @note Сбросим текущий статус GroupWaiter, чтобы дождаться onModalChanged и еще одного onAccounted
						signInGroupWaiter.reset(false);
					}
				}
			}

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.authorizationQuiz.accountMissedAlert : null

				function onModalChanged()
				{
					console.log("onModalChanged");
					console.assert(petrI.context.patient.authorizationQuiz.accountMissedAlert.isModal);
					petrI.context.patient.authorizationQuiz.accountMissedAlert.resolve(ActionAlert.Yes);
				}
			}

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

				function onActiveChanged()
				{
					console.log("onActiveChanged");
					console.info("Active riddle changed")
				}
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Sign in has not been completed for a long time");
			}
		}
	}

	Assertion {
		id: codeRiddleIndexAssertion

		actions: [codeRiddleLogAction]

		Assert {
			description: "Has the Riddle been switched?"

			targetAction: accountRiddleGoToAction

			onActionCompleted: {
				/// @todo Проверить, что мы действительно перешли к следующему вопросу (например, через индекс текущего Riddle)
				console.assert(true);
			}
		}

		Assert {
			description: "Are we in the appropriate phase?"

			targetAction: codeRiddleLogAction

			onActionPrepared: {
				if (![WelcomeQuiz.Phase.CodeUp, WelcomeQuiz.Phase.CodeIn].includes(petrI.context.patient.authorizationQuiz.model.phase)) {
					console.error("Unexpected authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
				}
			}
		}
	}

	Action {
		id: codeRiddleLogAction
		description: "Log a question from the Riddle"

		assertions: [codeRiddleQuestionAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: codeRiddleQuestionAssertion

		actions: [codeRiddleAnswerAction]

		Assert {
			description: "Has the question been logged?"

			targetAction: codeRiddleLogAction

			onActionCompleted: {
				console.assert(petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion().length > 0);
			}
		}

		Assert {
			description: "Is asked question needs an answer before continue?"

			targetAction: codeRiddleAnswerAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.CodeUp) {
					console.error("Unexpected authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
				} else if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("Moving on to the next question without an answer is unexpected");
				}
			}
		}
	}

	Action {
		id: codeRiddleAnswerAction
		description: "Answer the question about verification code"

		assertions: [codeRiddleAnswerAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.enterAnswer(petrI.context.patient.code);
		}
	}

	Assertion {
		id: codeRiddleAnswerAssertion

		actions: [codeRiddleGoToAction]

		Assert {
			description: "Has the answer been accepted by the Riddle?"

			targetAction: codeRiddleAnswerAction

			onActionCompleted: {
				/// @todo Проверить, что ответ реально сохранился внутри Риддла
				console.assert(true);
			}
		}

		Assert {
			description: "Are we allowed go to the next question and activate next Riddle?"

			targetAction: codeRiddleGoToAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.CodeUp) {
					console.error("Unexpected authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);
				} else if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("No any available questions ahead");
				}
			}
		}
	}

	Action {
		id: codeRiddleGoToAction
		description: "Go to the next Riddle"

		assertions: [lastRiddleIndexAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				/// @todo console.log("Riddle changed from a to b")
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Riddle have not been switched for a long time");
			}
		}
	}

	Assertion {
		id: lastRiddleIndexAssertion

		actions: []

		Assert {
			description: "Has the riddle been switched?"

			targetAction: codeRiddleGoToAction

			onActionCompleted: {
				/// @todo Проверить, что мы действительно перешли к следующему вопросу (например через индекс текущего вопроса для ориентации в пространстве)
				console.assert(true);
			}
		}
	}
}
