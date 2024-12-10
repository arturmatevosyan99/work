import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14

ListView {
	id: listView

	Component {
		id: headerDelegate
		Label {
			text: content
			font.pointSize: attributes.level === 1 ? 24 : attributes.level === 2 ? 20 : 16
			font.bold: true
			wrapMode: Text.Wrap
			width: listView.width * 0.9
			padding: 10
			color: Material.foreground
		}
	}

	Component {
		id: paragraphDelegate
		Label {
			textFormat: Text.RichText
			text: content
			font.pointSize: 14
			wrapMode: Text.Wrap
			width: listView.width * 0.9
			padding: 10
			color: Material.foreground
		}
	}

	Component {
			id: linkDelegate
			Label {
				text: content
				color: Material.accent
				font.underline: true
				wrapMode: Text.Wrap
				width: listView.width * 0.9
				padding: 10

				MouseArea {
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: {
						console.log(markdownDocument.model)
						markdownDocument.model.setPath(attributes.url)
					}
				}
			}
		}

	Component {
		id: imageDelegate
		Column {
			width: listView.width * 0.9
			spacing: 5
			anchors.horizontalCenter: parent.horizontalCenter
			Image {
				source: attributes.src
				fillMode: Image.PreserveAspectFit
				width: listView.width * 0.9
				height: implicitHeight
			}
			Label {
				text: content // Alt text
				font.pointSize: 12
				color: "gray"
				horizontalAlignment: Text.AlignHCenter
				wrapMode: Text.Wrap
				width: listView.width * 0.9
			}
		}
	}

	Component {
		id: tableDelegate
		Flickable {
			width: listView.width * 0.9
			height: tableContent.implicitHeight
			contentWidth: tableContent.width
			contentHeight: tableContent.height
			clip: true

			GridLayout {
				id: tableContent
				columns: attributes.columnCount
				rowSpacing: 1
				columnSpacing: 1
				width: parent.width

				Repeater {
					model: attributes.rows
					delegate: Repeater {
						model: modelData.cells
						delegate: Rectangle {
							width: (parent.width - ((attributes.columnCount - 1) * tableContent.columnSpacing)) / attributes.columnCount
							height: label.implicitHeight + 10  // Динамическая высота
							color: modelData.isHeader ? "#f0f0f0" : "white"
							border.width: 1
							border.color: "#cccccc"
							Label {
								id: label
								text: modelData.content
								wrapMode: Text.WordWrap
								width: parent.width
								padding: 5
								horizontalAlignment: Text.AlignHCenter
								verticalAlignment: Text.AlignVCenter
								font.bold: modelData.isHeader
								font.pointSize: 14
								color: Material.foreground
							}
						}
					}
				}
			}
		}
	}

	anchors.fill: parent
	spacing: 10
	delegate: Loader {
		property string type: model.type
		property string content: model.content
		property var attributes: model.attributes
		Component.onCompleted: {
			console.log("Delegate type:", type)
			console.log("Attributes:", JSON.stringify(attributes))
		}
		sourceComponent: type === "header" ? headerDelegate
						: type === "table" ? tableDelegate
						: type === "link" ? linkDelegate
						: type === "image" ? imageDelegate
						: paragraphDelegate
	}
	ScrollBar.vertical: ScrollBar {
		policy: ScrollBar.AsNeeded
	}
}
