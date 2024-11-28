import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.0

import Core.Net 0.1
import Core.Meta 0.1
import Core.View 0.1
import Core.Model 0.1
import Core.System 0.1

import Repository.Model 0.1
import Repository.View 0.1

import DrOblozhko.Model 0.1

import Conversation.Model 0.1
import Conversation.View 0.1

import "Core/View"

import "Conversation/View"
import "Conversation/View/Application"

import "Mentalgami/View"
import "Mentalgami/View/Application"

import "DrOblozhko/View"

ApplicationWindow {
	id: applicationWindow

	onClosing: {
		if (Native.Desktop === Native.deviceType) {
			Qt.quit();
		}
	}

	title: qsTr("Metabolic Plate") /// < ru: Метаболическая Тарелка, ua: ... , es: ...
	visible: true
	flags: Qt.Window | Qt.MaximizeUsingFullscreenGeometryHint

	Material.theme: Material.Light
	Material.accent: Material.Blue /// < #2196F3
	Material.primary: Material.LightBlue /// < #03A9F4
//	Material.foreground: default font
//	Material.background: default background

	MessageDialog {
		id: invalidSessionDialog

		visible: false
		title: qsTr("Oh!")
		text: qsTr("Session is obsolete or invalid, need to sign in into account again")
		standardButtons: StandardButton.Retry

		onAccepted: {
			stackView.replace(null, tourComponent, {}, StackView.Transition);
		}
	}

	MessageDialog {
		id: unavailableProlongationDialog

		visible: false
		title: qsTr("Ouch!")
		text: qsTr("Could not connect to server. Pelase, try again later.")
		standardButtons: StandardButton.Retry

		onAccepted: {
			session.prolongate(realm);
		}
	}

	MessageDialog {
		id: unavailableAppointmentDialog

		visible: false
		title: qsTr("Ouch!")
		text: qsTr("Sign up is temporarily unavailable. Please, try again later.")
		standardButtons: StandardButton.Ok

		onAccepted: {
			realm.endpoint.reach();
		}
	}

	AlertDialog {
		id: signOutDialog

		AlertState {
			name: "1"
			title: qsTr("Do you really want to sign out?")
			text: qsTr("All application data will be reset")

			AlertAction {
				text: qsTr("Yes")
				style: AlertAction.Ok
				onDone: {
					signOutDialog.show("2");
				}
			}
			AlertAction {
				text: qsTr("No")
				style: AlertAction.Cancel
			}
		}

		AlertState {
			name: "2"
			title: qsTr("Do you want to remove your account from our servers as well?")
			text: qsTr("We keep your chat history and other data in secure and restore when you back")

			AlertAction {
				text: qsTr("Keep")
				style: AlertAction.Ok
				onDone: {
					signOutDialog.state = "1";
					connectionAlert.flags = PopupAlert.Flag.NoAction;
					connectionAlert.show(qsTr("Signing out..."));
					session.signOut();
				}
			}
			AlertAction {
				text: qsTr("Remove")
				style: AlertAction.Discard
				onDone: {
					signOutDialog.show("3");
				}
			}
		}

		AlertState {
			name: "3"
			title: qsTr("Are you really sure?")
			text: qsTr("When you remove an account, everything associated with is also deleted permanently")

			AlertAction {
				text: qsTr("Delete forever")
				style: AlertAction.Discard
				onDone: {
					signOutDialog.state = "1";
					connectionAlert.flags = PopupAlert.Flag.NoAction;
					connectionAlert.show(qsTr("Account and private data are deleting..."));
					appointment.resign(session);
				}
			}
			AlertAction {
				text: qsTr("Keep account")
				style: AlertAction.Cancel
				onDone: {
					signOutDialog.state = "1";
				}
			}
		}
	}

	Connections {
		id: nativeConnections

		target: Native

		function checkApplicationUpdate()
		{
			Native.checkApplicationUpdate();
		}

		function onApplicationUpdated(ready)
		{
			if (ready) {
				Native.runApplicationUpdate();
			} else {
				Native.alertAction(qsTr("Oh!"), qsTr("Automatic update is failed due to unexpected error, please try to update manually"), ["Ok"]);
			}
		}

		function onMemoryExhausted()
		{
			Native.alert(qsTr("System memory is exhausted"), qsTr("Please, free some memory by stopping background applications"), ["Ok"]);
		}
	}

	Timer {
		id: maintenanceTimer

		running: false
		repeat: true
		interval: 10000

		onTriggered: {
			realm.endpoint.reach();
		}
	}

	PopupAlert {
		id: connectionAlert

		function setConfirmCallback(callback)
		{
			var handler = () =>
			{
				handler.apply(this, arguments);
				confirmed.disconnect(handler);
			}
			confirmed.connect(handler);
		}
	}

	Realm {
		id: realm

		synchronizationMode: Realm.Lazy

		onDeinitialized: {
			stackView.replace(null, splashComponent, {"appointment": session.poolId}, StackView.Transition);
		}

		endpoint.onReached: {
			maintenanceTimer.running = false;
			connectionAlert.hide();
			connectionAlert.action = "";
			connectionAlert.message = "";
			connectionAlert.flags = PopupAlert.Flag.None;
		}
		endpoint.onMaintenanced: {
			maintenanceTimer.running = true;
			connectionAlert.action = qsTr("Go Offline");
			connectionAlert.flags = PopupAlert.Flag.AutoClose | PopupAlert.Flag.AutoMute;
			connectionAlert.show(qsTr("We are currently undergoing scheduled maintenance. Please come back later."));
		}
		endpoint.onOutdated: {
			maintenanceTimer.running = false;
			if (side === Endpoint.Server) {
				connectionAlert.flags = PopupAlert.Flag.NoAction;
				connectionAlert.show(qsTr("Connection has been rejected because endpoint does not support this version."));
			} else if (side === Endpoint.Client) {
				maintenanceTimer.running = false;
				connectionAlert.setConfirmCallback(nativeConnections.checkApplicationUpdate);
				connectionAlert.action = qsTr("Update");
				connectionAlert.flags = PopupAlert.Flag.None;
				connectionAlert.show(qsTr("Version is outdated, please update this application to continue."));
			}
		}
	}

	Session {
		id: session

		flags: Session.AutoRettach

		onRealized: {
			console.log("Session::onRealized > A");
		}
		onAuthorized: {
			console.log("Session::onAuthorized > A");
		}
		onFaulted: {
			if (phase == Session.Prolongation && reject == Session.NetworkError && !session.hasAttached()) {
				unavailableProlongationDialog.open();
			}
		}
		onValidated: {
			if (!cold) {
				return;
			}
			appointment.point(realm.endpoint);
			/// @brief Начинаем восстанавливать ситуацию
			var session_history = realm.preference.getOption("ace_realm_session_history");
			if (session_history) { /// < Поищем в истории
				var application_ident = SchemeFactory.ident("Repository.Meta.ApplicationRender", "DrOblozhko", "AppointmentChannel");
				if (application_ident) {
					application_ident = application_ident.toString();
					for (var h = 0; h < session_history.length; h++) {
						if (session_history[h].scheme === application_ident) {
							/// @note Есть информация о канале, просто получим на него хэндл и прицепим модель к вьюхе
							var enrollmentView = stackView.replace(channelComponent, {}, StackView.Transition);
							enrollmentView.model.open(session.handle(session_history[h].id, session_history[h].handle, session_history[h].scheme, "Appointment Channel"));
							console.log("Session::onValidated > ", session_history[h].id, session_history[h].handle, session_history[h].scheme);
							return ;
						}
					}
				}
			}
			/// @note Информация о канале не сохарнилась в истории, открываем AppointmentChannel(по-хорошему, нужно как-то получать уже существующий дескриптор в Authorizedsession)
			appointment.reroll(session.poolId, session.poolHandle);
			console.log("Session::onValidated > ", session.poolId, session.poolHandle);
		}
		onRevalidated: {
			connectionAlert.flags ^= PopupAlert.Flag.Silence;
			if (!cold) {
				return;
			}
			/// @brief Сессия обновлена, откроем AppointmentChannel заново
			appointment.point(realm.endpoint);
			appointment.reroll(session.poolId, session.poolHandle);
		}
		onInvalidated: {
			if (!silent) {
				invalidSessionDialog.open();
			} else {
				connectionAlert.hide();
				stackView.replace(null, tourComponent, {}, StackView.Transition);
			}
		}
		onUpstreamed: {
			if (!hasUpstream) {
				realm.endpoint.reach();
			}
		}
	}

	Appointment {
		id: appointment

		signal repointed(string endpoint)

		property bool autoStack: true
		onAutoStackChanged: {
			if (autoStack == true && isEnrolled()) {
				stack();
			}
		}

		function stack()
		{
			var enrollmentView;
			if (!realm.isInitialized) {
				if (session.authorize(appointment.sessionId, appointment.sessionHandle, appointment.sessionScheme)) {
					enrollmentView = stackView.replace(channelComponent, {}, StackView.Transition);
					enrollmentView.model.open(session.handle(appointment.channelId, appointment.channelHandle, appointment.channelScheme, "Appointment Channel"));
				}
			} else {
				enrollmentView = stackView.replace(channelComponent, {}, StackView.Transition);
				enrollmentView.model.open(session.handle(appointment.channelId, appointment.channelHandle, appointment.channelScheme, "Appointment Channel"));
			}
		}

		onReset: {
			connectionAlert.hide();
			stackView.replace(null, tourComponent, {}, StackView.Transition);
		}
		onRepointed: {
			point(realm.endpoint, endpoint);
		}
		onRejected: {
			unavailableAppointmentDialog.open();
		}
		onIssued: {
			if (autoStack) {
				stack();
			}
		}
	}

	Component {
		id: tourComponent

		Tour {
			id: tour

			focus: true
			model: appointment

			property Splash splash: null

			Component.onCompleted: {
				appointment.autoStack = false;
			}

			onBoardingFinished: {
				appointment.autoStack = true;
			}
			onTimeoutExpired: {
				if (!splash) {
					splash = stackView.push(splashComponent, {"appointment": appointment.poolId, "helpInterval": 1}, StackView.Immediate);
				}
			}
			onActiveActionChanged: {
				if (tour.splash && !activeAction && stackView.currentItem == tour.splash) {
					splash = null;
					stackView.pop(StackView.PopTransition);
				}
			}
		}
	}

	Component {
		id: splashComponent

		Splash {
		}
	}

	Component {
		id: channelComponent

		ChannelFeed {
			id: channelFeed

			headerIcon: "/DrOblozhko/Asset/Image/HudeemOnline1.svg"
			headerTitle: qsTr("Chat")
			headerSubtitle: "§%1: %2".arg(model.descriptor.objectId).arg(qsTr("3 members"))
			headerShield: ConversationChannelShield {
				helpURL: qsTr("https://cadabra1.notion.site/843940dfe7f846908b84d7fc97f513f2")
				onSignOutRequested: {
					signOutDialog.show();
				}
			}
		}
	}

	StackView {
		id: stackView

		focus: true

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		Component.onCompleted: {
			stackView.push(splashComponent, {"appointment": session.poolId}, StackView.Immediate);
			if (!session.prolongate(realm)) {
				stackView.replace(null, tourComponent, {}, StackView.Transition);
			}
		}
	}
}
