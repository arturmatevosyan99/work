import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Core.View 0.1
import Mentalgami.Global 0.1
import Mentalgami.View 0.1 as View
import Mentalgami.Model 0.1 as Model

import "../../../Core/View"
import "../../../Qaos/QtQuick/DateTime.js" as DateTimeScript

View.TrackView {
	id: rootView

	height: descriptionLabel.implicitHeight

	model: Model.VideoTrack {
	}

	Image {
		id: iconImage

		anchors.top: parent.top
		anchors.left: parent.left

		height: descriptionLabel.height
		sourceSize.width: height

		source: "/Mentalgami/Asset/Image/Movie.svg"
		fillMode: Image.PreserveAspectFit
		verticalAlignment: Image.AlignVCenter
		horizontalAlignment: Image.AlignHCenter
	}

	ProgressCircle {
		id: progressCircle

		anchors.top: parent.top
		anchors.left: iconImage.right
		anchors.leftMargin: 2

		height: descriptionLabel.height
		width: descriptionLabel.height

		angle: Math.floor(rootView.model.actualProgress > 1 ? ((rootView.model.actualProgress * 360) / rootView.model.contentSize) : (rootView.model.actualProgress * 360))

		backgroundColor: rootView.model.isSynchronized ? "transparent" : "white"
		borderActiveColor: Material.color(Material.Blue, Material.Shade900)
		borderInactiveColor: Material.color(Material.Blue, Material.Shade100)
	}

	Label {
		id: descriptionLabel

		anchors.top: parent.top
		anchors.left: progressCircle.right
		anchors.leftMargin: 2
		anchors.right: parent.right
		anchors.bottom: parent.bottom
//		anchors.rightMargin: 2

		text: DateTimeScript.formatTimePoint(rootView.model.contentSize * 1000)
		verticalAlignment: Text.AlignHCenter
		horizontalAlignment: Text.AlignVCenter
		elide: Text.ElideRight

		font.pointSize: Theme.defaultFont.pointSize * 0.7
		font.weight: Font.ExtraLight
		color: "darkgray"
	}
}