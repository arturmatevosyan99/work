import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1

import Repository.Model 0.1
import DrOblozhko.Model 0.1
import Mentalgami.Model 0.1

import "qrc:/Conversation/View/Application" as ConversationView
import "qrc:/Mentalgami/View/Application" as MentalgamiView

/// @note Данная сеть пока обрабатывает только один случай - "чистую" регистрацию пациента

/// @todo Но существует еще пара других сценариев:
/// 1. восстановление доступа к ранее зарегистрированному аккаунту
/// 2. авторизация через пролонгацию уже существующей сессии

PetrI {
	id: petrI

	property Component tokenComponent: Component {
		Token {
			id: token

			property string account
			property string code

			property Avatar avatar: Avatar {
				session.skeletonName: "eureka"
				realm.synchronizationMode: Realm.Lazy
			}

			property ConversationView.ChannelFeed channelFeed: ConversationView.ChannelFeed {

			}

			property MentalgamiView.AuthorizationQuiz authorizationQuiz: MentalgamiView.AuthorizationQuiz {
				avatar: token.avatar
			}
		}
	}

	Component.onCompleted: {
		console.log(petrI + ".autoFire:", petrI.autoFire);
		if (!petrI.autoFire) {
			return ;
		}

		/// @note Данная строка будет выполняться только в том случае, если сеть является самостоятельной (не портовой)
		StressExtension.vary(value, getEnvironmentVariable("ACE_ENDPOINT", "http://127.0.0.1:8080"));

		for (var p = 0; p < getEnvironmentVariable("PATIENT_COUNT", 1); p++) {
			petrI.tokens.push(
								petrI.tokenComponent.createObject(
																	petrI,
																	{
																		"description": "Patient#" + p,
																		"positions": [petrI.preAssertion],
																		"account": "patient" + p + "@cadabra.local",
																		"code": 9999
																	}
								)
			);
		}

		console.log(petrI.tokens.length + " patients were tokenized");
	}

	preAssertion: Assertion {
		id: initialAssertion
		description: "UnifiedLogInCase preassertion"

		actions: [resumeSession]

		Assert {
			description: "Check before resuming session"

			targetAction: resumeSession

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.avatar);
				console.assert(!!currentToken.avatar.session);
				console.assert(!!currentToken.avatar.realm);
			}
		}
	}

	preActions: [
		Action {
			id: resumeSession
			description: "Resume session"

			assertions: [resumeSessionCheck]

			onActed: {
				currentToken.avatar.session.realize(currentToken.avatar.realm);
			}

			ObjectWaiter {
				target: currentToken ? currentToken.avatar.session : null

				function onRealized()
				{
					console.log("currentToken.avatar.session::onRealized");
				}
			}

			ObjectWaiter {
				target: currentToken ? currentToken.avatar.session : null

				function onFaulted()
				{
					console.error("currentToken.avatar.session::onFaulted");
				}
			}

			TimeWaiter {
				seconds: 10

				onDone: {
					console.error("Resume session timer is over");
				}
			}
		}
	]

	Assertion {
		id: resumeSessionCheck

		actions: [getWelcomeRiddleViewInformation]

		/// @ocd
		Assert {
			targetAction: resumeSession
			description: "Check after resuming session"

			onActionCompleted: {
				console.log("Resuming session is completed");
			}
		}

		Assert {
			targetAction: getWelcomeRiddleViewInformation
			description: "Check before getting welcomeRiddleView information"

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.SignIn) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}
			}
		}
	}

	Action {
		id: getWelcomeRiddleViewInformation
		description: "Get welcomeRiddleView information"

		assertions: [getWelcomeRiddleViewInformationCheck]

		onActed: {
			console.log("Question =", currentToken.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", currentToken.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getWelcomeRiddleViewInformationCheck

		actions: [getThroughWelcomeRiddleView]

		/// @ocd
		Assert {
			description: "Check after getting welcomeRiddleView information"

			targetAction: getWelcomeRiddleViewInformation

			onActionCompleted: {
				console.log("Getting welcomeRiddleView information is completed");
			}
		}

		Assert {
			description: "Check before getting through welcomeRiddleView"

			targetAction: getThroughWelcomeRiddleView

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.SignIn) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}

				if (currentToken.authorizationQuiz.controller.canGoToNextQuestion() == false) {
					console.error("There is no way to go further");
				};
			}
		}
	}

	Action {
		id: getThroughWelcomeRiddleView
		description: "Get through welcomeRiddleView"

		assertions: [getThroughWelcomeRiddleViewCheck]

		onActed: {
			currentToken.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				console.log("currentToken.authorizationQuiz.controller::onActiveChanged");
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through welcomeRiddleView timer is over");
			}
		}
	}

	Assertion {
		id: getThroughWelcomeRiddleViewCheck

		actions: [getAuthorizationRiddleViewInformation]

		Assert {
			description: "Check after getting through welcomeRiddleView"

			targetAction: getThroughWelcomeRiddleView

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве

				console.log("Check after getting through welcomeRiddleView is completed");
			}
		}

		Assert {
			description: "Check before getting authorizationRiddleView information"

			targetAction: getAuthorizationRiddleViewInformation

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.SignIn) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}
			}
		}
	}

	Action {
		id: getAuthorizationRiddleViewInformation
		description: "Get authorizationRiddleView information"

		assertions: [getAuthorizationRiddleViewInformationCheck]

		onActed: {
			console.log("Question =", currentToken.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", currentToken.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getAuthorizationRiddleViewInformationCheck

		actions: [answerAuthorizationRiddleView]

		/// @ocd
		Assert {
			targetAction: getAuthorizationRiddleViewInformation

			description: "Check after getting authorizationRiddleView information"

			onActionCompleted: {
				console.log("Getting authorizationRiddleView information is completed");
			}
		}

		Assert {
			description: "Check before answering authorizationRiddleView"

			targetAction: answerAuthorizationRiddleView

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);
				console.assert(!!currentToken.account);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.SignIn) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}

				if (currentToken.authorizationQuiz.controller.canGoToNextQuestion() == true) {
					console.error("Moving on without an answer to the next question is not allowed");
				};
			}
		}
	}

	Action {
		id: answerAuthorizationRiddleView
		description: "Answer authorizationRiddleView"

		assertions: [answerAuthorizationRiddleViewCheck]

		onActed: {
			currentToken.authorizationQuiz.controller.enterAnswer(currentToken.account);
		}
	}

	Assertion {
		id: answerAuthorizationRiddleViewCheck

		actions: [getThroughAuthorizationRiddleViewFirst]

		Assert {
			description: "Check after answering authorizationRiddleView"

			targetAction: answerAuthorizationRiddleView

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
			}
		}

		Assert {
			description: "Check before getting through authorizationRiddleView #1"

			targetAction: getThroughAuthorizationRiddleViewFirst

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.SignIn) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}

				if (currentToken.authorizationQuiz.controller.canGoToNextQuestion() == false) {
					console.error("There is no way to go further");
				};
			}
		}
	}

	Action {
		id: getThroughAuthorizationRiddleViewFirst
		description: "Get through authorizationRiddleView #1"

		assertions: [getThroughAuthorizationRiddleViewFirstCheck]

		onActed: {
			currentToken.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.authorizationQuiz : null

			function onAccounted()
			{
				console.log("currentToken.authorizationQuiz::onAccounted");
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through authorizationRiddleView #1 timer is over");
			}
		}
	}

	Assertion {
		id: getThroughAuthorizationRiddleViewFirstCheck

		actions: [getThroughAuthorizationRiddleViewSecond]

		Assert {
			description: "Check after getting through authorizationRiddleView #1"

			targetAction: getThroughAuthorizationRiddleViewFirst

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве
				console.log("Check after getting through authorizationRiddleView #1 is completed");
			}
		}

		Assert {
			description: "Check before getting through authorizationRiddleView #2"

			targetAction: getThroughAuthorizationRiddleViewSecond

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.SignIn) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}
			}
		}
	}

	Action {
		id: getThroughAuthorizationRiddleViewSecond
		description: "Get through authorizationRiddleView #2"

		assertions: [getThroughAuthorizationRiddleViewSecondCheck]

		onActed: {
			currentToken.authorizationQuiz.signUp();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.authorizationQuiz.controller : null

			function onActiveChanged() {
				console.log("currentToken.authorizationQuiz.controller::onActiveChanged", target.active);
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through authorizationRiddleView #2 timer is over");
			}
		}
	}

	Assertion {
		id: getThroughAuthorizationRiddleViewSecondCheck

		actions: [getCodeVerificationRiddleViewInformation]

		Assert {
			description: "Check after getting through authorizationRiddleView #2"

			targetAction: getThroughAuthorizationRiddleViewSecond

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве

				console.log("Check after getting through authorizationRiddleView #2 is completed");
			}
		}

		Assert {
			description: "Check before getting codeVerificationRiddleView information"

			targetAction: getCodeVerificationRiddleViewInformation

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.CodeUp) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}
			}
		}
	}

	Action {
		id: getCodeVerificationRiddleViewInformation
		description: "Get codeVerificationRiddleView information"

		assertions: [getCodeVerificationRiddleViewInformationCheck]

		onActed: {
			console.log("Question =", currentToken.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", currentToken.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getCodeVerificationRiddleViewInformationCheck

		actions: [answerCodeVerificationRiddleView]

		/// @ocd
		Assert {
			targetAction: getCodeVerificationRiddleViewInformation
			description: "Check after getting codeVerificationRiddleView information"

			onActionCompleted: {
				console.log("Getting codeVerificationRiddleView information is completed");
			}
		}

		Assert {
			description: "Check before answering codeVerificationRiddleView"

			targetAction: answerCodeVerificationRiddleView

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);
				console.assert(!!currentToken.code);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.CodeUp) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}

				if (currentToken.authorizationQuiz.controller.canGoToNextQuestion() == true) {
					console.error("Moving on without an answer to the next question is not allowed");
				};
			}
		}
	}

	Action {
		id: answerCodeVerificationRiddleView
		description: "Answer codeVerificationRiddleView"

		assertions: [answerCodeVerificationRiddleViewCheck]

		onActed: {
			currentToken.authorizationQuiz.controller.enterAnswer(currentToken.code);
		}
	}

	Assertion {
		id: answerCodeVerificationRiddleViewCheck

		actions: [getThroughCodeVerificationRiddleView]

		Assert {
			description: "Check after answering codeVerificationRiddleView"

			targetAction: answerCodeVerificationRiddleView

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
			}
		}

		Assert {
			targetAction: getThroughCodeVerificationRiddleView
			description: "Check before getting through codeVerificationRiddleView"

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.authorizationQuiz);
				console.assert(!!currentToken.authorizationQuiz.controller);

				console.assert(!!currentToken.authorizationQuiz.controller.active);
				console.assert(!!currentToken.authorizationQuiz.phase);

				if (currentToken.authorizationQuiz.phase !== MentalgamiView.AuthorizationQuiz.Phase.CodeUp) {
					console.error("Wrong phase at this stage of authorization:", currentToken.authorizationQuiz.phase);
				}

				if (currentToken.authorizationQuiz.controller.canGoToNextQuestion() == false) {
					console.error("There is no way to go further");
				};
			}
		}
	}

	Action {
		id: getThroughCodeVerificationRiddleView
		description: "Get through codeVerificationRiddleView"

		assertions: [getThroughCodeVerificationRiddleViewCheck]

		onActed: {
			currentToken.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				console.log("currentToken.authorizationQuiz.controller::onActiveChanged", target.active);
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through codeVerificationRiddleView timer is over");
			}
		}
	}

	Assertion {
		id: getThroughCodeVerificationRiddleViewCheck

		actions: []

		Assert {
			description: "Check after getting through codeVerificationRiddleView"

			targetAction: getThroughCodeVerificationRiddleView

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве

				console.log("Check after getting through codeVerificationRiddleView is completed");
			}
		}
	}
}
