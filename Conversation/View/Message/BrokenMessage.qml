import QtQuick 2.15
import QtQuick.Controls 2.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight

	onLayoutChanged: {
		label.text = qsTr("😵 This type of message cannot be rendered beacause of unexpected bug.");
	}

	Label {
		id: label
		anchors.centerIn: parent
		color: fontColor
		anchors.fill: parent
		anchors.margins: 12
		wrapMode: Label.Wrap
	}
}
