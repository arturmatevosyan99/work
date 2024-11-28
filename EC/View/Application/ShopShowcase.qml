import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15

import Core.View 0.1
import Core.System 0.1
import Core.Meta 0.1

import EC.Global 0.1
import EC.Model 0.1
import EC.Data 0.1

import "../../../Core/View"

Page {
	id: application

	Component.onCompleted: {
		console.log("ShopShowcase::onCompleted");
	}

	Component.onDestruction: {
		console.log("ShopShowcase::onDestruction");
	}

	property alias bundleFilter: bundleShowcase.filter
	property alias categoryFilter: categoryShowcase.filter

	property OnlineShop model: OnlineShop {
		Component.onCompleted: {
			basket.addPaymentPreference(EC.Engine, "ec.inappcharger");
		}

		showroom: [
			Showcase {
				id: bundleShowcase
				catalogue: Showcase.Bundle
				filter: []
			},
			Showcase {
				id: categoryShowcase
				catalogue: Showcase.Category
				filter: []
			}
		]

		function checkOut() {
			if (basket.isEmpty()) {
				generalMessageDialog.show(qsTr("Hm..."), qsTr("Basket is empty"));
			} else if (orderPurchase(qsTr("OnlineShop Order"), qsTr("InApplication purchase"))) {
				showPopupMessage(qsTr("Generating purchase order..."));
			} else {
				generalMessageDialog.show(qsTr("Ops!"), qsTr("Could not issue order purchase"));
			}
		}
	} onModelChanged: {
		console.debug("ShowShowcase::onModelChanged", application.model);
		modelConnection.target = model;
		application.model.opened();
	}

	Connections {
		id: modelConnection

		target: application.model

		function onOpened() {
			showPopupMessage(qsTr("Prepearing personal showcase..."));
			application.model.loadShowcase();
		}

		function onShowcaseLoaded(success) {
			if (!success) {
				showPopupMessage(qsTr("Sorry, we are experiencing technical difficulties"));
			} else {
				if (application.model.stockout) {
					showPopupMessage(qsTr("Sorry, we are out of stock"));
				} else {
					closePopupMessage();
				}
			}
		}

		function onPurchaseOrdered(success) {
			console.log("onPurchaseOrdered");
			closePopupMessage();
			if (!success) {
				orderFaultDialog.open();
			}
		}
	}

	MessageDialog {
		id: generalMessageDialog

		standardButtons: StandardButton.Ok

		function show(caption, body) {
			generalMessageDialog.title = caption;
			generalMessageDialog.text = body;
			generalMessageDialog.open();
		}
	}

	MessageDialog {
		id: orderFaultDialog

		title: qsTr("Oops!")
		text: qsTr("Could not generate purchase order, please try again")
		standardButtons: StandardButton.Abort
	}

	MessageDialog {
		id: proceedFaultDialog

		title: qsTr("Oops!")
		text: qsTr("Could not proceed with an asset, please update the application or contact us")
		standardButtons: StandardButton.Abort
	}

	Popup {
		id: statusPopup

		parent: Overlay.overlay
		x: Math.round((parent.width - width) / 2)		
		y: 0
		width: parent.width
		height: parent.height

		background: Rectangle {
			anchors.fill: parent
			color: stackBar.backgroundColor
		}

		Text {
			id: popupText
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.verticalCenter: parent.verticalCenter

			height: contentHeight

			padding: 10
			wrapMode: Text.WordWrap
			horizontalAlignment: Text.AlignHCenter

			color: stackBar.foregroundColor
		}

		enter: Transition {
			NumberAnimation {
				property: "y";
				from: 0.0;
				to: Math.round((application.height - statusPopup.height) / 2)
			}
		}

		exit: Transition {
			NumberAnimation {
				property: "y";
				from: Math.round((application.height - statusPopup.height) / 2)
				to: 0
			}
		}
	}

	function showPopupMessage(text) {
		popupText.text = text;
		if (!statusPopup.opened) {
			statusPopup.open();
		}
	}

	function closePopupMessage() {
		statusPopup.close();
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	background: Rectangle {
		anchors.fill: parent
		color: stackBar.foregroundColor
	}

	header: StackBar {
		id: stackBar

		titleText: ""

		stackView: application.StackView.view

		foregroundColor: "white"
		backgroundColor: accentColor
		backgroundGlass: false

		onBackClicked: {
		}

		optionStyle: StackBar.OptionStyle.None
		onOptionClicked: {
			optionMenu.open();
		}
/*
		Menu {
			id: optionMenu
			parent: stackBar.optionButton
			topMargin: stackBar.optionButton.height

			MenuItem {
				text: qsTr("Orders")
				onTriggered: {
					optionMenu.close();
				}
			}

			MenuItem {
				text: qsTr("Licenses")
				onTriggered: {
					optionMenu.close();
				}
			}

			MenuItem {
				text: qsTr("Basket")
				onTriggered: {
					optionMenu.close();
				}
			}

			MenuItem {
				text: qsTr("Close")
				onTriggered: {
					optionMenu.close();
				}
			}
		}
*/
	}

	property real bannerHeight: Math.max(Screen.desktopAvailableWidth, Screen.desktopAvailableHeight) / 3;
	property real thumbnailWidth: Math.min(Screen.desktopAvailableWidth, Screen.desktopAvailableHeight) / 3;

	ListView {
		id: listView

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
//		clip: true

		headerPositioning: ListView.PullBackHeader
		header: Component {
			Rectangle {
				width: ListView.view.width
				height: bannerHeight

				color: stackBar.accentColor
//				clip: true

				SwipeView {
					id: bundleSwipe

					width: parent.width
					height: parent.height
					rightPadding: 30

					currentIndex: 0
					Repeater {
						model: bundleShowcase
						Item {
							Item {
								id: bundleBanner

								height: bannerHeight * 0.50
								width: bundleBannerImage.width
								clip: true

								anchors.top: parent.top
								anchors.left: parent.left
								anchors.leftMargin: 10

								layer.enabled: true
								layer.effect: OpacityMask {
									maskSource: Rectangle {
										id: mask
										width: bundleBanner.width
										height: bundleBanner.height
										radius: 0.05 * height
									}
								}

								Image {
									id: bundleBannerImage
									height: parent.height

									anchors.centerIn: parent
									anchors.horizontalCenter: parent.horizontalCenter

									fillMode: Image.PreserveAspectFit
									source: application.model.getFileEndpoint(banner)
/*
									layer.enabled: true
									layer.effect: DropShadow {
										anchors.fill: parent
										horizontalOffset: 3
										verticalOffset: 3
										radius: 8.0
										samples: 17
										color: "#80000000"
									}
*/
								}
							}

							Text {
								id: bundleDescriptionText

								text: description

								color: "white"
								elide: Text.ElideRight
								wrapMode: Text.WordWrap
								verticalAlignment: Text.AlignTop
								horizontalAlignment: Text.AlignLeft

								fontSizeMode: Text.Fit
								minimumPointSize: 15

								topPadding: 5
								bottomPadding: 2

								anchors.top: bundleBanner.bottom
								anchors.left: bundleBanner.left
								anchors.right: parent.right
								anchors.bottom: bundleBuyButton.top
							}

							Button {
								id: bundleBuyButton

								anchors.bottom: parent.bottom
								anchors.bottomMargin: 5
								anchors.left: bundleDescriptionText.left

								contentItem: Text {
									text: qsTr("Buy") + " " + total_price
									leftPadding: 5
									rightPadding: 5
	//								font.weight: Font.ExtraBold
									opacity: enabled ? 1.0 : 0.3
									color: stackBar.foregroundColor
									horizontalAlignment: Text.AlignHCenter
									verticalAlignment: Text.AlignVCenter
									elide: Text.ElideRight
								}

								background: Rectangle {
									border.width: 0
									color: parent.down ? stackBar.accentColor : Qt.darker(stackBar.accentColor, 2.0)
									border.color: stackBar.backgroundColor
									radius: 16
								}

								onClicked: {
									application.model.customerBasket.addStock(bundleShowcase.stock(index), 1.0);
									application.model.checkOut();
								}
							}
						}
					}
				}
			}
		}

		section.property: "caption"
		section.criteria: ViewSection.FullString
		section.delegate: Component {
			Rectangle {
				width: ListView.view.width
				height: childrenRect.height

				Label {
					text: section
					font.bold: true
					font.pixelSize: 15
					padding: 5
				}
			}
		}

		model: categoryShowcase
		delegate: Rectangle {
			width: parent.width
			height: childrenRect.height
			color: stackBar.foregroundColor

			Column {
				spacing: 3
				width: listView.width

				Repeater {
					property alias stockIndex: index
					model: categoryShowcase.stock(stockIndex)

					delegate: Row {
						id: categoryRow

						height: thumbnailWidth
						width: listView.width

						leftPadding: 4
						spacing: 4

						Item {
							id: unitThumbnail
							clip: true
							height: thumbnailWidth
							width: thumbnailWidth

							layer.enabled: true
							layer.effect: OpacityMask {
								maskSource: Rectangle {
									id: unitThumbnailMask
									width: unitThumbnailImage.width
									height: unitThumbnailImage.height
									radius: 8
								}
							}

							Image {
								id: unitThumbnailImage
								height: parent.height
								anchors.centerIn: parent
								fillMode: Image.PreserveAspectFit
								source: application.model.getFileEndpoint(thumbnail)
							}
						}

						ColumnLayout {
							height: thumbnailWidth
							width: parent.width - thumbnailWidth

							Text {
								id:	unitNameText

								Layout.fillWidth: true
								height: lineHeight * 2
								Layout.leftMargin: 4
								Layout.rightMargin: 5

								verticalAlignment: Text.AlignTop
								wrapMode: Text.WordWrap
								font.pixelSize: 14
								elide: Text.ElideRight
								topPadding: 2
								rightPadding: 5

								text: name
							}

							Text {
								id: unitSpecText

								Layout.fillWidth: true
								Layout.fillHeight: true
								Layout.leftMargin: 4
								Layout.rightMargin: 5

								verticalAlignment: Text.AlignTop
								wrapMode: Text.WordWrap
								color: Material.color(Material.Grey)
								font.pixelSize: 14
								elide: Text.ElideRight
								rightPadding: 5

								text: specification
							}

							Row {
								id: buttonRow

								height: unitActionButton.height
								Layout.alignment: Qt.AlignBottom
								Layout.fillWidth: true
								Layout.leftMargin: 3
//								Layout.bottomMargin: 4
								spacing: 3

								Button {
									id: unitDetailsButton
									enabled: rich_specification

									contentItem: Text {
										text: qsTr("More...")
										font.weight: Font.ExtraBold
										font.pixelSize: 12
										opacity: enabled ? 1.0 : 0.3
										color: parent.down ? Qt.darker(stackBar.accentColor, 2.0) : stackBar.accentColor
										horizontalAlignment: Text.AlignHCenter
										verticalAlignment: Text.AlignVCenter
										elide: Text.ElideRight
									}

									background: Rectangle {
										border.width: 1
										border.color: stackBar.backgroundColor
										radius: 4
									}

									onClicked: {
									}
								}

								Button {
									id: unitActionButton
									enabled: action || available

									contentItem: Text {
										id: unitActionText
										text: action ? qsTr("Proceed") : qsTr("Buy") + " " + price
										font.weight: Font.ExtraBold
										font.pixelSize: 12
										opacity: enabled ? 1.0 : 0.3
										color: parent.down ? Qt.darker(stackBar.accentColor, 2.0) : stackBar.accentColor
										horizontalAlignment: Text.AlignHCenter
										verticalAlignment: Text.AlignVCenter
										elide: Text.ElideRight
									}

									background: Rectangle {
										border.width: 1
										border.color: stackBar.backgroundColor
										radius: 4
									}

									onClicked: {
										if (model.action) {
											var render = SchemeFactory.load("Repository.Meta.ApplicationRender", model.action.application_scheme);
											var component = Qt.createComponent(render.makeURL());
											if (component) {
												if (component.status !== Component.Ready) {
													console.log("Could not create(", component.status, ") Component for scheme:", model.action.application_scheme);
													proceedFaultDialog.open();
													if (component.status === Component.Error) {
														console.error("Error: ", component.errorString());
													}
													return ;
												}
												var view = application.StackView.view.push(component, {}, StackView.PushTransition);
												if (view) {
													application.model.session.attach(view.model, model.action.id, true);
												} else {
													console.error("Could not push Component to the StackView");
												}
											}
										} else {
											application.model.customerBasket.addStockItem(model, model.price_id, 1);
											application.model.checkOut();
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
