import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	MessageDialog {
		id: messageDialog

		title: "Ох!"
		text: ""
	}

	model: Model.AppletMessage {
		id: message

		onImported: {
			label.text = message.description;
			if (message.name.length > 0) {
				button.text = qsTr("Open") + " " + message.name;
			}
		}
	}

	Label {
		id: label
		text: qsTr("Interactive Application")
		color: fontColor
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.top: rootView.top
		anchors.bottom: rootView.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12
		wrapMode: Label.Wrap
	}

	Button {
		id: button
		text: qsTr("Open")

		flat: true
		down: true
		enabled: message.active

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		onClicked: {
			layout.popup(
							null,
							{
								"objectId": message.id,
								"applicationScheme": message.applicationScheme
							}
			);
		}
	}
}
