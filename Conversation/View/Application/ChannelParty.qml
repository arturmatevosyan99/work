import QtQuick 2.15
import QtQml 2.15
import QtQml.Models 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

import Core.View 0.1
import Conversation.Model 0.1

import "../../../Core/View"

Page {
	id: application

	Component.onCompleted: {
		console.log("ChannelParty::onCompleted");
	}

	Component.onDestruction: {
		console.log("ChannelParty::onDestruction");
	}

	///
	property bool syncAvailable: false
	///
	property GroupChannel model: GroupChannel {
	} onModelChanged: {
		console.debug("ChannelParty::onModelChanged", application.model);
		modelConnection.target = model;
		if (modelConnection.target.isOpened) {
			modelConnection.onOpened();
		}
	}

	Connections {
		id: syncConnection

		target: application.model.session
		enabled: false

		function onUpstreamed(onoff)
		{
			application.syncAvailable = application.model.session.hasUpstream;
		}
	}

	Connections {
		id: modelConnection

		target: model

		function onOpened()
		{
			///
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		stackView: application.StackView.view

		titleText: qsTr("Members")

		beatMode: syncAvailable ? StackBar.BeatMode.Stable : StackBar.BeatMode.Pulse

		keyboardCounterflow: true

		onBackClicked: {
		}

		optionStyle: StackBar.OptionStyle.None
		onOptionClicked: {
		}
	}

	ListView {
		id: listView

		anchors.left: parent.left
		anchors.top: parent.top
		width: parent.width
		height: parent.height

		orientation: Qt.Vertical

		/// @todo По умолчанию не открыта
		model: application.model.memberModel

		delegate: ItemDelegate {
			/// @todo swipe for remove, change role, etc
			text: "@" + subject_id
			width: listView.width
		}
	}
}
