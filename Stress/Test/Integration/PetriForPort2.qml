import QtQml 2.15

import Core.Test 0.1

PetrI {
	id: petriForPort

	tokens: [
		Token {
			positions: [assertion_A]
			description: "token of petriForPort2"
		}
	]

	preAssertion: Assertion {
		id:assertion_A
		description: "preAssertion of petriForPort2"
		actions: [action_B]

		Assert {
			onActionPrepared: {

			}
		}

		Assert {
			targetAction: action_B

			onActionPrepared: {

			}
		}
	}

	preActions: [
		Action {
			id: action_B
			description: "preAction of petriForPort2"
			assertions: [assertion_C]

			onActed: {

			}
		}
	]

	Assertion {
		id: assertion_C
		description: "assertion of petriForPort2"
		actions: [action_D]

		Assert {
			targetAction: action_B

			onActionCompleted: {

			}
		}

		Assert {
			targetAction: action_D

			onActionPrepared: {

			}
		}
	}

	Action {
		id: action_D
		description: "action of petriForPort2"
		assertions: [assertion_E]

		onActed: {

		}
	}

	Assertion {
		id: assertion_E
		description: "assertion of petriForPort2"
		actions: []

		Assert {
			targetAction: action_D

			onActionCompleted: {

			}
		}
	}
}
