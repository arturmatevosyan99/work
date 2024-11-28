import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Conversation.View 0.1 as View
import DrOblozhko.Model 0.1 as Model

import Core.System 0.1
import "../../../Qaos/QtQuick/URL.js" as URLScript

View.MessageView {
	id: rootView

	contentHeight: button.implicitHeight
	contentWidth: !layout ? (button.implicitWidth + label.implicitWidth) : layout.viewMinimumWidth
	bubble: !layout ? false : layout.flags == View.MessageLayout.OddIndex ? true : false
	gap: 0

	model: Model.InteractiveApplicationMessage {
		onImported: {
			label.text = rootView.model.description;
		}

		onDenied: {
			Native.alertAction(qsTr("Hm..."), qsTr("Application is not available for you by that time, feel free to contact us if you believe this is in error"), ["Ok"]);
		}

		onAccessed: {
			var params_object = URLScript.makeURLParamsObject(url);
			if (!params_object.object_id || !params_object.application_scheme) {
				Native.alertAction(qsTr("Hm..."), qsTr("Unexpected error has been occurred. Feel free to contact us."), ["Ok"]);
			} else {
				layout.popup(
								null,
								{
									"objectId": params_object.object_id,
									"applicationScheme": params_object.application_scheme,
									"headerTitle": rootView.model.name
								}
				);
			}
		}
	}

	Button {
		id: button

		anchors.left: rootView.left
		anchors.leftMargin: 4
		anchors.verticalCenter: rootView.verticalCenter

		width: rootView.contentWidth / 10

		flat: true
		down: !rootView.bubble
		enabled: rootView.model.active
		display: AbstractButton.IconOnly

		icon.source: "/DrOblozhko/Asset/Image/InteractiveApplication.svg"
//		icon.height: implicitHeight * 0.91
//		icon.width: rootView.contentWidth / 10
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
		text: qsTr("Application")

		MouseArea {
			anchors.fill: parent

			onClicked: {
				rootView.model.sign();
			}
		}
	}
}
