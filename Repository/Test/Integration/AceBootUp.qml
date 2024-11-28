import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.Meta 0.1

import Stress.Test 0.1

PetrI {
	id: petrI

	preAssertion: Assertion {
		id: initialAssertion

		actions: [preAction]

		Assert {
			description: "Do we have session to proceed?"

			targetAction: preAction

			onActionPrepared: {
				console.assert(!!currentToken.avatar.session);
			}
		}
	}

	preActions: [
		Action {
			id: preAction
			description: "Doing nothing"

			assertions: [generalAssertion]

			onActed: {

			}
		}
	]

	Assertion {
		id: generalAssertion

		actions: [aceShocker]
	}

	Shock {
		id: aceShocker
		description: "Let's make sure that Ace is online"

		assertions: [onlineAssertion]

		onoff: false
		shocker: currentToken ? currentToken.getProperty("aceShocker", null) : null
	}

	Assertion {
		id: onlineAssertion

		actions: [verifyAction]
		
		Assert {
			description: "Is Ace back and available?"

			targetAction: aceShocker
			onActionCompleted: {
				//console.assert(currentToken.avatar.realm.endpoint !== null);
				//console.assert(currentToken.avatar.session.hasUpstream == true);
				//console.assert(!!currentToken.avatar.session);
			}
		}
	}

	Action {
		id: verifyAction
		description: "Verify session"

		assertions: [finalAssertion]

		onActed: {
			currentToken.avatar.session.verify();
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			function onValidated(success: bool)
			{
				console.log("Session was validated"); /// < All applets are valid
			}
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			function onRevalidated(success: bool)
			{
				console.log("Session was revalidated"); /// < All applets are invalid now and model needs to be reattached
				/// @todo Need to close all application models&desriptior
			}
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			function onInvalidated(silent: bool)
			{
				console.error("Session was invalidated"); /// < No chance to continue
			}
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

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
		id: finalAssertion

		actions: []

		Assert {
			description: "Are we completely restored?"

			targetAction: verifyAction
			onActionCompleted: {
				console.assert(currentToken.avatar.session.hasUpstream == true);
				console.assert(currentToken.avatar.realm.endpoint !== null);
			}
		}
	}
}
