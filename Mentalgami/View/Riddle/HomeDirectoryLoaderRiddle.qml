import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import Mentalgami.View 0.1
import Mentalgami.Meta 0.1

RiddleView {
	id: rootView

	focus: true

	BusyIndicator {
		id: busyIndicator

		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		focus: true
		running: true
	}
}
