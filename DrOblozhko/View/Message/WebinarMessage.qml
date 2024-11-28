import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

import Core.View 0.1
import DrOblozhko.Model 0.1 as Model
import Conversation.View 0.1 as View

import "../../../Core/View"

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	MessageDialog {
		id: messageDialog

		title: "Ох!"
		text: ""
	}

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
			label.text = "<i>" + rootView.model.description + "</i>" + " (" + qsTr("WebinarHost") + ": <b>" + rootView.model.hostName + "</b>, " + qsTr("Live at") + ": " + new Date(rootView.model.broadcastDateTime).toLocaleString(Qt.locale(), "ddd MMMM d yy H:m") + ")";
		}

		onDenied: {
			messageDialog.text = qsTr("Webinar is not available for you by that time, feel free to contact us if you believe this is in error");
			messageDialog.open();
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
				messageDialog.text = qsTr("Unexpected error has been occurred. Feel free to contact us.");
				messageDialog.open();
			}
		}
	}

	Label {
		id: label

		text: qsTr("Webinar")
		color: fontColor
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.top: rootView.top
		anchors.bottom: button.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12
		wrapMode: Label.Wrap
		textFormat: Text.StyledText
	}

	Button {
		id: button

		enabled: rootView.model.active
		flat: true
		down: true
		text: qsTr("Go")

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		onClicked: {
			rootView.model.sign();
		}
	}
}
