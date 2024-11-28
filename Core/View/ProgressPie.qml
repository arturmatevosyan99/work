import QtQml 2.15
import QtQuick 2.15

/**
  *	@example ProgressPie {
  *		size: 160
  *		colorCircle: "#0092CC"
  *		colorBackground: "#E6E6E6"
  *		arcBegin: 0
  *		arcEnd: 270
  *		lineWidth: 10
  *	}
  */

/**
  *	@example ProgressPie {
  *		size: 160
  *		colorCircle: "#FF3333"
  *		colorBackground: "#E6E6E6"
  *		arcBegin: 0
  *		arcEnd: 220
  *	}
  */

/**
  *	@example ProgressPie {
  *		size: 160
  *		colorCircle: "#779933"
  *		colorBackground: "#E6E6E6"
  *		isPie: true
  *		arcBegin: 90
  *		arcEnd: 200
  *	}
  */

Item {
	id: rootItem

	property alias beginAnimation: animationArcBegin.enabled
	property alias endAnimation: animationArcEnd.enabled

	property int size: 200 /// < The size of the circle in pixel
	property real arcBegin: 0 /// < start arc angle in degree
	property real arcEnd: 270 /// < end arc angle in degree
	property real arcOffset: 0 /// < rotation
	property bool isPie: false /// < paint a pie instead of an arc
	property bool showBackground: false /// < a full circle as a background of the arc
	property real lineWidth: 20 /// < width of the line
	property string colorCircle: "#CC3333"
	property string colorBackground: "#779933"
	property int animationDuration: 200

	width: size
	height: size

	onArcBeginChanged: canvas.requestPaint()
	onArcEndChanged: canvas.requestPaint()

	Behavior on arcBegin {
		id: animationArcBegin

		enabled: true

		NumberAnimation {
			duration: rootItem.animationDuration
			easing.type: Easing.InOutCubic
		}
	}

	Behavior on arcEnd {
		id: animationArcEnd

		enabled: true

		NumberAnimation {
			duration: rootItem.animationDuration
			easing.type: Easing.InOutCubic
		}
	}

	Canvas {
		id: canvas

		anchors.fill: parent

		rotation: -90 + parent.arcOffset

		onPaint: {
			var context = getContext("2d");
			var x = width / 2;
			var y = height / 2;
			var start = Math.PI * (parent.arcBegin / 180);
			var end = Math.PI * (parent.arcEnd / 180);
			context.reset();

			if (rootItem.isPie) {
				if (rootItem.showBackground) {
					context.beginPath();
					context.fillStyle = rootItem.colorBackground;
					context.moveTo(x, y);
					context.arc(x, y, width / 2, 0, Math.PI * 2, false);
					context.lineTo(x, y);
					context.fill();
				}
				context.beginPath();
				context.fillStyle = rootItem.colorCircle;
				context.moveTo(x, y);
				context.arc(x, y, width / 2, start, end, false);
				context.lineTo(x, y);
				context.fill();
			} else {
				if (rootItem.showBackground) {
					context.beginPath();
					context.arc(x, y, (width / 2) - parent.lineWidth / 2, 0, Math.PI * 2, false);
					context.lineWidth = rootItem.lineWidth;
					context.strokeStyle = rootItem.colorBackground;
					context.stroke();
				}
				context.beginPath();
				context.arc(x, y, (width / 2) - parent.lineWidth / 2, start, end, false);
				context.lineWidth = rootItem.lineWidth;
				context.strokeStyle = rootItem.colorCircle;
				context.stroke();
			}
		}
	}
}
