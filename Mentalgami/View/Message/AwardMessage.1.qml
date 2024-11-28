import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Core.View 0.1
import Conversation.View 0.1 as ConversationView
import Mentalgami.View 0.1 as MentalgamiView
import Mentalgami.Model 0.1 as MentalgamiModel

ConversationView.MessageView {
	id: rootView

	contentHeight: layout.viewMinimumHeight
	contentWidth: layout.viewMinimumWidth
	bubble: false
	gap: 0

	model: MentalgamiModel.AwardMessage {
		onImported: {
			moji.load("award", crn);
		}
	}

	MentalgamiView.Moji {
		id: moji

		url: "qrc:/Mentalgami/Asset/Image/Award.svg"
		meaning: qsTr("Unknown award")
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
		anchors.bottom: label.top
		anchors.margins: 2

		source: moji.url

		fillMode: Image.PreserveAspectFit
		verticalAlignment: Image.AlignBottom
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

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom

		text: moji.meaning
		wrapMode: Label.Wrap
		color: Material.color(Material.Grey)
		horizontalAlignment: Text.AlignHCenter
		font.pointSize: Theme.defaultFont.pointSize * 0.5
		font.weight: Font.ExtraLight
	}
}
