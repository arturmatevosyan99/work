import QtQuick 2.15
import QtQuick.Controls 2.15

import Conversation.Model 0.1
import Core.System 0.1

Rectangle {
	id: rootView

	color: MessageLayout.isOwn ? "#29B227" : "#F3F4F6"
	radius: 10

	property bool hasPermission: false

	function resize() {
		rootView.width = Math.min(label.implicitWidth + 24, MessageLayout.viewWidth);
		rootView.height = label.implicitHeight + button.implicitHeight + 24;
	}

	function commentOut() {
		hasPermission = message.hasPermission();
		label.text = message.comment;
	}

	HealthRequestMessage {
		id: message
		objectName: "message"

		onImported: {
			commentOut();
		}

		onCommentChanged: {
			commentOut();
		}

		onPermissionChanged: {
			commentOut();
		}
	}

	Label {
		id: label
		text: hasPermission ? qsTr("Thank you, permission to the medical and fitness data was granted.") : qsTr("Please, press on the button bellow to allow access to the medical and fitness data.")
		color: MessageLayout.isOwn ? "white" : "black"
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.bottom: button.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12

		wrapMode: Label.Wrap
		onTextChanged: {
			resize();
		}
	}

	Button {
		id: button
		state: hasPermission ? "granted" : "requested"
		flat: true
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		states: [
					State {
							name: "granted"
							PropertyChanges {
												target: button;
												down: false
												text: qsTr("Access was granted!")
												enabled: false
							}
					},
					State {
							name: "requested"
							PropertyChanges {
												target: button;
												down: true
												text: qsTr("Allow access")
							}
					}
		]

		onClicked: {
			message.requestAccess();
		}
	}
}
