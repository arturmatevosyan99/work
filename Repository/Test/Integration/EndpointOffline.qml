import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3

import Core.Test 0.1

import Repository.Model 0.1
import DrOblozhko.Model 0.1
import Mentalgami.Model 0.1

import Stress.Test 0.1

PetrI {
	id: petrI

	property Component netShockerComponent: Component {
		NetShocker {
		}
	}

	preAssertion: Assertion {
		id: initialAssertion
		description: "Check endpoint before start"

		actions: [preAction]

		Assert {
			targetAction: preAction
			description: "EndpointOffline preassertion"

			onActionPrepared: {
				console.assert(!!petrI.context.patient);
				console.assert(!!petrI.context.patient.realm);
				console.assert(!!petrI.context.patient.realm.endpoint);
			}
		}
	}
	
	preActions: [
		Action {
			id: preAction
			description: "Initiate version request to make sure Ace has network access"

			assertions: [endpointReachAssertion]

			onActed: {
				endpointConnections.reach(petrI.context.patient.realm.endpoint);
			}

			ObjectWaiter {
				target: Connections {
					id: endpointConnections

					enabled: target

					ignoreUnknownSignals: true

					signal passed(var success)

					function reach(endpoint)
					{
						if (endpoint) {
							(target = endpoint).reach();
						}
					}

					function onMaintenanced()
					{
						console.log("maintenance");
						target.reach();
					}

					function onReached()
					{
						target = null;
						passed(true);
					}

					function onOutdated()
					{
						target = null;
						console.error("outdated");
						passed(false);
					}
				}

				function onPassed(success)
				{
					if (!success) {
						console.error("WTF?")
					} else {
						console.log("Reached!");
					}
				}
			}

			TimeWaiter {
				seconds: 40

				onDone: {
					console.error("oh no");
				}
			}
		}
	]

	Assertion {
		id: endpointReachAssertion

		actions: [setupAction]

		Assert {
			description: "Make sure that we still have our current session and can proceed"

			targetAction: setupAction
			onActionPrepared: {
				console.assert(!!petrI.context.patient.session)
			}
		}
	}

	Action {
		id: setupAction
		description: "netShocker setup"

		assertions: [sessionStatusAssertion]

		onActed: {
			//currentToken.setProperty
			petrI.context.setProperty
			(
				"netShocker",
				petrI.netShockerComponent.createObject
				(
					currentToken,
					{
						"endpoint": petrI.context.patient.realm.endpoint,
						/*

						*/
					}
				)
			);
		}
	}

	Assertion {
		id: sessionStatusAssertion

		actions: [netShockOnAction]

		Assert {
			description: "Make sure that we still have our current session and can proceed"

			targetAction: netShockOnAction
			onActionPrepared: {
				console.assert(!!petrI.context.patient.session)
			}
		}
	}

	Shock {
		id: netShockOnAction
		description: "Let's turn the NetShocker on!"
		assertions: [shockedAssertion]

		onoff: true

		onActed: {
			petrI.context.patient.session.verify();
			shocker = petrI.context ? petrI.context.getProperty("netShocker", null) : null
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onFaulted(phase, reject, message)
			{
				console.log( "onFaulted caught" );
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onInvalidated(silent: bool)
			{
				console.error("Session was invalidated on the Ace");
			}
		}

		TimeWaiter {
			seconds: 40

			onDone: {
				console.error("Nothing has happend for a long time :(");
			}
		}
	}

	Assertion {
		id: shockedAssertion

		actions: [netShockOffAction]

		Assert {
			description: "Make sure that we still have our current session and can proceed"

			targetAction: netShockOffAction
			onActionPrepared: {
				console.assert(!!petrI.context.patient.session)
			}
		}
	}

	Shock {
		id: netShockOffAction
		description: "Let's turn the NetShocker off!"
		assertions: [aftershockAssertion]

		onoff: false

		onActed: {
			//shocker = currentToken ? currentToken.getProperty("netShocker", null) : null
			shocker = petrI.context ? petrI.context.getProperty("netShocker", null) : null
		}
	}

	Assertion {
		id: aftershockAssertion

		actions: [verifyAction]

		Assert {
			description: "Make sure that we still have our current session and can proceed"

			targetAction: verifyAction
			onActionPrepared: {
				console.assert(!!petrI.context.patient.session)
			}
		}
	}

	Action{
		id: verifyAction
		description: "Verify current session"

		assertions: [finalAssertion]

		onActed: {
			petrI.context.patient.session.verify();/// < -> invalidated, validated, revalidated
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onFaulted(phase, reject, message)
			{
				console.error( "onFaulted caught - this shouldn't happen" );
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onVerified(success: bool)
			{
				console.error ("onVerified caught - this shouldn't happen");
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onRevalidated(cold: bool)
			{
				console.log ("Caught signal - session was revalidated");
			}
		}

		ObjectWaiter {
			target: petrI.context ? petrI.context.patient.session : null

			function onInvalidated(silent: bool)
			{
				console.error("Caught signal - session was invalidated on the Ace");
			}
		}

		TimeWaiter {
			seconds: 40

			onDone: {
				console.error("Nothing has happend for a long time :(");
			}
		}
	}

	Assertion {
		id: finalAssertion

		actions: []

		Assert {
			targetAction: verifyAction
			description: "it's finalAssertion"
			onActionCompleted: {

			}
		}
	}
}
