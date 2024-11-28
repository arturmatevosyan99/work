import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import Mentalgami.View 0.1 as View
import Mentalgami.Model 0.1 as Model
import Mentalgami.Meta 0.1

View.RiddleView {
	id: rootView

	focus: true

	model: Model.BoolRiddle {

	}

	function validateAnswer(answer) {
		if (rootView.model.validate(answer)) {
			rootView.specifications |= RiddleInformation.ForwardActive;
		} else {
			rootView.specifications &= ~RiddleInformation.ForwardActive;
		}
	}

	onAnswerReceived: {
		validateAnswer(answer);
	}

	Row {
		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.horizontalCenter: parent.horizontalCenter

		ButtonGroup {
			id: group

			onCheckedButtonChanged: {
				validateAnswer(checkedButton == yesButton);
			}
		}

		RadioButton {
			id: yesButton

			text: qsTr("Yes")
			ButtonGroup.group: group

			enabled: !rootView.model.isReadOnly
		}

		RadioButton {
			id: noButton

			text: qsTr("No")
			ButtonGroup.group: group

			enabled: !rootView.model.isReadOnly
		}
	}
}
