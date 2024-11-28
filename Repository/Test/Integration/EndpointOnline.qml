import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.Meta 0.1

import Repository.Meta 0.1
import Repository.Model 0.1

import Stress.Test 0.1

PetrI {
	id: petrI

	property Component netShockerComponent: Component {
		NetShocker {
		}
	}

	preAssertion: Assertion {
		id: initialAssertion
		description: "EndpointOnlineShocker preassertion"

		actions: [preAction]

		Assert {
			targetAction: preAction

			onActionPrepared: {
				/// @todo Проверить что есть upstream
				console.assert(currentToken.avatar.session.hasUpstream == true)
			}
		}
	}

	preActions: [
		Action {
			id: preAction
			description: "PreAction of AceStressOn"

			assertions: [generalCheck]

			onActed: {
				currentToken.setProperty(
										"offlineShocker",
										petrI.netShockerComponent.createObject
										(
											currentToken,
											{
												"endpoint": currentToken.avatar.realm.endpoint
											}
										)
				);

				/// @todo Проверить что есть корректная сессия
				/// token.session.verify() -> invalidated, validated, revalidated
				console.log(" attempting session verification ...");
				currentToken.avatar.session.verify();
			}

			ObjectWaiter {
				target: currentToken ? currentToken.avatar.session : null

				function onVerified(success: bool)
				{
					console.log("onVerified caught");
				}
			}

			ObjectWaiter {
				target: currentToken ? currentToken.avatar.session : null

				function onFaulted(phase, reject, message)
				{
					console.error("onFaulted caught");
				}
			}
		}
	]

	Assertion {
		id: generalCheck
		description: "Сhecking before enabling the shocker"

		actions: [offlineOnShocker]

		Assert {
			targetAction: offlineOnShocker
			onActionPrepared: {
				console.assert(!!currentToken.avatar.session);
			}
		}
	}

	Shock {
		id: offlineOnShocker
		description: "offlineOnShocker"

		assertions: [aftershockOn]

		shocker: currentToken.getProperty("offlineShocker") ? currentToken.getProperty("offlineShocker") : null
		onoff: true
	}

	Assertion {
		id: aftershockOn

		actions: [weCheck]

		Assert {
			description: "Shocker is on"

			targetAction: offlineOnShocker

			onActionCompleted: {
				/// убедимся, что связи действительно нетб то есть avatar.session.hasUpstream == false
				console.assert(!!currentToken.avatar.session);
				console.assert(currentToken.avatar.session.hasUpstream == false);
			}
		}
	}

	Action {
		id: weCheck
		description: "Waiters go here"

		assertions: [finalA]

		onActed: {
			console.log("attempting verification");
			currentToken.avatar.session.verify();
		}
			
		ObjectWaiter {
			objectName: "xyz"

			target: currentToken ? currentToken.avatar.session : null

			WaiterCallback {
				name: "faulted"
				callee: function()
				{
					console.log("onFaulted(phase, reject, message)");
					if (
						this.phase == Session.Verification
						&&
						this.reject == Session.NetworkError
					) {
						console.log(" expected parametres for onFaulted caught!");
						console.log(this.phase, this.reject, this.message);
					}
				}
			}
		}

		ObjectWaiter {
			target: currentToken ? currentToken.avatar.session : null

			function onInvalidated(silent: bool)
			{
				console.log(" onInvalidated caught!");
			}
		}

	}

	Assertion {
		id: finalA

		actions: []

		Assert {
			targetAction: weCheck

			onActionCompleted: {
				console.assert(currentToken.avatar.session.hasUpstream == false)
			}
		}
	}
}
