import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.15

FocusScope {
	id: rootScope

	signal inputCompleted(string value);

	property alias text: codeInput.text
	property int fontPointSize: 32
	property int sectionWidth: 20
	property int sectionGap: 0
	property int sectionRadius: 0
	property color sectionBorderinlineColor: Material.accent
	property color sectionBorderoutlineColor: Material.color(Material.Grey, Material.Shade100)
	property color sectionBackgroundColor: Material.background

	function clear()
	{
		codeInput.text = "    ";
		codeInput.forceActiveFocus();
		codeInput.cursorPosition = 0;
	}

	Row {
		id: numberRow

		anchors.horizontalCenter: parent.horizontalCenter
		spacing: sectionGap

		Repeater {
			id: numberRepeater

			model: 4
			Rectangle {
				color: rootScope.sectionBackgroundColor

				width: sectionWidth
				height:	width
				radius: sectionRadius

				border.color: index === (
											codeInput.selectedText.length > 0
											? Math.min(codeInput.selectionStart, codeInput.cursorPosition)
											: codeInput.cursorPosition
										)
										? rootScope.sectionBorderinlineColor
										: rootScope.sectionBorderoutlineColor
				border.width: 3

				Label {
					anchors.fill: parent

					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter

					text: codeInput.length > index ? codeInput.text[index] : ""
					font.pointSize: fontPointSize
					font.bold: true

					MouseArea {
						anchors.fill: parent

						onClicked: {
							codeInput.forceActiveFocus();
							if (codeInput.text.length > index) {
								codeInput.select(index, index + 1);
							}
						}
					}
				}
			}
		}
	}
	TextInput {
		id: codeInput

		focus: true

		anchors.left: rootScope.left
		anchors.top: rootScope.top

		text: "    "
		maximumLength: 4
		visible: false
		inputMethodHints: Qt.ImhDigitsOnly
		overwriteMode: false /// < Overwrite какой-то ненастоящий ;(

		onTextEdited: {
			if (cursorPosition >= 4) {
				inputCompleted(text);
			} else if (text.length > cursorPosition) {
				select(cursorPosition, cursorPosition + 1);
			}
		}
	}
}
