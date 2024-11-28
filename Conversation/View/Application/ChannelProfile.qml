import QtQml 2.15
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

import Core.View 0.1
import Conversation.Model 0.1

import "../../../Core/View"

Page {
	id: application

	Component.onCompleted: {
		console.log("ChannelProfile::onCompleted");
	}

	Component.onDestruction: {
		console.log("ChannelProfile::onDestruction");
	}

	property bool syncAvailable: false
	property GroupChannel model: GroupChannel {
	} onModelChanged: {
		console.debug("ChannelProfile::onModelChanged", application.model);
		modelConnection.target = model;
		if (modelConnection.target.isOpened) {
			modelConnection.onOpened();
		}
	}

	Connections {
		id: syncConnection

		target: application.model.session
		enabled: false

		function onUpstreamed(onoff)
		{
			application.syncAvailable = application.model.session.hasUpstream;
		}
	}

	Connections {
		id: modelConnection

		target: application.model

		function onOpened()
		{
			syncConnection.enabled = true;
			syncAvailable = target.session.hasUpstream;
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		stackView: application.StackView.view

		titleText: qsTr("Profile")

		beatMode: syncAvailable ? StackBar.BeatMode.Stable : StackBar.BeatMode.Pulse

		keyboardCounterflow: true

		onBackClicked: {
		}

		optionStyle: StackBar.OptionStyle.Stack
		onOptionClicked: {
			optionMenu.open();
		}

		Menu {
			id: optionMenu
			parent: stackBar.optionButton
			topMargin: stackBar.optionButton.height

			MenuItem {
				text: qsTr("Close")
				enabled: true

				onTriggered: {
					optionMenu.close();
				}
			}
			MenuItem {
				text: qsTr("Save")
				enabled: nameTextarea.isModified

				onTriggered: {
				}
			}
			MenuItem {
				text: qsTr("Restore")
				enabled: nameTextarea.isModified

				onTriggered: {
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
/*
			ListElement {
				type: "picture"
				group: qsTr("About")
			}
*/

			TextareaDelegate {
				id: nameTextarea
				property string group: qsTr("About")

				width: listView.width
				enabled: false

				text: application.model.descriptor.objectName
				placeholderText: qsTr("Channel topic")

				onEdited: {
				}
			}

			CallbackDelegate {
				property string group: qsTr("Menu")

				width: listView.width
				enabled: false

				iconText: qsTr("Leave")
				iconSource: "/DrOblozhko/Asset/Image/InfoCircle.svg"

				onCalled: {

				}
			}

			CallbackDelegate {
				property string group: qsTr("Menu")

				width: listView.width
				enabled: false

				iconText: qsTr("Mute")
				iconSource: "/DrOblozhko/Asset/Image/InfoCircle.svg"

				onCalled: {

				}
			}

			CallbackDelegate {
				property string group: qsTr("Menu")

				width: listView.width
				enabled: false

				iconText: qsTr("Search")
				iconSource: "/DrOblozhko/Asset/Image/InfoCircle.svg"

				onCalled: {

				}
			}

			CallbackDelegate {
				property string group: qsTr("Menu")

				width: listView.width
				enabled: false

				iconText: qsTr("Member")
				iconSource: "/DrOblozhko/Asset/Image/InfoCircle.svg"

				onCalled: {

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
