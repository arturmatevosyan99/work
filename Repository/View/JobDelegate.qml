import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.View 0.1
import Repository.Global 0.1

ItemDelegate {
	id: rootView

	signal requested()
	signal scheduled(var success)
	signal launched()
	signal finished(var success)

	property alias iconSource: rootView.icon.source
	property alias iconText: rootView.text
	property bool busy: null !== job ? Repository.IdleJobState === job.state : false
	property var job: null
	onJobChanged: {
		if (job !== null && job.state === Repository.PlanJobState) {
			messageDialog.show(qsTr("Job has been successfully scheduled"));
			rootView.scheduled(true);
			job = null;
		}
	}

	icon.color: "transparent"
	enabled: !busy

	Connections {
		target: rootView.job
		enabled: rootView.job !== null

		function onStateChanged() {
			if (job.state === Repository.PlanJobState) {
				messageDialog.show(qsTr("Job has been successfully scheduled"));
				rootView.scheduled(true);
			} else if (job.state === Repository.FaultJobState) {
				messageDialog.show(qsTr("Job has not been scheduled due to unexpected error"));
				rootView.scheduled(false);
			}
			rootView.job = null;
		}
	}

	MessageDialog {
		id: messageDialog

		title: qsTr("Attention")
		standardButtons: StandardButton.Ok

		function show(message) {
			text = message;
			if (message.length > 0) {
				open();
			}
		}
	}

	background: Rectangle {
		border.width: 0
		color: "white"

		ProgressBar {
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			contentItem.implicitHeight: 1
			indeterminate: rootView.state === busy
		}
	}

	onLaunched: {

	}

	onClicked: {
		if (rootView.job === null) {
			rootView.requested();
		}
	}
}
