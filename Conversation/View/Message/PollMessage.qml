import QtQuick 2.15
import QtQuick.Controls 2.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: column.width
	contentHeight: column.height

	onMinimumWidthChanged: {
		if (message.isImported) {
			label.fit();
		}
	}

	model: Model.PollMessage {
		id: message

		onImported: {
		}
	}

	ButtonGroup {
		id: radioGroup

		exclusive: !message.multiplyMode

		onClicked: {
			message.vote(button.objectName, exclusive);
		}
	}

	Column {
		id: column

		anchors.top: rootView.top
		anchors.left: rootView.left
		anchors.margins: 12

		width: label.width

		spacing: 0

		Label {
			id: label

			function fit() {
				wrapMode = Text.NoWrap;
				width = Math.min(contentWidth, rootView.minimumWidth);
				wrapMode = Text.Wrap;
				height = contentHeight;
			}

			text: message.question
			wrapMode: Label.Wrap
			color: fontColor

			onTextChanged: {
				fit();
			}
		}

		Repeater {
			model: message.optionCount

			RadioButton {
				enabled: message.votable
				ButtonGroup.group: radioGroup

				text: message.describe(index)
				objectName: message.opt(index)
				checked: message.check(objectName)
			}
		}

		Button {
			text: qsTr("Vote")
			visible: !radioGroup.exclusive
			enabled: message.votable
			flat: true
			down: true
			anchors.left: column.left
			anchors.right: column.right
			anchors.leftMargin: 12
			anchors.rightMargin: 12
			anchors.bottomMargin: 12

			onClicked: {
				message.sync();
			}
		}
	}
}
