import QtQml 2.15
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Core.System 0.1

Rectangle {
	id: testView
	objectName: "Blocked alert subsystem"

	InputAlert {
		id: inputAlert
		objectName: "INPUT_ALERT"

		title: "Test Input Box"
		placeholder: "Something"
		flags: InputAlert.CancelOption | InputAlert.HeadFull

		onCompleted: {

		}

		onCanceled: {
			actionAlert.warn();
		}
	}

	ActionAlert {
		id: actionAlert
		objectName: "ACTION_ALERT"

		title: "Test Action Message"
		text: "Something"
		flags: ActionAlert.HeadFull

		AlertOption {
			caption: "Ok!"
			flag: AlertOption.Ok
		}
	}

	Button {
		anchors.centerIn: parent

		text: "Action!"

		onClicked: {
			inputAlert.warn();
		}
	}
}
