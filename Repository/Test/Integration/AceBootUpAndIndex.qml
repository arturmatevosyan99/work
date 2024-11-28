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
		description: "AceBootUpAndIndex preassertion"

		actions: [aceShockAction]

		Assert {
			description: "Assert all essentials before we start"

			targetAction: aceShockAction

			onActionPrepared: {
				console.assert(!!petrI.context.patient.session);
			}
		}
	}

	preActions: [
		Shock {
			id: aceShockAction
			description: "Let's turn the AceShocker off!"

			assertions: [aceOnlineAssertion]

			onoff: false
			onActed: {
				shocker = petrI.context ? petrI.context.getProperty("aceShocker", null) : null
			}
		}
	]

	Assertion {
		id: aceOnlineAssertion

		actions: [verifyAction]
		
		Assert {
			description: "Make sure that we still have our current session and can proceed"

			targetAction: aceShockAction
			onActionCompleted: {
				//console.assert(petrI.context.patient.realm.endpoint !== null);
				console.assert(petrI.context.patient.session.hasUpstream == false);
				//console.assert(!!petrI.context.patient.session);
			}
		}
	}

	Action {
		id: verifyAction
		description: "Initiate session verfification and learn current session state"

		assertions: [sessiomStatusAssertion]

		onActed: {
			petrI.context.patient.session.verify();
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onValidated(success: bool)
			{
				console.error("Session was validated - this shouldn't have happened if we activated the schoker earlier "); /// < All applets are valid
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onRevalidated(success: bool)
			{
				console.log("Session was revalidated"); /// < All applets are invalid now and model needs to be reattached
				/// @todo Need to close all application models&desriptior
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onVerified(success: bool)
			{
				console.log("Session was verified"); /// < All applets are invalid now and model needs to be reattached
				/// @todo Need to close all application models&desriptior
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onInvalidated(silent: bool)
			{
				console.error("Session was invalidated"); /// < No chance to continue
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onFaulted(phase, reject, message)
			{
				console.error("Verification fault(" + phase + ", " +  reject + "): " + message);
			}
		}

		TimeWaiter {
			seconds: 30
		
			onDone: {
				console.error("Verfication timer is over");
			}
		}
	}

	Assertion {
		id: sessiomStatusAssertion

		actions: [waiterAction]

		Assert {
			description: "Make sure that UpStream is active"

			targetAction: indexingAction
			onActionCompleted: {
				console.assert(!!petrI.context.patient.session);
				console.assert(petrI.context.patient.session.hasUpstream == true);
			}
		}
	}

	/*Action /*{
		id: waiterAction
		description: "Actively monitor changes in onUpstreamed to see if Ace is back online and available"

		assertions: [stateRestoredAssertion]

		onActed: {
			console.log("Expecting waiters ");
		}

		GroupWaiter {

			ObjectWaiter {
				target: currentToken ? petrI.context.patient.session : null
				function onUpstreamed(onoff : bool)   {

					if ( onoff == true ) {
						console.log("Success! UpStream is up");
					} else {
						console.error("Nope");
					}
				}
			}
		}

		TimeWaiter {
			seconds: 20

			onDone: {
				console.error("Expected changes did not occure");
			}
		}
	}*/

	/*Assertion /*{
		id: stateRestoredAssertion

		actions: [indexingAction]

		Assert {
			description: "Make sure that Ace is back online and available"

			targetAction: verifyAction
			onActionCompleted: {
				console.assert(petrI.context.patient.session.hasUpstream == true);
				console.assert(petrI.context.patient.realm.endpoint !== null);
			}
		}
	}*/

	Action {
		id: indexingAction
		description: "Initiate Home Directory indexing and compare new indexes to the saved pre-shocker values"

		assertions: [finalAssertion]

		onActed: {
			var promise = petrI.context.patient.home.index();
			if (promise.isIdle == true || ( promise.isFaulty == true && promise.isFinished == true ) ) { /// < ok, так как  Directory потерял свой descriptor/аттач
				console.log("Promise is idle - new attach/open required");
				petrI.context.patient.indexHome();
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.home : null

			function onIndexed()
			{
				console.log("Session was indexed!");

				for (var i = 0; i < petrI.context.patient.home.indexModel.rowCount(); i++) {
					var index = petrI.context.patient.home.indexModel.index(i, 0);
					console.log("Current index: " + petrI.context.patient.home.indexModel.data(index, FolderIndex.Id) );
					console.log("Saved index: " + currentToken.savedHomeIndex[i]);
					if (currentToken.savedHomeIndex[i] != petrI.context.patient.home.indexModel.data(index, FolderIndex.Id) ){
						console.error("Post-shocker index doesn't match the original!");
					}
				}

			}
		}

		TimeWaiter {
			seconds: 40

			onDone: {
				console.error("Session wasnt't indexed");

			}
		}
	}

	Assertion {
		id: finalAssertion
		actions: []

		/// @ocd
		Assert {
			description: "it's finalAssertion"

			targetAction: indexingAction

			onActionCompleted: {
				//console.assert();
				console.log("Getting index is completed");
			}
		}
	}

}
