import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.15

Button {
	id: rootButton

	property alias textColor: contentText.color
	property alias color: backgroundRectangle.color
	property alias radius: backgroundRectangle.radius
	property double progress: 0
	property double implicitProgress: 0

	PropertyAnimation {
		loops: 1
		alwaysRunToEnd: false
		running: rootButton.progress < 0
		target: rootButton
		property: "implicitProgress"
		from: 0.0001
		to: 0.9999
		duration: 1200
		easing.type: Easing.InQuad
		onFinished: {
			[to, from] = [from, to];
			running = rootButton.progress < 0;
		}
	}

	function click() {
		if (enabled) {
			forceActiveFocus();
			Qt.callLater(rootButton.clicked);
		}
	}

	contentItem: Text {
		id: contentText

		anchors.fill: parent
		text: parent.text
		opacity: parent.enabled ? 1.0 : 0.5
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		font.capitalization: Font.MixedCase
		font.pointSize: parent.font.pointSize
	}

	background: Rectangle {
		id: backgroundRectangle

		anchors.fill: parent
		opacity: parent.pressed ? 0.5 : 1.0

		gradient: Gradient {
			orientation: Gradient.Horizontal
			GradientStop {
				position: 0
				color: rootButton.color
			}
			GradientStop {
				position: rootButton.progress >= 0 ? rootButton.progress : rootButton.implicitProgress
				color: Qt.lighter(rootButton.color, 1.15)
			}
			GradientStop {
				position: 1
				color: rootButton.color
			}
		}
	}
}

