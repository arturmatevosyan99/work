import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.3

import Core.System 0.1
import Conversation.View 0.1 as ConversationView
import SmartCare.View 0.1 as SmartCareView
import SmartCare.Model 0.1 as SmartCareModel

ConversationView.MessageView {
	id: rootView
	//"The achievement badge has been awarded"
	//"Награда за достижение вручена"
	contentWidth: rootView.minimumWidth
	contentHeight: image.implicitHeight + label.implicitHeight
	bubble: false

	model: SmartCareModel.AwardMessage {
		onImported: {
			moji.load("award", crn);
		}
	}

	SmartCareView.Moji {
		id: moji

		url: "qrc:/SmartCare/Asset/Image/LoadingAward.svg"
		onLoaded: {
			if (!success) {
				set("qrc:/SmartCare/Asset/Image/UnknownAward.svg", qsTr("Unknown award"), 127989/*🏵️*/);
			}
		}
	}

	MessageDialog {
		id: dialog

		visible: false
		standardButtons: StandardButton.Ok

		title: qsTr("Motivation")
		text: rootView.model.motive
	}

	Image {
		id: image

		anchors.top: rootView.top
		anchors.left: rootView.left
		anchors.right: rootView.right

		source: moji.url
		sourceSize.height: rootView.minimumHeight / 2.4

		fillMode: Image.PreserveAspectFit
		verticalAlignment: Image.AlignVCenter
		horizontalAlignment: Image.AlignHCenter

		MouseArea {
			anchors.fill: parent
			onDoubleClicked: {
				if (rootView.model.isImported && rootView.model.motive.length > 0) {
					dialog.open();
				}
			}
		}
	}
	Label {
		id: label

		anchors.top: image.bottom
		anchors.left: rootView.left
		anchors.right: rootView.right

		text: moji.meaning
		font.italic: false
		wrapMode: Label.Wrap
		color: Material.color(Material.Grey)
		horizontalAlignment: Text.AlignHCenter
	}
}
