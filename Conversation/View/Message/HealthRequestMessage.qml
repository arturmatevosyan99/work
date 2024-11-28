import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model
import Core.System 0.1

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	property bool hasPermission: false

	MessageDialog {
		id: notapplicableDialog

		visible: false
		title: qsTr("Oops...")
		text: qsTr("Your device does not support `Health` service, this feature will be not available")
		standardButtons: StandardButton.Ok
	}

	function commentOut() {
		hasPermission = message.hasPermission();
		label.text = message.comment;
	}

	model: Model.HealthRequestMessage {
		id: message

		onImported: {
			commentOut();
		}

		onCommentChanged: {
			commentOut();
		}

		onPermissionChanged: {
			if (!message.hasPermission()) {
				notapplicableDialog.open();
			} else {
				commentOut();
			}
		}
	}

	Label {
		id: label
		text: hasPermission ? qsTr("Thank you, permission to the medical and fitness data was granted.") : qsTr("Please, press on the button bellow to allow access to the medical and fitness data.")
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
		state: hasPermission ? "granted" : "requested"
		flat: true
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		states: [
					State {
							name: "granted"
							PropertyChanges {
												target: button
												down: false
												text: qsTr("Access was granted!")
												enabled: false
							}
					},
					State {
							name: "requested"
							PropertyChanges {
												target: button
												down: true
												text: qsTr("Allow access")
												enabled: true
							}
					}
		]

		onClicked: {
			message.requestAccess();
		}
	}
}
