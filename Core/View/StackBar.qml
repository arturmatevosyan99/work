import QtQml 2.15
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

import "../../Qaos/QtQuick/View"
import Core.View 0.1
import Core.System 0.1

ToolBar {
	id: rootView

	enum BackBehaviour {
		Ignore = 0,
		Signal,
		SignalAndPop
	}

	enum OptionStyle {
		None = 0,
		Custom = 1,
		Stack = 2,
		Magnifier = 3,
		Help = 4,
		Refresh = 5
	}

	enum BeatMode {
		None = 0,
		Stable = 1,
		Pulse = 2
	}

	property bool isHidden: false
	onIsHiddenChanged: {
		rootView.opacity = isHidden ? 0 : 1;
	}

	property StackView stackView: null

	property int backBehaviour: StackBar.BackBehaviour.SignalAndPop
	readonly property bool backStack: stackView ? (stackView.depth > 1) : false

	property alias rowLayout: layout

	property alias bookmarkText: bookmarkLabel.text
	property alias badgeText: backButton.text

	property alias titleLabel: label1
	property alias titleText: label1.text
	property alias subtitleLabel: label2
	property alias subtitleText: label2.text

	property int beatMode: StackBar.BeatMode.Stable

	property alias iconImage: image
	property alias iconSource: image.source

	property alias optionButton: optButton
	property alias optionStyle: optButton.iconStyle
	property alias optionEnabled: optButton.enabled
	property alias optionOpacity: optButton.opacity
	property url optionIconSource: ":/Core/Asset/Image/DotCube3.svg"

	Material.foreground: foregroundColor
	property color accentColor: Material.accent
	property color foregroundColor: "black"
	property color backgroundColor: Qt.rgba(1, 1, 1, 1)
	property bool backgroundGlass: true

	property bool keyboardCounterflow: false
	property real additionalTopPadding: 0

	signal backClicked()
	signal optionClicked()
	signal titleClicked()
	signal titleDoubleClicked()

	Behavior on opacity {
		NumberAnimation {
			duration: 400
		}
	}

	StackView.onActivated: {
		StatusBar.theme = StatusBar.Dark;
	}

	Connections {
		target: Native

		function onBackRequested()
		{
			if (rootView.StackView.visible) {
				back();
			}
		}
	}

	Connections {
		target: Native
		enabled: rootView.keyboardCounterflow && Qt.application.state === Qt.ApplicationActive && rootView.stackView && rootView.StackView.status === StackView.Active

		function onScreenKeyboardChanged()
		{
			rootView.stackView.anchors.topMargin = Native.screenKeyboardScreenHeight;
			console.log("StackBar::onScreenKeyboardChanged", Native.screenKeyboardVisible, Native.screenKeyboardScreenHeight);
		}

		function onScreenKeyboardDisplayed()
		{
			rootView.stackView.anchors.topMargin = Native.screenKeyboardScreenHeight;
			console.log("StackBar::onScreenKeyboardDisplayed", Native.screenKeyboardVisible, Native.screenKeyboardScreenHeight);
		}
	}

	function back()
	{
		if (backBehaviour === StackBar.BackBehaviour.SignalAndPop && backStack)  {
			pop();
		}
		if (backBehaviour !== StackBar.BackBehaviour.Ignore) {
			rootView.backClicked();
		}
	}

	function option()
	{
		rootView.optionClicked();
	}

	function pop()
	{
		backButton.enabled = false;
		backButton.visible = true;
		stackView.pop(StackView.PopTransition);
	}

	function profile()
	{
		console.log(Screen.height, Screen.desktopAvailableHeight);
	}

	topPadding: WindowManager.topSafePadding + additionalTopPadding
	leftPadding: 0
	rightPadding: 10
//	visible: opacity > 0

	background: Rectangle {
		anchors.fill: parent
		color: backgroundColor
		opacity: backgroundGlass ? 0.95 : 1

		Rectangle {
			id: bottomRectangle

			anchors.bottom: parent.bottom

			width: parent.width
			height: 1
			color: "transparent"
			visible: StackBar.BeatMode.None !== beatMode
			border.color: accentColor

			transform: Scale {
				id: heartbeatScale

				origin.x: bottomRectangle.x
				origin.y: bottomRectangle.y
				yScale: 1
				xScale: 1
			}

			SequentialAnimation {
				running: StackBar.BeatMode.Pulse === beatMode
				alwaysRunToEnd: true
				loops: Animation.Infinite

				PropertyAnimation {
					target: heartbeatScale
					property: "xScale"
					from: 1
					to: 0
					duration: 2000
					easing.type: Easing.InCubic
				}

				PropertyAnimation {
					target: heartbeatScale
					property: "xScale"
					from: 0
					to: 1
					duration: 1000
					easing.type: Easing.InOutCubic
				}
			}
		}
	}

	RowLayout {
		id: layout

		anchors.fill: parent

		spacing: 1

		Rectangle {
			id: bookmark

			Layout.leftMargin: -10
			Layout.fillWidth: false
			Layout.fillHeight: true
			Layout.minimumWidth: -Layout.leftMargin
			Layout.maximumWidth: bookmarkLabel.contentHeight - Layout.leftMargin
			Layout.preferredWidth: Layout.maximumWidth

			radius: Layout.minimumWidth / 2
			color: bookmarkLabel.text.length > 0 ? rootView.accentColor : "transparent"

			Label {
				id: bookmarkLabel

				x: parent.width
				y: 0
				width: parent.height
				height: parent.width
				leftPadding:2
				rightPadding: 2

				rotation: 90
				transformOrigin: Item.TopLeft

				text: ""
				color: rootView.backgroundColor
				elide: Text.ElideRight
				fontSizeMode: Text.Fit
				horizontalAlignment: Text.AlignVCenter
				verticalAlignment: Text.AlignHCenter
//				minimumPointSize: Theme.defaultFont.pointSize * 0.45
				font.weight: Font.ExtraLight
			}
		}

		ToolButton {
			id: backButton

			Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
			Layout.fillWidth: false
			Layout.fillHeight: true
			Layout.minimumWidth: implicitWidth

			visible: (StackBar.BackBehaviour.SignalAndPop === backBehaviour && backStack)
					 ||
					 (StackBar.BackBehaviour.Signal === backBehaviour)
					 ||
					 (text.length > 0)

			contentItem: Row {
				IconImage {
					height: parent.height
					width:  parent.height
					sourceSize.height: height - 8
					sourceSize.width: width - 8
					fillMode: Image.Pad

					source: backButton.icon.source

					verticalAlignment: Image.AlignVCenter
					horizontalAlignment: Image.AlignHCenter

					color: backButton.down ? "gray" : rootView.accentColor
				}
				Rectangle {
					visible: backText.text.length > 0
					anchors.verticalCenter: parent.verticalCenter

					height: backText.implicitHeight + 4
					width: backText.implicitWidth + 8

					radius: height / 2

					color: backButton.text.length > 0 ? rootView.accentColor : "transparent"
					border.color: "transparent"
					border.width: 0

					Text {
						id: backText

						anchors.fill: parent

						text: backButton.text
						font: backButton.font
						color: rootView.backgroundColor
						opacity: enabled ? 1.0 : 0.3

						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter
						elide: Text.ElideRight
					}
				}
			}

			background: null

			icon.source: "/Core/Asset/Image/Back2.svg"

			font.pointSize: label2.font.pointSize
			font.weight: Font.ExtraBold

			onClicked: rootView.back()
		}

		RoundImage {
			id: image

			Layout.fillWidth: false
			Layout.fillHeight: false
			Layout.margins: status === Image.Null ? 0 : 4
			Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

			sourceSize.height: title.height - 8
			sourceSize.width: title.height - 8
			fillMode: Image.Pad

			MouseArea {
				anchors.fill: parent
				onClicked: rootView.profile()
			}
		}

		Column {
			id: title

			Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
			Layout.fillHeight: false
			Layout.fillWidth: true
			Layout.minimumHeight: 40
			Layout.preferredHeight: label1.implicitHeight * 1.65

			Label {
				id: label1

				width: parent.width
				height: parent.height - label2.height

				text: ""
				textFormat: Text.PlainText

				verticalAlignment: label2.text.length == 0 ? Text.AlignVCenter : Text.AlignBottom
				horizontalAlignment: Text.AlignLeft

				elide: Text.ElideMiddle
				color: "black"

//				fontSizeMode: Text.VerticalFit
				font.pointSize: Theme.title2Font.pointSize
				font.weight: Font.Bold

				MouseArea {
					anchors.fill: parent

					onClicked: rootView.titleClicked()
					onDoubleClicked: rootView.titleDoubleClicked()
				}
			}

			Label {
				id: label2

				width: parent.width
				height: text.length > 0 ? implicitHeight : 0

				text: ""
				textFormat: Text.PlainText
				wrapMode: Text.WordWrap
				maximumLineCount: 1

				verticalAlignment: Text.AlignTop
				horizontalAlignment: Text.AlignLeft

				elide: Text.ElideRight
				color: "grey"

//				fontSizeMode: Text.VerticalFit
				font.pointSize: Theme.overlineFont.pointSize
				font.weight: Font.ExtraLight

				topPadding: 2
				bottomPadding: 2

				MouseArea {
					anchors.fill: parent

					onClicked: rootView.titleClicked()
					onDoubleClicked: rootView.titleDoubleClicked()
				}
			}
		}

		ToolButton {
			id: optButton

			property int iconStyle: StackBar.OptionStyle.Stack

			Layout.fillWidth: false
			Layout.fillHeight: true
			Layout.minimumWidth: height
			Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

//			visible: optionVisible
			opacity: 1.0
			enabled: true
			padding: 0

			display: AbstractButton.IconOnly

//			icon.height: optButton.height / 2
//			icon.width: icon.height
			icon.source: {
				switch (optButton.iconStyle) {
					case StackBar.OptionStyle.Custom:
						return rootView.optionIconSource;
					case StackBar.OptionStyle.Stack:
						return "/Core/Asset/Image/Stack1.svg";
					case StackBar.OptionStyle.Magnifier:
						return "/Core/Asset/Image/Magnifier.svg";
					case StackBar.OptionStyle.Help:
						return "/Core/Asset/Image/Lifebuoy2.svg";
					case StackBar.OptionStyle.Refresh:
						return "/Core/Asset/Image/Refresh2.svg";
					default:
						return "";
				}
			}

			contentItem: Item {
				IconImage {
					height: parent.height
					width:  parent.height
					sourceSize.height: height - 8
					sourceSize.width: width - 8
					fillMode: Image.Pad

					source: optButton.icon.source

					verticalAlignment: Image.AlignVCenter
					horizontalAlignment: Image.AlignHCenter

					color: optButton.down ? "gray" : rootView.accentColor
				}
			}

			background: null

			onClicked: rootView.option()
		}
	}
}
