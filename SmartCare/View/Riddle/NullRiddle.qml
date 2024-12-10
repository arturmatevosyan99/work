import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import SmartCare.View 0.1 as View
import SmartCare.Model 0.1 as Model
import SmartCare.Meta 0.1

View.RiddleView {
	id: rootView

	focus: true

	model: Model.NullRiddle {

	}

	Image {
		id: emojiImage

		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		anchors.margins: 25

		fillMode: Image.PreserveAspectFit
		anchors.verticalCenter: parent.verticalCenter

		source: rootView.model.imageSource
	}
}
