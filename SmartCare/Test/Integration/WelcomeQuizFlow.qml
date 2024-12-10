import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.Meta 0.1
import Core.System 0.1

import Repository.Meta 0.1

import Repository.Model 0.1
import SmartCare.Model 0.1

import "qrc:/Conversation/View/Application" as ConversationApplication
import "qrc:/SmartCare/View/Application" as SmartCareApplication

PetrI {
	id: petrI

	Component.onCompleted: {
		if (petrI.autoFire) {
			console.error("This net is intended to use just in a port");
		}
	}

	preAssertion: Assertion {
		id: initialAssertion
		description: "WelcomeQuizFlow preassertion"

		actions: [getNameRiddleInformationAction]

		Assert {
			description: "Assertion authorization stage and all essentials before start"

			targetAction: getNameRiddleInformationAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.log("Current authorization phase:", petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.model.phase);
				}
			}
		}
	}

	preActions: [
		Action {
			id: getNameRiddleInformationAction
			description: "Log a question from nameRiddle"

			assertions: [getNameRiddleInformationAssertion]

			onActed: {
				console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
				console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
			}
		}
	]

	Assertion {
		id: getNameRiddleInformationAssertion

		actions: [answerNameRiddleAction]

		/// @ocd
		Assert {
			description: "Has the question been logged?"

			targetAction: getNameRiddleInformationAction

			onActionCompleted: {
				console.log("Getting nameRiddle information is completed");
			}
		}

		Assert {
			description: "Can we answer nameRiddle and is the answer required to proceed further?"

			targetAction: answerNameRiddleAction

			onActionPrepared: {

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				} else if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("Moving on without an answer to the next question is not allowed");
				}
			}
		}
	}

	Action {
		id: answerNameRiddleAction
		description: "Answer nameRiddle"

		assertions: [answerNameRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.enterAnswer("Name");
		}
	}

	Assertion {
		id: answerNameRiddleAssertion

		actions: [getThroughNameRiddleAction]

		/// @ocd
		Assert {
			description: "Has the answer been accepted by nameRiddle?"

			targetAction: answerNameRiddleAction

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
				console.log("Submiting an answer for nameRiddle is completed");
			}
		}

		Assert {
			description: "Are we allowed to go to the next question and activate the next Riddle?"

			targetAction: getThroughNameRiddleAction

			onActionPrepared: {
				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				} else if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: getThroughNameRiddleAction
		description: "Go to the next Riddle"

		assertions: [getThroughNameRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				console.log("petrI.context.patient.authorizationQuiz.controller::onActiveChanged", target.active);
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through nameRiddle timer is over");
			}
		}
	}

	Assertion {
		id: getThroughNameRiddleAssertion

		actions: [getGenderRiddleInformationAction]

		/// @ocd
		Assert {
			description: "Has the Riddle been switched?"

			targetAction: getThroughNameRiddleAction

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве
				console.log("Getting through nameRiddle is completed");
			}
		}

		Assert {
			description: "Are we ready to get genderRiddle information?"

			targetAction: getGenderRiddleInformationAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("Moving on without an answer to the next question is not allowed");
				}
			}
		}
	}

	Action {
		id: getGenderRiddleInformationAction
		description: "Log a question from genderRiddle"

		assertions: [getGenderRiddleInformationAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getGenderRiddleInformationAssertion

		actions: [answerGenderRiddleAction]

		/// @ocd
		Assert {
			description: "Has the question been logged?"

			targetAction: getGenderRiddleInformationAction

			onActionCompleted: {
				console.log("Getting genderRiddle information is completed");
			}
		}

		Assert {
			description: "Can we answer genderRiddle and is the answer required to proceed further?"

			targetAction: answerGenderRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("Moving on without an answer to the next question is not allowed");
				}
			}
		}
	}

	Action {
		id: answerGenderRiddleAction
		description: "Answer genderRiddle"

		assertions: [answerGenderRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.enterAnswer("F");
		}
	}

	Assertion {
		id: answerGenderRiddleAssertion

		actions: [getThroughGenderRiddleAction]

		/// @ocd
		Assert {
			description: "Has the answer been accepted by genderRiddle?"

			targetAction: answerGenderRiddleAction

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
				console.log("Submiting an answer for genderRiddle is completed");
			}
		}

		Assert {
			description: "Are we allowed to go to the next question and activate the next Riddle?"

			targetAction: getThroughGenderRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: getThroughGenderRiddleAction
		description: "Go to the next Riddle"

		assertions: [getThroughGenderRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				console.log("petrI.context.patient.authorizationQuiz.controller::onActiveChanged", target.active);
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through genderRiddle timer is over");
			}
		}
	}

	 Assertion {
		id: getThroughGenderRiddleAssertion

		actions: [getDateRiddleInformationAction]

		/// @ocd
		Assert {
			description: "Has the Riddle been switched?"

			targetAction: getThroughGenderRiddleAction

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве
				console.log("Getting through diseasesRiddle is completed");
			}
		}

		Assert {
			description: "Are we ready to get dateRiddle information?"

			targetAction: getDateRiddleInformationAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: getDateRiddleInformationAction
		description: "Log a question from dateRiddle"

		assertions: [getDateRiddleInformationAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getDateRiddleInformationAssertion

		actions: [answerDateRiddleAction]

		/// @ocd
		Assert {
			description: "Has the question been logged?"

			targetAction: getDateRiddleInformationAction

			onActionCompleted: {
				console.log("Getting dateRiddle information is completed");
			}
		}

		Assert {
			description: "Can we answer dateRiddle and is the answer required to proceed further?"

			targetAction: answerDateRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion() == false) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: answerDateRiddleAction
		description: "Answer dateRiddle"

		assertions: [answerDateRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.enterAnswer(new Date("2000-01-20"));
		}
	}

	 Assertion {
		id: answerDateRiddleAssertion

		actions: [getThroughDateRiddleAction]

//		/// @ocd
		Assert {
			description: "Has the answer been accepted by dateRiddle"

			targetAction: answerDateRiddleAction

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
				console.log("Submiting an answer for dateRiddle is completed");
			}
		}

		Assert {
			description: "Are we allowed to go to the next question and activate the next Riddle?"

			targetAction: getThroughDateRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: getThroughDateRiddleAction
		description: "Go to the next Riddle"

		assertions: [getThroughDateRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				console.log("petrI.context.patient.authorizationQuiz.controller::onActiveChanged", target.active);
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through dateRiddle timer is over");
			}
		}
	}

	Assertion {
		id: getThroughDateRiddleAssertion

		actions: [getWeightRiddleInformationAction]

//		/// @ocd
		Assert {
			description: "Has the Riddle been switched?"

			targetAction: getThroughDateRiddleAction

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве

				console.log("Getting through dateRiddle is completed");
			}
		}

		Assert {
			description: "Are we ready to get weightRiddle information?"

			targetAction: getWeightRiddleInformationAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		 }
	}

	Action {
		id: getWeightRiddleInformationAction
		description: "Log a question from the Riddle"

		assertions: [getWeightRiddleInformationAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getWeightRiddleInformationAssertion

		actions: [answerWeightRiddleAction]

//		/// @ocd
		Assert {
			description: "Has the question been logged?"

			targetAction: getWeightRiddleInformationAction

			onActionCompleted: {
				console.log("Getting weightRiddle information is completed");
			}
		}

		Assert {
			description: "Can we answer weightRiddle and is the answer required to proceed further?"

			targetAction: answerWeightRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: answerWeightRiddleAction
		description: "Answer weightRiddle"

		assertions: [answerWeightRiddleAssertion]

		onActed: {
			var patientWeight = 80;
			petrI.context.patient.authorizationQuiz.controller.enterAnswer(patientWeight);
		}
	}

	Assertion {
		id: answerWeightRiddleAssertion

		actions: [getThroughWeightRiddleAction]

//		/// @ocd
		Assert {
			description: "Has the answer been accepted by the weightRiddle?"

			targetAction: answerWeightRiddleAction

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
				console.log("Submiting an answer for weightRiddle is completed");
			}
		}

		Assert {
			description: "Are we allowed to go to the next question and activate the next Riddle?"

			targetAction: getThroughWeightRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: getThroughWeightRiddleAction
		description: "Go to the next Riddle"

		assertions: [getThroughWeightRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				console.log("petrI.context.patient.authorizationQuiz.controller::onActiveChanged", target.active);
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through weightRiddle timer is over");
			}
		}
	}

	Assertion {
		id: getThroughWeightRiddleAssertion

		actions: [getHeightRiddleInformationAction]

//		/// @ocd
		Assert {
			description: "Has the Riddle been switched?"

			targetAction: getThroughWeightRiddleAction

			onActionCompleted: {
//				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
//				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве

				console.log("Getting through weightRiddle is completed");
			}
		}

		Assert {
			description: "Are we ready to get heightRiddle information?"

			targetAction: getHeightRiddleInformationAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		 }
	}

	Action {
		id: getHeightRiddleInformationAction
		description: "Log a question from heightRiddle"

		assertions: [getHeightRiddleInformationAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getHeightRiddleInformationAssertion

		actions: [answerHeightRiddleAction]

		/// @ocd
		Assert {
			description: "Has the question been logged?"

			targetAction: getHeightRiddleInformationAction

			onActionCompleted: {
				console.log("Getting heightRiddle information is completed");
			}
		}

		Assert {
			description: "Can we answer heightRiddle and is the answer required to proceed further?"

			targetAction: answerHeightRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: answerHeightRiddleAction
		description: "Answer heightRiddle"

		assertions: [answerHeightRiddleAssertion]

		onActed: {
			var patientHeight = 165;
			petrI.context.patient.authorizationQuiz.controller.enterAnswer(patientHeight);
		}
	}

	Assertion {
		id: answerHeightRiddleAssertion

		actions: [getThroughHeightRiddleAction]

		/// @ocd
		Assert {
			description: "Has the answer been accepted by heightRiddle"

			targetAction: answerHeightRiddleAction

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
				console.log("Submiting an answer for heightRiddle is completed");
			}
		}

		Assert {
			description: "Are we allowed to go to the next question and activate the next Riddle?"

			targetAction: getThroughHeightRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: getThroughHeightRiddleAction
		description: "Go to the next Riddle"

		assertions: [getThroughHeightRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

			function onActiveChanged()
			{
				console.log("petrI.context.patient.authorizationQuiz.controller::onActiveChanged", target.active);
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Get through heightRiddle timer is over");
			}
		}
	}

	Assertion {
		id: getThroughHeightRiddleAssertion

		actions: [getDiseasesRiddleInformationAction]

		/// @ocd
		Assert {
			description: "Has the Riddle been switched?"

			targetAction: getThroughHeightRiddleAction

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве

				console.log("Getting through heightRiddle is completed");
			}
		}

		Assert {
			description: "Are we ready to get diseasesRiddle information?"

			targetAction: getDiseasesRiddleInformationAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		 }
	}

	Action {
		id: getDiseasesRiddleInformationAction
		description: "Log a question from diseasesRiddle"

		assertions: [getDiseasesRiddleInformationAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getDiseasesRiddleInformationAssertion

		actions: [answerDiseasesRiddleAction]

		/// @ocd
		Assert {
			description: "Has the question been logged?"

			targetAction: getDiseasesRiddleInformationAction

			onActionCompleted: {
				console.log("Getting diseasesRiddle information is completed");
			}
		}

		Assert {
			description: "Can we answer diseasesRiddle and is the answer required to proceed further?"

			targetAction: answerDiseasesRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

	Action {
		id: answerDiseasesRiddleAction
		description: "Answer diseasesRiddle"

		assertions: [answerDiseasesRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.enterAnswer(["F99", "E10", "N80"]);
		}
	}

	Assertion {
		id: answerDiseasesRiddleAssertion

		actions: [getThroughDiseasesRiddleAction]

		/// @ocd
		Assert {
			description: "Has the answer been accepted by diseasesRiddle"

			targetAction: answerDiseasesRiddleAction

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
				console.log("Submiting an answer for diseasesRiddle is completed");
			}
		}

		Assert {
			description: "Are we allowed to go to the next question and activate the next Riddle?"

			targetAction: getThroughDiseasesRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}
			}
		}
	}

		Action {
			id: getThroughDiseasesRiddleAction
			description: "Go to the next Riddle"

			assertions: [getThroughDiseasesRiddleAssertion]

			onActed: {
				petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
			}

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.authorizationQuiz.controller : null

				function onActiveChanged()
				{
					console.log("petrI.context.patient.authorizationQuiz.controller::onActiveChanged", target.active);
				}
			}

			TimeWaiter {
				seconds: 10

				onDone: {
					console.error("Get through diseasesRiddle timer is over");
				}
			}
		 }

	 Assertion {
		id: getThroughDiseasesRiddleAssertion

		actions: [getReadyRiddleInformationAction]

		/// @ocd
		Assert {
			description: "Has the Riddle been switched?"

			targetAction: getThroughDiseasesRiddleAction

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве
				console.log("Getting through diseasesRiddle is completed");
			}
		}

		Assert {
			description: "Are we ready to get readyRiddle information?"

			targetAction: getReadyRiddleInformationAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("Moving on without an answer to the next question is not allowed");
				}
			}
		}
	}

	Action {
		id: getReadyRiddleInformationAction
		description: "Log a question from readyRiddle"

		assertions: [getReadyRiddleInformationAssertion]

		onActed: {
			console.log("Question =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleQuestion());
			console.log("Description =", petrI.context.patient.authorizationQuiz.controller.getCurrentRiddleDescription());
		}
	}

	Assertion {
		id: getReadyRiddleInformationAssertion

		actions: [answerReadyRiddleAction]

		/// @ocd
		Assert {
			description: "Has the question been logged?"

			targetAction: getReadyRiddleInformationAction

			onActionCompleted: {
				console.log("Getting readyRiddle information is completed");
			}
		}

		Assert {
			description: "Can we answer readyRiddle and is the answer required to proceed further?"

			targetAction: answerReadyRiddleAction

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("Moving on without an answer to the next question is not allowed");
				};
			}
		}
	}

	Action {
		id: answerReadyRiddleAction
		description: "Answer readyRiddle"

		assertions: [answerReadyRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.enterAnswer(true);
		}
	}

	Assertion {
		id: answerReadyRiddleAssertion
		actions: [getThroughReadyRiddleAction]


		/// @ocd
		Assert {
			description: "Has the answer been accepted by readyRiddle"

			targetAction: answerReadyRiddleAction

			onActionCompleted: {
				/// @note Как проверить, что ответ реально сохранился внутри Риддла?
				console.log("Submiting an answer for readyRiddle is completed");
			}
		}

		Assert {
			description: "Are we allowed to go to the next question and activate the next Riddle?"

			targetAction: getThroughReadyRiddleAction

			onActionPrepared: {

				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase at this stage of authorization:", petrI.context.patient.authorizationQuiz.phase);
				}

				if (!petrI.context.patient.authorizationQuiz.controller.canGoToNextQuestion()) {
					console.error("There is no way to go further");
				}

				if (!NotificationCenter.hasPermission) {
					/// @note Если разрешение не было запрошено ранее, необходимо ожидать запроса на следующем Риддле
				}
			}
		}
	}

	Action {
		id: getThroughReadyRiddleAction
		description: "Go to the next Riddle"

		assertions: [getThroughReadyRiddleAssertion]

		onActed: {
			petrI.context.patient.authorizationQuiz.controller.goToNextQuestion();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.authorizationQuiz : null

//			function onActiveChanged()
			function onFinished()
			{
				console.log("caught onCompleted");
			}
		}

		TimeWaiter {
			seconds: 20

			onDone: {
				console.error("Get through readyRiddle timer is over");
			}
		}
	}

	Assertion {
		id: getThroughReadyRiddleAssertion

		actions: []

		/// @ocd
		Assert {
			description: "Has the Riddle been switched?"

			targetAction: getThroughReadyRiddleAction

			onActionCompleted: {
				/// @note Хотелось бы как-нибудь проверить, что мы действительно перешли к следующему вопросу
				/// В идеале, наверно, нужна возможность получать индекс текущего вопроса для подобной ориентации в пространстве

				console.log("Getting through readyRiddle is completed");
			}
		}
	}
