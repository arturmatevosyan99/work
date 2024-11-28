import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model
import Core.System 0.1

View.MessageView {
	id: rootView

	contentWidth: button.implicitWidth
	contentHeight: button.implicitHeight

	model: Model.AttachmentMessage {
		id: message

		onImported: {
			if (Model.AttachmentMessage.Cached === message.cacheState) {
				console.log("VoiceMessage::onImported ", message.cachePath);
			}
			console.log("VoiceMessage::onImported ", message.fileName);
			message.cache();
		}

		onCached: {
		}
	}

	Button {
		id: button

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.top: rootView.top
		anchors.margins: 2
		padding: 2

		text: message.fileName
		enabled: message.isImported && message.isCached
		down: true
		flat: true
		display: AbstractButton.TextBesideIcon

		icon.source: "/Conversation/Asset/Image/Attachment.svg"
		icon.color: "transparent"

		background: Rectangle {
			color: "transparent"
			anchors.topMargin: 0
			anchors.bottomMargin: 0
		}

		onClicked: {
			Native.openFile(message.cachePath);
		}
	}
}
