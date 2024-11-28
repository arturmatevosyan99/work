import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Repository.Meta 0.1

FocusScope {
	id: rootScope

	property var dataModel: undefined
	readonly property var tokenModel: undefined === dataModel ? null : dataModel.tokens
//	readonly property var tokenModel: /*undefined === dataModel ? */null /*: dataModel.tokens*/
	property alias placeholderText: textField.placeholderText
	property alias topPadding: textField.anchors.topMargin
	property alias bottomPadding: textField.anchors.bottomMargin
	property alias leftPadding: textField.anchors.leftMargin
	property alias rightPadding: textField.anchors.rightMargin
	property alias progress: backgroundRectangle.gradientStopPosition

	x: textField.x
	y: textField.y
	height: textField.implicitHeight + textField.topPadding + textField.bottomPadding

	TextField {
		id: textField

		anchors.fill: parent

		topPadding: 6
		bottomPadding: 6
		rightPadding: 4
		leftPadding: 4 + listView.width
		onLeftPaddingChanged: {
			console.info("FilterField::textField.onLeftPaddingChanged", leftPadding);
			ensureVisible(0);
		}

	//	inputMethodComposing: false
		inputMethodHints: Qt.ImhNoPredictiveText
		verticalAlignment: TextInput.AlignVCenter
		font.pointSize: 14
		color: "black"
		focus: true

		onAccepted: {
			if (rootScope.dataModel.tokenize(text)) {
				textField.clear();
			}
		}

		Keys.onPressed: {
			if (event.key === Qt.Key_Backspace && text.length == 0 && rootScope.tokenModel.length > 0) {
				rootScope.dataModel.removeToken(rootScope.tokenModel.length - 1);
				event.accepted = true;
			}
		}

		background: Rectangle {
			id: backgroundRectangle

			property double gradientStopPosition: 1

			radius: 8

			border.color: "transparent"
			color: Material.color(Material.Grey, Material.Shade200)
			gradient: Gradient {
				orientation: Gradient.Vertical

				GradientStop {
					position: 0.0
					color: backgroundRectangle.color
				}
				GradientStop {
					position: backgroundRectangle.gradientStopPosition - 0.000002
					color: backgroundRectangle.color
				}
				GradientStop {
					position: backgroundRectangle.gradientStopPosition - 0.000001
					color: textField.placeholderTextColor
				}
				GradientStop {
					position: 1.0
					color: textField.placeholderTextColor
				}
			}

			ListView {
				id: listView

				property real maximumWidth: parent.width <= 0 ? 0 : parent.width * 0.7

				anchors.top: parent.top
				anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.leftMargin: 1
				anchors.topMargin: 1
				anchors.bottomMargin: 1

				orientation: ListView.Horizontal
				spacing: 2
				clip: true

				width: Math.min(count <= 0 ? 0 : contentWidth, maximumWidth)
				onWidthChanged: {
					console.info("FilterField::listView.onWidthChanged", width);
					Qt.callLater(listView.positionViewAtEnd);
				}

				onCountChanged: {
					console.info("FilterField::listView.onCountChanged", count);
				}

				function getTypeColor(type, active) {
					var alpha = active ? 1 : 0.6;
					switch (type) {
						case EssenceToken.Tag:
							return Qt.rgba(240/255., 230/255., 140/255., alpha); // khaki

						case EssenceToken.Object:
							return Qt.rgba(255/255., 215/255., 0/255., alpha); // gold

						case EssenceToken.Subject:
							return Qt.rgba(154/255., 205/255., 50/255., alpha); // yellow green

						case EssenceToken.Group:
							return Qt.rgba(255/255., 127/255., 80/255., alpha); // coral

						case EssenceToken.Attribute:
							return Qt.rgba(255/255., 240/255., 245/255., alpha); // lavender blush

						case EssenceToken.Path:
							return Qt.rgba(135/255., 206/255., 250/255., alpha); // light sky blue

						case EssenceToken.Any:
							return Qt.rgba(181/255., 181/255., 183/255., alpha); // grey

						case EssenceToken.AfterDate:
						case EssenceToken.BeforeDate:
						case EssenceToken.TimeFrame:
							return Qt.rgba(241/255., 229/255., 250/255., alpha); // pink

						default:
							return Qt.rgba(255/255., 255/255., 255/255., alpha); // white
					}
				}

				model: rootScope.tokenModel
				delegate: Button {
					id: tokenButton

					height: listView.height
					flat: true
					down: true
					text: "%1%2".arg(sign).arg(value)

					background: Rectangle {
						radius: 8
						opacity: enabled ? 1 : 0.3
						color: listView.getTypeColor(type, active)
					}

					onPressed: {
						if (!listView.activeFocus) {
							listView.focus = true;
						}
					}

					onClicked: {
						rootScope.dataModel.removeToken(index, false);
					}
				}

				onMovementStarted: {
					if (!activeFocus) {
						focus = true;
					}
				}
			}
		}
	}
}
