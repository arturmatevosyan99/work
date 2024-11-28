import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Core.View 0.1
import DrOblozhko.Model 0.1 as Model
import Conversation.View 0.1 as View

import "../../../Core/View"

View.MessageView {
	id: rootView

	contentHeight: button.implicitHeight
	contentWidth: !layout ? (button.implicitWidth + label.implicitWidth) : layout.viewMinimumWidth
	bubble: !layout ? false : layout.flags == View.MessageLayout.OddIndex ? true : false
	gap: 0

	Component {
		id: multimediaPlayerComponent

		MultimediaPlayer {
			id: player

			Component.onCompleted: {
			}

			onStopped: {
				rootView.model.progress = playback_progress;
			}
		}
	}

	model: Model.VideoLessonMessage {
		onImported: {
			label.text = rootView.model.description;
		}

		onDenied: {
			Native.alertAction(qsTr("Hm..."), qsTr("Lesson is not available for you by that time, please check your internet connection"), ["Ok"]);
		}

		onAccessed: {
			var target = url.toString();
			if (target.startsWith("http")) {
				if (target.endsWith(".mp4") || target.endsWith(".avi") || target.endsWith(".mpeg") || target.endsWith(".m3u8")) {
					layout.popup(
									multimediaPlayerComponent,
									{
										source: url,
										title: rootView.model.description
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
									title: rootView.model.description
								}
				);
			} else if (target.length > 0) {
				Native.alertAction(qsTr("Oh!"), qsTr("Unexpected error has been occurred. Feel free to contact us."), ["Ok"]);
			}
		}
	}

	RoundButton {
		id: button

		property real size: implicitHeight / 2

		anchors.left: rootView.left
		anchors.leftMargin: 4
		anchors.verticalCenter: rootView.verticalCenter

		padding: 0
		topInset: 1
		bottomInset: 1
		flat: true
		down: !rootView.bubble
		enabled: rootView.model.active

		icon.source: "/Core/Asset/Image/Play.svg"
		icon.height: size
		icon.width: size
		icon.color: Material.accent

		onClicked: {
			rootView.model.sign();
		}
	}

	Label {
		id: label

		anchors.left: button.right
		anchors.right: rootView.right
		anchors.top: rootView.top
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 4

		elide: Text.ElideRight
		wrapMode: Label.Wrap
		color: fontColor
		verticalAlignment: Text.AlignVCenter
		text: qsTr("Video lesson")

		MouseArea {
			anchors.fill: parent

			enabled: rootView.model.active

			onClicked: {
				rootView.model.sign();
			}
		}
	}
}
