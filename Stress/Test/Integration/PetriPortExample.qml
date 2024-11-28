import QtQml 2.15

import Core.Test 0.1

PetrI {
	id: petrI

	tokens: [
		Token {
			positions: [assertion_A]
			description: "token of PetriPortExample"
		}
	]

	Port {
		id: petriForPort1

		source: "PetriForPort1.qml"
	}

	Port {
		id: petriForPort3

		source: "PetriForPort3.qml"
	}

	preAssertion: Assertion {
		id:assertion_A
		description: "preAssertion of PetriPortExample"
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
			description: "preAction of PetriPortExample"
			assertions: [assertion_C]

			onActed: {

			}
		}
	]

	Assertion {
		id: assertion_C
		description: "assertion of PetriPortExample"
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
		description: "action of PetriPortExample"
		assertions: [assertion_E]

		onActed: {

		}
	}

	Assertion {
		id: assertion_E
		description: "assertion of PetriPortExample"
		actions: []

		Assert {
			targetAction: action_D

			onActionCompleted: {

			}
		}
	}
}
