import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.Meta 0.1

import Repository.Meta 0.1
import Repository.Model 0.1

import Stress.Test 0.1


PetrI {
	id: petrI

	property Component aceShockerComponent: Component {
		AceShocker {
		}
	}

	preAssertion: Assertion {
		id: initialAssertion
		description: "IndexAndAceCrashDown preassertion"

		actions: [pickUpAndIndexAction]

		Assert {
			targetAction: pickUpAndIndexAction
			description: "Assert all essentials before we start"

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient);
			}
		}
	}

	preActions: [
		Action {
			id: pickUpAndIndexAction
			description: "Initiate pick-up attempt and index Home Directory"

			assertions: [indexedCheckAssertion]

			onActed: {
				petrI.context.patient.pickUpProfile();
				console.log("pickupProfile attempted ... ");
			}

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.home : null

				function onIndexed()
				{
					console.log("Home Directory indexed");

					for (var i = 0; i < petrI.context.patient.home.indexModel.rowCount(); i++) {
						var index = petrI.context.patient.home.indexModel.index(i, 0);
						petrI.context.patient.savedHomeIndex.push(petrI.context.patient.home.indexModel.data(index, FolderIndex.Id));
							break;
					}
					console.log("Home Directory index saved: "+petrI.context.patient.savedHomeIndex);
				}   
			}

			TimeWaiter {
				seconds: 40

				onDone: {
					console.error("Directory wasn't indexed");
				}
			}
		}
	]

	Assertion {
		id: indexedCheckAssertion
		actions: [shockerSetupAction]

		/// @ocd
		Assert {
			description: "Make sure that Home Directory index was saved properly and != null "

			targetAction: pickUpAndIndexAction

			onActionCompleted: {
				console.assert(!!petrI.context.patient.savedHomeIndex);
				console.log("Getting index is completed");
			}
		}
	}

	Action {
		id: shockerSetupAction
		description: "Setup AceShocker and verify current session"

		assertions: [preschockAssertion]

		onActed: {
			petrI.context.setProperty
			(
				"aceShocker",
				petrI.aceShockerComponent.createObject
				(
					petrI,
					{
						"endpoint": petrI.context.patient.realm.endpoint,
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
			petrI.context.patient.session.verify();/// < -> invalidated, validated, revalidated
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onVerified(success: bool)
			{
				(success ? console.log : console.error)("Session verification success: " + success);
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onFaulted(phase, reject, message)
			{
				console.error("Session verification error: " + message);
			}
		}
	}

	Assertion {
		id: preschockAssertion

		actions: [aceShockAction]

		Assert {
			description: "Check essentials before activating AceShocker"

			targetAction: aceShockAction

			onActionPrepared: {
				/// @??? Why do we need this here
				console.assert(!!petrI.context.patient.session);
			}
		}
	}

	Shock {
		id: aceShockAction
		description: "Let's turn the AceShocker on!"

		assertions: [finalAssertion]

		onoff: true
		onActed: {
			shocker = petrI.context ? petrI.context.getProperty("aceShocker", null) : null
		}

		GroupWaiter {

//			ObjectWaiter{
//				target: currentToken ? petrI.context.patient.session : null
//				function onActivated(onoff : bool)	{
//					if ( onoff == false ) {
//						console.log("Success! session is no longer active");
//					} else {
//						console.error("Nope");
//					}
//				}
//			}

			ObjectWaiter {
				target: petrI.context ? petrI.context.patient.session : null
				function onUpstreamed(onoff : bool)   {
					if ( onoff == false ) {
						console.log("Success! UpStream is down");
					} else {
						console.error("AceShocker was turned on but UpStream is up");
					}
				}

			}
		}

		TimeWaiter {
			seconds: 10

			onDone: {
				console.error("Expected onUpstreamed changes did not occure");
			}
		}
	}

	/*Assertion {
		id: aftershockAssertion

		actions: [verifyAction]

		Assert {
			description: "Make sure that that Ace has crashed and is now unavailable?"

			targetAction: aceShockAction

			onActionCompleted: {
				//console.assert(!petrI.context.patient.session.isActive);
				console.assert(!petrI.context.patient.session.hasUpstream);
			}
		}
	}*/

	/*Action {
		id: verifyAction
		description: " Initiate session verfification and learn it's current state - 'faulted' expected "

		assertions: [finalAssertion]

		onActed: {
			petrI.context.patient.session.verify();
		}

		ObjectWaiter {
			target: currentToken ? petrI.context.patient.session : null

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
							console.log(" As expected, session is currently 'faulted' ! ");
						}
					} else {

						console.error("Unexpected session fault(", this.phase, this.reject, "): " +  this.message);
					}
				}
			}
		}

		ObjectWaiter {
			target: currentToken ? petrI.context.patient.session : null

			function onInvalidated(silent: bool)
			{
				/// @note Это фатальная ситуация
				console.error("Session was invalidated on the Ace");
			}
		}

		ObjectWaiter {
			target: currentToken ? petrI.context.patient.session : null

			function onValidated(success: bool)
			{
				console.log("Session was validated"); /// < All applets are valid
			}
		}

		ObjectWaiter {
			target: currentToken ? petrI.context.patient.session : null

			function onRevalidated(success: bool)
			{
				console.log("Session was revalidated"); /// < All applets are invalid now and model needs to be reattached
				/// @todo Need to close all application models&desriptior
			}
		}

		TimeWaiter {
			seconds: 40

			onDone: {
				console.error("Nothing has happend for a long time :(");
			}
		}
	}*/

	Assertion {
		id: finalAssertion

		actions: []

		Assert {
			description: "it's finalAssertion - make sure that we are still 'shocked' before moving on to the next port "

			targetAction: aceShockAction

			onActionCompleted: {
				console.assert(petrI.context.patient.session.hasUpstream == false)
			}
		}
	}
}

