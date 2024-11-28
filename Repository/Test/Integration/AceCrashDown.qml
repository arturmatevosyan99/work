import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.Meta 0.1

import Stress.Test 0.1

PetrI {
	id: petrI

	property Component aceShockerComponent: Component {
		AceShocker {
		}
	}

	preAssertion: Assertion {
		id: initialAssertion

		actions: [preAction]

		Assert {
			description: "Do we have alive Ace in the current Session?"

			targetAction: preAction

			onActionPrepared: {
				console.assert(currentToken.avatar.session.isActive);
				console.assert(currentToken.avatar.session.hasUpstream);
			}
		}
	}

	preActions: [
		Action {
			id: preAction
			description: "Setup shocker and verify current session"

			assertions: [generalAssertion]

			onActed: {
				currentToken.setProperty
				(
					"aceShocker",
					petrI.aceShockerComponent.createObject
					(
						currentToken,
						{
							"endpoint": currentToken.avatar.realm.endpoint,
							"signal" : AceShocker.SegmentationViolation
							/*
							aceShockceShocker.OutOfMemory /// < SIGOOM: Чрезмерное выделение памяти, которое закончится SIGKILL
							aceShock.Kill /// < SIGKILL
							aceShock.SegmentationViolation /// < SIGSEGV: обращение к памяти через невалидный указатель
							aceShock.Abort /// < SIGABRT: вызов abort
							aceShock.FloatingPointException /// < SIGFPE: деление на ноль
							*/
						}
					)
				);
				currentToken.avatar.session.verify();/// < -> invalidated, validated, revalidated
			}

			ObjectWaiter {
				target: currentToken ? currentToken.avatar.session : null

				function onVerified(success: bool)
				{
					(success ? console.log : console.error)("Session verification success: " + success);
				}
			}

			ObjectWaiter {
				target: currentToken ? currentToken.avatar.session : null

				function onFaulted(phase, reject, message)
				{
					console.error("Session verification error: " + message);
				}
			}
		}
	]

	Assertion {
		id: generalAssertion

		actions: [aceShock]

		Assert {
			description: "Check something without any particular reason"

			targetAction: aceShock

			onActionPrepared: {
				/// @??? Why do we need this here
				console.assert(!!currentToken.avatar.session);
			}
		}
	}

	Shock {
		id: aceShock
		description: "Let's shock the Ace!"

		assertions: [shockAssertion]

		onoff: true
		shocker: currentToken ? currentToken.getProperty("aceShocker", null) : null
	}

	Assertion {
		id: shockAssertion

		actions: [verifyAction]

		Assert {
			description: "Is Ace crashed and unavailable?"

			targetAction: aceShock

			onActionCompleted: {
				//console.assert(!currentToken.avatar.session.isActive);
				console.assert(!currentToken.avatar.session.hasUpstream);
			}
		}
	}

	Action {
		id: verifyAction
		description: "Make sure that session cannot be verified"

		assertions: [finalAssertion]

		onActed: {
			currentToken.avatar.session.verify();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			WaiterCallback {
				name: "faulted"
				callee: function()
				{
					//console.log("phase=", Session.Verification, "=", this.phase);
					//console.log("reject=", Session.NetworkError, "=", this.reject);
					if (this.phase == Session.Verification) {
						if (this.reject != Session.NetworkError) {
							console.error("Unexpected verification fault(", this.reject, "): " +  this.message);
						} else {
							/// @note Все идет по плану, на это и расчитывали
						}
					} else {

						console.error("Unexpected session fault(", this.phase, this.reject, "): " +  this.message);
					}
				}
			}
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			function onInvalidated(silent: bool)
			{
				/// @note Это фатальная ситуация
				console.error("Session was invalidated on the Ace");
			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Nothing has happend for a long time :(");
			}
		}
	}

	Assertion {
		id: finalAssertion

		actions: []

		Assert {
			description: "Are we shocked but still here?"

			targetAction: verifyAction

			onActionCompleted: {
				//console.assert(currentToken.avatar.session.hasUpstream == false)
			}
		}
	}
}
