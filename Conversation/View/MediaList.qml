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

import "../../Core/View"

import "../../Qaos/QtQuick/ListView.js" as ListViewScript

ListView {
	id: rootView

	required property GroupChannel channelModel

	property int messagesProactive: 20
	property int firstVisibleIndex: 0
	property int lastVisibleIndex: 0

	readonly property int layout: orientation === ListView.Vertical ? Conversation.VerticalLayout : Conversation.HorizontalLayout

	signal delegateCompleted(var index)
	signal delegateDestructed(var index)
	signal loadRequested(var end)
	signal modifyRequested(var meta)
	signal popupRequested(var component, var properties)

	function validate()
	{
		console.warn("MediaList::ListView::validate");
		if (rootView.count <= 0) {
			return;
		}

		var index_map = ListViewScript.traverse(rootView);
		[rootView.firstVisibleIndex, rootView.lastVisibleIndex] = [index_map[ListView.Beginning], index_map[ListView.End]];

		if (rootView.channelModel.hasUnswappedMessages && (rootView.count - rootView.lastVisibleIndex) <= rootView.messagesProactive) {
			rootView.loadRequested(GroupChannel.Tail);
		}
	}

	spacing: 4

	Timer {
		id: validationTimer

		running: false
		repeat: false
		interval: 500

		onTriggered: {
			rootView.delegateCompleted.disconnect(validationTimer.restart);
			rootView.validate();
		}
	}

	model: rootView.channelModel.swapEntity
	onModelChanged: {
		rootView.delegateCompleted.connect(validationTimer.restart);
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

		Component.onCompleted: {
			render(scheme, rootView.layout);
			rootView.delegateCompleted(index);
		}

		Component.onDestruction: {
			rootView.delegateDestructed(index);
		}

		width: rootView.width
		viewMinimumWidth: width
		viewMinimumHeight: 10
		imageWidth: 0
		statusHeight: 0
		side: MessageLayout.Left
		flags: index % 2 == 0 ? MessageLayout.None : MessageLayout.OddIndex

		NumberAnimation on opacity {
			from: 0.0
			to: 1.0
			duration: 250
			running: true
			alwaysRunToEnd: true
			easing.type: Easing.InOutQuad
		}

		onRendered: {
			load(channelModel, id, uid, reply_id, author_subject_id, body, new Date(post_timestamp), new Date(modification_timestamp));
		}

		onPopup: {
			rootView.popupRequested(component, properties);
		}

		onModified: {
			rootView.modifyRequested(model.exportMeta());
		}

		onViewHeld: {
		}
	}

	onContentYChanged: {
		rootView.validate();
	}

	onContentXChanged: {
		rootView.validate();
	}
}