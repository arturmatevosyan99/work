import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Core.System 0.1
import Conversation.View 0.1 as View
import ERP.Model 0.1 as Model
import EC.Model 0.1 as Model
import EC.Global 0.1

View.MessageView {
	id: rootView

	contentWidth: columnLayout.width
	contentHeight: columnLayout.height

	MessageDialog {
		id: messageDialog

		title: qsTr("Payment processing")
		text: ""

		function popUp(message) {
			messageDialog.text = message;
			messageDialog.open();
		}
	}

	model: Model.OfferMessage {
		id: message

		onImported: {
			if (message.state == Model.OfferMessage.Price) {
				message.loadPrice();
			}
		}

		onStateChanged: {
			if (message.state == Model.OfferMessage.Basket) {
				/// @todo просто показываем уже сохраненную корзину
			} else if (message.state == Model.OfferMessage.Order) {
				/// @todo показываем PurchaseOrder
			}
		}

		onFaulted: {
			messageDialog.popUp(message);
		}

		onCompleted: {
			messageDialog.popUp(success ? qsTr("Order has been paid successfully") : qsTr("Payment has been canceled"));
		}
	}

	ColumnLayout {
		id: columnLayout

		anchors.top: rootView.top
		anchors.topMargin: rootView.gap / 2
		anchors.left: rootView.left
		anchors.leftMargin: rootView.gap / 2

		spacing: 4

		Label {
			id: offerLabel

			Layout.preferredHeight: implicitHeight
			Layout.preferredWidth: implicitWidth
			Layout.maximumWidth: rootView.minimumWidth

			font.bold: true
			wrapMode: Text.WordWrap
			text: message.note.length > 0 ? message.note : qsTr("Special offer")
		}

		Repeater {
			id: offerRepeater

			model: message.onlineShop.purchaseBasket
			delegate: Label {
				Layout.preferredHeight: implicitHeight
				Layout.preferredWidth: implicitWidth
				Layout.maximumWidth: rootView.minimumWidth

				color: fontColor
				textFormat: Text.StyledText
				wrapMode: Label.Wrap
				text: (index+1) + ") " + name + " x " + quantity + " = " + cost + "<br />"
			}
		}

		Button {
			id: button

			Layout.alignment: Qt.AlignCenter
			Layout.preferredWidth: offerLabel.implicitWidth
			Layout.maximumWidth: rootView.minimumWidth
			Layout.fillWidth: true
//			Layout.preferredWidth: implicitWidth

			enabled: true
			flat: true
			down: true

			states: [
				State {
					when: message.state <= Model.OfferMessage.Price
					PropertyChanges {
						target: button
						text: qsTr("Checking specials offers...")
						enabled: false
					}
				},
				State {
					when: message.state === Model.OfferMessage.Basket && message.onlineShop.purchaseBasket.isEmpty
					PropertyChanges {
						target: button
						text: qsTr("Getting special price...")
						enabled: false
					}
				},
				State {
					when: message.state === Model.OfferMessage.Basket && !message.onlineShop.purchaseBasket.isEmpty
					PropertyChanges {
						target: button
						text: qsTr("Purchase") + " (" + message.onlineShop.purchaseBasket.total + ")"
						enabled: true
					}
				},
				State {
					when: message.state === Model.OfferMessage.Order
					PropertyChanges {
						target: button
						text: "%1 (%2)".arg(qsTr("Pay")).arg(message.onlineShop.purchaseBasket.total)
						enabled: true
					}
				},
				State {
					when: message.state === Model.OfferMessage.Busy
					PropertyChanges {
						target: button
						text: qsTr("...")
						enabled: false
					}
				},
				State {
					when: message.state === Model.OfferMessage.Payment
					PropertyChanges {
						target: button
						text: qsTr("Paid")
						enabled: false
					}
				},
				State {
					when: message.state === Model.OfferMessage.Invalid
					PropertyChanges {
						target: button
						text: qsTr("Not available")
						enabled: false
					}
				}
			]

			onClicked: {
				if (message.state == Model.OfferMessage.Basket) {
					/// @note в onOrderIssued автоматически переходим к оплате
					message.issueOrder();
				} else if (message.state == Model.OfferMessage.Order) {
					/// @note в onOrderResolved автоматически переходим к оплате
					message.resolveOrder();
				}
			}
		}
	}
}
