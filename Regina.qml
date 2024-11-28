import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12

import Qt.labs.settings 1.0

import Core.Global 0.1
import Core.System 0.1
import Core.View 0.1
import Core.Meta 0.1

import "Repository/View"
import Repository.Global 0.1
import Repository.View 0.1
import Repository.Model 0.1

import "Conversation/View"
import "Conversation/View/Application"
import Conversation.View 0.1

ApplicationWindow {
	id: applicationWindow

	onClosing: {
		if (Native.Desktop == Native.deviceType) {
			Qt.quit();
		}
	}

	title: qsTr("CadabrApp")
	flags: Qt.Window | Qt.MaximizeUsingFullscreenGeometryHint
	visible: true

	Material.theme: Material.Light
	Material.accent: Material.Blue
	Material.primary: Material.LightBlue
//	Material.foreground: "white"  default font
//	Material.background: Material.ShadeA700 /// default background

	MessageDialog {
		id: invalidSessionDialog

		title: qsTr("Oh!")
		text: qsTr("Could not validate active session, please sign in again")
		standardButtons: StandardButton.Ok
	}

	MessageDialog {
		id: unavailableSessionDialog

		title: qsTr("Ouch!")
		text: qsTr("Could not connect to server. Connection will be restored automatically in the background mode when server become reachable")
		standardButtons: StandardButton.Retry
	}

	MessageDialog {
		id: badAuthenticationDialog

		title: qsTr("Hm...")
		text: qsTr("Could not authenticate, please check credentials and try again to proceed")
		standardButtons: StandardButton.Ok
	}

	MessageDialog {
		id: unavailableAuthenticationDialog

		title: qsTr("Hm...")
		text: qsTr("Could not authenticate, please check network connection and try again to proceed")
		standardButtons: StandardButton.Ok
	}

	MessageDialog {
		id: obsoleteAuthorizationDialog

		title: qsTr("Hm...")
		text: qsTr("Authorization is obsolete or revoked, please sign in once again")
		standardButtons: StandardButton.Ok
	}

	MessageDialog {
		id: incompleteAttachDialog

		title: qsTr("Ouch!")
		text: qsTr("Application cannot be opened within current session, please check internet connection and try again")
		standardButtons: StandardButton.Ok
	}

	MessageDialog {
		id: sessionErrorDialog

		title: qsTr("Session error")
		text: ""
		standardButtons: StandardButton.Ok

		onTextChanged: {
			open();
		}
	}

	MessageDialog {
		id: domainErrorDialog

		title: qsTr("Domain error")
		text: ""
		standardButtons: StandardButton.Ok

		onTextChanged: {
			open();
		}
	}

	MessageDialog {
		id: realmErrorDialog

		title: qsTr("Realm error")
		text: ""
		standardButtons: StandardButton.Ok

		onTextChanged: {
			open();
		}
	}

	Realm {
		id: realm

		synchronizationMode: Realm.Lazy

		onInitialized: {

		}
		onDeinitialized: {

		}
		onFaulted: {
			realmErrorDialog.text = message;
		}
	}

	Session {
		id: realmSession

		flags: Session.Default

		onRealized: {
			console.log("Session::onRealized");
		}
		onAuthorized: {
			console.log("Session::onAuthorized");
		}
		onDeclined: {
			console.log("Session::onDeclined");
			badAuthenticationDialog.open();
		}
		onFaulted: {
			console.log("Session::onFaulted");
			if (reject == Session.NetworkError) {
				if (Session.Prolongation === phase) {
					if (!realmDomain.autoProlongationMode && Domain.Unknown != realmDomain.authenticationState) {
						unavailableSessionDialog.open();
						realmDomain.autoProlongationMode = true;
					}
				} else if (Session.Verification === phase) {
					if (Domain.Active == realmDomain.authenticationState) {
						unavailableSessionDialog.open();
					}
				} else if (Session.Authentication === phase) {
					unavailableAuthenticationDialog.open();
				} else {
					sessionErrorDialog.text = message
				}
			} else {
				sessionErrorDialog.text = message ?? "Unknown error"
			}
		}
		onValidated: {
			console.log("Session::onValidated");
		}
		onRevalidated: {
			console.log("Session::onRevalidated");
		}
		onInvalidated: {
			console.log("Session::onInvalidated");
			if (realmDomain.authenticationState != Domain.Deactive) {
				invalidSessionDialog.open();
			}
		}
		onAttached: {
			if (fact !== Session.OpenComplete) {
				incompleteAttachDialog.open();
			}
		}
	}

	Domain {
		id: realmDomain

		onAuthenticationStateChanged: {
			if (state === Domain.Progress && realmDomain.authenticationState === Domain.Unknown) {
				badAuthenticationDialog.open();
			} else if (state === Domain.Active && realmDomain.authenticationState === Domain.Unknown) {
				obsoleteAuthorizationDialog.open();
			} else if (state === Domain.Reactive && realmDomain.authenticationState === Domain.Unknown) {
				/// @note Need an alert?!
			}
		}
		onOpened: {
			objectNavigator.model.open(descriptor);
			channelNavigator.model.open(descriptor);

			var navigator_index = realm.preference.getOption("navigator_index");
			if (navigator_index) {
				tabBar.defaultTabIndex = navigator_index;
			}
		}
		onExceptionThrew: {
			if ([Core.FatalMessage, Core.CriticalMessage].includes(level)) {
				domainErrorDialog.text = message;
			}
		}
	}

	StackView {
		id: applicationStack

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		initialItem: Page {
			id: navigatorView

			Component.onCompleted: {
				realmDomain.open(realmSession, realm);
			}

			Component.onDestruction: {
				realm.preference.setOption("navigator_index", tabBar.currentIndex);
			}

			footer: TabBar {
				id: tabBar

				currentIndex: 0
				onCurrentIndexChanged: {
					realm.preference.setOption("navigator_index", currentIndex);
				}

				property int defaultTabIndex: -1
				onDefaultTabIndexChanged: {
					if (defaultTabIndex >= 0) {
						tabBar.currentIndex = defaultTabIndex;
					}
				}

				anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.right: parent.right
				bottomPadding: 5

				background: Rectangle {
					anchors.fill: parent
					color: Material.color(Material.Grey, Material.Shade100)

					Rectangle {
						id: bottomRectangle
						width: parent.width
						height: 1
						anchors.bottom: parent.top
						color: "transparent"
						border.color: realmDomain.authenticationState == Domain.Fault ? Material.color(Material.Red) : realmDomain.syncState ? Material.accent : Material.color(Material.Grey)

						transform: Scale {
							id: heartbeatScale
							origin.x: bottomRectangle.x
							origin.y: bottomRectangle.y
							yScale: 1
							xScale: 1
						}

						SequentialAnimation {
							running: (realmDomain.authenticationState == Domain.Progress || realmDomain.authenticationState == Domain.Reactive || realmDomain.syncState)
							alwaysRunToEnd: true
							loops: Animation.Infinite

							PropertyAnimation {
								target: heartbeatScale
								property: "xScale"
								from: 1
								to: 0
								duration: 2000
								easing.type: Easing.InCubic
							}

							PropertyAnimation {
								target: heartbeatScale
								property: "xScale"
								from: 0
								to: 1
								duration: 1000
								easing.type: Easing.InOutCubic
							}
						}
					}
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					display: AbstractButton.TextUnderIcon
					icon.source: realmSession.isActive ? "/Repository/Asset/Image/DashboardGo.svg" : "/Repository/Asset/Image/DashboardStop.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: realmSession.isActive
					display: AbstractButton.TextUnderIcon
					icon.source: "/Repository/Asset/Image/SubjectNavigator.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: realmSession.isActive
					display: AbstractButton.TextUnderIcon
					icon.source: "/Repository/Asset/Image/ObjectNavigator.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: realmSession.isActive
					display: AbstractButton.TextUnderIcon
					icon.source: "/Conversation/Asset/Image/ChatNavigator.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: realmSession.isActive
					display: AbstractButton.TextUnderIcon
					icon.source: "/ERP/Asset/Image/DocumentNavigator.svg"
				}
			}

			StackLayout {
				id: navigatorStack

				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom

				currentIndex: realmSession.isActive ? tabBar.currentIndex : 0

				Dashboard {
					id: dashboardView

					Layout.fillWidth: true
					Layout.fillHeight: true

					domain: realmDomain

					onComponentSelected: {
						stackComponent(item, properties);
					}
				}

				Page {
					id: subjectNavigator

					Label {
						text: "Subject navigator is coming soon..."

						color: Material.color(Material.Grey)
						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter
						anchors.centerIn: parent
					}
				}

				ObjectNavigator {
					id: objectNavigator

					model.objectProxy.preferredIndexes: (realmDomain.assemblyMethod === Domain.Lazy) ? Repository.NetCursor : (Repository.DataRecord | Repository.NetCursor)

					Layout.fillWidth: true
					Layout.fillHeight: true

					onObjectSelected: {
						stackObject(id, render);
					}
					onComponentSelected: {
						stackComponent(item, properties);
					}
				}

				ChannelNavigator {
					id: channelNavigator

					model.objectProxy.preferredIndexes: (realmDomain.assemblyMethod === Domain.Lazy) ? Repository.NetCursor : (Repository.DataRecord | Repository.NetCursor)

					Layout.fillWidth: true
					Layout.fillHeight: true

					Component {
						id: channelComponent

						ChannelFeed {
							headerTitle: qsTr("Chat")
							headerSubtitle: "§%1: %2".arg(model.descriptor.objectId).arg(qsTr("3 members"))
							headerShield: ChannelShield {
								menuModeSet: ChannelFeed.MenuMode.Collapse | ChannelFeed.MenuMode.Profile | ChannelFeed.MenuMode.Party | ChannelFeed.MenuMode.Media
							}
						}
					}

					onChannelSelected: {
						var view = applicationStack.push(channelComponent, {}, StackView.PushTransition);
						if (view) {
							view.model = model;
						}
					}
					onComponentSelected: {
						stackComponent(item, properties);
					}
				}

				Page {
					id: documentNavigator

					Label {
						text: "Document navigator is coming soon..."

						color: Material.color(Material.Grey)
						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter
						anchors.centerIn: parent
					}
				}
			}
		}
	}

	function stackDescriptor(descriptor, property_map = {})
	{
		var render = SchemeFactory.load("Repository.Meta.ApplicationRender", descriptor.objectApplicationScheme);
		var component = Qt.createComponent(render.makeURL());
		if (component) {
			var view = applicationStack.replace(navigatorView, component, property_map, StackView.PushTransition);
			if (view) {
				view.model.open(descriptor);
				return true;
			} else {
				/// @todo error dialog
				console.log("Could not push Component to the StackView");
			}
		} else {
			/// @todo error dialog
			console.log("Could not create Component for scheme:", descriptor.objectApplicationScheme);
		}
		return false;
	}

	function stackApplication(model, property_map = {})
	{
		var render = SchemeFactory.load("Repository.Meta.ApplicationRender", model.descriptor.objectApplicationScheme);
		var component = Qt.createComponent(render.makeURL());
		if (component) {
			var view = applicationStack.push(component, property_map, StackView.PushTransition);
			if (view) {
				view.model = model;
				return true;
			} else {
				/// @todo error dialog
				console.log("Could not push Component to the StackView");
			}
		} else {
			/// @todo error dialog
			console.log("Could not create Component for scheme:", model.descriptor.objectApplicationScheme);
		}
		return false;
	}

	function stackObject(id, render, property_map = {})
	{
		var component = Qt.createComponent(render.makeURL());
		if (component) {
			var view = applicationStack.push(component, property_map, StackView.PushTransition);
			if (view) {
				return realmSession.attach(view.model, id, true);
			} else {
				console.log("Could not push Component to the StackView");
			}
		} else {
			/// @todo error dialog
			console.log("Could not create Component for scheme:", render.application_scheme);
		}
		return false;
	}

	function stackComponent(item, property_map = {})
	{
		applicationStack.push(item, property_map, StackView.PushTransition);
		return true;
	}
}
