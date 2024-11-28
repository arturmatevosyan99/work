import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.AssistView {
	id: rootView

	height: columnView.childrenRect.height
	width: layoutWidth

	model: Model.VerticalLayout {
		id: assist

		onImported: {
			assistRepeater.model = items;
		}
	}

	Column {
		id: columnView

		anchors.top: rootView.top
		anchors.left: rootView.left
		anchors.margins: 1

		spacing: 1

		width: rootView.width

		Repeater {
			id: assistRepeater

			delegate: View.AssistLayout {
				anchors.horizontalCenter: columnView.horizontalCenter
				anchors.margins: 0

				height: implicitHeight
				width: implicitWidth

				Component.onCompleted: {
					render(scheme);
				}

				onRendered: {
					if (hasModel && model.import(assist.channelModel, uid, body, affectedSubjectQdList, targetLayout)) {
						visible = true;
					}
				}
			}
		}
	}
}
