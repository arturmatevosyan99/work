import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

import Core.System 0.1
import Conversation.View 0.1 as View
import SmartCare.Model 0.1 as Model

import "../../../Qaos/QtQuick/URL.js" as URLScript

View.MessageView {
	id: rootView

	contentWidth: childrenRect.width
	contentHeight: childrenRect.height

	model: Model.SmartCourseMessage {

	}

	Label {
		id: label

		anchors.left: rootView.left
		anchors.top: rootView.top
		anchors.margins: 12
		width: implicitWidth / rootView.minimumWidth <= 1 ? implicitWidth : rootView.minimumWidth
		onWidthChanged: {
			height = (implicitHeight / lineCount) * Math.min(lineCount, maximumLineCount)
		}
/*
		background: Image {
			anchors.fill: label

			opacity: 0.3
			source: rootView.model.artworkURL ?? ""
			fillMode: Image.PreserveAspectCrop
			horizontalAlignment: Image.AlignHCenter
			verticalAlignment: Image.AlignVCenter
		}
*/
		text: rootView.model.isImported ? (rootView.model.description.length > 0 ? "<b>‎%1</b> %2".arg(rootView.model.name).arg(rootView.model.description) : rootView.model.name) : qsTr("Smart Course")
		textFormat: Text.StyledText
		color: fontColor
		wrapMode: Label.Wrap
		maximumLineCount: 6
		elide: Text.ElideRight
		font.italic: true
	}

	Button {
		id: button

		text: qsTr("Open")

		flat: true
		down: true
		enabled: rootView.model && rootView.model.isImported

		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.top: label.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		onClicked: {
			rootView.layout.popup(
									null,
									{
										"objectId": rootView.model.id,
										"applicationScheme": rootView.model.applicationScheme,
										"attachOption": {"subject_id": rootView.model.subjectId},
										"headerTitle": rootView.model.name
									}
			);
			/// @todo add timer for a couple of seconds
		}
	}
}
