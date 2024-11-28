import QtQml 2.15

import Core.Test 0.1

PetrI {
	id: petriForPort

	tokens: [
		Token {
			positions: [assertion_A]
			description: "token of petriForPort3"
		}
	]

	preAssertion: Assertion {
		id:assertion_A
		description: "preAssertion of petriForPort3"
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
			description: "preAction of petriForPort3"
			assertions: [assertion_C]

			onActed: {

			}
		}
	]

	Assertion {
		id: assertion_C
		description: "assertion of petriForPort3"
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
		description: "action of petriForPort3"
		assertions: [assertion_E]

		onActed: {

		}
	}

	Assertion {
		id: assertion_E
		description: "assertion of petriForPort3"
		actions: []

		Assert {
			targetAction: action_D

			onActionCompleted: {

			}
		}
	}
}
