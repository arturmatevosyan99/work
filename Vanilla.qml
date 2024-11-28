import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.Global 0.1
import Core.View 0.1
import Core.Meta 0.1
import Core.System 0.1

import "Repository/View"
import Repository.Global 0.1
import Repository.Model 0.1
import Repository.View 0.2

import "Conversation/View"
import "Conversation/View/Application"
import Conversation.View 0.1

ApplicationWindow {
	id: applicationWindow

	onClosing: {
		if (Native.Desktop === Native.deviceType) {
			Qt.quit();
		}
	}

	title: qsTr("Vanilla")
	visible: true
	flags: (Native.Desktop === Native.deviceType) ? Qt.Window : (Qt.Window | Qt.MaximizeUsingFullscreenGeometryHint)
	width: WindowManager.defaultSize.width
	height: WindowManager.defaultSize.height

	/// @todo Use Core.Theme as source
	Material.theme: Material.Light
	Material.accent: Material.Blue
	Material.primary: Material.LightBlue

/*
	objectNavigator.model.open(descriptor);
	channelNavigator.model.open(descriptor);

	var navigator_index = realm.preference.getOption("navigator_index");
	if (navigator_index) {
		tabBar.defaultTabIndex = navigator_index;
	}
*/

	StateIsland {
		alert: Alert
		interval: 5000
	}

	Avatar {
		id: avatar

		session.flags: Session.StubbornProlongation

		/**
		 * @brief Удалось подтвердить валидность сессии
		 * @param cold(bool) Холодная(сразу после старта qApp) или Горячая(в процессе работы qApp) валидации
		 */
		session.onValidated: {
			if (cold) {
//				relaunch();
			} else {
				/// @note По идее, валидировали все на (горячем) ходу и ничего не изменилось, то есть все открытые апплеты по-прежнему актуальны
			}
		}

		/**
		 * @brief Сессия была обновлена, поэтому старые сервлеты утеряны, а локальные больше не актуальны
		 * @param cold(bool) Холодная(сразу после старта qApp) или Горячая(в процессе работы qApp) валидации
		 */
		session.onRevalidated: {
			if (cold) {
//				relaunch();
			} else {
				/// @todo По идее, все открытые апплеты нужно закрыть и попробовать открыть последний(не дефолтный) апплет еще раз
				/// @todo Показать перед закрытием/открытием ActionAlert
			}
		}

		/**
		 * @brief Сессия утеряла актуальность
		 */
		session.onInvalidated: {
			Alert.action
			(
				"SESSION_INVALID",
				qsTr("Error"),
				qsTr("Session is obsolete or invalid, need to sign in into account again"),
				AlertOption.Ok,
				ActionAlert.Default,
				DialogAlert.Warning
			);
//			stackView.replace(null, authorizationQuizComponent, {"avatar": avatar}, StackView.Transition);
		}

		/**
		 * @brief Attach-операция завершена
		 * @param application(AbstractApplication) Модель приложения
		 * @param fact(AttachFact) Чем завершился attach Applet к Servlet
		 */
		session.onAttached: {
			if (Session.OpenComplete !== fact) {
				Alert.action
				(
					"SESSION_ATTACH",
					qsTr("Error"),
					qsTr("Application cannot be opened within current session, please check internet connection and try again"),
					AlertOption.Ok,
					ActionAlert.Default,
					DialogAlert.Error
				);
			}
		}

		realm.synchronizationMode: Realm.Lazy

		/**
		 * @brief Похоже, начинаем все с самого начала...
		 */
		realm.onDeinitialized: {
//			stackView.replace(null, splashComponent, null, StackView.Transition);
		}

		/**
		 * @brief Несовместимость client-server версий
		 */
		realm.endpoint.onOutdated: {
			if (Endpoint.Client === side) {
				/// @note Endpoint не поддерживает такую старую версию нас — нужно предложить сделать апдейт
				var popup = stackView.replace(null, popupAlertComponent, {"action": qsTr("Update"), "flags": PopupAlert.Flag.None}, StackView.Transition);
				popup.confirmed.connect(Native.runApplicationUpdate); /// @todo Just go to AppStore or PlayMarket
				popup.show(qsTr("Version is outdated, please update this application to continue"));
			} else {
				/// @note Endpoint не поддерживает такую новую версию нас - game over
				var popup = stackView.replace(null, popupAlertComponent, {"action": qsTr("Sign Out"), "flags": PopupAlert.Flag.None}, StackView.Transition);
				popup.confirmed.connect(avatar.session.signOut);
				popup.show(qsTr("Connection has been rejected because endpoint does not support this version"));
			}
		}

/*
		endpoints: [
			Endpoint {
				url: "aces://realm13.cadabra.systems"
				account: Endpoint.Email
				regexp: "*.?@(t.est|apple.com|@google.com|@huawei.com|@microsoft.com)"
			}
		]
*/
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
//				domain.open(session, realm);
			}

			Component.onDestruction: {
				avatar.realm.preference.setOption("navigator_index", tabBar.currentIndex);
			}

			footer: TabBar {
				id: tabBar

				currentIndex: 0
				onCurrentIndexChanged: {
					avatar.realm.preference.setOption("navigator_index", currentIndex);
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
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					display: AbstractButton.TextUnderIcon
					icon.source: avatar.session.isActive ? "/Repository/Asset/Image/DashboardGo.svg" : "/Repository/Asset/Image/DashboardStop.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: avatar.session.isActive
					display: AbstractButton.TextUnderIcon
					icon.source: "/Repository/Asset/Image/SubjectNavigator.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: avatar.session.isActive
					display: AbstractButton.TextUnderIcon
					icon.source: "/Repository/Asset/Image/ObjectNavigator.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: avatar.session.isActive
					display: AbstractButton.TextUnderIcon
					icon.source: "/Conversation/Asset/Image/ChatNavigator.svg"
				}

				TabButton {
					bottomPadding: WindowManager.bottomSafeInset

					enabled: avatar.session.isActive
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

				currentIndex: avatar.session.isActive ? tabBar.currentIndex : 0

				Dashboard {
					id: dashboardView

					Layout.fillWidth: true
					Layout.fillHeight: true

					userAvatar: avatar

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

					model.objectProxy.preferredIndexes: Repository.NetCursor

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

					model.objectProxy.preferredIndexes: Repository.NetCursor

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
				return avatar.session.attach(view.model, id, true);
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
