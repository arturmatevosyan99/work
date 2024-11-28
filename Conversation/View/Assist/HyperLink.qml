import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.AssistView {
	id: rootView

	width: linkButton.implicitWidth
	height: linkButton.implicitHeight

	model: Model.HyperLink {
		id: assist
	}

	Button {
		id: linkButton
		enabled: assist.isImported

		anchors.fill: rootView

		text: assist.title.length > 0 ? assist.title : qsTr("Go")
		flat: true
		down: true

		onClicked: {
			Qt.openUrlExternally(assist.target);
		}
	}
}
