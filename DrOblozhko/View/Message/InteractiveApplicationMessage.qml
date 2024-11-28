import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

import Conversation.View 0.1 as View
import DrOblozhko.Model 0.1 as Model

import Core.System 0.1
import "../../../Qaos/QtQuick/URL.js" as URLScript

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

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

	Label {
		id: label

		text: qsTr("Interactive Application")
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

		text: qsTr("Open")

		flat: true
		down: true
		enabled: rootView.model.active

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
