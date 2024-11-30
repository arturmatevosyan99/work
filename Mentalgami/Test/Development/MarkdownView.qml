import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14
import Mentalgami.Model 0.1

Rectangle {
    id: root
    width: parent.width
    height: parent.height
    color: Material.background

    MarkdownModel {
        id: markdownModel
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 10
        model: markdownModel

        delegate: Loader {
            property string type: model.type
            property string content: model.content
            property var attributes: model.attributes

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

    // Header Delegate
    Component {
        id: headerDelegate
        Label {
            text: content
            font.pointSize: attributes.level == 1 ? 24 : attributes.level == 2 ? 20 : 16
            font.bold: true
            wrapMode: Text.Wrap
            width: parent.width * 0.9
            padding: 10
            color: Material.foreground
        }
    }

    // Paragraph Delegate
    Component {
        id: paragraphDelegate
        Label {
            textFormat: Text.RichText
            text: content
            font.pointSize: 14
            wrapMode: Text.Wrap
            width: parent.width * 0.9
            padding: 10
            color: Material.foreground
        }
    }

    // Link Delegate
    Component {
        id: linkDelegate
        Label {
            text: content
            color: Material.accent
            font.underline: true
            wrapMode: Text.Wrap
            width: parent.width * 0.9
            padding: 10
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: Qt.openUrlExternally(attributes.url)
            }
        }
    }

    // Image Delegate
    Component {
        id: imageDelegate
        Column {
            width: parent.width * 0.9
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                source: attributes.src
                fillMode: Image.PreserveAspectFit
                width: parent.width * 0.9
                height: implicitHeight
            }

            Label {
                text: content // Alt text
                font.pointSize: 12
                color: "gray"
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    // Table Delegate
    Component {
        id: tableDelegate
        Flickable {
            contentWidth: tableContent.implicitWidth
            contentHeight: tableContent.implicitHeight
            width: parent.width * 0.9
            height: tableContent.implicitHeight
            clip: true

            GridLayout {
                id: tableContent
                columns: attributes.columnCount
                rowSpacing: 1
                columnSpacing: 1
                anchors.top: parent.top

                Repeater {
                    model: attributes.cells
                    delegate: Rectangle {
                        width: (tableContent.width - (attributes.columnCount - 1)) / attributes.columnCount
                        height: implicitHeight
                        color: modelData["isHeader"] ? "#f0f0f0" : "white"
                        border.width: 1
                        border.color: "#cccccc"

                        Label {
                            text: modelData["content"]
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            font.bold: modelData["isHeader"]
                            font.pointSize: 14
                            color: Material.foreground
                            padding: 5
                        }
                    }
                }
            }
        }
    }
}
