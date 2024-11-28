import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

import DrOblozhko.Model 0.1

import Core.View 0.1
import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	function commentOut() {
		label.text = message.isDescribed ? "%1 (%2)".arg(message.fileDescription).arg(message.fileName) : message.fileName;
	}

	model: Model.PDFMessage {
		id: message

		onImported: {
			commentOut();
		}

		onCached: {
			console.log("PDFMessage::onCached ", message.cachePath);
			Qt.openUrlExternally(message.cachePath);
		}
	}

	Label {
		id: label
		text: qsTr("PDF Document")
		color: fontColor
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.top: rootView.top
		anchors.bottom: rootView.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12
		wrapMode: Label.Wrap
	}

	Button {
		id: button
		text: qsTr("View")

		flat: true
		down: true
		enabled: PDFMessage.Loading !== message.cacheState

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		onClicked: {
			if (Model.PDFMessage.Cached === message.cacheState) {
				console.log("PDFMessage::onClicked ", message.cachePath);
				Qt.openUrlExternally(message.cachePath);
			} else {
				console.log("PDFMessage::onClicked ", message.fileName);
				message.cache();
			}
		}
	}
}
