import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.AssistView {
	id: rootView

	height: listView.contentItem.childrenRect.height
	width: layoutWidth

	model: Model.CarouselLayout {
		id: assist

		onImported: {
			listView.model = items;
		}
	}

	ListView {
		id: listView

		anchors.fill: rootView
		anchors.margins: 1

		orientation: ListView.Horizontal
		spacing: 2
		clip: true

		headerPositioning: ListView.PullBackHeader
		header: Label {
			text: "→"
			height: listView.height
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			font.pointSize: 10
			color: "lightgrey"
			padding: 10
		}

		footerPositioning: ListView.PullBackFooter
		footer: Label {
			text: "←"
			height: listView.height
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			font.pointSize: 10
			color: "lightgrey"
			padding: 10
		}

		delegate: View.AssistLayout {
			height: implicitHeight
			width: implicitWidth

			Component.onCompleted: {
				render(scheme);
			}

			onRendered: {
				if (hasModel) {
					model.import(assist.channelModel, uid, body, affectedSubjectQdList, targetLayout);
				}
			}
		}
	}
}
