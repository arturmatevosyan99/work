import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Core.System 0.1

import Repository.View 0.2
import Repository.Model 0.1

WidgetView {
	id: rootView

	Component.onCompleted: {
		if (rootView.available) {
			rootView.state = WidgetLayout.userAvatar.reviveSession() ? "reactive" : "idle";
		} else {
			rootView.state = "unready";
		}
	}

	horizontalSizeType: WidgetView.MaxHorizontal
	verticalSizeType: WidgetView.CustomVertical
	height: childrenRect.height + 20

	enabled: available && !WidgetLayout.userAvatar.session.isActive
	available: WidgetLayout && WidgetLayout.userAvatar
	priority: 1

	title: qsTr("Authorization")
	state: "idle"

	Connections {
		target: rootView.available ? WidgetLayout.userAvatar.session : null
		enabled: target

		function onFaulted(phase, reject, message)
		{
			rootView.state = "idle";

			switch (phase) {
				case Session.Authentication:
					title = qsTr("Authorization error");
					/// @todo AuthenticationError | ReflectionError | ExpirationError
					break;

				case Session.Prolongation:
					title = qsTr("Prolongation error");
					break;

				default:
					title = qsTr("Sesison fault");
					break;
			}

			Alert.action
			(
				"SESSION_FAULT",
				title,
				message,
				AlertOption.Ok,
				ActionAlert.Default,
				DialogAlert.Error
			);
		}

		function onDeclined()
		{
			rootView.state = "idle";
		}

		function onAuthorized()
		{
			rootView.state = "active";
		}

		function onValidated(cold)
		{
			rootView.state = "active";
		}

		function onRevalidated(cold)
		{
			rootView.state = "active";
		}

		function onInvalidated(silent)
		{
			rootView.state = "idle";
		}
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

		enabled: rootView.state == "idle"

		TextField {
			id: endpointHostnameField

			width: endpointRow.width - endpointSSLField.width

			placeholderText: "Endpoint"
			text: WidgetLayout && WidgetLayout.userAvatar ? "%1:%2".arg(WidgetLayout.userAvatar.realm.endpoint.host).arg(WidgetLayout.userAvatar.realm.endpoint.port) : ""

			KeyNavigation.tab: loginField
			KeyNavigation.priority: KeyNavigation.BeforeItem
		}

		Switch {
			id: endpointSSLField

			text: "SSL"
			checked: WidgetLayout && WidgetLayout.userAvatar && WidgetLayout.userAvatar.realm.endpoint.secure
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

		Button {
			id: authenticationButton

			leftPadding: 5
			width: parent.width - authenticationButton.leftPadding

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
								name: "unready"
								PropertyChanges {
									target: authenticationButton
									text: qsTr("...")
									enabled: false
								}
						},
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
									text: qsTr("Connected")
									enabled: true
								}
						}
			]

			onClicked: {
				if (state === "idle") {
					if (avatar.pointEnd("%1://%2".arg(endpointSSLField.checked ? "aces" : "ace").arg(endpointHostnameField.text))) {
						if (loginField.text.indexOf("@") > -1) {
							avatar.session.signInEmail(loginField.text, passwordField.text);
						} else {
							avatar.session.signInPhone(loginField.text, passwordField.text);
						}
						rootView.state = "progress";
					}
				} else if (state === "active") {
					// sign out
				} else if (state === "reactive") {
					// prolongation cancel
				}
			}
		}
	}
}
