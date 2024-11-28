import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.XmlListModel 2.15

import Core.Model 0.1
import Mentalgami.View 0.1
import Mentalgami.Meta 0.1

RiddleView {
	id: rootView

	focus: true

	question: qsTr("Enter your phone number")
	description: qsTr("Before proceeding further, check whether the data you entered is correct")
	specifications: RiddleInformation.BackwardAvailable | RiddleInformation.ForwardAvailable | RiddleInformation.BackwardActive

	onAnswerReceived: {
		answerField.text = answer
	}

	property alias answer: answerField.text

	property string name: "Unknown"
	property string code: "?"
	property string emoji: "🏳️"
	property string mask: ""

	XmlListModel {
		id: xmlListModel

		source: "qrc:/Mentalgami/Asset/E164CountryCodeSet.xml"
		query: "/resources/string-array/item"

		XmlRole {
			name: "name"
			query: "name/string()"
		}

		XmlRole {
			name: "code"
			query: "code/string()"
		}

		XmlRole {
			name: "mask"
			query: "mask/string()"
		}

		XmlRole {
			name: "emoji"
			query: "emoji/string()"
		}
	}

	SortFilterProxyModel {
		id: sortFilterProxyModel

		sourceModel: xmlListModel
		filterRole: !xmlListModel ? Qt.DisplayRole : sortFilterProxyModel.getRoleHashKey("name")
	}

	Drawer {
		id: drawer

		width: parent.width
		height: parent.height * 0.8

		edge: Qt.BottomEdge
		interactive: visible

		background: Item {
			Rectangle {
				id: drawerToggler

				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				height: 20

				radius: 10

				Rectangle {
					anchors.centerIn: parent
					anchors.margins: 6
					width: 20
					height: 8

					radius: 2

					color: Material.color(Material.Grey, Material.Shade200)
				}
			}
			Rectangle {
				anchors.top: drawerToggler.bottom
				anchors.topMargin: -drawerToggler.radius
				anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.right: parent.right
			}
		}

		TextField {
			id: searchTextField

			anchors.top: parent.top
			anchors.topMargin: 4
			anchors.left: parent.left
			anchors.right: parent.right

			height: 50
			leftPadding: 20

			placeholderText: qsTr("Search")

			onTextChanged: {
				sortFilterProxyModel.filterPattern = text + "*"
			}
		}

		ListView {
			id: countryListView

			anchors.top: searchTextField.bottom
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom

			model: sortFilterProxyModel
			delegate: ItemDelegate {
				id: countryListViewItem

				height: 40
				width: countryListView.width

				required property string name
				required property string code
				required property string emoji
				required property string mask

				contentItem: Item {
					Text {
						anchors.left: parent.left
						anchors.verticalCenter: parent.verticalCenter

						text: name
						verticalAlignment: Text.AlignVCenter
						elide: Text.ElideRight
					}

					Text {
						anchors.right: parent.right
						anchors.verticalCenter: parent.verticalCenter

						text: '+' + code
						verticalAlignment: Text.AlignVCenter
					}
				}

				background: Rectangle {
					implicitWidth: countryListViewItem.width
					implicitHeight: countryListViewItem.height

					opacity: enabled ? 1 : 0.3
					color: countryListViewItem.down ? Material.color(Material.Grey, Material.Shade200) : "transparent"

					Rectangle {
						width: parent.width
						height: 1
						color: countryListViewItem.down ? Material.color(Material.Grey, Material.Shade200) : Material.color(Material.Grey)
						anchors.bottom: parent.bottom
					 }
				}

				onClicked: {
					rootView.name = name;
					rootView.code = code;
					rootView.emoji = emoji;
					rootView.mask = mask + ";_";
					answerField.text = "";
					answerField.focus = true;
					drawer.close();
				}
			}

			clip: true

			section {
				property: "name"
				criteria: ViewSection.FirstCharacter
				delegate: Rectangle {
					required property string section

					height: 30
					width: parent.width

					color: Material.color(Material.Grey, Material.Shade200)

					Text {
						leftPadding: 10
						anchors.verticalCenter: parent.verticalCenter

						text: parent.section
						font.bold: true
					}
				}
			}
		}
	}

	Row {
		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		spacing: 10

		Button {
			id: countryButton

			anchors.top: parent.top

			width: Math.min(parent.width / 3, 150)

			enabled: xmlListModel.count > 0

			text: rootView.emoji + " +" + rootView.code
			flat: true

			contentItem: Text {
				text: countryButton.text
				font: answerField.font
				opacity: enabled ? 1.0 : 0.3
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignRight
				elide: Text.ElideRight
			}

			onClicked: {
				drawer.open();
			}
		}

		TextField {
			id: answerField

			anchors.top: parent.top
			anchors.topMargin: 4
			width: Math.min(parent.width / 2, 150)

			focus: true
			placeholderText: qsTr("123-456-78-90")
			inputMask: rootView.mask
			inputMethodHints: Qt.ImhDigitsOnly
			verticalAlignment: TextInput.AlignVCenter

			onActiveFocusChanged: {
				if (answerField.activeFocus) {
					var tmp = answerField.displayText.search(/_/);
					answerField.cursorPosition = tmp < 0 ? 0 : tmp;
				}
			}

			onAcceptableInputChanged: {
				if (acceptableInput) {
					rootView.specifications |= RiddleInformation.ForwardActive
				} else {
					rootView.specifications &= ~RiddleInformation.ForwardActive
				}
			}

			onAccepted: {
				if (acceptableInput) {
					rootView.answered();
				}
			}
		}
	}
}
