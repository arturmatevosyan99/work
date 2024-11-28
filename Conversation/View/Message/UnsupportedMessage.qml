import QtQuick 2.15
import QtQuick.Controls 2.15

import Core.View 0.1

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight

	onLayoutChanged: {
		label.text = qsTr("This type of message is unsupported, please check if application is up to date.");
	}

	Label {
		id: label

		anchors.fill: parent
		anchors.margins: 12

		text: "{...}"
		font: Theme.body1Font
		color: fontColor
		linkColor: fontColor
		wrapMode: Label.Wrap
	}
}
