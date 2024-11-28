import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.View 0.1

TextArea {
	id: rootView

	signal edited()

	readonly property bool isModified: privateObject.isEdited

	height: (contentHeight / lineCount) * 3
	text: ""
	placeholderText: ""
	wrapMode: TextEdit.Wrap
	verticalAlignment: TextEdit.AlignTop

	enabled: true

	rightPadding: 4
	leftPadding: 4

	QtObject {
		id: privateObject

		property bool isEdited: false
	}

	onEditingFinished: {
		privateObject.isEdited = true;
		edited();
	}

	background: Rectangle {
		border.color: "transparent"

		Rectangle {
			height: 1
			border.width: 0
			color: "lightgray"
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right
		}
		Rectangle {
			height: 1
			border.width: 0
			color: "lightgray"
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			anchors.right: parent.right
		}
	}
}
