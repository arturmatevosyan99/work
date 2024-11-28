import QtQuick 2.15
import QtQuick.Controls 2.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model
import Core.System 0.1

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	property bool hasToken: false

	function commentOut() {
		hasToken = message.hasToken();
		label.text = message.comment;
	}

	model: Model.PushRequestMessage {
		id: message

		onImported: {
			commentOut();
		}

		onCommentChanged: {
			commentOut();
		}

		onTokenChanged: {
			commentOut();
		}
	}

	Label {
		id: label

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.top: rootView.top
		anchors.bottom: button.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12

		wrapMode: Label.Wrap
		text: hasToken ? qsTr("Thank you, permission was granted.") : qsTr("Please, press on the button bellow to allow push notifications us.")
		color: fontColor
	}

	Button {
		id: button

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		state: hasToken ? "granted" : "requested"
		flat: true

		states: [
					State {
							name: "granted"
							PropertyChanges {
												target: button
												down: false
												text: qsTr("Granted!")
												enabled: false
							}
					},
					State {
							name: "requested"
							PropertyChanges {
												target: button
												down: true
												text: qsTr("Allow push")
												enabled: true
							}
					}
		]

		onClicked: {
			message.requestToken();
		}
	}
}
