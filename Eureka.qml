import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15
import QtQml.Models 2.15

import Qaos 0.1

import Core.System 0.1
import Core.Meta 0.1
import Core.Model 0.1
import Core.View 0.1

import Repository.Meta 0.1
import Repository.Model 0.1
import Repository.View 0.1

import Conversation.Model 0.1
import Conversation.View 0.1

import Mentalgami.Meta 0.1
import Mentalgami.Model 0.1
import Mentalgami.View 0.1

import "Core/View"

import "Conversation/View"
import "Conversation/View/Application"

import "Mentalgami/View"
import "Mentalgami/View/Application"

/// @todo Выкинуть (после замены Splash)
import "DrOblozhko/View"

ApplicationWindow {
	id: applicationWindow

	onClosing: {
		if (Native.Desktop == Native.deviceType) {
			Qt.quit();
		}
	}

	title: qsTr("Smart Care")
	visible: true
	flags: Qt.Window | Qt.MaximizeUsingFullscreenGeometryHint

	Material.theme: Material.Light
	Material.accent: Material.Blue /// < #2196F3
	Material.primary: Material.LightBlue /// < #03A9F4
//	Material.foreground: default font
//	Material.background: default background

	readonly property var defaultApplicationScheme: SchemeFactory.ident("Repository.Meta.ApplicationRender", "Mentalgami", "SmartChannel")

	/**
	 * @brief Первичный запуск дефолтного апплета(SmartChannel)
	 * Производится поиск по домашней директории и выполняется попытка запустить SmartChannel.
	 * Логично вызывать, например, после успешного прохождения AuthorizationQuiz (в результате индексирования домашней директории).
	 */
	function launch()
	{
		avatar.home.indexed.disconnect(applicationWindow.launch);
		if (defaultAppletScheme) {
			var default_application_scheme = applicationWindow.defaultApplicationScheme.toString();

			var directory_model = avatar.home.indexModel;
			if (directory_model) {
				for (var i = 0; i < directory_model.rowCount(); i++) {
					var index = directory_model.index(i, 0);
					var application_scheme = directory_model.data(index, FolderIndex.ApplicationScheme);
					var object_id = directory_model.data(index, FolderIndex.Id);

					if (application_scheme === default_application_scheme) {
						stack(object_id, application_scheme);
						return true;
					}
				}
			}
		}

		/// @??? А что дальше
		Alert.action(
						"DEFAULT_APPLICATION_MISSED",
						qsTr("Hm..."),
						qsTr("Default application was not found. Please, contact customer service."),
						AlertOption.Ok,
						ActionAlert.Default,
						DialogAlert.Error
		);
		return false;
	}

	/**
	 * @brief Повторный запуск дефолтного апплета (после перезапуска авторизованного queen)
	 * Производится поиск по истории сессии и в зависимости от того, что было открыто ранее,
	 * выполняется попытка запустить либо SmartChannel, либо WelcomeQuiz
	 *
	 * @todo Нужна возможность запуска последнего запущенного, а не только дефолтного (при hot revalidation)
	 */
	function relaunch()
	{
		if (applicationWindow.defaultAppletScheme) {
			var default_application_scheme = applicationWindow.defaultApplicationScheme.toString();

			var history_model = avatar.session.sliceHistory();
			if (history_model) {
				for (var i = 0; i < history_model.rowCount(); i++) {
					var index = history_model.index(i, 0);

					var application_scheme = history_model.data(index, "application_scheme").toString();
					var object_id = history_model.data(index, "object_id");

					if (application_scheme === default_application_scheme) {
						stack(object_id, application_scheme);
						return true;
					}
				}
			}
		}

		/// @??? Почему-так
		avatar.session.signOut();
		return false;
	}

	/**
	 * @brief Открыть приложение на стэке
	 */
	function stack(id, scheme, component_parameter = {}, application_option = {})
	{
		var render = SchemeFactory.load("Repository.Meta.ApplicationRender", scheme);
		if (render) {
			var component = Qt.createComponent(render.makeURL());
			if (component) {
				var view = stackView.replace(component, component_parameter, StackView.PushTransition);
				if (view) {
					return avatar.session.attach(view.model, id, true, application_option);
				}
				console.log("Eureka::stack() > Could not push Component to the StackView");
			} else {
				console.log("Eureka::stack() > Could not create Component for scheme:", scheme);
			}
		} else {
			console.error("Eureka::stack() > Application render for the scheme `", scheme,"` not found");
		}
		return false;
	}

	Component {
		id: popupAlertComponent

		PopupAlert {
			id: popupAlert
		}
	}

	Component {
		id: splashComponent

		/// @todo Нужен эквивалент из Metabolic-расширения и после этого избавиться от import DrOblozhko
		Splash {
		}
	}

	Component {
		id: authorizationQuizComponent

		AuthorizationQuiz {
			id: authorizationQuiz

			focus: true

			onWelcomed: {
				authorizationQuiz.avatar.realm.preference.setOption("queen_eureka_welcomequiz_object_id", qd);
			}

			onFinalized: {
				avatar.home.indexed.connect(applicationWindow.launch);
				authorizationQuiz.avatar.pickUpProfile();
			}
		}
	}

	/**
	 * @brief Ветвление при "выходе" из аккаунта
	 * @todo Мигрировать на новое API(Core::System::Alert) и выкинуть реализацию AlertDialog, AlertState, AlertAction, etc.
	 */
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
					avatar.session.signOut(false);
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
					/// @todo Blured AlertPopup
					avatar.session.signOut(true);
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

	StateIsland {
		alert: Alert
		interval: 5000
	}

	Avatar {
		id: avatar

		realm.synchronizationMode: Realm.Lazy
		session.flags: Session.StubbornProlongation

		/**
		 * @brief Похоже, начинаем все с самого начала...
		 */
		realm.onDeinitialized: {
			stackView.replace(null, splashComponent, null, StackView.Transition);
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

		/**
		 * @brief Удалось подтвердить валидность сессии
		 * @param cold(bool) Холодная(сразу после старта qApp) или Горячая(в процессе работы qApp) валидации
		 */
		session.onValidated: {
			if (cold) {
				relaunch();
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
				relaunch();
			} else {
				/// @todo По идее, все открытые апплеты нужно закрыть и попробовать открыть последний(не дефолтный) апплет еще раз
				/// @todo Показать перед закрытием/открытием ActionAlert
			}
		}

		/**
		 * @brief Сессия утеряла актуальность
		 */
		session.onInvalidated: {
			Alert.action(
							"SESSION_INVALID",
							qsTr("Error"),
							qsTr("Session is obsolete or invalid, need to sign in into account again"),
							AlertOption.Ok,
							ActionAlert.Default,
							DialogAlert.Warning
			);
			stackView.replace(null, authorizationQuizComponent, {"avatar": avatar}, StackView.Transition);
		}

		/**
		 * @brief Attach-операция завершена
		 * @param application(AbstractApplication) Модель приложения
		 * @param fact(AttachFact) Чем завершился attach Applet к Servlet
		 */
		session.onAttached: {
			if (Session.OpenComplete !== fact) {
				Alert.action(
								"SESSION_ATTACH",
								qsTr("Error"),
								qsTr("Application cannot be opened within current session, please check internet connection and try again"),
								AlertOption.Ok,
								ActionAlert.Default,
								DialogAlert.Error
				);
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
		id: stackView

		focus: true

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		/**
		 * @brief Показываем при старте приложения Splash-экран и
		 * пытаемся воспользоваться(пролонгировать) существующей в преферансах сессией, если она есть,
		 * в противном случае меняем Splash-экран на Квиз с авторизацией
		 */
		Component.onCompleted: {
			stackView.push(splashComponent, null, StackView.Immediate);
			if (!avatar.reviveSession()) {
				stackView.replace(null, authorizationQuizComponent, {"avatar": avatar}, StackView.Transition);
			} else {
				/// @note По идее, нужно убирать Splash и выкатывать последний открытый апплет и если будет Session::faulted(), то подвесится AlertState
			}
		}
	}
}
