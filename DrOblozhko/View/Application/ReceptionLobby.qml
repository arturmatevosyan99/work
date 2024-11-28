import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

import Core.View 0.1
import DrOblozhko.Model 0.1 as Model

import "../../../Core/View"

Page {
	id: application

	property Model.Reception model: Model.Reception {
	} onModelChanged: {
		console.debug("ReceptionLobby::onModelChanged", model);
		modelConnection.target = model;
		application.model.opened();
	}

	Connections {
		id: modelConnection

		target: application.model

		function onOpened()
		{
			console.debug("ReceptionLobby.model::onOpened");
		}

		function onJobScheduled()
		{
			console.debug("ReceptionLobby.model::onIndexed");
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		stackView: application.StackView.view

		titleText: qsTr("Reception")
		subtitleText: qsTr("Lobby")

		beatMode: StackBar.BeatMode.Pulse

		onBackClicked: {
		}

		optionStyle: StackBar.OptionStyle.None
		onOptionClicked: {
			optionMenu.open();
		}

		Menu {
			id: optionMenu
			parent: stackBar.optionButton
			topMargin: stackBar.optionButton.height

			Component.onCompleted: {
///				application.menuList.unshift();
			}

			MenuItem {
				text: qsTr("Close")
				onTriggered: {
					optionMenu.close();
				}
			}
		}
	}

	ListView {
		id: contactsView

		anchors.left: parent.left
		anchors.top: parent.top
		width: parent.width
		height: parent.height

		orientation: Qt.Vertical

		model: ListModel {
			id: listModel

			ListElement {
				type: "info"
				group: "common"
			}
			ListElement {
				type: "softcleanup"
				group: "cleanup"
			}
			ListElement {
				type: "hardcleanup"
				group: "cleanup"
			}
		}

		delegate: DelegateChooser {
			id: delegateChooser

			role: "type"

			DelegateChoice {
				roleValue: "info"
				ItemDelegate {
				}
			}
			DelegateChoice {
				roleValue: "softcleanup"
				ItemDelegate {
				}
			}
			DelegateChoice {
				roleValue: "hardcleanup"
				ItemDelegate {
				}
			}
		}

		section {
			property: "group"
			criteria: ViewSection.FullString
			delegate: Rectangle {
				width: container.width
				height: childrenRect.height
				color: "lightsteelblue"

				required property string section

				Text {
					text: parent.section
					font.bold: true
					font.pixelSize: 20
				}
			}
		}
	}
}
