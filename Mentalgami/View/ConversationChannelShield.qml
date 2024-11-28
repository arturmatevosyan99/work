import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15

import "../../Core/View"

import "../../Repository/View"
import "../../Repository/View/Application"

import "../../Conversation/View"
import "../../Conversation/View/Application"

ChannelShield {
	id: rootLayout

	Component.onCompleted: {
		if (rootLayout.helpURL.length > 0) {
			rootLayout.modeListView.menu.addAction(helpActionComponent.createObject(modeListView.menu));
		}
	}

	signal signOutRequested()

	property string helpURL: ""

	menuModeSet: ChannelFeed.MenuMode.Collapse | ChannelFeed.MenuMode.Media

	Component {
		id: diagnosticComponent

		DiagnosticCollector {
		}
	}

	Component {
		id: helpComponent

		WebApplication {
			id: webApplication

			headerTitle: qsTr("Help")
			Component.onCompleted: {
				if (rootLayout.helpURL.length > 0) {
					webApplication.model.accessed(rootLayout.helpURL, "");
				}
			}
		}
	}

	Component {
		id: helpActionComponent

		Action {
			property var enumber: ChannelFeed.MenuMode.Custom
			text: qsTr("Help")
			icon.source: "/Core/Asset/Image/Help1.svg"
			enabled: true
			onTriggered: {
				rootLayout.customComponentRequested(helpComponent);
			}
		}
	}

	bottomComponent: Component {
		RowLayout {
			id: bottomRowLayout

			height: Math.max(diagnosticCollectorButton.implicitHeight, signOutButton.implicitHeight)

			Button {
				id: diagnosticCollectorButton

				Layout.fillHeight: true
				Layout.minimumWidth: indicator.height
				Layout.maximumWidth: indicator.height
				Layout.margins: 4

				flat: true
				display: AbstractButton.IconOnly

				indicator: Image {
					anchors.verticalCenter: diagnosticCollectorButton.verticalCenter

					height: signOutButtonText.implicitHeight * 1.2
					source: "/Core/Asset/Image/Lifebuoy1.svg"
					sourceSize.height: height
					fillMode: Image.PreserveAspectFit
					verticalAlignment: Image.AlignVCenter
				}

				onClicked: {
					rootLayout.customComponentRequested(diagnosticComponent);
				}
			}
			Rectangle {
				Layout.fillHeight: true
				Layout.fillWidth: true
				/// @todo версия и может быть сессия?
				border.width: 0
				border.color: "transparent"
				color: "transparent"
			}
			Button {
				id: signOutButton

				Layout.fillHeight: true
				Layout.minimumWidth: implicitContentWidth + 4
				Layout.maximumWidth: implicitContentWidth + 4
				Layout.rightMargin: 4

				flat: true
				text: qsTr("Sign Out")
				display: AbstractButton.TextBesideIcon

				contentItem: Row {
					Text {
						id: signOutButtonText

						text: signOutButton.text
						font.pointSize: signOutButton.font.pointSize
						font.capitalization: Font.Capitalize
						anchors.verticalCenter: parent.verticalCenter
						rightPadding: 4
					}
					Image {
						anchors.verticalCenter: parent.verticalCenter

						height: signOutButtonText.implicitHeight * 1.2
						source: "/Core/Asset/Image/Exit1.svg"
						sourceSize.height: height
						fillMode: Image.PreserveAspectFit
						verticalAlignment: Image.AlignVCenter
					}
				}

				onClicked: {
					rootLayout.signOutRequested();
					rootLayout.modeRequested(ChannelFeed.MenuMode.Collapse);
				}
			}
		}
	}
}
