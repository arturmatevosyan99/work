import QtQuick 2.15

Item {
    id: textBlock
    property string text

    Text {
        text: textBlock.text
        anchors.fill: parent
    }
}
