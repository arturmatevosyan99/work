import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Conversation.View 0.1 as View
import Mentalgami.Model 0.1 as Model

import "../../../Qaos/QtQuick/URL.js" as URLScript

View.MessageView {
	id: rootView

	contentHeight: button.implicitHeight
	contentWidth: !layout ? (button.implicitWidth + label.implicitWidth) : layout.viewMinimumWidth
	bubble: !layout ? false : layout.flags == View.MessageLayout.OddIndex ? true : false
	gap: 0

	model: Model.SmartCourseMessage {
	}

	function popup()
	{
		if (!model.isImported) {
			return false;
		}

		rootView.layout.popup(
								null,
								{
									"objectId": rootView.model.id,
									"applicationScheme": rootView.model.applicationScheme,
									"attachOption": {"subject_id": rootView.model.subjectId},
									"headerTitle": rootView.model.name
								}
		);
		return true;
	}

	Button {
		id: button

		anchors.left: rootView.left
		anchors.leftMargin: 4
		anchors.verticalCenter: rootView.verticalCenter

		width: rootView.contentWidth / 10

		flat: true
		down: !rootView.bubble
		enabled: rootView.model.isImported
		display: AbstractButton.IconOnly

		icon.source: "/Mentalgami/Asset/Image/List3.svg"
//		icon.height: implicitHeight * 0.91
//		icon.width: rootView.contentWidth / 10
		icon.color: Material.accent

		onClicked: {
			rootView.popup();
			/// @todo add timer for a couple of seconds
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
		text: rootView.model.name ?? qsTr("Smart Course")

		MouseArea {
			anchors.fill: parent

			onClicked: {
				rootView.popup();
				/// @todo add timer for a couple of seconds
			}
		}
	}
}
