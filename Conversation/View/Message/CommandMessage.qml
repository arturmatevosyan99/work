import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.3

import Core.System 0.1
import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: childrenRect.width
	contentHeight: childrenRect.height
	bubble: !message.active

	Binding {
		when: message.active
		target: rootView
		property: 'contentWidth'
		value: parent.width
	}

	MessageDialog {
		id: outputDialog

		visible: false
		title: message.success == 0 ? qsTr("Success") : qsTr("Error")
		text: label.font.italic ? message.argument + "\n" + message.output : message.output
		standardButtons: StandardButton.Ok

		onAccepted: {
		}
	}

	model: Model.CommandMessage {
		id: message

		onImported: {
			if (label.text.length > 0) {
				var height = label.height / label.lineCount;
				label.text = "";
				label.height = height;
			}
			label.text = message.active ? message.action : "/" + message.argument
		}
	}

	Label {
		id: label

		anchors.left: rootView.left
		anchors.top: rootView.top
		anchors.margins: 12

		text: "/"
		horizontalAlignment: message.active ? Text.AlignHCenter : Text.AlignLeft
		font.italic: message.active
		wrapMode: Label.Wrap
		color: message.active ? Material.color(Material.Grey) : fontColor

		onContentHeightChanged: {
			height = label.contentHeight;
		}

		onContentWidthChanged: {
			width = message.active ? Math.max(rootView.minimumWidth, label.contentWidth) : Math.min(rootView.minimumWidth, label.contentWidth);
		}

		MouseArea {
			anchors.fill: parent
			onDoubleClicked: {
				if (message.isImported) {
					outputDialog.open();
				}
			}
		}
	}
}
