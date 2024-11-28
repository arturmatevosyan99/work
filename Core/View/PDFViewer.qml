import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

import Core.View 0.1
import Core.Model 0.1
import Core.System 0.1

Rectangle {
	id: rootView

	color: "black"

	BusyIndicator {
		id: busyIndicator
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		running: false
	}
}
