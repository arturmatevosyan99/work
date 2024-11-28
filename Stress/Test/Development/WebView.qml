import QtQml 2.15
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "../../../Core/View/"

Rectangle {
	id: testView

	color: "lightsteelblue"

	ColumnLayout {
		anchors.fill: parent

		RowLayout {
			Layout.maximumHeight: textInput.contentHeight + 20
			Layout.preferredHeight: Layout.maximumHeight
			Layout.fillWidth: true

			TextInput {
				id: textInput

				Layout.fillHeight: true
				Layout.fillWidth: true

				verticalAlignment: TextInput.AlignVCenter

				text: "https://smart.hudeem-online.ru/docs"
			}
			Button {
				id: button

				Layout.fillHeight: true
				Layout.maximumWidth: testView.width / 5
				Layout.preferredWidth: Layout.maximumWidth

				text: "Go"
				onClicked: {
					webView.url = textInput.text;
				}
			}
		}

		WebView {
			id: webView

			Layout.fillHeight: true
			Layout.fillWidth: true
		}
	}
}
