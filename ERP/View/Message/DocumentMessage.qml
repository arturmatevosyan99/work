import QtQuick 2.15
import QtQuick.Controls 2.15

import Core.System 0.1

import ERP.Model 0.1 as Model
import Conversation.View 0.1 as View

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	function refresh() {
		if (message.title.length == 0) {
			label.text = message.description + " №" + message.no;
		} else {
			label.text = message.description + " №" + message.no + "(" + message.title + ")";
		}
	}

	model: Model.DocumentMessage {
		id: message

		onImported: {
			refresh();
		}

		onStateChanged: {
			refresh();
		}
	}

	Label {
		id: label

		text: qsTr("Document")
		color: fontColor
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.bottom: button.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12
		wrapMode: Label.Wrap
	}

	Button {
		id: button

		state: message.document.isHeld ? "held" : "issued"
		enabled: true
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12
		text: "🧾 "  + qsTr("View Document")

		onClicked: {			
			message.viewMore();
		}
	}
}
