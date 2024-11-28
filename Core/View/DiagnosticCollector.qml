import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.3

import Core.View 0.1
import Core.System 0.1

Item {
	id: rootView

	Component.onCompleted: {
		/// @xxx Buggy
//		Native.checkApplicationUpdate();
	}

	MessageDialog {
		id: updateDialog

		visible: false
		title: qsTr("Update is available")
		text: qsTr("Would you like to update application?")
		standardButtons: StandardButton.No | StandardButton.Yes

		onYes: {
			Native.runApplicationUpdate();
		}
	}

	Connections {
		target: Native

		function onApplicationUpdated(ready) {
			if (ready) {
				updateDialog.open();
			}
		}
	}

	DiagnosticBundle {
		id: diagnosticBundle

		onCollected: {
			rootView.state = "upload";
			upload();
		}

		onLoaded: {
			uploadIndicator.value = progress;
			if (progress >= 1) {
				state = "complete";
			}
		}

		onFaulted: {
			state = "fault";
			stateLabel.text = error;
		}
	}

	BusyIndicator {
		id: collectIndicator

		width: 127
		height: 115
		anchors.horizontalCenter: rootView.horizontalCenter
		anchors.verticalCenter: rootView.verticalCenter
		running: false
	}

	ProgressBar {
		id: uploadIndicator

		anchors.top: collectIndicator.bottom
		anchors.topMargin: 5
		anchors.horizontalCenter: rootView.horizontalCenter

		value: 0
		padding: 2
	}

	Label {
		id: stateLabel

		anchors.top: uploadIndicator.bottom
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.margins: 5

		height: contentHeight

		wrapMode: Text.WordWrap
		horizontalAlignment: Text.AlignHCenter
	}

	RoundButton {
		id: actionButton

		anchors.top: stateLabel.bottom
		anchors.horizontalCenter: rootView.horizontalCenter
		anchors.margins: 40

		onClicked: {
			if (rootView.state === "idle") {
				rootView.state = "collect";
				Qt.callLater(diagnosticBundle.collect);
			} else {
				rootView.StackView.view.pop(StackView.PopTransition);
			}
		}
	}



	Label {
		id: closeLabel

		enabled: (rootView.state !== "upload" && rootView.state !== "collect")

		anchors.bottom: versionLabel.top
		anchors.horizontalCenter: versionLabel.horizontalCenter

		height: contentHeight
		width: contentWidth

		text: qsTr("Close")

		horizontalAlignment: Text.AlignRight

		MouseArea {
			anchors.fill: parent
			enabled: parent.enabled

			onClicked: {
				rootView.StackView.view.pop(StackView.PopTransition);
			}
		}
	}
	Label {
		id: versionLabel

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.bottomMargin: WindowManager.safeAreaInset.bottom + 4

		color: "gray"
		text: Native.frameworkVersion

		horizontalAlignment: Text.AlignHCenter
	}

	state: "idle"
	states: [
		State {
			name: "idle"
			PropertyChanges {
				target: collectIndicator

				visible: false
				running: false
			}

			PropertyChanges {
				target: stateLabel

				visible: true
				text: qsTr("Please, press the button to share system information. This will help us improve the application.")
			}

			PropertyChanges {
				target: uploadIndicator

				visible: false
				value: 0
			}

			PropertyChanges {
				target: actionButton

				visible: true
				text: "Ok"
			}
		},
		State {
			name: "collect"
			PropertyChanges {
				target: collectIndicator

				visible: true
				running: true
			}

			PropertyChanges {
				target: stateLabel

				visible: true
				text: qsTr("Collecting diagnostic data...")
			}

			PropertyChanges {
				target: uploadIndicator

				visible: true
				indeterminate: true
				value: 0
			}

			PropertyChanges {
				target: actionButton

				visible: false
			}
		},
		State {
			name: "upload"
			PropertyChanges {
				target: collectIndicator

				visible: false
				running: false
			}

			PropertyChanges {
				target: stateLabel

				visible: true
				text: qsTr("Upload diagnostic data...")
			}

			PropertyChanges {
				target: uploadIndicator

				indeterminate: false
				visible: true
				value: 0
			}

			PropertyChanges {
				target: actionButton

				visible: false
			}
		},
		State {
			name: "fault"
			PropertyChanges {
				target: collectIndicator

				visible: false
				running: false
			}

			PropertyChanges {
				target: stateLabel

				visible: true
				text: qsTr("Unexpected error, please try again later")
			}

			PropertyChanges {
				target: uploadIndicator

				visible: false
				value: 0
			}

			PropertyChanges {
				target: actionButton

				visible: true
				text: "\u2713"
			}
		},
		State {
			name: "complete"
			PropertyChanges {
				target: collectIndicator

				visible: false
				running: false
			}

			PropertyChanges {
				target: stateLabel

				visible: true
				text: qsTr("Succesfully completed!")
			}

			PropertyChanges {
				target: uploadIndicator

				visible: false
				value: 0
			}

			PropertyChanges {
				target: actionButton

				visible: true
				text: "\u2713"
			}
		}

	]
}