/*
	Action {
		id: finalRiddleView
		description: " in finalRiddleView we Assertion for onIndexed "

		assertions: [finalRiddleViewIndexedAssertion]

		onActed: {
			currentToken.avatar.pickUpProfile();
			console.log("pickupProfile attempted...");
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.home : null

			function onIndexed()
			{
				console.log("Home Directory indexed");

				for (var i = 0; i < currentToken.avatar.home.indexModel.rowCount(); i++) {
					var index = currentToken.avatar.home.indexModel.index(i, 0);

					if (currentToken.avatar.home.indexModel.data(index, FolderIndex.ApplicationScheme) === "-8663136378210484223") {
						console.log("Appointment Channel found");
	
						var render = SchemeFactory.load("Repository.Meta.ApplicationRender", currentToken.avatar.home.indexModel.data(index, FolderIndex.ApplicationScheme));
						var component = Qt.createComponent(render.makeURL());

						if (component.status === Component.Ready) {
							currentToken.channelFeed = component.createObject(currentToken);
							console.log("Appointment Channel created");
							currentToken.channelFeedId = currentToken.avatar.home.indexModel.data(index, FolderIndex.Id);
							console.log("FeedId saved");
						} else {
							console.error("Appointment Channel creation failed");
						}

						break;
					}
				}
			}
		}
	
		TimeWaiter {
			seconds: 40

			onDone: {
				console.error("directory wasn't indexed");
			}
		}
	}

	Assertion {
		id: finalRiddleViewIndexedAssertion

		actions: [attachChannelFeed]

		/// @ocd
		Assert {
			description: "Assertion after getting index"

			targetAction: finalRiddleView

			onActionCompleted: {
				console.log("Getting index is completed");
			}
		}
	}

	Action {
		id: attachChannelFeed
		description: "attachChannelFeed"

		assertions: [attachChannelAssertion]

		onActed: {
			console.log("currentToken.channelFeed.application = ", currentToken.channelFeed.application);
			currentToken.avatar.session.attach(currentToken.channelFeed.application, currentToken.channelFeedId, true, {});
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			WaiterCallback {
				name: "attached"
				callee: function() {
					console.log("onAttached(application, fact)");
					if (
						this.fact == Session.AttachFact.NonApplicable
						||
						this.fact == Session.AttachFact.HandleError
						||
						this.fact == Session.AttachFact.OpenError
						||
						this.fact == Session.AttachFact.CoherenceError
					) {
						console.error("Session cannot be attached");
					}
				}
			}
		}
			
		TimeWaiter {
			seconds: 20

			onDone: {
				console.error("Get through readyRiddle timer is over");
			}
		}
	 }

	Assertion {
		id: attachChannelAssertion

		actions: []
		/// @ocd
		Assert {
			description: "Assertion after onAttached"

			targetAction: attachChannelFeed

			onActionCompleted: {
				console.log("session attached sucessfully!");
				if (currentToken.channelFeed.application.isOpened !== true) {
					console.error("channel not opened");
				} else {
					console.log("channel attached and opened");
				}
			}
		}
	}

	Assert {
		targetAction: basicPush

		onActionPrepared: {
			console.assert(!currentToken.groupChannel.hasUnloadedMessages);
		}
	}
*/
}
/*
	/// @note Все, что ниже, нужно порефакторить
	Action {
		id: getHomeId

		description: "pickUp profile failed or completed?"
		assertions: [getHomeIdAssertion]

		onActed: {
			console.log("GetHomeID!");
			currentToken.avatar.session.pickUpProfile();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			function onProfilePickUpCompleted(id: int)
			{
				console.log("onProfilePickUpCompleted", id);
				currentToken.directoryId = id;
			}

			function onProfilePickUpFailed()
			{
				console.log("onProfilePickUpFailed");
				console.error("Something went wrong");
			}
		}

		TimeWaiter {
			seconds: 60

			onDone: {
				console.error("pickUp timer is over");
			}
		}
	}

	Assertion {
		id: getHomeIdAssertion

		actions: [homeSessionAttach]

		Assert {
			description: "Assertion we got that ID"

			targetAction: getHomeId

			onActionCompleted: {
				console.log("we got that id");
				console.assert(!!currentToken.avatar.session);
				console.assert(!!currentToken.avatar.home);
				console.assert(!!currentToken.directoryId);
				console.log("Directory ID", currentToken.directoryId);
			}
		}
	}

	Action {
		id: homeSessionAttach
		description: "session attach home dir"

		assertions: [homeSessionAttachAssertion]

		onActed: {
			console.log("Directory ID", currentToken.directoryId);
			console.log("Directory", currentToken.avatar.home);
			currentToken.avatar.session.attach(currentToken.avatar.home, currentToken.directoryId, true);
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			function onAttached() {
				console.log("session attached");
			}

		}

		TimeWaiter {
			seconds: 60

			onDone: {
				console.error("session attach timer is over");
			}
		}

	}

	Assertion {
		id: homeSessionAttachAssertion

		actions: [dirIndex]

		Assert {
			description: "was the session attached?"

			targetAction: homeSessionAttach

			onActionCompleted: {
				console.assert(!!currentToken.avatar.session);
				console.assert(!!currentToken.avatar.home);
				console.assert(!!currentToken.directoryId);
				console.log("session attached sucessfully");
			}
		}
	}

	Action {
		id: dirIndex
		description: "lets index home directory"

		assertions: [dirIndexAssertion]

		onActed: {
			currentToken.avatar.home.index();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.home : null

			function onIndexed() {
				console.log("home directory indexed");
				for (var i = 0; i < avatar.home.indexModel.rowCount(); i++) {
					var index = avatar.home.indexModel.index(i, 0);
					console.log(avatar.home.indexModel.data(index, FolderIndex.Name));
				}
			}

		}

		TimeWaiter {
			seconds: 60

			onDone: {
				console.error("could not index");
			}
		}

	}

	Assertion {
		id: dirIndexAssertion

		actions: []

		Assert {
			description: "was the session attached?"

			targetAction: homeSessionAttach

			onActionCompleted: {
				console.assert(!!currentToken.avatar.session);
				console.assert(!!currentToken.avatar.home);
				console.assert(!!currentToken.directoryId);
				console.assert(!!currentToken.avatar.home.indexModel);
				console.log("session attached sucessfully");
			}
		}
	}
*/
