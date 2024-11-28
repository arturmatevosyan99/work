import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.12

Popup {
	id: rootView

	enum Flag {
		None = 0,
		Silence = 1,
		Block = 2,
		Maximum = 4,
		AutoMute = 8,
		AutoClose = 16,
		NoAction = 32
	}

	signal confirmed()

	property alias action: actionButton.text
	property alias message: messageText.text
	property int flags: PopupAlert.Flag.None

	onFlagsChanged: {
		if ((rootView.flags & PopupAlert.Flag.Silence) === PopupAlert.Flag.Silence) {
			rootView.hide();
		}
	}

	function show(message) {
		rootView.message = message;
		if (!rootView.visible && (rootView.flags & PopupAlert.Flag.Silence) !== PopupAlert.Flag.Silence) {
			rootView.open();
		}
	}

	function hide() {
		if (rootView.visible) {
			rootView.close();
		}
	}

	y: 0
	x: Math.round((parent.width - width) / 2)
	width: parent.width
	height: parent.height
	parent: Overlay.overlay
	background: Rectangle {
		anchors.fill: parent
		color: "black"
		opacity: 0.7
	}

	GaussianBlur {
		anchors.fill: background
		source: background
		radius: 8
		samples: 16
	}

	Text {
		id: messageText

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter

		height: contentHeight

		padding: 10
		wrapMode: Text.WordWrap
		horizontalAlignment: Text.AlignHCenter
		color: "white"
	}

	Button {
		id: actionButton

		anchors.top: messageText.bottom
		anchors.horizontalCenter: messageText.horizontalCenter
		anchors.topMargin: 10

		text: "Ok"
		highlighted: true
		visible: !((flags & PopupAlert.Flag.NoAction) === PopupAlert.Flag.NoAction)

		onClicked: {
			rootView.confirmed();

			if ((flags & PopupAlert.Flag.AutoMute) === PopupAlert.Flag.AutoMute) {
				flags |= PopupAlert.Flag.Silence;
			} else if ((flags & PopupAlert.Flag.AutoClose) === PopupAlert.Flag.AutoClose) {
				hide();
			}
		}
	}

	enter: Transition {
		NumberAnimation {
			property: "y"
			from: 0.0
//			to: Math.round((applicationWindow.height - rootView.height) / 2)
		}
	}

	exit: Transition {
		NumberAnimation {
			property: "y"
//			from: Math.round((applicationWindow.height - rootView.height) / 2)
			to: 0.0
		}
	}
}
