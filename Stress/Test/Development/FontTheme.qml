import QtQml 2.15
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Core.View 0.1
import Core.System 0.1

Rectangle {
	id: testView

	TextArea {
		id: textArea1

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right

		text: "(TextArea1: %1, %2)\n(System: %3, %4)".arg(font.family).arg(font.pointSize).arg(Theme.defaultFont.family).arg(Theme.defaultFont.pointSize)
	}

	Label {
		id: label1

		anchors.top: textArea1.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		text: "(Label1: %1, %2 pt) съешь ещё этих мягких французских булок, да выпей чаю".arg(font.family).arg(font.pointSize)
	}

	Text {
		id: text1

		anchors.top: label1.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		wrapMode: Text.WordWrap

		text: " (Text1: %1, %2 pt) съешь ещё этих мягких французских булок, да выпей чаю".arg(font.family).arg(font.pointSize)
	}

	Label {
		id: label2

		anchors.top: text1.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		text: "(Label2: %1, %2 pt) 😀 😃 😄 😁 😆 😅 🤣 😂 🙂 😉 😊 😇 🥰 😍 🤩 😘 😗 ☺️ 😚 😙 🥲 😏".arg(font.family).arg(font.pointSize)
	}

	Text {
		id: text2

		anchors.top: label2.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		wrapMode: Text.WordWrap

		text: "(Text2: %1, %2 pt) 😀 😃 😄 😁 😆 😅 🤣 😂 🙂 😉 😊 😇 🥰 😍 🤩 😘 😗 ☺️ 😚 😙 🥲 😏".arg(font.family).arg(font.pointSize)
	}


	Button {
		anchors.bottom: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter

		text: "Test"

		onClicked: {
			textArea1.text += "\n" + (Native.defaultFontScale).toString();
		}
	}
}
