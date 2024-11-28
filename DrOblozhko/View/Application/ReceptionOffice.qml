import QtQml 2.15
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

import Core.View 0.1
import Repository.View 0.1
import DrOblozhko.Model 0.1

import "../../../Core/View/"

import "../../../Repository/View/"

/// @todo Break into pieces(QML Components) for reuse

Page {
	id: application

	property Reception model: Reception {
	} onModelChanged: {
		console.debug("ReceptionOffice::onModelChanged", application.model);
		modelConnection.target = model;
		application.model.opened();
	}

	Connections {
		id: modelConnection

		target: application.model

		function onOpened()
		{
			console.debug("ReceptionOffice.model::onOpened");
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		titleText: qsTr("Reception")
		subtitleText: qsTr("Office")

		stackView: application.StackView.view

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
		id: listView

		anchors.left: parent.left
		anchors.top: parent.top
		width: parent.width
		height: parent.height

		orientation: Qt.Vertical

		model: ObjectModel {
			id: objectModel

			JobDelegate {
				property string group: qsTr("Common")

				height: implicitContentHeight * 2
				width: listView.width

				iconText: "Active Smart subscriptions: unknown"
				iconSource: "/DrOblozhko/Asset/Image/InfoCircle.svg"
			}
			JobDelegate {
				id: softcleanupDelegate

				property string group: qsTr("Cleanup")

				height: implicitContentHeight * 2
				width: listView.width

				iconText: qsTr("Request soft cleanup")
				iconSource: "/DrOblozhko/Asset/Image/RecycleBin.svg"

				onRequested: {
					job = application.model.cleanup("soft");
				}
			}
			JobDelegate {
				id: hardcleanupDelegate

				property string group: qsTr("Cleanup")

				height: implicitContentHeight * 2
				width: listView.width

				iconText: qsTr("Request hard cleanup")
				iconSource: "/DrOblozhko/Asset/Image/RecycleBin.svg"

				onRequested: {
					job = application.model.cleanup("hard");
				}
			}
		}

		section {
			property: "group"
			criteria: ViewSection.FullString
			delegate: Rectangle {
				required property string section

				width: ListView.view.width
				height: 20
				color: "transparent"
				border.width: 0
/*
				Rectangle {
					height: 1
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: parent.top
					color: Material.color(Material.Grey)
				}

				Rectangle {
					height: 1
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.bottom: parent.bottom
					color: Material.color(Material.Grey)
				}
*/
			}
		}
	}
}
