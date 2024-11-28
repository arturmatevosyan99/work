import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

import Core.System 0.1
import Core.View 0.1
import Conversation.View 0.1 as View
import DrOblozhko.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	MessageDialog {
		id: messageDialog

		title: "Ох!"
		text: ""
	}

	model: Model.HandBookMessage {
		onImported: {
			label.text = description;
		}

		onDenied: {
			messageDialog.text = qsTr("Handbook is not available for you by that time, feel free to contact us if you believe this is in error");
			messageDialog.open();
		}

		onAccessed: {
			var target = url.toString();
			if (target.length > 0) {
				Native.openFile(url);
			} else {
				messageDialog.text = qsTr("Unexpected error has been occurred. Feel free to contact us.");
				messageDialog.open();
			}
		}
	}

	Label {
		id: label

		text: qsTr("Handbook")
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
		enabled: rootView.model.active

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		onClicked: {			
			rootView.model.sign();
		}
	}
}
