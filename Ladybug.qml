import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQml.Models 2.15

import Core.Global 0.1
import Core.View 0.1
import Core.Meta 0.1
import Core.System 0.1

import Stress.Global 0.1
import Stress.System 0.1

import "Core/View"

ApplicationWindow {
	id: applicationWindow

	Component.onCompleted: {
		if (getBootMode() === "test-all-model") {
			testThread.start(StressExtension.checkList, false);
		}
	}

	onClosing: {
		if (Native.Desktop === Native.deviceType) {
			Qt.quit();
		}
	}

	title: qsTr("Ladybug")
	visible: true
	flags: (Native.Desktop === Native.deviceType) ? Qt.Window : (Qt.Window | Qt.MaximizeUsingFullscreenGeometryHint)
	width: WindowManager.defaultSize.width
	height: WindowManager.defaultSize.height

	Material.theme: Material.Light
	Material.accent: Material.Blue
	Material.primary: Material.LightBlue

	function addInteger(key)
	{
		StressExtension.vary(key, 0);
	}

	function addDouble(key)
	{
		StressExtension.vary(key, 0.0);
	}

	function addString(key)
	{
		StressExtension.vary(key, "text");
	}

	function addRepository(uri)
	{
		if (!StressExtension.attach(uri, true, true)) {
			Alert.action("TEST_REPO_ATTACH_ERROR", qsTr("Hm..."), qsTr("Unknown type of repository"), AlertOption.Ok, ActionAlert.HeadFull);
		} else {
			Alert.action("TEST_REPO_ATTACH_SUCCESS", qsTr("Done"), qsTr("Repository was successfully attached"), AlertOption.Ok, ActionAlert.HeadFull);
		}
	}

	function addReplica(uri)
	{
		if (!StressExtension.replicate(uri, true, true)) {
			Alert.action("LOG_REPLICA_ATTACH_ERROR", qsTr("Hm..."), qsTr("Unknown type of server"), AlertOption.Ok, ActionAlert.HeadFull);
		} else {
			Alert.action("LOG_REPLICA_ATTACH_SUCCESS", qsTr("Done"), qsTr("Server was successfully attached"), AlertOption.Ok, ActionAlert.HeadFull);
		}
	}

	function addDebugger(uri)
	{
		if (!StressExtension.debug(uri)) {
			Alert.action("QML_DEBUGGER_OFF", qsTr("Done"), qsTr("Debugger will be shutdown next boot"), AlertOption.Ok, ActionAlert.HeadFull);
		} else {
			Alert.action("QML_DEBUGGER_ON", qsTr("Done"), qsTr("Debugger will be run next boot"), AlertOption.Ok, ActionAlert.HeadFull);
		}
	}

	footer: TabBar {
		id: tabBar

		currentIndex: 0

		TabButton {
			bottomPadding: WindowManager.bottomSafeInset

			display: AbstractButton.TextUnderIcon
			icon.source: "/Stress/Asset/Image/EnvConfig.svg"
		}

		TabButton {
			bottomPadding: WindowManager.bottomSafeInset

			enabled: !testThread.isRunning
			display: AbstractButton.TextUnderIcon
			icon.source: enabled ? "/Stress/Asset/Image/FlyStop.svg" : "/Stress/Asset/Image/FlyGo.svg"

			onDoubleClicked: {
				testThread.test();
			}

			onPressAndHold: {
				testThread.test();
			}
		}

		TabButton {
			bottomPadding: WindowManager.bottomSafeInset

			display: AbstractButton.TextUnderIcon
			icon.source: "/Stress/Asset/Image/LogFeed.svg"
		}
	}

	StackLayout {
		id: navigatorStack

		anchors.fill: parent

		currentIndex: tabBar.currentIndex

		Page {
			header: StackBar {
				id: environmentStackBar

				titleText: qsTr("Environment")

				optionStyle: StackBar.OptionStyle.Stack
				onOptionClicked: {
					if (environmentOptionMenu.visible) {
						environmentOptionMenu.close();
					} else {
						environmentOptionMenu.open();
					}
				}

				Menu {
					id: environmentOptionMenu

					parent: environmentStackBar.optionButton
					topMargin: environmentStackBar.optionButton.height

					MenuItem {
						text: qsTr("Add integer")
						enabled: true

						onTriggered: {
							environmentOptionMenu.close();
							var input_alert = Alert.input("INTEGER_ENV_VAR", qsTr("Integer variable"), "Name", InputAlert.CancelOption | InputAlert.HeadFull);
							input_alert.completed.connect(function() { applicationWindow.addInteger(input_alert.text); } );
						}
					}
					MenuItem {
						text: qsTr("Add double")
						enabled: true

						onTriggered: {
							environmentOptionMenu.close();
							var input_alert = Alert.input("DOUBLE_ENV_VAR", qsTr("Double variable"), "Name", InputAlert.CancelOption | InputAlert.HeadFull);
							input_alert.completed.connect(function() { applicationWindow.addDouble(input_alert.text); } );
						}
					}
					MenuItem {
						text: qsTr("Add string")
						enabled: true

						onTriggered: {
							environmentOptionMenu.close();
							var input_alert = Alert.input("STRING_ENV_VAR", qsTr("String variable"), "Name", (InputAlert.CancelOption | InputAlert.HeadFull));
							input_alert.completed.connect(function() { applicationWindow.addString(input_alert.text); });
						}
					}
				}
			}

			ListView {
				id: environmentListView

				anchors.fill: parent
				anchors.leftMargin: 10
				anchors.rightMargin: 10

				model: environmentModel

				delegate: Row {
					/// @todo разные делегаты под разные типы переменных рабочего окружения

					height: 40
					width: parent.width
					spacing: 10

					leftPadding: 5

					/// @todo имя переменной вынести в секцию так, чтобы была возможность занять всю ширину
					Text {
						id: valueText

						width: parent.width / 2
						anchors.verticalCenter: parent.verticalCenter

						text: variable
						elide: Text.ElideRight
					}

					TextField {
						id: valueTextField

						width: parent.width - valueText.width - parent.spacing
						anchors.verticalCenter: parent.verticalCenter

						text: value
						placeholderText: qsTr("Value")

						onEditingFinished: {
							StressExtension.vary(variable, valueTextField.text);
						}
					}
				}
			}
		}

		Page {
			focus: true
			header: StackBar {
				id: modelStackBar

				titleText: qsTr("Models")

				optionStyle: StackBar.OptionStyle.Stack
				optionButton.enabled: !testThread.isRunning
				onOptionClicked: {
					var menu = (modelStackView.empty) ? modelRepositoryOptionMenu : modelTestOptionMenu;
					return menu.visible ? menu.close() : menu.open();
				}

				Menu {
					id: modelTestOptionMenu

					parent: modelStackBar.optionButton
					topMargin: modelStackBar.optionButton.height

					MenuItem {
						text: qsTr("Close")
						//enabled: undefined != modelSwipeView.currentItem

						onTriggered: {
							modelTestOptionMenu.close();
							modelStackView.clear(StackView.PopTransition);
						}
					}
				}

				Menu {
					id: modelRepositoryOptionMenu

					parent: modelStackBar.optionButton
					topMargin: modelStackBar.optionButton.height

					MenuItem {
						text: qsTr("Go!")
						//enabled: modelListView.count() > 0

						onTriggered: {
							modelRepositoryOptionMenu.close();
							testThread.test();
						}
					}
					MenuSeparator {

					}
					MenuItem {
						text: qsTr("QML Debugger")
						enabled: true

						onTriggered: {
							modelRepositoryOptionMenu.close();
							var input_alert = Alert.input("QML_DEBUGGER_URI", qsTr("Debugger options"), "port:12150", InputAlert.CancelOption | InputAlert.HeadFull);
							input_alert.completed.connect(function() { applicationWindow.addRepository(input_alert.text); });
						}
					}
					MenuItem {
						text: qsTr("Attach")
						enabled: true

						onTriggered: {
							modelRepositoryOptionMenu.close();
							var input_alert = Alert.input("TEST_REPO_URI", qsTr("Remote repository"), "webdav://username:password@minibagroff.cadabra:8080/dav/", InputAlert.CancelOption | InputAlert.HeadFull);
							input_alert.completed.connect(function() { applicationWindow.addRepository(input_alert.text); });
						}
					}
				}
			}

			StackView {
				id: modelStackView

				anchors.fill: parent

				focus: true

				ListView {
					id: modelListView

					anchors.fill: parent

					property var sectionMap: new Map()
					property var collapsed: ({})

					readonly property var icoList: ["🔬", "🔌", "⚙️", "💀", "👮", "🛠", "🚲"]

					Component {
						id: sectionButtonGroupComponent

						ButtonGroup {
							exclusive: false
						}
					}

					function isSectionExpanded(section)
					{
						return !(section in collapsed);
					}

					function showSection(section)
					{
						delete collapsed[section];
						/*emit*/ collapsedChanged();
					}

					function hideSection(section)
					{
						collapsed[section] = true;
						/*emit*/ collapsedChanged();
					}

					function toggleSection(section)
					{
						if (isSectionExpanded(section)) {
							hideSection(section);
						} else {
							showSection(section);
						}
					}

					function groupSection(section)
					{
						if (modelListView.sectionMap.has(section)) {
							return modelListView.sectionMap.get(section);
						}
						var retval = sectionButtonGroupComponent.createObject(modelListView);
						modelListView.sectionMap.set(section, retval);
						return retval;
					}

					focus: false
					snapMode: ListView.NoSnap

					model: StressExtension ? StressExtension.checkList : null
					delegate: Row {
						id: itemDelegate

						property bool expanded: modelListView.isSectionExpanded(module_name)

						state: status_state
						spacing: modelListView.width - (checkBox.width + textState.width + 10)

						width: modelListView.width
						height: expanded ? 40 : 0
						Behavior on height {
							NumberAnimation { duration: 300 }
						}
						onHeightChanged: {
							if (expanded) {
								firstOpacityAnimator.start();
							} else {
								secondOpacityAnimator.start();
							}
						}

						OpacityAnimator {
							id: firstOpacityAnimator

							target: itemDelegate
							from: 0
							to: 1
							duration: 500
						}

						OpacityAnimator {
							id: secondOpacityAnimator

							target: itemDelegate
							from: 1
							to: 0
							duration: 500
						}

						CheckBox {
							id: checkBox

							readonly property string ico: variation > modelListView.icoList.length ? "❓" : modelListView.icoList[variation - 1]

							ButtonGroup.group: modelListView.groupSection(factory_alias)

							width: modelListView.width - textState.implicitWidth
							anchors.verticalCenter: parent.verticalCenter
							enabled: !testThread.schemeModel
							checked: check_state
							text: "%1 %2\\%3".arg(ico).arg(module_display_name).arg(model_name)

							contentItem: Text {
								leftPadding: checkBox.indicator.width + checkBox.spacing
								text: checkBox.text
								font: checkBox.font
								opacity: enabled ? 1.0 : 0.3
								verticalAlignment: Text.AlignVCenter
								elide: Text.ElideRight
							}

							onCheckedChanged: {
								if (check_state !== checked) {
									check_state = checked;
								}
							}

							onPressAndHold: {
								if (Core.General !== factory) {
									return;
								} else if (Core.QML === dialect) {
									modelStackView.push
									(
										module_absoulte_path + "/" + model_file_name,
										{
											focus: true
										},
										StackView.PushTransition
									);
								}
							}
						}

						states: [
							State {
								name: "ready"
								PropertyChanges {
									target: textState
									text: "READY"
									color: "black"
								}
							},

							State {
								name: "busy"
								PropertyChanges {
									target: textState
									text: "BUSY"
									color: "orange"
								}
							},

							State {
								name: "pass"
								PropertyChanges {
									target: textState
									text: "PASS"
									color: "green"
								}
							},

							State {
								name: "fail"
								PropertyChanges {
									target: textState
									text: "FAIL"
									color: "red"
								}
							}
						]

						Text {
							id: textState

							anchors.verticalCenter: parent.verticalCenter

							MouseArea {
								id: resultArea

								anchors.fill: parent
								acceptedButtons: Qt.LeftButton

								onClicked: {
									if ((textState.text !== "SKIP") && (textState.text !== "READY")) {
										logText.testIndex = index;
										tabBar.currentIndex = 2;
									}
								}
							}
						}
					}
					section {
						property: "factory_alias"
						criteria: ViewSection.FullString

						delegate: SwipeDelegate {
							required property string section

							height: 40
							width: parent.width

							background: Rectangle {
								color: "lightGrey"
							}

							contentItem: Rectangle {
								id: sectionRectangle

								color: "transparent"

								CheckBox {
									id: sectionCheckBox

									anchors.leftMargin: 4
									anchors.left: parent.left
									anchors.verticalCenter: parent.verticalCenter

									enabled: !testThread.isRunning

									Binding {
										property ButtonGroup buttonGroup: modelListView.groupSection(section)

										target: sectionCheckBox
										property: "checkState"
										value: buttonGroup.checkState
									}

									Binding {
										target: modelListView.groupSection(section)
										property: "checkState"
										value: sectionCheckBox.checkState
									}
								}

								Text {
									anchors.left: sectionCheckBox.right
									anchors.right: parent.right

									anchors.verticalCenter: parent.verticalCenter

									rightPadding: 2
									font: sectionCheckBox.font

									text: section
									elide: Text.ElideMiddle
									horizontalAlignment: Text.AlignLeft
									verticalAlignment: Text.AlignVCenter
								}
							}

							swipe.right: Label {
								id: sectionLabel

								anchors.right: parent.right

								height: parent.height
								padding: 4

								text: qsTr("Dettach")
								color: "white"
								verticalAlignment: Label.AlignVCenter

								background: Rectangle {
									color: sectionLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
								}

								SwipeDelegate.onClicked: {
									if (!StressExtension.attach(parent.section, true, false)) {
										Alert.action("DETACH_TEST_REPO_ERROR", "Ah!", qsTr("Could not detach repository"), AlertOption.Ok, ActionAlert.HeadFull);
									}
									swipe.close();
								}

								MouseArea {
									anchors.fill: parent

									onClicked: sectionLabel.SwipeDelegate.clicked()
								}
							}
						}
					}
				}
			}

			TestThread {
				id: testThread

				function test()
				{
					if (isRunning) {
						return false;
					}

					StressExtension.logList.clear();
					logText.testIndex = -1;
					return (start(StressExtension.checkList, true) > 0);
				}

				onRan: {
					StressExtension.checkList.setStatusState(index, "busy");
				}

				onCompleted: {
					StressExtension.checkList.setStatusState(index, result ? "pass" : "fail");
				}

				onLogged: {
					StressExtension.logList.flush(message);
				}
			}
		}

		Page {
			header: StackBar {
				id: logsStackBar

				titleText: qsTr("Logs")

				optionStyle: StackBar.OptionStyle.Stack
				onOptionClicked: {
					if (logOptionMenu.visible) {
						logOptionMenu.close();
					} else {
						logOptionMenu.open();
					}
				}

				Menu {
					id: logOptionMenu

					parent: logsStackBar.optionButton
					topMargin: logsStackBar.optionButton.height

					MenuItem {
						text: qsTr("Filter reset")
						enabled: logText.testIndex != -1

						onTriggered: {
							logText.testIndex = -1;
						}
					}

					MenuItem {
						text: qsTr("Copy to clipboard")
						enabled: logText.text.length > 0

						onTriggered: {
							Qt.inputMethod.commit();
							Native.copyToClipboard(logText.text);
						}
					}

					MenuItem {
						text: qsTr("Truncate")
						enabled: logText.text.length > 0

						onTriggered: {
							testThread.truncateLog();
							StressExtension.logList.clear();
						}
					}

					Menu {
						title: qsTr("Replicas")
						enabled: true

						MenuItem {
							text: "Add"

							onTriggered: {
								logOptionMenu.close();
								var input_alert = Alert.input("LOG_REPLICA_URI", "Address", "tcp://10.10.1.3:1234", InputAlert.CancelOption | InputAlert.HeadFull);
								input_alert.completed.connect(function() { applicationWindow.addReplica(input_alert.text); } );
							}
						}
						MenuSeparator {
						}
						Repeater {
							model: StressExtension ? StressExtension.logList : null

							delegate: MenuItem {
								text: alias
								onTriggered: {
									if (!StressExtension.replicate(endpoint, true, false)) {
										logOptionMenu.close();
										Alert.action("LOG_REPLICA_DETACH_ERROR", "Ah!", qsTr("Could not detach log server"), AlertOption.Ok, ActionAlert.HeadFull);
									}
								}
							}
						}
					}
				}
			}

			ScrollView {
				anchors.fill: parent

				Text {
					id: logText

					property int testIndex: -1

					anchors.fill: parent
					padding: 4
					text: testIndex < 0 && StressExtension ? StressExtension.logList.cache : testThread.fetchLog(testIndex);
				}
			}
		}
	}
}
