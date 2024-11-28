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

GridView {
	id: rootView

	required property GroupChannel channelModel

	signal loadRequested(var end)
	signal modifyRequested(var meta)
	signal popupRequested(var component, var properties)

	anchors.leftMargin: 2
	anchors.rightMargin: 2
	cellWidth: rootView.width / 4
	cellHeight: cellWidth * 1.5

	model: rootView.channelModel.swapEntity

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
			render(scheme, Conversation.GridLayout);
		}

		width: rootView.cellWidth
		viewMinimumWidth: rootView.cellWidth
		viewMinimumHeight: rootView.cellHeight
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
}
