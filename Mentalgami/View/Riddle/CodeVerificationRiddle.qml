import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import Mentalgami.View 0.1
import Mentalgami.Meta 0.1

import "../../../Core/View"

RiddleView {
	id: rootView

	property alias answer: pinInput.text

	focus: true
	question: qsTr("Enter the code we just sent to you")
	description: qsTr("Pay attention, the process of receiving the code can take up to a minute")
	specifications: RiddleInformation.BackwardAvailable | RiddleInformation.ForwardAvailable | RiddleInformation.BackwardActive

	onAnswerReceived: {
		pinInput.text = answer;
	}

	function clear()
	{
		pinInput.clear();
	}

	PinInput {
		id: pinInput

		focus: true

		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		fontPointSize: 32
		sectionWidth: 50
		sectionGap: 10
		sectionRadius: 10

		onTextChanged: {
			if (text.length == 4 && text.indexOf(' ') < 0) {
				rootView.specifications |= RiddleInformation.ForwardActive;
			} else {
				rootView.specifications &= ~RiddleInformation.ForwardActive;
			}
		}

		onInputCompleted: {
			rootView.answered();
		}
	}
}
