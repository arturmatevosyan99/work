import QtQml 2.15
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

import Core.System 0.1
import Core.Model 0.1
import Core.Meta 0.1
import Repository.Model 0.1
import Conversation.Global 0.1
import Conversation.Model 0.1
import Conversation.View 0.1

import "../../Qaos/QtQuick/ListView.js" as ListViewScript

ListView {
	id: rootView

	required property GroupChannel channelModel

	readonly property bool isPopulated: (children.length > 0)
	property int defferedPositionIndex: 0
	property int firstVisibleIndex: 0
	property int lastVisibleIndex: 0
	property bool messagesAhead: (lastVisibleIndex > 0)
	property int messagesProactive: 20
	property int messageMargin: 10
	property int messageRowSpacing: 6
	property real messageMinimumWidth: width * 0.6
	property real messageMinimumHeight: height * 0.85
	readonly property int currentItemHeight: !currentItem ? 0 : currentItem.height
	readonly property int headerItemHeight: !headerItem ? 0 : headerItem.height

	signal delegateCompleted(var index)
	signal delegateDestructed(var index)
	signal delegateListed(var index)
	signal loadRequested(var end)
	signal repushRequested(var uid)
	signal withdrawRequested(var qd)
	signal discardRequested(var uid)
	signal redactionRequested(var model)
	signal modifyRequested(var meta)
	signal popupRequested(var component, var properties)
	signal messageRead(int offset)

	function validate()
	{
		if (rootView.count <= 0) {
			return;
		}

		var index_map = ListViewScript.traverse(rootView);
		[rootView.firstVisibleIndex, rootView.lastVisibleIndex] = [index_map[ListView.Beginning], index_map[ListView.End]];

		if (rootView.channelModel.hasUnloadedMessages && (rootView.count - rootView.lastVisibleIndex) <= rootView.messagesProactive) {
			rootView.loadRequested(GroupChannel.Tail);
		}

		if (rootView.lastVisibleIndex >= 0) {
			rootView.messageRead(0);
		}
	}

	function scroll(index)
	{
		if (index < 0) {
			return;
		}

		var item = rootView.itemAtIndex(index);
		if (!item || !item.hasView) {
			console.warn("FeedView::scroll(", index, ") Cannot scroll to unready item", item, ", postponed...");
			defferedPositionIndex = index;
			return ;
		}
		console.debug("FeedView::scroll(", index, ") Scrolling...");
		currentIndex = index;
	}

	orientation: Qt.Vertical
	verticalLayoutDirection: ListView.BottomToTop
	spacing: 12
	focus: true

	highlightMoveDuration: 5000
	highlightFollowsCurrentItem: false
	highlightRangeMode: (rootView.currentIndex == 0) ? ListView.StrictlyEnforceRange : ListView.NoHighlightRange

	preferredHighlightBegin: ListView.NoHighlightRange == highlightRangeMode ? 0 : headerItemHeight
	preferredHighlightEnd: ListView.NoHighlightRange == highlightRangeMode ? 0 : (headerItemHeight + currentItemHeight)

	highlight: Rectangle {
		id: lightRectangle

		width: rootView.width + rootView.messageMargin
		height: rootView.currentItem ? rootView.currentItem.height : 0
		x: -rootView.messageMargin
		y: rootView.currentItem ? rootView.currentItem.y : -1

		color: "#f3f4f6"
		visible: rootView.currentItem

		Behavior on y {
			enabled: y >= 0
			SequentialAnimation {
				SpringAnimation {
					spring: 3
					damping: 0.2
				}
				ScriptAction {
					script: {
						lightAnimation.start();
					}
				}
			}
		}

		NumberAnimation {
			id: lightAnimation

			target: lightRectangle
			running: false
			alwaysRunToEnd: true
			duration: 3500
			easing.type: Easing.InElastic

			property: "opacity"
			from: 1
			to: 0
		}
	}

	footerPositioning: ListView.InlineFooter
	footer: Column {
		width: rootView.width

		Label {
			id: textLabel

			width: parent.width
			horizontalAlignment: Text.AlignHCenter
			font.pointSize: 10
			color: "lightgrey"
			padding: 10
		}

		states: [
			State {
				name: "complete"

				when: rootView.channelModel.isInitialized && !rootView.channelModel.hasUnloadedMessages
				PropertyChanges {
					target: textLabel
					text: qsTr("This is the very beginning of the neverending chat")
				}
			},
			State {
				name: "prefetch"

				when: rootView.channelModel.hasUnloadedMessages && !rootView.channelModel.hasEnqueuedFeeds
				PropertyChanges {
					target: textLabel
					text: qsTr("Pull down a bit more to load previous messages")
				}
			},
			State {
				name: "fetch"

				when: !rootView.channelModel.isInitialized || (rootView.channelModel.hasUnloadedMessages && rootView.channelModel.hasEnqueuedFeeds)
				PropertyChanges {
					target: textLabel
					text: qsTr("Loading messages from history...")
				}
			}
		]
	}

//	headerPositioning: (rootView.currentIndex == 0) ? ListView.InlineHeader : ListView.OverlayHeader
	headerPositioning: ListView.InlineHeader
	header: Column {
		readonly property int assistantCount: assistantRepeater.count

		anchors.left: parent.left
		anchors.leftMargin: 0 - rootView.messageMargin
		anchors.right: parent.right

		padding: 0
		spacing: 1

		Repeater {
			id: assistantRepeater

			model: rootView.channelModel.assistantModel

			delegate: AssistLayout {
				required property int index
				required property var uid
				required property var affected_subject_qd_list
				required property var scheme
				required property var layout
				required property var body

				anchors.margins: 0

				width: parent.width
				height: implicitHeight

				Component.onCompleted: {
					render(scheme);
				}

				onRendered: {
					if (hasModel && model.import(rootView.channelModel, uid, body, affected_subject_qd_list, layout)) {
						visible = true;
					}
				}
			}
		}
	}

	model: rootView.channelModel.messageEntity

	Connections {
		target: model

		function onRowsInserted(parent, first, last)
		{
			if (!rootView.messagesAhead && first >= rootView.lastVisibleIndex) {
				Qt.callLater(rootView.scroll, 0);
			} else {
				Qt.callLater(rootView.validate);
			}
		}
	}

	Connections {
		target: rootView

		enabled: rootView.defferedPositionIndex >= 0

		function onDelegateListed(index)
		{
			if (rootView.defferedPositionIndex === index) {
				rootView.currentIndex = index;
				rootView.defferedPositionIndex = -1;
				Qt.callLater(rootView.validate);
			}
		}
	}

	delegate: MessageLayout {
		required property int index
		required property var uid
		required property var id
		required property var reply_id
		required property var author_subject_id
		required property var scheme
		required property var body
		required property date post_timestamp
		required property date modification_timestamp
		readonly property date timestamp: !modification_timestamp ? post_timestamp : modification_timestamp > post_timestamp ? modification_timestamp : post_timestamp

		Component.onCompleted: {
			console.log("ListView::MessageLayout[", index, "]::Component.onCompleted");
			render(scheme, Conversation.FeedLayout);
			rootView.delegateCompleted(index);
		}

		Component.onDestruction: {
			console.log("ListView::MessageLayout[", index, "]::Component.onDestruction");
			rootView.delegateDestructed(index);
		}

		ListView.onViewChanged: {
			console.log("ListView::MessageLayout[", index, "]::ListView.onViewChanged");
			rootView.delegateListed(index);
		}

		width: rootView.width - rootView.messageMargin
		viewMinimumWidth: MessageLayout.Left === side ? (rootView.messageMinimumWidth - imageWidth) : rootView.messageMinimumWidth
		viewMinimumHeight: rootView.messageMinimumHeight
		side: !rootView.channelModel || !rootView.channelModel.descriptor ? MessageLayout.Left : rootView.channelModel.descriptor.isOwner(author_subject_id) ? MessageLayout.Right : MessageLayout.Left
		flags: (index > 10 ? MessageLayout.AsyncRender : MessageLayout.None) | MessageLayout.LeftSideImage | MessageLayout.EditModifier | MessageLayout.ReceiveModifier | MessageLayout.ReadModifier | MessageLayout.StateLabel | MessageLayout.DateTimeLabel

		function rerender()
		{
			render(scheme, Conversation.FeedLayout);
		}

		function reload()
		{
			load(rootView.channelModel, id, uid, reply_id, author_subject_id, body, new Date(post_timestamp), new Date(modification_timestamp));
		}

		NumberAnimation on opacity {
			from: 0.0
			to: 1.0
			duration: 250
			running: !rootView.isPopulated
			alwaysRunToEnd: true
			easing.type: Easing.InOutQuad
		}

		onTimestampChanged: {
			if (hasModel) {
				if (model.schemeQd !== scheme) {
					Qt.callLater(rerender);
				} else {
					Qt.callLater(reload);
				}
			}
		}

		onViewCompleted: {

		}

		onViewHeld: {
			if (!canceled && hasModel) {
				contextMenu.overlay(position, model);
			}
		}

		onRendered: {
			load(rootView.channelModel, id, uid, reply_id, author_subject_id, body, new Date(post_timestamp), new Date(modification_timestamp));
		}

		onModified: {
			rootView.modifyRequested(model.exportMeta());
		}

		onPopup: {
			rootView.popupRequested(component, properties);
		}
	}

	onContentYChanged: {
//		console.warn("onContentYChanged:", contentY, height)
		validate();
	}

	onFlickStarted: {
		contextMenu.messageModel = undefined;
		rootView.currentIndex = -1;
	}

	onFlickEnded: {
		if (rootView.defferedPositionIndex >= 0 && !itemAtIndex(rootView.defferedPositionIndex)) {
			console.error("Need to scroll");
			/// @todo Need to locate in case if index wont reachable because corresonding item wasnt loaded yet
		}
	}

	add: Transition {
		enabled: !(rootView.moving || rootView.dragging || rootView.flicking)
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
		enabled: !(rootView.moving || rootView.dragging || rootView.flicking)
		NumberAnimation {
			properties: "x"
			duration: 250
			easing.type: Easing.OutBounce
		}
	}

	ScrollBar.vertical: ScrollBar {
		interactive: false
	}

	Menu {
		id: contextMenu

		property point messagePosition: Qt.point(0, 0)
		property var messageModel: undefined
		property list<MessageLayout> messageList

		width: 200 /// < messageLayout.viewWidth
		parent: Overlay.overlay
		x: messagePosition.x
		y: messagePosition.y
		padding: 2
		spacing: 1
		visible: !rootView.flicking && messageModel !== undefined

		onAboutToHide: {
			for (var m = 0; m < messageList.length; ++m) {
				messageList[m].flags ^= MessageLayout.BlurEffect
			}
			messageList = [];
		}

		function overlay(position, model)
		{
			messagePosition.x = Math.min(rootView.width - rootView.messageMargin - width, position.x - rootView.messageMargin);
			messagePosition.y = position.y;
			messageModel = model;
			Native.vibrate();
			for (var m = rootView.firstVisibleIndex; m <= rootView.lastVisibleIndex; ++m) {
				var message = rootView.itemAtIndex(m);
				if (message && message.id !== messageModel.id) {
					messageList.push(message);
					message.flags |= MessageLayout.BlurEffect;
				}
			}
		}

		function takeMessageModel()
		{
			var retval = messageModel;
			messageModel = undefined;
			return retval;
		}

		background: Rectangle {
			implicitWidth: parent.width
			implicitHeight: 40
			color: Material.accent
			border.color: "transparent"
			radius: 8
		}

		delegate: MenuItem {
			id: contextMenuItem

			implicitWidth: parent.width
			implicitHeight: contextMenuItem.contentItem.contentHeight

			arrow: Item {
			}

			indicator: Item {
			}

			contentItem: Text {
				leftPadding: contextMenuItem.indicator.width
				rightPadding: contextMenuItem.arrow.width
				text: contextMenuItem.text
				font: contextMenuItem.font
				opacity: enabled ? 1.0 : 0.3
				color: "white"
				horizontalAlignment: Text.AlignLeft
				verticalAlignment: Text.AlignVCenter
				elide: Text.ElideRight
			}

			background: Rectangle {
				implicitWidth: contextMenuItem.implicitWidth
				implicitHeight: 40
				opacity: enabled ? 1 : 0.3
				color: "transparent"
			}
		}

		Action {
			text: qsTr("Copy")
			enabled: contextMenu.messageModel ? contextMenu.messageModel.hasClipboard : false

			onTriggered: {
				if (contextMenu.messageModel) {
					contextMenu.takeMessageModel().clip();
				}
			}
		}

		Action {
			text: contextMenu.messageModel && (contextMenu.messageModel.progress !== 100) ? qsTr("Retry") : qsTr("Edit")
			enabled: contextMenu.messageModel ? (contextMenu.messageModel.progress <= 0) || (contextMenu.messageModel.isGlobal && contextMenu.messageModel.isEditable) : false

			onTriggered: {
				if (contextMenu.messageModel) {
					if (contextMenu.messageModel.progress === 100) {
						rootView.redactionRequested(contextMenu.takeMessageModel());
					} else if (contextMenu.messageModel.progress <= 0) {
						rootView.repushRequested(contextMenu.takeMessageModel().uid);
					}
				}
			}
		}

		Action {
			text: qsTr("Delete")
			enabled: contextMenu.messageModel ? (contextMenu.messageModel.progress <= 0) || (contextMenu.messageModel.isGlobal && contextMenu.messageModel.isErasable) : false

			onTriggered: {
				if (contextMenu.messageModel) {
					if (contextMenu.messageModel.progress === 100) {
						rootView.withdrawRequested(contextMenu.takeMessageModel().selfQd);
					} else if (contextMenu.messageModel.progress <= 0) {
						rootView.discardRequested(contextMenu.takeMessageModel().uid);
					}
				}
			}
		}

		MenuSeparator {

		}

		Action {
			text: qsTr("Close")

			onTriggered: {
				contextMenu.takeMessageModel();
			}
		}
	}

	RoundButton {
		id: jumpButton

		anchors.bottom: rootView.bottom
		anchors.leftMargin: rootView.messageMargin
		anchors.bottomMargin: 5
		width: height

		Material.background: Material.accent
		highlighted: true
		opacity: 0.0

		text: "v"

		states: [
			State {
				when: messagesAhead
				PropertyChanges {
					target: jumpButton
					opacity: 1.0
				}
			},
			State {
				when: !messagesAhead
				PropertyChanges {
					target: jumpButton
					opacity: 0.0
				}
			}
		]
		transitions: [
			Transition {
				NumberAnimation {
					property: "opacity"
					duration: 300
				}
			}
		]

		onClicked: {
			rootView.positionViewAtBeginning();
			rootView.scroll(0);
		}
	}
}
