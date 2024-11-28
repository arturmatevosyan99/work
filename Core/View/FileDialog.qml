import QtQuick 2.15
import QtQuick.Dialogs 1.3

import Core.System 0.1

Item {
	id: rootView

	clip: true

	Component.onCompleted: {
		console.log("FileDialog::onCompleted");		
	}

	Component.onDestruction: {
		console.log("FileDialog::onDesctruction");
		nativeDialog.close();
	}

	property alias title: nativeDialog.title
	property alias folder: nativeDialog.folder

	signal picked(url filename)
	signal discarded()

	function show() {
		nativeDialog.open();
	}

	function hide() {
		nativeDialog.close();
	}

	FileDialog {
		id: nativeDialog

		title: qsTr("Please choose a file")
		visible: false

		onAccepted: {
			var filename = Qt.resolvedUrl(nativeDialog.fileUrls[0]);
			picked(filename);
		}
		onRejected: {
			discarded();
		}
	}
}
