import QtQml 2.15

import Core.Test 0.1

PetrI {
	id: testModel

	property bool isEnabled: true
	property int smallRandomBorder: 10
	property int largeRandomBorder: 100

	tokens: [
		Token {
			description: "Math operation #1"
			positions: [checkConditions]

			property int leftOperand
			property int rightOperand
			property double result
		},

		Token {
			description: "Math operation #2"
			positions: [checkConditions]

			property int leftOperand
			property int rightOperand
			property double result
		}
	]

	preAssertion: Assertion {
		id: checkConditions
		description: "Check initial conditions / Check conditions before preAction"
		actions: [generateSmallValues, generateLargeValues]

		Assert {
			onActionPrepared: {
				console.log("Assert initial conditions");
				console.assert(getEnvironmentVariable("boolean") === true);
			}
		}

		Assert {
			targetAction: generateSmallValues

			onActionPrepared: {
				console.log("Assert before generateSmallValues");
				console.assert(testModel.smallRandomBorder >= 1);
			}
		}

		Assert {
			targetAction: generateLargeValues

			onActionPrepared: {
				console.log("Assert before generateLargeValues");
				console.assert(testModel.largeRandomBorder >= 1);
			}
		}
	}

	preActions: [
		Action {
			id: generateSmallValues
			description: "Generate small non-negative values"
			assertions: [validateLeftOperand, validateRightOperand]

			onActed: {
				console.log(description);
				currentToken.leftOperand = Math.floor(Math.random() * testModel.smallRandomBorder);
				currentToken.rightOperand = Math.floor(Math.random() * testModel.smallRandomBorder);
				console.log(currentToken.leftOperand);
				console.log(currentToken.rightOperand);
			}
		},

		Action {
			id: generateLargeValues
			description: "Generate large non-positive values"
			assertions: [validateLeftOperand, validateRightOperand]

			onActed: {
				console.log(description);
				currentToken.leftOperand = -Math.floor(Math.random() * testModel.largeRandomBorder);
				currentToken.rightOperand = -Math.floor(Math.random() * testModel.largeRandomBorder);
				console.log(currentToken.leftOperand);
				console.log(currentToken.rightOperand);
			}
		}
	]

	assertions: [
		Assertion {
			id: validateLeftOperand
			description: "Validate left value generation / Check left value before future math operation"
			actions: [performAddition, performDidision]

			Assert {
				targetAction: generateSmallValues

				onActionCompleted: {
					console.log("Assert left value after generateSmallValues");
					console.assert(0 <= currentToken.leftOperand && currentToken.leftOperand <= testModel.smallRandomBorder - 1);
				}
			}

			Assert {
				targetAction: generateLargeValues

				onActionCompleted: {
					console.log("Assert left value after generateLargeValues");
					console.assert(-testModel.largeRandomBorder + 1 <= currentToken.leftOperand && currentToken.leftOperand <= 0);
				}
			}

			Assert {
				targetAction: performAddition

				onActionPrepared: {
					console.log("Assert left value before performAddition");
					console.assert(true);
				}
			}

			Assert {
				targetAction: performDidision

				onActionPrepared: {
					console.log("Assert left value before performDidision");
					console.assert(true);
				}
			}
		},

		Assertion {
			id: validateRightOperand
			description: "Validate right value generation / Check right value before future math operation"
			actions: [performAddition, performDidision]

			Assert {
				targetAction: generateSmallValues

				onActionCompleted: {
					console.log("Assert right value after generateSmallValues");
					console.assert(0 <= currentToken.rightOperand && currentToken.rightOperand <= testModel.smallRandomBorder - 1);
				}
			}

			Assert {
				targetAction: generateLargeValues

				onActionCompleted: {
					console.log("Assert right value after generateLargeValues");
					console.assert(-testModel.largeRandomBorder + 1 <= currentToken.rightOperand && currentToken.rightOperand <= 0);
				}
			}

			Assert {
				targetAction: performAddition

				onActionPrepared: {
					console.log("Assert right value before performAddition");
					console.assert(true);
				}
			}

			Assert {
				targetAction: performDidision

				onActionPrepared: {
					console.log("Assert right value before performDidision");
					console.assert(currentToken.rightOperand !== 0);
				}
			}
		},

		Assertion {
			id: validateResult
			description: "Validate final result of operation"
			actions: []

			Assert {
				targetAction: performAddition

				onActionCompleted: {
					console.log("Assert after performAddition");
					console.assert(testModel.result !== NaN);
				}
			}

			Assert {
				targetAction: performDidision

				onActionCompleted: {
					console.log("Assert after performDidision");
					console.assert(testModel.result !== Infinity);
				}
			}
		}
	]

	actions: [
		Action {
			id: performAddition
			description: "Perform addition operation"
			assertions: [validateResult]

			onActed: {
				console.log(description);
				currentToken.result = currentToken.leftOperand + currentToken.rightOperand;
				console.log(currentToken.result);
			}
		},

		Action {
			id: performDidision
			description: "Perform division operation"
			assertions: [validateResult]

			onActed: {
				console.log(description);
				currentToken.result = currentToken.leftOperand / currentToken.rightOperand;
				console.log(currentToken.result);
			}
		}
	]
}
