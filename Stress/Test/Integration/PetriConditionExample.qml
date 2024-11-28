import QtQml 2.15

import Core.Test 0.1

PetrI {
	id: testModel

	onCurrentTokenChanged: {
		console.log("onCurrentTokenChanged::currentToken =", currentToken);
		console.log("onCurrentTokenChanged::currentToken.badActor =", currentToken.badActor);
	}

	tokens: [
		Token {
			description: "Patient #0"
			positions: [initialAssertion]

			property bool badActor: false

			onDescriptionChanged: {
				console.log("0000000000000");
			}
		},

		Token {
			description: "Patient #1"
			positions: [initialAssertion]

			property bool badActor: true

			onDescriptionChanged: {
				console.log("1111111111111");
			}
		}
	]

	preAssertion: Assertion {
		id: initialAssertion
		description: "PreAssertion"
		actions: [printDebugInformation]

		Assert {
			targetAction: printDebugInformation
			description: "Check before printing debug information"

			onActionPrepared: {
				console.assert(!!currentToken);
			}
		}
	}

	preActions: [
		Action {
			id: printDebugInformation
			description: "Print debug information"
			assertions: [lastCommonAssertion]

			onActed: {
				console.log("printDebugInformation::currentToken =", currentToken);
				console.log("printDebugInformation::currentToken.badActor =", currentToken.badActor);
			}
		}
	]

	Assertion {
		id: lastCommonAssertion
		description: "Last check before different routes can be chosen"
		actions: [someShockerAction, someEchoAction]

		Assert {
			targetAction: printDebugInformation
			description: "Check after printing debug information"

			onActionCompleted: {
				console.assert(!!currentToken);
			}
		}
	}

	Action {
		id: someShockerAction
		description: "First action on shocker route"
		assertions: [someShockerAssertion]

		condition: currentToken ? currentToken.badActor === true : false

		onConditionChanged: {
			console.log("someShockerAction::condition", condition);
		}

		onActed: {
			console.log("someShockerAction::currentToken =", currentToken);
			console.log("someShockerAction::currentToken.badActor =", currentToken.badActor);
		}
	}

	Assertion {
		id: someShockerAssertion
		description: "someShockerAssertion"
		actions: []

		Assert {
			targetAction: someShockerAction

			onActionCompleted: {
				console.log("someShockerAssertion::currentToken =", currentToken);
				if (!currentToken.badActor) {
					console.error("someShockerAssertion::currentToken.badActor =", currentToken.badActor)
				} else {
					console.log("someShockerAssertion::currentToken.badActor =", currentToken.badActor);
				}
//				console.assert(currentToken.badActor === true);
			}
		}
	}

	Action {
		id: someEchoAction
		description: "First action on echo route"
		assertions: [someEchoAssertion]

		condition: currentToken ? currentToken.badActor === false : false

		onConditionChanged: {
			console.log("someEchoAction::condition", condition);
		}

		onActed:{
			console.log("someEchoAction::currentToken =", currentToken);
			console.log("someEchoAction::currentToken.badActor =", currentToken.badActor);
		}
	}

	Assertion {
		id: someEchoAssertion
		description: "someEchoAssertion"
		actions: []

		Assert {
			targetAction: someEchoAction

			onActionCompleted: {
				console.log("someEchoAssertion::currentToken =", currentToken);
				console.log("someEchoAssertion::currentToken.badActor =", currentToken.badActor);
				console.assert(currentToken.badActor === false);
			}
		}
	}
}
