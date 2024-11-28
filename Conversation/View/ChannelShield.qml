import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

import Core.Meta 0.1
import Core.Model 0.1
import Repository.Meta 0.1
import Repository.Model 0.1

import "../../Core/View"

import "../View"
import "../View/Application"

ColumnLayout {
	id: rootLayout

	signal revealed()
	signal modeRequested(var enumber)
	signal applicationRequested(var object_id, var application_scheme, var option_map)
	signal customComponentRequested(var component)

	property ChannelFeed channelFeed: null
	onChannelFeedChanged: {
		if (!channelFeed || !applicationConnection.target || !applicationConnection.target.isOpened) {
			return ;
		} else if (applicationConnection.target.isInitialized) {
			applicationConnection.onInitialized(false, true);
		}
	}

	property alias applicationListView: applicationListView
	property alias modeListView: modeListView
	property Component bottomComponent: null
	property var titleSubject: undefined
	property int menuModeSet: ChannelFeed.MenuMode.Collapse | ChannelFeed.MenuMode.Profile | ChannelFeed.MenuMode.Party | ChannelFeed.MenuMode.Media | ChannelFeed.MenuMode.Search
	readonly property Menu modeActionMenu: Menu {
		Action {
			readonly property int enumber: ChannelFeed.MenuMode.Profile
			text: qsTr("Profile")
			icon.source: "/Core/Asset/Image/Profile1.svg"
			enabled: true
			onTriggered: {
				rootLayout.modeRequested(enumber);
			}
		}
		Action {
			readonly property int enumber: ChannelFeed.MenuMode.Party
			text: qsTr("Members")
			icon.source: "/Conversation/Asset/Image/Party1.svg"
			enabled: true
			onTriggered: {
				rootLayout.modeRequested(enumber);
			}
		}
		Action {
			readonly property int enumber: ChannelFeed.MenuMode.Media
			text: qsTr("Media")
			icon.source: "/Core/Asset/Image/Media1.svg"
			enabled: true
			onTriggered: {
				rootLayout.modeRequested(enumber);
			}
		}
		Action {
			readonly property int enumber: ChannelFeed.MenuMode.Search
			text: qsTr("Search")
			icon.source: "/Core/Asset/Image/Search1.svg"
			enabled: true
			onTriggered: {
				rootLayout.modeRequested(enumber);
			}
		}
		Action {
			readonly property int enumber: ChannelFeed.MenuMode.Collapse
			text: qsTr("Collapse")
			icon.source: "/Core/Asset/Image/Collapse1.svg"
			enabled: true
			onTriggered: {
				rootLayout.modeRequested(enumber);
			}
		}
	}

	onRevealed: {
	}

	property Component menuActionComponent: Component {
		Menu {
		}
	}

	property Component modeActionComponent: Component {
		Action {
			property var enumber: ChannelFeed.MenuMode.Custom
			text: qsTr("Close")
			icon.source: "/DrOblozhko/Asset/Image/Attachment.svg"
			enabled: true
			onTriggered: {
				rootLayout.modeRequested(enumber);
			}
		}
	}

	property Component applicationActionComponent: Component {
		Action {
			property var objectId
			property var applicationScheme
			property var optionMap
			text: ""
			icon.source: "/Conversation/Asset/Image/Attachment.svg"
			enabled: true
			onTriggered: {
				rootLayout.applicationRequested(objectId, applicationScheme, optionMap);
			}
		}
	}

	Connections {
		id: applicationConnection

		target: rootLayout.channelFeed ? rootLayout.channelFeed.model : null

		function onIndexed()
		{
			var menu = rootLayout.menuActionComponent.createObject(null, {});
			for (var i = 0; i < target.indexModel.rowCount(); i++) {
				var index = target.indexModel.index(i, 0);
				menu.addAction(
								rootLayout.applicationActionComponent.createObject
								(
									menu,
									{
										objectId: target.indexModel.data(index, FolderIndex.Id),
										applicationScheme: target.indexModel.data(index, FolderIndex.ApplicationScheme),
										optionMap: target.indexModel.data(index, FolderIndex.Option),
										text: target.indexModel.data(index, FolderIndex.Name),
										enabled: true
									}
								)
				);
			}
			applicationListView.menu = menu;
		}

		function onInitialized(dump, success)
		{
			if (!success) {
				return;
			} else if (!rootLayout.titleSubject) {
				rootLayout.titleSubject = applicationConnection.target.descriptor.subjectKey;
//				applicationConnection.onIndexed();
			}
		}
	}

	Item {
		id: titleSubjectItem

		Layout.minimumHeight: memberPictureImage.implicitHeight
		Layout.maximumHeight: memberPictureImage.implicitHeight
		Layout.fillWidth: true

		Layout.margins: 10

		Image {
			id: memberPictureImage

			anchors.top: parent.top
			anchors.left: parent.left
			anchors.leftMargin: 4
			anchors.rightMargin: 4

			width: memberNameLabel.implicitHeight + memberIdLabel.implicitHeight
			sourceSize.width: width
			sourceSize.height: width
			source: "/DrOblozhko/Asset/Image/HudeemOnline2.svg"
			fillMode: Image.PreserveAspectFit

			layer.enabled: true
			layer.effect: OpacityMask {
				maskSource: Item {
					width: memberPictureImage.width
					height: memberPictureImage.height
					Rectangle {
						anchors.centerIn: parent
						width:  memberPictureImage.width
						height: memberPictureImage.height
						radius: Math.min(width, height)
					}
				}
			}
		}
		Label {
			id: memberNameLabel

			anchors.top: parent.top
			anchors.left: memberPictureImage.right
			anchors.leftMargin: 4
			anchors.right: parent.right
			anchors.rightMargin: 4

			width: parent.width
			height: implicitHeight

			text: rootLayout.titleSubject ? rootLayout.titleSubject.displayName : "Anonyumous"
			font.bold: true
			font.weight: Font.ExtraBold
			font.capitalization: Font.Capitalize
		}
		Label {
			id: memberIdLabel

			anchors.top: memberNameLabel.bottom
			anchors.left: memberPictureImage.right
			anchors.leftMargin: 4
			anchors.right: parent.right
			anchors.rightMargin: 4

			width: parent.width
			height: implicitHeight

			color: "#d3d3d3" // "grey"
			text: rootLayout.titleSubject ? "@" + rootLayout.titleSubject.displayId : ""
			font.pointSize: memberNameLabel.font.pointSize * 0.8
		}
	}

	Item {
		Layout.fillHeight: true
		Layout.fillWidth: true
	}

	ListView {
		id: applicationListView

		property Menu menu: Menu {
		}

		Layout.fillWidth: true
		Layout.minimumHeight: applicationListView.width / 4
		Layout.maximumHeight: applicationListView.width / 4
		Layout.leftMargin: 10

		orientation: ListView.Horizontal
		spacing: 8

		model: applicationListView.menu ? applicationListView.menu.count : 0
		delegate: ItemDelegate {
			id: applicationButton

			action: applicationListView.menu.actionAt(index)
			property var render: SchemeFactory.load("Repository.Meta.ApplicationRender", action.applicationScheme);

			height: applicationListView.height
			width: applicationListView.height
			enabled: action.enabled
			display: AbstractButton.TextBesideIcon

			contentItem: Label {
				anchors.left: parent.left
				anchors.top: parent.indicator.bottom
				anchors.bottom: parent.bottom
				anchors.right: parent.right
				anchors.margins: 2

				text: applicationButton.action.text
				font.pointSize: applicationButton.font.pointSize * 0.6
				lineHeight: 0.7
				maximumLineCount: 3
				verticalAlignment: Text.AlignLeft
				elide: Text.ElideRight
				wrapMode: Text.Wrap
			}
			indicator: Image {
				anchors.left: applicationButton.left
				anchors.top: applicationButton.top
				anchors.margins: 2

				width: applicationButton.width * 0.4

				source: !applicationButton.render ? "qrc:/Repository/Asset/Icon/Object.svg" : applicationButton.render.makeIconURL()
				sourceSize.width: width
				fillMode: Image.PreserveAspectFit
				verticalAlignment: Image.AlignLeft
			}
			background: Rectangle {
				anchors.fill: applicationButton

				radius: 10
				opacity: enabled ? 1 : 0.3
				color: !applicationButton.down ? "#f4f4f4" : Qt.darker("#f4f4f4", 1.2)
			}
		}
		add: Transition {
			enabled: !(applicationListView.moving || applicationListView.dragging || applicationListView.flicking)
			NumberAnimation {
				property: "x"
				duration: 250
				easing.type: Easing.OutBounce
			}
		}
		remove: Transition {
			ParallelAnimation {
				NumberAnimation {
					property: "opacity"
					to: 0
					duration: 250
				}
				NumberAnimation {
					properties: "x,y"
					to: 100
					duration: 250
				}
			}
		}
		displaced: Transition {
			enabled: !(applicationListView.moving || applicationListView.dragging || applicationListView.flicking)
			NumberAnimation {
				properties: "x"
				duration: 250
				easing.type: Easing.OutBounce
			}
		}
	}

	ListView {
		id: modeListView

		property Menu menu: Menu {
			Component.onCompleted: {
				for (var a = 0; a < rootLayout.modeActionMenu.count; a++) {
					var action = rootLayout.modeActionMenu.actionAt(a);
					if ((rootLayout.menuModeSet & action.enumber) === action.enumber) {
						addAction(action);
					}
				}
			}
		}

		Layout.fillWidth: true
		Layout.minimumHeight: 0
		Layout.preferredHeight: modeListView.contentHeight
		Layout.maximumHeight: parent.height - (titleSubjectItem.height * 2) - bottomItem.height - applicationListView.height

		verticalLayoutDirection: ListView.BottomToTop
		orientation: ListView.Vertical
		spacing: 8

		model: modeListView.menu ? modeListView.menu.count : 0
		delegate: ItemDelegate {
			id: modeButton

			action: modeListView.menu.actionAt(index)

			height: contentItem.contentHeight * 2

			width: modeListView.width
			enabled: action.enabled
			display: AbstractButton.TextBesideIcon

			contentItem: Label {
				anchors.left: parent.indicator.right
				leftPadding: 10

				text: modeButton.action.text
				font: modeButton.font
				verticalAlignment: Text.AlignVCenter
				elide: Text.ElideRight
				maximumLineCount: 1
				wrapMode: Text.Wrap
			}
			indicator: Image {
				anchors.leftMargin: 10
				anchors.left: modeButton.left

				height: modeButton.implicitContentHeight * 1.2
				source: modeButton.action.icon.source
				sourceSize.height: height
				fillMode: Image.PreserveAspectFit
				verticalAlignment: Image.AlignVCenter
				horizontalAlignment: Image.AlignHCenter
				anchors.verticalCenter: modeButton.verticalCenter
			}
		}
		add: Transition {
			enabled: !(modeListView.moving || modeListView.dragging || modeListView.flicking)
			NumberAnimation {
				property: "x"
				duration: 250
				easing.type: Easing.OutBounce
			}
		}
		remove: Transition {
			ParallelAnimation {
				NumberAnimation {
					property: "opacity"
					to: 0
					duration: 250
				}
				NumberAnimation {
					properties: "x,y"
					to: 100
					duration: 250
				}
			}
		}
		displaced: Transition {
			enabled: !(modeListView.moving || modeListView.dragging || modeListView.flicking)
			NumberAnimation {
				properties: "x"
				duration: 250
				easing.type: Easing.OutBounce
			}
		}
	}

	Rectangle {
		Layout.maximumHeight: 1
		Layout.minimumHeight: 1
		Layout.preferredHeight: 1
		Layout.fillWidth: true
		Layout.leftMargin: 10
		Layout.rightMargin: 10

		height: 1
		color: "#d3d3d3" // grey2
		border.width: 0
		border.color: "transparent"
	}

	Item {
		id: bottomItem

		Layout.fillWidth: true
		Layout.maximumHeight: bottomItemLoader.height
		Layout.minimumHeight: bottomItemLoader.height

		Loader {
			id: bottomItemLoader

			anchors.left: parent.left
			anchors.right: parent.right
			anchors.leftMargin: 10
			anchors.rightMargin: 10

			sourceComponent: rootLayout.bottomComponent
		}
	}
}
