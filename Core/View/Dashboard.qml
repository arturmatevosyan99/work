import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
	visible: true
	width: 640
	height: 480
	title: qsTr("Худеем.Онлайн")

	SwipeView {
		id: swipeView
		anchors.fill: parent
		currentIndex: tabBar.currentIndex
/*
		Chat {
		}

		Page2Form {
		}
*/
	}

	footer: TabBar {
		id: tabBar
		currentIndex: swipeView.currentIndex

		TabButton {
			text: qsTr("Настройки")
		}
		TabButton {
			text: qsTr("Рецепты")
		}
		TabButton {
			text: qsTr("Чаты")
		}
	}
}

/*
SwipeView {
	id: swipeView
	//currentIndex: tabBar.currentIndex

	header: ToolBar {
		contentHeight: toolButton.implicitHeight

		ToolButton {
			id: tool_button
			text: stackView.depth > 1 ? "\u25C0" : "\u2630"
			font.pixelSize: Qt.application.font.pixelSize * 1.6
			onClicked: {
				if (stackView.depth > 1) {
					stackView.pop()
				} else {
					drawer.open()
				}
			}
		}

		Label {
			text: stackView.currentItem.title
			anchors.centerIn: parent
		}
	}

	header: TabBar {
		TabButton { text: "Tab1" }
		TabButton { text: "Tab2" }
		TabButton { text: "Tab3" }
		onCurrentIndexChanged: stackView.replace(pages.itemAt(currentIndex))
	}
}
*/




