import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import SmartCare.View 0.1
import SmartCare.Meta 0.1

RiddleView {
	id: rootView

	property alias answer: answerField.text

	focus: true

	question: qsTr("Enter your email address")
	description: qsTr("Before proceeding further, check whether the data you entered is correct")
	specifications: RiddleInformation.BackwardAvailable | RiddleInformation.ForwardAvailable | RiddleInformation.BackwardActive

	onAnswerReceived: {
		answerField.text = answer
	}

	TextField {
		id: answerField

		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		focus: true

		placeholderText: qsTr("example@email.com")
		inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
		validator: RegularExpressionValidator {
			regularExpression: /(\w+([\-+_.]\w+)*@\w+([\-+_.]\w+)*\.\w+([\-+_.]\w+)*)/
		}

		onAcceptableInputChanged: {
			if (acceptableInput) {
				rootView.specifications |= RiddleInformation.ForwardActive
			} else {
				rootView.specifications &= ~RiddleInformation.ForwardActive
			}
		}

		onAccepted: {
			if (acceptableInput) {
				rootView.answered();
			}
		}
	}
}
