import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1
import Core.Meta 0.1

import Repository.Meta 0.1
import Repository.Model 0.1

PetrI {
	id: petrI

	preAssertion: Assertion {
		id: initialAssertion
		description: "DirectoryIndex preassertion"

		actions: [finalRiddleView]

		Assert {
			targetAction: finalRiddleView
			description: "Check before indexing"

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!petrI.context.patient);
			}
		}
	}

	preActions: [
		Action {
			id: finalRiddleView
			description: "in finalRiddleView we check for onIndexed"

			assertions: [finalRiddleViewIndexedCheck]

			onActed: {
				petrI.context.patient.pickUpProfile();
				console.log("pickupProfile attempted ... ");
			}

			ObjectWaiter {
				target: petrI ? petrI.context.patient.home : null

				function onIndexed()
				{
					console.log("Home Directory indexed");

					for (var i = 0; i < petrI.context.patient.home.indexModel.rowCount(); i++) {
						var index = petrI.context.patient.home.indexModel.index(i, 0);

						if (petrI.context.patient.home.indexModel.data(index, FolderIndex.ApplicationScheme) === "588829656820482053") {
							//
							//588829656820482053=mentalgami.smartchannel (queen-eureka)
							//-8663136378210484223=droblozhko.appointmentchannel (queen-afrodita)
							//-8034428181974876159=conversation.groupchannel (n/a)
							//
							console.log("Conversation Channel found");

							var render = SchemeFactory.load("Repository.Meta.ApplicationRender", petrI.context.patient.home.indexModel.data(index, FolderIndex.ApplicationScheme));
							var component = Qt.createComponent(render.makeURL());

							if (component.status === Component.Ready) {
								petrI.context.patient.channelFeed = component.createObject(currentToken);
								console.log("Appointment Channel created");
								petrI.context.patient.channelFeedId = petrI.context.patient.home.indexModel.data(index, FolderIndex.Id);
								console.log("FeedId saved");
							} else {
								console.error("Appointment Channel creation failed");
							}

							break;
						}
					}
				}
			}

			TimeWaiter {
				seconds: 40

				onDone: {
					console.error("directory wasn't indexed");
				}
			}
		}
	]

	Assertion {
		id: finalRiddleViewIndexedCheck
		actions: []

		/// @ocd
		Assert {
			description: "Check after getting index"

			targetAction: finalRiddleView

			onActionCompleted: {
				console.assert(!!petrI.context.patient.channelFeedId);
				console.log("Getting index is completed");
			}
		}
	}
}
