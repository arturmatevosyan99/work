import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import Core.View 0.1
import Core.System 0.1

import "../../Core/View"

Item {
	id: rootItem

	property alias helpInterval: helpTimer.interval

	Component.onCompleted: {
		helpTimer.start();
	}

	Component.onDestruction: {
		helpTimer.stop();
	}

	Image {
		id: bannerImage

		width: parent.width * 0.7
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter

		source: "../Asset/Image/Banner.png"
		fillMode: Image.PreserveAspectFit
	}


	ProgressBar {
		id: progressBar

		indeterminate: true

		anchors.top: bannerImage.bottom
		anchors.topMargin: 10
		anchors.left: bannerImage.left
		anchors.right: bannerImage.right
	}

	Label {
		text: qsTr("Burning calories...")
		anchors.top: progressBar.bottom
		anchors.topMargin: 10
		anchors.horizontalCenter: parent.horizontalCenter
	}

	ActionButton {
		id: helpButton

		anchors.bottomMargin: 20
		anchors.bottom: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter

		visible: false
		radius: 10
		text: qsTr("Something goes wrong...")

		font.pointSize: 22
		textColor: "white"
		color: "#1a86a1"

		onClicked: {
			Native.alertAction(
								qsTr("Troubleshooting"),
								qsTr("We will be glad to help you"),
								[
//									qsTr("Reach us using WhatsApp"),
//									qsTr("Send us an email"),
									qsTr("Share system info"),
									qsTr("Restart application")
								],
								Native.ActionSheet | Native.ActionCancel
			);
		}
	}

	Component {
		id: diagnosticComponent

		DiagnosticCollector {
		}
	}

	Connections {
		target: Native

		enabled: helpButton.visible

		function onActionAlertDone(index) {
			var body = qsTr("I'm writing you from application(%1/%2) where the spinner is endlesly rotating").arg(Native.platformName).arg(Native.frameworkVersion);
			switch (index) {
				case 0:
					Qt.openUrlExternally("https://wa.me/79385195632?text=" + body);
					break;

				case 1:
					Qt.openUrlExternally("mailto:app@oblozhko.ru?subject=App&body=" + body);
					break;

				case 2:
					rootItem.StackView.view.push(diagnosticComponent, {}, StackView.PushTransition);
					break;

				case 3:
					Qt.quit();
					break;
			}
		}
	}

	Timer {
		id: helpTimer

		running: false
		repeat: false
		interval: 15000

		onTriggered: {
			helpButton.visible = true;
		}
	}
}
