import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

import Core.System 0.1
import Core.View 0.1
import DrOblozhko.Model 0.1 as Model
import Conversation.View 0.1 as View

import "../../../Core/View"

View.MessageView {
	id: rootView

	contentHeight: Math.max(label.implicitHeight, image.implicitHeight) + 8
	contentWidth: !layout ? (label.implicitWidth + image.implicitWidth) : layout.viewMinimumWidth
	gap: 0

	Component {
		id: multimediaPlayerComponent

		MultimediaPlayer {
			id: player

			Component.onCompleted: {
			}
		}
	}

	model: Model.WebinarMessage {
		onImported: {
			label.text = "<i>" + rootView.model.description + "</i><br /><br /><b>" + rootView.model.hostName + "</b><br />" + new Date(rootView.model.broadcastDateTime).toLocaleString(Qt.locale(), "ddd MMMM d yy H:m");
		}

		onDenied: {
			Native.alertAction(qsTr("Hm..."), qsTr("Webinar is not available for you by that time, feel free to contact us if you believe this is in error"), ["Ok"]);
		}

		onAccessed: {
			var target = url.toString();
			if (target.startsWith("http")) {
				if (target.endsWith(".mp4") || target.endsWith(".avi") || target.endsWith(".mpeg") || target.endsWith(".m3u8")) {
					layout.popup(
									multimediaPlayerComponent,
									{
										source: url,
										title: rootView.model.name
									}
					);
				} else {
					Qt.openUrlExternally(url)
				}
			} else if (target.startsWith("cube://") || target.startsWith("youtube://")) {
				layout.popup(
								multimediaPlayerComponent,
								{
									source: url,
									title: rootView.model.name
								}
				);
			} else if (target.length > 0) {
				Native.alertAction(qsTr("Oh!"), qsTr("Unexpected error has been occurred. Feel free to contact us"), ["Ok"]);
			}
		}
	}

	MouseArea {
		id: area

		anchors.fill: parent

		enabled: rootView.model.active

		Image {
			id: image

			anchors.left: parent.left
			anchors.top: parent.top
			anchors.margins: 4

			width: rootView.contentWidth / 3

			fillMode: Image.PreserveAspectCrop
			source: "/Conversation/Asset/Image/YoutubeMQDefaultThumbnail.jpg"
		}

		Label {
			id: label

			anchors.left: image.right
			anchors.right: parent.right
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.margins: 4

			color: fontColor
			text: qsTr("Webinar")
			textFormat: Text.StyledText
			wrapMode: Label.Wrap
		}

		onClicked: {
			rootView.model.sign();
		}
	}
}
