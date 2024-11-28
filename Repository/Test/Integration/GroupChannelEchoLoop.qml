import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.Meta 0.1
import Repository.Meta 0.1

import Repository.Model 0.1
import DrOblozhko.Model 0.1
import Mentalgami.Model 0.1

import Stress.Test 0.1

import "qrc:/Conversation/View/Application" as ConversationApplication
import "qrc:/Mentalgami/View/Application" as MentalgamiApplication

PetrI {
	id: petrI

	preAssertion: Assertion {
		id: initialAssertion
		description: "GroupChannelEcho preassertion"

		actions: [attachChannelFeed]

		Assert {
			description: "Check before attaching channelFeed"

			targetAction: attachChannelFeed

			onActionPrepared: {
				console.assert(!!petrI.context.patient);
				console.assert(!!petrI.context.patient.authorizationQuiz);
				console.assert(!!petrI.context.patient.authorizationQuiz.controller);

				console.assert(!!petrI.context.patient.authorizationQuiz.controller.active);
				console.assert(!!petrI.context.patient.authorizationQuiz.model.phase);

				if (petrI.context.patient.authorizationQuiz.model.phase !== WelcomeQuiz.Phase.OnBoard) {
					console.error("Wrong phase", petrI.context.patient.authorizationQuiz.model.phase);
				}
			}
		}
	}

	preActions: [
		Action {
			id: attachChannelFeed
			description: "attachChannelFeed"

			assertions: [attachChannelCheck]

			onActed: {
				console.log("petrI.context.patient.channelFeed = ", petrI.context.patient.channelFeed);
				petrI.context.patient.session.attach(petrI.context.patient.channelFeed.model, petrI.context.patient.channelFeedId, true, {});
				petrI.context.patient.n = 0;
				petrI.context.patient.iterationsCounter = 100;

			}

			GroupWaiter {
				ObjectWaiter {
					objectName: "xxxx"
					target: petrI.context ? petrI.context.patient.session : null

					WaiterCallback {
						name: "attached"
						callee: function()
						{
							console.log("onAttached(application, fact)");
							if (this.fact == Session.AttachFact.NonApplicable || this.fact == Session.AttachFact.HandleError || this.fact == Session.AttachFact.OpenError || this.fact == Session.AttachFact.CoherenceError) {
								console.error("servlet-applet attachment not completed");
							}
						}
					}
				}

				ObjectWaiter {
					target: petrI.context ? petrI.context.patient.channelFeed.model : null

					function onInitialized(dump: bool, success: bool)
					{
						if ( success ) {
							console.log("Initialized successful: dump=" + dump + "; success=" + success);
						} else {
							console.error("Initialized unsuccessful: dump=" + dump + "; success=" + success);
						}
					}
				}
			}

			TimeWaiter {
				seconds: 20

				onDone: {
					console.error("session.attach timer is over");
				}
			}
		}
	]

	Assertion {
		id: attachChannelCheck

		actions: [basicPush]

		/// @ocd
		Assert {
			description: "Check after onAttached"

			targetAction: attachChannelFeed

			onActionCompleted: {
				console.log("session attached sucessfully!");
				if (petrI.context.patient.channelFeed.model.isOpened !== true ) {
					console.error("Conversation Channel not opened");
				} else {
					console.log("Conversation Channel attached and opened");
				}
			}
		}
	}
/*
	Assertion {
		id: fedCheck
		description: "has unloaded messages?"

		actions: [basicPush]

		Assert {
			targetAction: fetching

			onActionCompleted: {
				//console.assert(!!petrI.context.patient.groupChannel.handle);
				//console.assert(!!petrI.context.patient.groupChannel.isOpened);
			}
		}

		Assert {
			targetAction: basicPush

			onActionPrepared: {
				console.assert(!petrI.context.patient.groupChannel.hasUnloadedMessages);
			}
		}
	}
*/
	Action {
		id: basicPush
		description: "attempting to push?"

		assertions: [pushCheck]

		onActed: {
			var uniqueText = "/echo 1234_" + petrI.context.patient.account + "message№_" + petrI.context.patient.n;
			var sentText = "1234_" + petrI.context.patient.account + "message№_" + petrI.context.patient.n;
			pushPullConnections.pushtext(petrI.context.patient.channelFeed.controller,petrI.context.patient.channelFeed.model,uniqueText);
			petrI.context.patient.n = 0;
		}

		ObjectWaiter {
			target: Connections {
				id: pushPullConnections

				enabled: target

				target: petrI.context.patient.channelFeed.model

				ignoreUnknownSignals: true

				signal passed(var success)

			function pushtext(controller,model,textExample)
			{
				if (controller && model && textExample) {
					target = model;
					console.log("pushing...");
					controller.text(textExample);
				}
			}

			function onPulled (upper_log_bound, lower_log_bound, flags )
			{
				var sentText = "1234_" + petrI.context.patient.account + "message№_" + petrI.context.patient.n;
				var uniqueText = "/echo 1234_" + petrI.context.patient.account + "message№_" + petrI.context.patient.n;

				if (
					SchemeFactory.ident("Conversation.Meta.MessageRender", "Conversation", "EchoMessage")
					!==
					target.lastMessage.schemeQd
				) {
					target = null;
					console.error("Last message is not an echo message: " + target.lastMessage.schemeQd);
					passed(false);
				}
				if(target.lastMessage.action != sentText){
					target = null;
					console.error("Wrong text: " + petrI.context.patient.channelFeed.model.lastMessage.action + " vs: " + sentText );
					passed(false);
				}
				if ( petrI.context.patient.n >= petrI.context.patient.iterationsCounter){
					console.log("last message sent and pulled sucesfully");
					target = null;
					passed(true);
				}
				else
				{
					console.log("another message sent and pulled sucesfully," + petrI.context.patient.n + "+1 messages were sent and pulled");
					petrI.context.patient.n++;
					sentText = "1234_" + petrI.context.patient.account + "message№_" + petrI.context.patient.n;
					uniqueText = "/echo 1234_" + petrI.context.patient.account + "message№_" + petrI.context.patient.n;
					pushPullConnections.pushtext(petrI.context.patient.channelFeed.controller,target,uniqueText);
				}
			}
			}

			function onPassed(success)
			{
				if (!success) {
					console.error("WTF?")
				} else {
					console.log("all done!");
				}
			}

			}

		TimeWaiter {
			seconds: 120

			onDone: {
				console.error("Attempting to send the massage takes too much time");
			}
		}
	}

	Assertion {
		id: pushCheck

		actions: []

		Assert {
			targetAction: basicPush

			onActionCompleted: {
				console.log("Make Abra Cadabra Again!")
			}
		}
	}
}
