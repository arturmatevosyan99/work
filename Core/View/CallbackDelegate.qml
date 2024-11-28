import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.View 0.1

ItemDelegate {
	id: rootView

	signal called();

	height: implicitContentHeight * 2

	property alias iconText: rootView.text
	property alias iconSource: rootView.icon.source
	property bool callable: true

	text: qsTr("Call")
	icon.source: iconSource ? iconSource : ""
	icon.color: "transparent"

	onClicked: {
		if (callable) {
			called();
		}
	}

	background: Rectangle {
		border.width: 0
		color: "white"

		Rectangle {
			height: 1
			border.width: 0
			color: Material.accent
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			anchors.right: parent.right
		}
	}
}
