import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

import Core.View 0.1
import Core.Meta 0.1
import Repository.View 0.1
import Repository.Model 0.1

import "../../Core/View"

Page {
	id: application

	signal objectSelected(var id, var render)
	signal componentSelected(var item, var properties)

	property date currentDate: new Date()

	padding: 0
	leftInset: 0
	rightInset: 0

	property Infrastructure model: Infrastructure {
//		prefferedIndexes:
//		objectProxy.prefferedIndexes: application.localIndex ?
	}

	header: StackBar {
		id: stackBar

		focus: true

		titleText: qsTr("Objects")
		subtitleText: model.objectModel.path.breadcrumbs

		beatMode: StackBar.BeatMode.Stable

		backBehaviour: (model.objectModel.tokens.length > 0 || model.objectModel.path.depth > 1) ? StackBar.BackBehaviour.Signal : StackBar.BackBehaviour.Ignore
		onBackClicked: {
			if (model.objectModel.tokens.length > 0) {
				model.objectModel.removeToken(model.objectModel.removeLastToken(model.objectModel.tokens.length - 1, false));
			} else {
				model.objectModel.path.backward();
			}
		}

		optionStyle: StackBar.OptionStyle.Stack
		onOptionClicked: {
		}
	}

	ListView {
		id: listView

		anchors.fill: parent

		verticalLayoutDirection: ListView.TopToBottom

		headerPositioning: ListView.InlineHeader
		header: FilterField {
			topPadding: 4
			bottomPadding: 4
			leftPadding: 4
			rightPadding: 4

			width: ListView.view.width

			dataModel: listView.model
			placeholderText: qsTr("Search locally or inside /path recursively by @usernames, %groups, §objects, #tags and :attributes")
		}

		model: application.model.objectModel
		delegate: MouseArea {
			height: 68
			width: ListView.view.width

			property var render: SchemeFactory.load("Repository.Meta.ApplicationRender", application_scheme);

			onClicked: {
				if (listView.activeFocus) {
					application.header.focus = true;
				}
				/// @todo if folder interface is supported otherwise act as doubleClicked
				listView.model.path.forward(id);
			}

			onPressAndHold: {
				/// @todo or act as regular click(single) if just FolderInterface is supported
				if (render) {
					objectSelected(id, render);
				} else {
					listView.model.path.forward(id);
				}
			}

			onDoubleClicked: {
				/// @todo or act as regular click(single) if just FolderInterface is supported
				if (render) {
					objectSelected(id, render);
				} else {
					listView.model.path.forward(id);
				}
			}

			Row {
				height: 60

				anchors.right: parent.right
				anchors.left: parent.left
				anchors.leftMargin: 4
				anchors.top: parent.top
				anchors.topMargin: 4

				Image {
					id: icoImage
					source: !render ? "qrc:/Repository/Asset/Icon/Object.svg" : render.makeIconURL();

					fillMode: Image.PreserveAspectFit
					sourceSize.height: parent.height
					sourceSize.width: parent.height
				}

				ColumnLayout {
					id: infoLayout

					width: parent.width - icoImage.width - controlItem.width
					height: parent.height

					Label {
						id: nameLabel

						Layout.fillHeight: true
						Layout.fillWidth: true
						Layout.leftMargin: 5
						Layout.rightMargin: 5

						verticalAlignment: Text.AlignTop
						font.bold: true
						elide: Text.ElideRight

						text: name
					}
					Label {
						id: descriptionLabel

						Layout.fillHeight: true
						Layout.fillWidth: true
						Layout.leftMargin: 5
						Layout.rightMargin: 5

						horizontalAlignment: Text.AlignLeft
						verticalAlignment: Text.AlignTop
						elide: Text.ElideRight
						font.pointSize: 12
						color: Material.color(Material.Grey)

						text: "%1: %2 %3: %4".arg("Id").arg(id).arg(qsTr("Type")).arg(!render ? qsTr("Unknown") : render.modelName)
					}
					Label {
						id: datetimeLabel

						Layout.fillHeight: true
						Layout.fillWidth: true
						Layout.leftMargin: 5
						Layout.rightMargin: 5

						horizontalAlignment: Text.AlignLeft
						verticalAlignment: Text.AlignTop
						elide: Text.ElideRight
						font.pointSize: 12
						color: Material.color(Material.Grey)

						text: "%1(%2: %3 %4: %5)".arg(Qt.formatDateTime(new Date(creation_datetime), "d MMM, hh:mm")).arg(qsTr("Modification")).arg(Qt.formatDateTime(new Date(modification_datetime), "d MMM, hh:mm")).arg(qsTr("Access")).arg(Qt.formatDateTime(new Date(access_datetime), "d MMM, hh:mm"))
					}
					Label {
						id: tagLabel

						Layout.fillHeight: true
						Layout.fillWidth: true
						Layout.leftMargin: 5
						Layout.rightMargin: 5

						horizontalAlignment: Text.AlignLeft
						verticalAlignment: Text.AlignTop
						elide: Text.ElideRight
						font.pointSize: 12
						color: Material.color(Material.Grey)

						text: "%1: %2".arg(qsTr("Tags")).arg((tag_set !== undefined && tag_set.count !== 0) ? tag_set : "—")
					}
				}
				Item {
					id: controlItem

					width: 20
					height: parent.height

					Button {
						id: optButton

						anchors.centerIn: parent
						width: parent.width

						text: "⋮"
						flat: true
						font.pointSize: 20
						font.weight: Font.ExtraBold

						onClicked: {
							optionMenu.open();
						}

						Menu {
							id: optionMenu

							parent: optButton
							topMargin: optButton.height

							width: 200
							padding: 2
							spacing: 1

							background: Rectangle {
								implicitWidth: parent.width
								implicitHeight: 40
								color: Material.accent
								border.color: "transparent"
								radius: 8
							}

							delegate: MenuItem {
								id: optionMenuItem

								implicitWidth: parent.width
								implicitHeight: optionMenuItem.contentItem.contentHeight

								arrow: Item {
								}

								indicator: Item {
								}

								contentItem: Text {
									leftPadding: optionMenuItem.indicator.width
									rightPadding: optionMenuItem.arrow.width
									text: optionMenuItem.text
									font: optionMenuItem.font
									opacity: enabled ? 1.0 : 0.3
									color: "white"
									horizontalAlignment: Text.AlignLeft
									verticalAlignment: Text.AlignVCenter
									elide: Text.ElideRight
								}

								background: Rectangle {
									implicitWidth: optionMenuItem.implicitWidth
									implicitHeight: 40
									opacity: enabled ? 1 : 0.3
									color: "transparent"
								}
							}

							Action {
								text: qsTr("Run")
								enabled: render !== undefined
								onTriggered: {
									objectSelected(id, render);
								}
							}
							MenuSeparator {
							}
							Action {
								text: qsTr("Delete")
								enabled: false
								onTriggered: {
								}
							}
							Action {
								text: qsTr("Open")
								onTriggered: {
									listView.model.path.forward(id);
								}
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
