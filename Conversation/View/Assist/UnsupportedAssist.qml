import QtQuick 2.15
import QtQuick.Controls 2.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.AssistView {
	id: rootView

	width: label.contentWidth
	height: label.contentHeight

	Label {
		id: label

		text: qsTr("This type of assistant is unsupported, please check if application is up to date.")

		anchors.margins: 12

		wrapMode: Label.Wrap
	}
}
