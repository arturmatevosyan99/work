import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Core.Meta 0.1
import Core.Model 0.1
import Core.System 0.1

import SmartCare.View 0.1
import SmartCare.Model 0.1
import SmartCare.Controller 0.1
import SmartCare.Meta 0.1

import "../../View"

import "../../../Core/View"

Page {
	id: application

	StackView.onActivated: {
		swipeView.forceActiveFocus();
	}

	property alias headerHeight: nextButton.height /// @xxx Ugh!
	property bool isFinished: false

	/// @note Final state has been reached
	signal finished();

	/**
	 * @brief Model which provides riddle list
	 */
	property GenericQuiz model: GenericQuiz {

	}

	onModelChanged: {
		console.debug("PatientQuiz::onModelChanged", model);
		modelConnections.target = model;
		if (modelConnections.target.isOpened) {
			modelConnections.onOpened();
		}
	}

	Connections {
		id: modelConnections

		target: application.model

		function onAsked(there_is_more)
		{
			if (!there_is_more) {
				application.isFinished = true;
				finished();
			}
		}

		function onAskFaulted(message)
		{
			Alert.action("GENERICQUIZ_ASK_ERROR", qsTr("Error"), message ?? qsTr("Unexpected error(1)"), AlertOption.Ok);
		}

		function onAnswered(there_is_more)
		{
			if (application.isFinished) {
				finished();
				return ;
			}

			if (!there_is_more) {
				application.isFinished = true;
			}

			controller.nextRequested();
		}

		function onAnswerFaulted(message)
		{
			Alert.action("GENERICQUIZ_ANSWER_ERROR", qsTr("Error"), message ?? qsTr("Unexpected error(2)"), AlertOption.Ok);
		}

		function onOpened()
		{
			application.model.name();
			application.model.ask();
		}

		function onExceptionThrew(level, message)
		{
			Alert.action("GENERICQUIZ_EXCEPTION", qsTr("Error"), message ?? qsTr("Unexpected error(0)"), AlertOption.Ok);
		}
	}

	/**
	 * @brief Flow controller
	 */
	property QuizFlow controller: QuizFlow {
		riddleList: application.model.riddleList

		delegate: swipeView

		onNextRequested: {
			swipeView.goToNext();
		}

		onPreviousRequested: {
			delegate.decrementCurrentIndex();
		}

		function getCurrentRiddleQuestion()
		{
			return swipeView.currentItem.question;
		}

		function getCurrentRiddleDescription()
		{
			return swipeView.currentItem.description;
		}
/*
		function answer(answer)
		{
			swipeView.currentItem.answerReceived(answer);
			if (!goToNextQuestion()) {
				return false;
			}
			return false;
		}
*/
		function enterAnswer(answer)
		{
			swipeView.currentItem.answerReceived(answer);
		}

		function canGoToNextQuestion()
		{
			return nextButton.enabled && nextButton.visible;
		}

		function goToNextQuestion()
		{
			if (nextButton.enabled && nextButton.visible) {
				nextButton.clicked();
				return true;
			}
			return false;
		}

		function canGoToPreviousQuestion()
		{
			return previousButton.enabled && previousButton.visible;
		}

		function goToPreviousQuestion()
		{
			if (previousButton.enabled && previousButton.visible) {
				previousButton.clicked();
				return true;
			}
			return false;
		}

		function isThisTheEnd()
		{
			return swipeView.currentIndex == swipeView.count - 1;
		}

		function exitFromQuiz()
		{
			stackBar.back();
		}
	}

	title: model.title
	header: StackBar {
		id: stackBar

		titleText: qsTr("Survey")
		subtitleText: application.model.title

		stackView: application.StackView.view

		optionStyle: StackBar.OptionStyle.Stack
		onOptionClicked: {
			return riddleMenu.visible ? riddleMenu.close() : riddleMenu.open()
		}

		Menu {
			id: riddleMenu

			parent: stackBar.optionButton
			topMargin: stackBar.optionButton.height

			MenuItem {
				text: qsTr("Exit")

				onTriggered: {
					riddleMenu.close();
					stackBar.back();
				}
			}
		}
	}

	ColumnLayout {
		id: headerLayout

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right

		Item {
			Layout.fillWidth: true
			Layout.preferredHeight: Math.max(previousButton.visible ? previousButton.height : 0, nextButton.visible ? nextButton.height : 0)
			Layout.minimumHeight: Layout.preferredHeight
			Layout.maximumHeight: Layout.preferredHeight

			Button {
				id: previousButton

				anchors.top: parent.top
				anchors.left: parent.left

				flat: true
				visible: swipeView.currentItem ? (swipeView.currentItem.specifications & RiddleInformation.BackwardAvailable) : false
				enabled: (swipeView && swipeView.currentItem) ? (swipeView.currentIndex != 0) && (swipeView.currentItem.specifications & RiddleInformation.BackwardAvailable) && (swipeView.currentItem.specifications & RiddleInformation.BackwardActive) : false

				text: "← " + qsTr("Previous")

				onClicked: {
					swipeView.decrementCurrentIndex();

					if (swipeView.currentItem.type !== "Generic" && swipeView.currentItem.type !== "Custom") {
						application.model.riddleList.removeTail(swipeView.currentIndex);
					}

					application.isFinished = false;
				}
			}

			Button {
				id: nextButton

				anchors.top: parent.top
				anchors.right: parent.right

				flat: true
				visible: swipeView.currentItem ? (swipeView.currentItem.specifications & RiddleInformation.ForwardAvailable) : false
				enabled: (swipeView && swipeView.currentItem) ? (swipeView.currentItem.specifications & RiddleInformation.ForwardAvailable) && (swipeView.currentItem.specifications & RiddleInformation.ForwardActive) && !swipeView.currentItem.isBusy : false

				text: !application.isFinished ? qsTr("Next") + " →" : qsTr("Finish")

				onClicked: {
					swipeView.currentItem.answered();
				}
			}
		}

		ProgressBar {
			id: progressBar
			Layout.fillWidth: true

			value: swipeView.currentIndex !== -1 ? (application.model.approximateLength !== 0 ? (swipeView.currentIndex + 1) / application.model.approximateLength : (swipeView.currentIndex + 1) / swipeView.count) : 0;

			Behavior on value {
				NumberAnimation {
					id: numberAnimation

					duration: 250
				}
			}

//			SequentialAnimation {
//				running: swipeView.currentItem ? swipeView.currentItem.model && swipeView.currentItem.isBusy : false
//				alwaysRunToEnd: true

//				PropertyAnimation {
//					target: progressBar
//					property: "value"
//					from: swipeView.currentIndex !== -1 && swipeView.count !== 0 ? swipeView.currentIndex / swipeView.count : 0
//					to: 1
//					duration: 750
//					easing.type: Easing.InCubic
//				}

//				PropertyAnimation {
//					target: progressBar
//					property: "value"
//					from: 1
//					to: swipeView.currentIndex !== -1 && swipeView.count !== 0 ? swipeView.currentIndex / swipeView.count : 0
//					duration: 750
//					easing.type: Easing.InCubic
//				}
//			}
		}

		Label {
			Layout.fillWidth: true
			Layout.preferredHeight: visible ? contentHeight : 0
			Layout.minimumHeight: Layout.preferredHeight
			Layout.maximumHeight: Layout.preferredHeight

			Layout.leftMargin: 4
			Layout.rightMargin: 4

			visible: swipeView.currentItem ? swipeView.currentItem.question.size !== 0 : false
			text: visible ? swipeView.currentItem.question : ""
			font.pointSize: nextButton.font.pointSize * 1.5
			font.bold: true
			horizontalAlignment: Text.AlignHCenter

			wrapMode: Text.WordWrap
		}

		Label {
			Layout.fillWidth: true
			Layout.preferredHeight: visible ? contentHeight : 0
			Layout.minimumHeight: Layout.preferredHeight
			Layout.maximumHeight: Layout.preferredHeight

			Layout.leftMargin: 4
			Layout.rightMargin: 4

			visible: swipeView.currentItem ? swipeView.currentItem.description.size !== 0 : false
			text: visible ? swipeView.currentItem.description : ""
			font.pointSize: nextButton.font.pointSize
			font.bold: false
			horizontalAlignment: Text.AlignHCenter

			wrapMode: Text.WordWrap
		}
	}

	SwipeView {
		id: swipeView

		property int delayIndex: -1

		Connections {
			id: delayConnections

			target: application.controller
			enabled: swipeView.delayIndex > -1

			function onStacked(view, index)
			{
				swipeView.delayIndex = -1;
				swipeView.goToNext();
			}
		}

		function goToNext() {
			if (swipeView.currentIndex + 1 < swipeView.count) {
				swipeView.incrementCurrentIndex();
			} else {
				swipeView.delayIndex = swipeView.currentIndex + 1;
			}
		}

		anchors.top: headerLayout.bottom
		anchors.topMargin: 10
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		focus: true
		interactive: false

		onCurrentItemChanged: {
			controller.activate(currentItem);
		}
	}
	Connections {
		id: riddleConnections

		target: swipeView.currentItem
		ignoreUnknownSignals: true

		function onAnswered()
		{
			if (!!swipeView.currentItem.model && swipeView.currentItem.type !== "Custom") {
				application.model.answer(swipeView.currentItem.model);
			}
		}
	}

	onFinished: {
		stackBar.back();
	}
}
