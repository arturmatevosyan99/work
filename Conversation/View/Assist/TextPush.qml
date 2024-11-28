import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.AssistView {
	id: rootView

	width: pushButton.implicitWidth
	height: pushButton.implicitHeight

	model: Model.TextPush {
		id: assist

		onImported: {
			pushButton.text = assist.text();
		}
	}

	Timer {
		id: pusherTimer

		interval: 5000
		running: !pushButton.enabled
		repeat: false

		onTriggered: {
			pushButton.enabled = true;
		}
	}

	Button {
		id: pushButton
		enabled: true

		anchors.fill: rootView

		flat: true
		down: true

		onClicked: {
			assist.channelModel.push(assist.value);
			pushButton.enabled = false;
		}
	}
}
