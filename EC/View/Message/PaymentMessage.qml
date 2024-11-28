import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Core.System 0.1
import Conversation.View 0.1 as View
import ERP.Model 0.1 as Model
import EC.Model 0.1 as Model
import ERP.Global 0.1
import EC.Global 0.1

View.MessageView {
	id: rootView

	contentWidth: columnLayout.width
	contentHeight: columnLayout.height

	MessageDialog {
		id: messageDialog

		title: qsTr("Payment processing")
		text: ""

		function popUp(text) {
			messageDialog.text = text;
			messageDialog.open();
		}
	}

	model: Model.PaymentMessage {
		id: message

		onImported: {
			if (!message.isValid) {
				return;
			}
		}

		onFaulted: {
			messageDialog.popUp(message);
			button.state = "issued";
		}

		onCompleted: {
			messageDialog.popUp(success ? qsTr("Order has been paid successfully") : qsTr("Payment has been canceled"));
			if (!success) {
				button.state = "issued";
			}
		}
	}

	ColumnLayout {
		id: columnLayout

		anchors.top: parent.top
		anchors.topMargin: rootView.gap / 2
		anchors.left: parent.left
		anchors.leftMargin: rootView.gap / 2

		spacing: 4

		Label {
			id: documentLabel

			Layout.preferredHeight: implicitHeight
			Layout.preferredWidth: implicitWidth
			Layout.maximumWidth: rootView.minimumWidth

			font.bold: true
			wrapMode: Text.WordWrap
			text: message.document.title
		}

		Repeater {
			id: offerRepeater

			model: message.document.itemList
			delegate: Label {
				Layout.preferredHeight: implicitHeight
				Layout.preferredWidth: implicitWidth
				Layout.maximumWidth: rootView.minimumWidth

				color: fontColor
				textFormat: Text.StyledText
				wrapMode: Label.Wrap
				text: (index+1) + ") " + name + " x " + quantity + " = " + cost + "<br />"
			}
		}

		Button {
			id: button

			Layout.alignment: Qt.AlignCenter
			Layout.preferredWidth: documentLabel.implicitWidth
			Layout.maximumWidth: rootView.minimumWidth
			Layout.fillWidth: true

			enabled: true
			flat: true
			down: true

			state: message.document.state === ERP.Held ? "held" : "issued"
			states: [
						State {
								name: "held"
								PropertyChanges {
													target: button
													text: qsTr("View Invoice")
													enabled: true
								}
						},
						State {
								name: "issued"
								PropertyChanges {
													target: button
													text: qsTr("Pay") + " " + message.document.total
													enabled: true
								}
						},
						State {
								name: "acquired"
								PropertyChanges {
													target: button
													text: "..."
													enabled: false
								}
						}

			]

			onClicked: {
				if ("issued" == state && message.resolveOrder()) {
					button.state = "acquired";
				}
			}
		}
	}
}
