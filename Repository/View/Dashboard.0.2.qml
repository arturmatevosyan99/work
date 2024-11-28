import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import "../../Core/View/"
import Core.System 0.1
import Core.View 0.1

import "Widget/"
import Repository.Model 0.1
import Repository.View 0.2

/**
 * @todo Ввести понятие размер Widget(xs, s, m, l, xl), который будет зависеть от устройства
 */
Page {
	id: rootView

	signal componentSelected(var item, var properties)

	property alias userAvatar: widgetLayout.userAvatar

	padding: 0
	leftInset: 0
	rightInset: 0

	Component {
		id: diagnosticComponent

		DiagnosticCollector {
		}
	}

	header: StackBar {
		id: stackBar

		titleText: qsTr("Dashboard")

		beatMode: StackBar.BeatMode.Stable

		optionButton.enabled: rootView.userAvatar ? rootView.userAvatar.session.isActive : false
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
					Alert.action
					(
						"SESSION_SIGNOUT",
						qsTr("Session"),
						qsTr("Do you really want to sign out?"),
						AlertOption.Yes | AlertOption.No,
						ActionAlert.Default,
						DialogAlert.Warning
					).yes.connect(
									function()
									{
										rootView.userAvatar.session.signOut(false);
									}
					);
				}
			}

			MenuItem {
				text: qsTr("Share diagnostic")
				onTriggered: {
					optionMenu.close();
					rootView.componentSelected(diagnosticComponent, {});
				}
			}
		}
	}

	WidgetLayout {
		id: widgetLayout

		anchors.fill: parent

		AuthenticationWidget {

		}
	}
}
