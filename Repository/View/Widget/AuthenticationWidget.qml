import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Repository.View 0.1
import Repository.Model 0.1

Widget {
	id: rootView

	Component.onCompleted: {
		if (onboard && board.hasDomain) {
			domainConnection.target = board.domain;
			domainConnection.enabled = true;
		}
	}

	title: qsTr("Authorization")
	priority: 1
	width: Math.min(boardRect.width, 400)
	height: childrenRect.height + 20
	enabled: (!onboard || !board.hasDomain) ? false : (board.domain.authenticationState === Domain.Unknown || board.domain.authenticationState === Domain.Fault)
	visible: height > 0
	state: "idle"
	available: true

	Connections {
		id: domainConnection
		enabled: false
		ignoreUnknownSignals: true

		function onAuthenticationStateChanged(state) {
			switch (target.authenticationState) {
				case Domain.Unknown:
					rootView.state = "idle";
					break;

				case Domain.Fault:
					rootView.state = "idle";
					break;

				case Domain.Progress:
					rootView.state = "progress";
					break;

				case Domain.Active:
					rootView.state = "active";
					break;

				case Domain.Reactive:
					rootView.state = "reactive";
					break;
			}

			if (target.authenticationState === Domain.Active) {
				collapseAnimation.from = rootView.childrenRect.height + 20;
				collapseAnimation.to = 0;
			} else if (state === Domain.Active) {
				collapseAnimation.from = 0
				collapseAnimation.to = rootView.childrenRect.height + 20;
			} else {
				return;
			}
			collapseAnimation.start();
		}
	}

	NumberAnimation {
		id: collapseAnimation

		target: rootView
		running: false
		alwaysRunToEnd: true
		duration: 1500
		easing.type: Easing.InElastic

		property: "height"
		from: 0
		to: 0
	}

	Row {
		id: endpointRow
		height: childrenRect.height

		anchors.top: parent.top
		anchors.topMargin: 10
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		TextField {
			id: endpointHostnameField

			width: endpointRow.width - endpointSSLField.width

			placeholderText: "Endpoint"
			text: !onboard || !board.hasDomain ? "" : board.domain.hostEndpoint

			KeyNavigation.tab: loginField
			KeyNavigation.priority: KeyNavigation.BeforeItem
		}

		Switch {
			id: endpointSSLField
			enabled: rootView.enabled

			text: "SSL"
			checked: !onboard || !board.hasDomain  ? false : board.domain.sslEndpoint
		}
	}
	TextField {
		id: loginField

		anchors.top: endpointRow.bottom
		anchors.topMargin: 20
		anchors.left: endpointRow.left
		anchors.right: endpointRow.right

		activeFocusOnTab: true
		KeyNavigation.tab: passwordField

		placeholderText: "Email/Phone"
	}
	TextField {
		id: passwordField

		anchors.top: loginField.bottom
		anchors.topMargin: 20
		anchors.left: endpointRow.left
		anchors.right: endpointRow.right

		activeFocusOnTab: true
		KeyNavigation.tab: authenticationButton

		placeholderText: "Password"

		echoMode: TextInput.Password
		passwordMaskDelay: 1000
	}

	Row {
		anchors.top: passwordField.bottom
		anchors.topMargin: 20
		anchors.left: passwordField.left
		anchors.right: passwordField.right

		height: childrenRect.height

		CheckBox {
			id: syncCheckbox

			text: qsTr("Sync")
			tristate: false
			checkState: Qt.Checked
		}

		Button {
			id: authenticationButton

			leftPadding: 5
			width: parent.width - syncCheckbox.width - authenticationButton.leftPadding

			activeFocusOnTab: true

			Material.elevation: 0
			Material.background: Material.Blue
			Material.foreground: Material.Grey
			text: qsTr("Sign In!")

			contentItem: Text {
				text: parent.text
				font: parent.font
				opacity: parent.enabled ? 1.0 : 0.3
				color: "white"
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				elide: Text.ElideRight
			}

			background: Rectangle {
				anchors.fill: parent
				opacity: parent.enabled ? 1 : 0.3
				color: parent.down ? Material.color(Material.Blue, Material.Shade300) : Material.color(Material.Blue)
				radius: 5
				border.width: 0
			}

			state: rootView.state
			states: [
						State {
								name: "idle"
								PropertyChanges {
									target: authenticationButton
									text: qsTr("Sign In")
									enabled: true
								}
						},
						State {
								name: "progress"
								PropertyChanges {
									target: authenticationButton
									text: qsTr("Authenticating...")
									enabled: false
								}
						},
						State {
								name: "reactive"
								PropertyChanges {
									target: authenticationButton
									text: qsTr("Connecting...")
									enabled: false
								}
						},
						State {
								name: "active"
								PropertyChanges {
									target: authenticationButton
									text: qsTr("Sign Out")
									enabled: true
								}
						}
			]

			onClicked: {
				if (state === "idle") {
					domain.sslEndpoint = endpointSSLField.checked;
					domain.hostEndpoint = endpointHostnameField.text;
					domain.signIn(loginField.text, passwordField.text, syncCheckbox.checkState, syncCheckbox.checkState !== Qt.Unchecked);
				} else if (state === "active") {
					// sign out
				} else if (state === "reactive") {
					// prolongation cancel
				}
			}
		}
	}
}
