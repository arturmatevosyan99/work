import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

import Core.View 0.1
import Repository.Global 0.1
import Repository.View 0.1
import Conversation.Model 0.1

import "../../../Core/View"

import "../../../Repository/View"

Page {
	id: application

	signal channelSelected(var model)
	signal componentSelected(var item, var properties)
	property date currentDate: new Date()
	property Air model: Air {
		id: air
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	function formatTime(date)
	{
		if (Qt.formatDate(date, "dd.MM.yyyy") === Qt.formatDate(currentDate, "dd.MM.yyyy")) {
			return Qt.formatDateTime(new Date(date), "hh:mm")
		}
		return Qt.formatDateTime(new Date(date), "d MMM, hh:mm")
	}

	function colorString(str)
	{
		var i, hash = 0;
		for (i = 0; i < str.length; i++) {
			hash = str.charCodeAt(i) + ((hash << 5) - hash);
		}
		var color = '#';
		for (i = 0; i < 3; i++) {
		var value = (hash >> (i * 8)) & 0xFF;
			color += ('00' + value.toString(16)).substr(-2);
		}
		return color;
	}

	function colorVersa(hex)
	{
		/// @note If hex --> Convert it to RGB: http://gist.github.com/983661
		hex = +("0x" + hex.slice(1).replace(hex.length < 5 && /./g, '$&$&'));

		var r = hex >> 16;
		var g = hex >> 8 & 255;
		var b = hex & 255;
		var hsp = Math.sqrt(0.299 * (r * r) + 0.587 * (g * g) +	0.114 * (b * b));

		if (hsp > 127.5) { /// < light
			return "#000000";
		} else { /// < dark
			return "#FFFFFF";
		}
	}

	header: StackBar {
		id: stackBar

		focus: true

		titleText: qsTr("Chats")
		subtitleText: qsTr("%1 in the list").arg(listView.count)

		accentColor: application.model.objectModel.isLive ? Material.accentColor : Material.color(Material.Red)

		beatMode: application.model.objectModel.isReady ? StackBar.BeatMode.Stable : StackBar.BeatMode.Pulse

		optionStyle: StackBar.OptionStyle.Stack
		onOptionClicked: {
			if (optionMenu.visible) {
				optionMenu.close();
			} else {
				optionMenu.open();
			}
		}

		Menu {
			id: optionMenu
			parent: stackBar.optionButton
			topMargin: stackBar.optionButton.height

			MenuItem {
				text: qsTr("Close")
				enabled: true

				onTriggered: {
					optionMenu.close();
				}
			}

			MenuItem {
				text: qsTr("Newsletter")
				enabled: listView.count > 0

				onTriggered: {
					newsletterPopup.open();
				}

				Popup {
					id: newsletterPopup

					property var job: null
					onJobChanged: {
						if (null !== job) {
//							target == newsletterPopup.job
						}
					}

					onOpened: {
						listView.enabled = false;
						Qt.callLater(newsletterTextArea.forceActiveFocus);
					}

					onClosed: {
						listView.enabled = true;
					}

					parent: Overlay.overlay
					x: 5
					y: WindowManager.topSafePadding
					width: application.width - 10
					height: 240
					focus: false

					Connections {
						target: newsletterPopup.job
						enabled: null !== newsletterPopup.job
						function onStateChanged() {
							if (Repository.PlanJobState === newsletterPopup.job.state) {
								newsletterPopup.close();
							}
							newsletterPopup.job = null;
						}
					}

					FocusScope {
						anchors.fill: parent

						ScrollView {
							id: newsletterScrollView

							anchors.top: parent.top
							anchors.left: parent.left
							anchors.right: parent.right
							anchors.bottom: newsletterButton.top
							anchors.bottomMargin: 5

							TextArea {
								id: newsletterTextArea

								property bool empty: preeditText.length === 0 && text.length === 0

								x: 0
								y: 0
								width: parent.width
								height: parent.height
								placeholderText: qsTr("Message text")
								verticalAlignment: TextEdit.AlignTop
								wrapMode: TextArea.Wrap
								focus: true
								text: ""
							}
						}

						Button {
							id: newsletterButton

							Material.elevation: 0
							Material.background: Material.Blue
							Material.foreground: Material.Grey

							anchors.bottom: parent.bottom
							anchors.left: parent.left
							anchors.right: parent.right

							states: [
								State {
									when: newsletterPopup.job !== null
									PropertyChanges {
										target: newsletterButton
										enabled: false
										text: qsTr("Launching a remote job...")
									}
								},
								State {
									when: newsletterPopup.job === null
									PropertyChanges {
										target: newsletterButton
										enabled: !newsletterTextArea.empty
										text: listView.count > 1 ? qsTr("Send to all") : qsTr("Send")
									}
								}
							]

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
							onClicked: {
								Qt.inputMethod.commit();
								if (newsletterTextArea.text.length > 0) {
									newsletterPopup.job = air.spread(newsletterTextArea.text);
									newsletterTextArea.clear();
								}
							}
						}
					}
				}
			}

			Action {
				checkable: true
				text: qsTr("Live mode")
				checked: air.objectModel.isLive

				onToggled: {
					air.objectModel.isLive = !application.model.objectModel.isLive;
				}
			}

			Action {
				checkable: true
				text: qsTr("Reverse sorting")
				checked: air.objectModel.isReverse

				onToggled: {
					air.objectModel.isReverse = !application.model.objectModel.isReverse;
				}
			}

			Action {
				text: qsTr("Synchronize")
				enabled: !air.objectModel.isLive && air.objectModel.isReady

				onTriggered: {
					air.objectModel.update();
				}
			}
		}
	}

	ListView {
		id: listView

		anchors.fill: parent

		verticalLayoutDirection: ListView.TopToBottom

		headerPositioning: ListView.InlineHeader
		header: FilterField {
			id: filterField

			topPadding: 4
			bottomPadding: 4
			leftPadding: 4
			rightPadding: 4

			width: ListView.view.width

			dataModel: air.objectModel
			placeholderText: qsTr("Search locally or inside /path recursively by @usernames, %groups, §channels, #tags and :attributes")

			Connections {
				target: listView

				function onVerticalOvershootChanged() {
					if (listView.verticalOvershoot > 0) {
						return ;
					} else if (application.model.objectModel.isLive || !application.model.objectModel.isReady) {
						filterField.progress = 1;
					}
					var header_overshoot = 1.0 - (Math.abs(listView.verticalOvershoot) * 4 / listView.height);
					if (header_overshoot < 1 && header_overshoot > 0) {
						filterField.progress = Math.max(0, header_overshoot);
					} else if (header_overshoot <= 0 && listView.boundsBehavior != Flickable.StopAtBounds) {
						listView.boundsMovement = Flickable.StopAtBounds;
						listView.boundsBehavior = Flickable.StopAtBounds;
						air.objectModel.update();
						filterField.progress = 1;
					}
				}

				function onMovementEnded() {
					listView.boundsMovement = Flickable.FollowBoundsBehavior;
					listView.boundsBehavior = Flickable.DragAndOvershootBounds;
				}
			}
		}

		add: Transition {
			PropertyAction {
				property: "transformOrigin"
				value: Item.TopLeft
			}
			NumberAnimation {
				property: "opacity"
				duration: 200
				from: 0.0
				to: 1.0
			}
			NumberAnimation {
				property: "scale"
				duration: 200
				from: 0.0
				to: 1.0
			}
		}

		move: Transition {
			SequentialAnimation {
				NumberAnimation {
					property: "opacity"
					duration: 200
					from: 1.0
					to: 0.5
				}
				NumberAnimation {
					properties: "x,y"
					duration: 300
					easing.type: Easing.OutBack
				}
				PropertyAction {
					property: "opacity"
					value: 1
				}
			}
		}

		displaced: Transition {
			PropertyAction {
				properties: "opacity, scale"
				value: 1
			}

			NumberAnimation {
				properties: "x,y"
				duration: 200
			}
		}

		model: air.objectModel

		delegate: ItemDelegate {
			height: 68
			width: ListView.view.width

			Loader {
				anchors.fill: parent
				active: undefined !== id
				sourceComponent: MouseArea {
					Component.onDestruction: {
						console.debug("ChannelNavigator.listView[", index, "]::onDestruction");
					}

					property var tint: colorString(name)
					property var channel: air.getChannel(id, !listView.moving)

					onClicked: {
						if (listView.activeFocus) {
							application.header.focus = true;
						}
						channelSelected(air.getChannel(id));
					}

					Row {
						height: 60

						anchors.right: parent.right
						anchors.left: parent.left
						anchors.leftMargin: 4
						anchors.top: parent.top
						anchors.topMargin: 4

						Label {
							id: idLabel
							width: parent.height
							height: parent.height
							padding: 10

							verticalAlignment: Text.AlignVCenter
							horizontalAlignment: Text.AlignHCenter
							fontSizeMode: Text.Fit
							font.bold: true
							font.pointSize: 60
							minimumPointSize: 10

							color: colorVersa(tint)
							text: "§%1".arg(id)

							background: Rectangle {
								radius: 30
								color: tint
								border.width: 0
							}

							property bool tipVisible: false
							ToolTip.delay: 300
							ToolTip.timeout: 3000
							ToolTip.text: tag_set !== undefined && tag_set.count !== 0 ? tag_set : qsTr("No tags")
							ToolTip.visible: tipVisible
							ToolTip.toolTip.parent: Overlay.overlay
							ToolTip.toolTip.x: x + (width / 2)
							ToolTip.toolTip.y: y + (height / 2)

							ToolTip.toolTip.onAboutToHide: {
								tipVisible = false;
							}

							MouseArea {
								anchors.fill: parent
								onClicked: {
									idLabel.tipVisible = true;
								}
							}
						}
						ColumnLayout {
							width: parent.width - 60 - 3
							height: parent.height
							spacing: 2

							RowLayout {
								Layout.preferredHeight: nameLabel.contentHeight
								Layout.maximumHeight: nameLabel.contentHeight
								Layout.fillWidth: true
								spacing: 1

								Label {
									id: nameLabel

									Layout.fillWidth: true
									Layout.fillHeight: true

									verticalAlignment: Text.AlignTop
									font.pointSize: Theme.title3Font.pointSize
									font.bold: true
									elide: Text.ElideRight

									text: name
								}
								Label {
									id: timeLabel

									Layout.fillHeight: true
									Layout.preferredWidth: contentWidth

									horizontalAlignment: Text.AlignRight
									verticalAlignment: Text.AlignVCenter
									font.pointSize: Theme.title4Font.pointSize

									text: !channel || !channel.lastMessage.isImported ? "--:--" : application.formatTime(channel.lastMessage.actualDateTime)
								}
							}
							RowLayout {
								Layout.fillHeight: true
								Layout.fillWidth: true
								Layout.leftMargin: 5
								spacing: 1

								Label {
									id: previewLabel

									Layout.alignment: Qt.AlignTop | Qt.AlignLeft
									Layout.fillHeight: true
									Layout.fillWidth: true
									Layout.leftMargin: 5

									horizontalAlignment: Text.AlignLeft
									verticalAlignment: Text.AlignTop
									textFormat: Text.AutoText
									wrapMode: Text.WrapAnywhere
									elide: Text.ElideRight
									maximumLineCount: 2
									font.pointSize: 12
									color: Material.color(Material.Grey)
									padding: 2

									text: (!channel ? "" : channel.lastMessage.preview)
								}

								Label {
									id: counterLabel

									Layout.alignment: Qt.AlignTop
									Layout.preferredHeight: contentHeight
									Layout.preferredWidth: contentHeight

									horizontalAlignment: Text.AlignRight

									visible: channel && channel.hasUnreadMessages

									text: " "
									background: Rectangle {
										radius: parent.implicitHeight / 2
										border.width: 0
										color: Material.accent
									}
								}
							}
							Rectangle {
								Layout.preferredHeight: 1
								Layout.maximumHeight: 1
								Layout.fillWidth: true

								color: "transparent"
								border.color: Material.color(Material.Grey)
							}
						}
					}
				}
			}
		}

		ScrollBar.vertical: ScrollBar {
			id: channelScroll
			interactive: false
		}

		onMovementStarted: {
			if (listView.activeFocus) {
				application.header.focus = true;
			}
		}
	}
}
