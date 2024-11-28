import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15

import Core.View 0.1
import Core.System 0.1
import Core.Meta 0.1
import EC.Global 0.1
import EC.Model 0.1
import EC.Data 0.1

import "../../Core/View"

Page {
	id: rootView

	Component.onCompleted: {
		console.log("SubjectProfile::onCompleted");
	}

	Component.onDestruction: {
		console.log("SubjectProfile::onDestruction");
	}

	property OnlineShop model: OnlineShop {
		showroom: [
			Showcase {
				catalogue: Showcase.History
				filter: []
			}
		]
	} onModelChanged: {
		console.debug("SubjectProfile::onModelChanged", rootView.model);
		modelConnection.target = model;
		rootView.model.opened();
	}

	Connections {
		id: modelConnection

		target: rootView.model

		function onOpened()
		{
			rootView.model.loadShowcase();
		}

		function onShowcaseLoaded(success)
		{
		}

		function onPurchaseOrdered(success)
		{
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		titleText: qsTr("Subject Profile")

		stackView: rootView.StackView.view

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
///				rootView.menuList.unshift();
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

		/// @todo go to SubjectProfile(Last Modification, Avatar, Home, Accounts, Active Sessions)
		model: ObjectModel {
			CallbackDelegate {
				property string group: qsTr("Realm Id")

				height: implicitContentHeight * 2
				width: listView.width

				iconText: "1234 (name)" /// < if name differs from id
				iconSource: "/DrOblozhko/Asset/Image/InfoCircle.svg" /// @
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
			}
		}
	}
}
