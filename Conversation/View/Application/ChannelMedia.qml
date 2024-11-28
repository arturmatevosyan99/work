import QtQuick 2.15
import QtQml 2.15
import QtQml.Models 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

import Core.View 0.1
import Core.Meta 0.1

import Conversation.Global 0.1
import Conversation.View 0.1
import Conversation.Model 0.1

import "../../../Core/View"

import "../../../Conversation/View"

Page {
	id: application

	Component.onCompleted: {
		console.log("ChannelMedia::onCompleted");

	}

	Component.onDestruction: {
		console.log("ChannelMedia::onDestruction");
		model.swapoff();
	}

	padding: 0
	leftInset: 0
	rightInset: 0
	header: StackBar {
		id: stackBar

		titleText: qsTr("Media")
		accentColor: "transparent"
		stackView: application.StackView.view

		optionStyle: StackBar.OptionStyle.None
	}

	property GroupChannel model: GroupChannel {
	} onModelChanged: {
		console.debug("ChannelMedia::onModelChanged", application.model);
		modelConnection.target = model;
		if (modelConnection.target.isOpened) {
			modelConnection.onOpened();
		}
	}
	Connections {
		id: modelConnection

		target: model

		function onOpened()
		{
			if (tabBar.currentItem) {
				target.swapon(tabBar.currentItem.hash);
			}
		}
	}

	TabBar {
		id: tabBar

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		z: 2

		background: Rectangle {
			anchors.fill: parent
			color: stackBar.background.color
			opacity: stackBar.background.opacity
		}

		Repeater {
			model: SchemeModel {
				factoryAlias: "Conversation.Meta.MessageRender"

				filterList: [
					Filter {
						condition: Filter.Greater
						roleName: "layout_set"
						roleValue: 0
					}
				]

				orderList: [
					Order {
						roleName: "artefactname"
						roleOrder: Qt.AscendingOrder
					}
				]
			}

			delegate: TabButton {
				required property var hash
				required property int layout_set
				required property string artefactname
				text: artefactname
				width: implicitContentWidth + leftPadding + rightPadding
//				width: Math.max(100, tabBar.width / 5)
			}
		}

		/**
		 * @todo Carousel support
		 */
		onCurrentItemChanged: {
			if (!tabBar.currentItem) {
				return ;
			} else if ((tabBar.currentItem.layout_set & Conversation.VerticalLayout) === Conversation.VerticalLayout) {
				componentLoader.sourceComponent = vlistComponent;
			} else if ((tabBar.currentItem.layout_set & Conversation.HorizontalLayout) === Conversation.HorizontalLayout) {
				componentLoader.sourceComponent = hlistComponent;
			} else if ((tabBar.currentItem.layout_set & Conversation.GridLayout) === Conversation.GridLayout) {
				componentLoader.sourceComponent = gridComponent;
			} else {
				return ;
			}
			model.swapon(tabBar.currentItem.hash);
		}
	}

	Loader {
		id: componentLoader

		anchors.topMargin: 2
		anchors.top: tabBar.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		z: tabBar.z - 1;

		asynchronous: true
	}

	Label {
		text: qsTr("There is nothing here yet")

		color: Material.color(Material.Grey)
		horizontalAlignment: Text.AlignHCenter
		visible: !componentLoader.item ? false : componentLoader.item.count === 0

		anchors.centerIn: componentLoader
	}

	Component {
		id: vlistComponent

		MediaList {
			orientation: ListView.Vertical
			channelModel: application.model

			onLoadRequested: {
				application.model.fetch(GroupChannel.SwapRotate, end);
			}

			onModifyRequested: {
				application.model.modify(meta);
			}

			onPopupRequested: {
				popup(component, properties);
			}
		}
	}

	Component {
		id: hlistComponent

		MediaList {
			orientation: ListView.Horizontal
			channelModel: application.model

			onLoadRequested: {
				application.model.fetch(GroupChannel.SwapRotate, end);
			}

			onModifyRequested: {
				application.model.modify(meta);
			}

			onPopupRequested: {
				popup(component, properties);
			}
		}
	}

	Component {
		id: gridComponent

		MediaGrid {
			channelModel: application.model

			onLoadRequested: {
				application.model.fetch(GroupChannel.SwapRotate, end);
			}

			onModifyRequested: {
				application.model.modify(meta);
			}

			onPopupRequested: {
				popup(component, properties);
			}
		}
	}

	Component {
		id: carouselComponent

		MediaCarousel {
			channelModel: application.model

			onLoadRequested: {
				application.model.fetch(GroupChannel.SwapRotate, end);
			}

			onModifyRequested: {
				application.model.modify(meta);
			}

			onPopupRequested: {
				popup(component, properties);
			}
		}
	}

	function popup(component, component_parameter, application_option)
	{
		if (!component) {
			var object_id = component_parameter.objectId;
			var application_scheme = component_parameter.applicationScheme;
			if (!object_id || !application_scheme) {
				console.log();
				return ;
			}
			delete component_parameter.objectId;
			delete component_parameter.applicationScheme;

			var render = SchemeFactory.load("Repository.Meta.ApplicationRender", application_scheme);
			if (render) {
				component = Qt.createComponent(render.makeURL());
				if (component) {
					var view = application.StackView.view.push(component, component_parameter, StackView.PushTransition);
					if (view) {
						application.model.session.attach(view.model, object_id, true, application_option);
					} else {
						console.log("Could not push Component to the StackView");
					}
				} else {
					console.log("Could not create Component for scheme:", application_scheme);
				}
			}
		} else {
			application.StackView.view.push(component, component_parameter, StackView.Transition);
		}
	}
}
