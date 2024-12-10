import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Core.View 0.1
import SmartCare.View 0.1 as View

View.TrackView {
	id: rootView

	height: descriptionLabel.implicitHeight

	Image {
		id: iconImage

		anchors.top: parent.top
		anchors.left: parent.left

		height: descriptionLabel.height
		sourceSize.width: height

		source: "/SmartCare/Asset/Image/CloudLink.svg"
		fillMode: Image.PreserveAspectFit
		verticalAlignment: Image.AlignVCenter
		horizontalAlignment: Image.AlignHCenter
	}

	Label {
		id: descriptionLabel

		anchors.top: parent.top
		anchors.left: iconImage.right
		anchors.leftMargin: 2
		anchors.right: parent.right
		anchors.bottom: parent.bottom
//		anchors.rightMargin: 2

		text: qsTr("Unknown track")
		verticalAlignment: Text.AlignHCenter
		horizontalAlignment: Text.AlignVCenter
		elide: Text.ElideRight

		font.pointSize: Theme.defaultFont.pointSize * 0.7
		font.weight: Font.ExtraLight
		color: "darkgray"
	}
}
