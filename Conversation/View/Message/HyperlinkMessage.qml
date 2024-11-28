import QtQuick 2.15
import QtQuick.Controls 2.15

import Core.System 0.1
import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: label.implicitWidth
	contentHeight: label.implicitHeight + button.implicitHeight

	model: Model.HyperlinkMessage {
		id: message

		onImported: {
			label.text = message.title;
		}
	}

	Label {
		id: label		
		color: fontColor
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.top: rootView.top
		anchors.bottom: button.top
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.topMargin: 12

		wrapMode: Label.Wrap
	}

	Button {
		id: button
		state: message.isHyper ? "active" : "missed"
		flat: true
		anchors.left: rootView.left
		anchors.right: rootView.right
		anchors.bottom: rootView.bottom
		anchors.leftMargin: 12
		anchors.rightMargin: 12
		anchors.bottomMargin: 12

		states: [
					State {
							name: "missed"
							PropertyChanges {
												target: button
												down: false
												text: qsTr("No hyperlink")
												enabled: false
							}
					},
					State {
							name: "active"
							PropertyChanges {
												target: button
												down: true
												text: qsTr("Follow hyperlink")
												enabled: true
							}
					}
		]

		onClicked: {			
			message.browse();
		}
	}
}
