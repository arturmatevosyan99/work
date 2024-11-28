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

	property string uniqueText: "/echo 1234_" + petrI.context.patient.account;

	Component.onCompleted: {
		if (petrI.autoFire) {
			console.error("This net is intended to use just in a port");
		}
	}

	preAssertion: Assertion {
		id: initialAssertion
		description: "GroupChannelEcho preassertion"

		actions: [attachChannelFeed]

		Assert {
			description: "Check before attaching channelFeed"

			targetAction: attachChannelFeed

			onActionPrepared: {
				console.assert(!!currentToken);
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
			}

			GroupWaiter {
				ObjectWaiter {
					objectName: "xxxx"
					target: petrI ? petrI.context.patient.session : null

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
					target: petrI ? petrI.context.patient.channelFeed.model : null

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
				//console.assert(!!currentToken.groupChannel.handle);
				//console.assert(!!currentToken.groupChannel.isOpened);
			}
		}

		Assert {
			targetAction: basicPush

			onActionPrepared: {
				console.assert(!currentToken.groupChannel.hasUnloadedMessages);
			}
		}
	}
*/
	Action {
		id: basicPush
		description: "attempting to push?"

		assertions: [pushCheck]

		onActed: {
			//console.log(currentToken.groupChannel.lastMessage)
			//var txtForPush = "Abra-Cadabra!"
			console.log("pushing...");
			/// @todo Вместо 1234 генерировать случайное число и ожидать именно его поздней
			console.log(petrI.uniqueText);
			if (petrI.context.patient.channelFeed.controller.text(petrI.uniqueText)) {
//			if (currentToken.channelFeed.controller.command("echo", ["1234"])) {
//				currentToken.channelFeed.feed();
			}
		}

		ObjectWaiter { /// < ждем т.е. отправки сообщения
			target: petrI ? petrI.context.patient.channelFeed.model : null

			WaiterCallback {
				name: "pulled"
				callee: function()
				{
					console.log("onPulled(upper_log_bound=" + this.upper_log_bound + ", lower_log_bound=" + this.lower_log_bound + ", flags=" + this.flags + ")");
					if (
						SchemeFactory.ident("Conversation.Meta.MessageRender", "Conversation", "EchoMessage")
						!==
						petrI.context.patient.channelFeed.model.lastMessage.schemeQd
					) {
						console.error("Last message is not an echo message: " + petrI.context.patient.channelFeed.model.lastMessage.schemeQd);
					}
					console.assert( petrI.uniqueText == "/echo " + petrI.context.patient.channelFeed.model.lastMessage.action );
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
