import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Core.View 0.1

import "../../Repository/View/Widget/"
import Repository.Model 0.1
import Repository.View 0.1
import Repository.View.Widget 0.1

import "../../Core/View/"

Page {
	id: rootView

	signal componentSelected(var item, var properties)
	property alias domain: widgetBoard.domain

	padding: 0
	leftInset: 0
	rightInset: 0

	MessageDialog {
		id: hardSignOutDialog

		title: qsTr("Hm...")
		text: qsTr("Would you like to hard reset current session and sign out?")
		standardButtons: StandardButton.Yes | StandardButton.No

		onYes: {
			domain.signOut(true);
		}
	}

	Component {
		id: diagnosticComponent

		DiagnosticCollector {
		}
	}

	header: StackBar {
		id: stackBar

		titleText: qsTr("Dashboard")

		beatMode: StackBar.BeatMode.Stable

		optionButton.enabled: domain ? (domain.authenticationState === Domain.Active || domain.authenticationState === Domain.Reactive) : false
		optionStyle: StackBar.OptionStyle.Stack
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
				text: qsTr("Sign Out")
				onTriggered: {
					optionMenu.close();
					if (!domain.signOut(false)) {
						hardSignOutDialog.open();
					}
				}
			}

			MenuItem {
				text: qsTr("Share diagnostic")
				onTriggered: {
					optionMenu.close();
					componentSelected(diagnosticComponent, {});
				}
			}
		}
	}

	Widgetboard {
		id: widgetBoard

		anchors.fill: parent

		AuthenticationWidget {

		}
	}
}
