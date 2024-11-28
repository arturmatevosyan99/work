import QtQuick 2.15
import QtQuick.Controls 2.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model
import Core.System 0.1

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	function commentOut() {		
		label.text = message.comment;
	}

	model: Model.AuthorizationMessage {
		id: message

		onImported: {
			commentOut();
		}
	}

	Label {
		id: label
		text: qsTr("Please, press on the button bellow to change authorize")
		color: fontColor
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.top: rootView.top
		anchors.bottom: button.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12
		wrapMode: Label.Wrap		
	}

	Button {
		id: button		
		flat: true
		down: true
		enabled: true

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		text: qsTr("Change authorization")

		onClicked: {
			message.authorize();
		}
	}
}
