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

	property string redeemText: "/redeem PB_30";

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

		actions: [basicRedeemPush]

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

		actions: [basicRedeemPush]

		Assert {
			targetAction: fetching

			onActionCompleted: {
				//console.assert(!!currentToken.groupChannel.handle);
				//console.assert(!!currentToken.groupChannel.isOpened);
			}
		}

		Assert {
			targetAction: basicRedeemPush

			onActionPrepared: {
				console.assert(!currentToken.groupChannel.hasUnloadedMessages);
			}
		}
	}
*/
	Action {
		id: basicRedeemPush
		description: "attempting to push?"

		assertions: [pushCheck]

		onActed: {
			if (petrI.context.patient.channelFeed.controller.text(petrI.redeemText)) {
//			if (currentToken.channelFeed.controller.command("echo", ["1234"])) {
//				currentToken.channelFeed.feed();
			}
		}

		GroupWaiter {
			id: buttonMessageWaiter

			ObjectWaiter { /// < ждем т.е. отправки сообщения
				target: petrI ? petrI.context.patient.channelFeed.model : null

				WaiterCallback {
					name: "pulled"
					callee: function()
					{
						console.log("onPulled(upper_log_bound=" + this.upper_log_bound + ", lower_log_bound=" + this.lower_log_bound + ", flags=" + this.flags + ")");
						let thisScheme = String(petrI.context.patient.channelFeed.model.lastMessage.schemeQd);

						switch (thisScheme) {
							case '-8034428181974876151':
								console.log("Last message pulled is a command message: " + petrI.context.patient.channelFeed.model.lastMessage.schemeQd);
								if ( petrI.context.patient.channelFeed.model.lastMessage.action.includes("Ваучер не был найден") )
									{
										console.error(petrI.context.patient.channelFeed.model.lastMessage.action);
									}
								else if ( petrI.context.patient.channelFeed.model.lastMessage.action.includes("Ваучер успешно погашен") )
									{
										console.log("It's the command message we were expecting: " + petrI.context.patient.channelFeed.model.lastMessage.action);
									}
							case '-8034428181974876159':
								console.log("Last message pulled is a text message: " + petrI.context.patient.channelFeed.model.lastMessage.schemeQd);
								if ( petrI.context.patient.channelFeed.model.lastMessage.action.includes("Идентификатор активации #") )
									{
										console.log("It's the ident message we were expecting: " + petrI.context.patient.channelFeed.model.lastMessage.action);
									}
								buttonMessageWaiter.reset(false);
							case '-588829656820482051':
								console.log("Last message pulled is a button message: " + petrI.context.patient.channelFeed.model.lastMessage.schemeQd);
								console.assert( petrI.context.patient.channelFeed.model.lastMessage.action.includes("Смарт-курс «"));
								buttonMessageWaiter.reset(true);
							default:
								console.log("While we were waiting for a button message another unexpected message was pushed: " + petrI.context.patient.channelFeed.model.lastMessage.action + petrI.context.patient.channelFeed.model.lastMessage.schemeQd);
								buttonMessageWaiter.reset(false);
						}
					}
				}
			}
		}

		TimeWaiter {
			seconds: 300

			onDone: {
				console.error("Attempting to send the massage takes too much time");
				}
			}
		}

	Assertion {
		id: pushCheck

		actions: []

		Assert {
			targetAction: basicRedeemPush

			onActionCompleted: {
				console.log("Make Abra Cadabra Again!")
			}
		}
	}
}
