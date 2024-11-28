import QtQml 2.15
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Core.View 0.1
import Core.System 0.1

Rectangle {
	id: testView

	Label {
		id: statusBarLabel

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
	}

	Label {
		id: topPaddingLabel

		anchors.top: statusBarLabel.bottom
		anchors.left: parent.left
		anchors.right: parent.right
	}

	Label {
		id: topInsetLabel

		anchors.top: topPaddingLabel.bottom
		anchors.left: parent.left
		anchors.right: parent.right
	}

	Button {
		anchors.bottom: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter

		text: "Refresh"

		onClicked: {
			statusBarLabel.text = "Statusbar height: " + StatusBar.height;
			topPaddingLabel.text = "Top padding: " + WindowManager.topSafePadding;
			topInsetLabel.text = "Top inset: " + WindowManager.topSafeInset;
		}
	}
}
