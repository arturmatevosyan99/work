import QtQuick 2.15

Item {
    id: imageBlock
    property url source

    Image {
        source: imageBlock.source
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }
}
