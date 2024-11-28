import QtQml 2.15
import QtQuick 2.15

import Core.View 0.1
import Core.System 0.1
import Core.Model 0.1
import Core.Meta 0.1
import Repository.Model 0.1
import Conversation.Model 0.1

Item {
	GroupChannel {
		id: applicationModel
	 
		onOpened: {
			console.log("GroupChannel::onOpened");
		}
	 
		onDumped: {
			console.log("GroupChannel::onDumped");
		}
	 
		onPulled: {
			console.log("GroupChannel::onPulled");
			if (flags === GroupChannel.PushMessage) {

			} else if (flags === GroupChannel.ReceiveMessage) {

			}
		 }

		 onIndexed: {
			 console.log("ChannelFeed.applicationConnection::onIndexed");
		 }

		 onProgressed: {
			 console.log("GroupChannel::onProgressed", value);
		 }
	}

	function test()
	{
		// where session is authorized Session {} and backend model was opened somehow elsewhere so we have object_id, app_handle and app_scheme
		applicationModel.open(session.handle(object_id_in_the_realm, application_model_handle_in_the_session_pool, application_scheme, "Channel(Object) Name"));

		// otherwise we need to attach to remote session our front-end application model
		session.attach(applicationModel, object_model, true);

		// dummy check if application model was opened ("linked" with backend model)
		
		if (applicationModel.isOpened) {
			// Commit/Confirm reading messages upto 0 from the head (in other words: all currently available in the feed)
			applicationModel.read(0, GroupChannel.FeedOffset);
		} else {
			console.fatal("Omg! Game over");
		}

		// lazy history load: ask backend model for a page of messages which were not loaded yet
		applicationModel.fetch(GroupChannel.FeedRotate, GroupChannel.Tail);

		// send a text
		applicationModel.push("Abra-Cadabra!")

		// ... voice, where audioRecorder is AudioRecorder {}
		applicationModel.push(audioRecorder.buffer, GroupChannel.AudioBinary);

		// ... picture, where imageThumbnail is ImageThumbnail {}
		applicationModel.push(imageThumbnail.buffer, GroupChannel.ImageBinary);

		// try again and push message one more time
		applicationModel.repush(uid);

		// ... or remove this draft
		applicationModel.discard(uid);

		// erase a message by qd(special Qd-type where just very long integer inside, otherwise JS engine will crop)
		applicationModel.withdraw(qd);

		return true;
	}
}
