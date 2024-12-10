import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import SmartCare.View 0.1 as View
import SmartCare.Model 0.1 as Model
import SmartCare.Meta 0.1

View.RiddleView {
	id: rootView

	focus: SwipeView.isCurrentItem

	model: Model.TextRiddle {

	}

	property alias loaderItem: loader.item

	function validateAnswer(answer) {
		if (rootView.model.validate(answer)) {
			rootView.specifications |= RiddleInformation.ForwardActive;
		} else {
			rootView.specifications &= ~RiddleInformation.ForwardActive;
		}
	}

	onAnswerReceived: {
		loaderItem.answerPlace = answer;
	}

	Loader {
		id: loader

		focus: SwipeView.isCurrentItem

		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		property int preferableComponent: rootView.model.component

		Component {
			id: textFieldComponent

			TextField {
				id: answerField

				focus: SwipeView.isCurrentItem

				property alias answerPlace: answerField.text

				enabled: !rootView.model.isReadOnly
				placeholderText: rootView.model.placeholder
				inputMethodHints: Qt.ImhNoPredictiveText

				onTextChanged: {
					validateAnswer(text);
				}
			}
		}

		Component {
			id: textAreaComponent

			TextArea {
				id: answerField

				focus: SwipeView.isCurrentItem

				property alias answerPlace: answerField.text

				enabled: !rootView.model.isReadOnly
				placeholderText: rootView.model.placeholder

				onTextChanged: {
					validateAnswer(text);
				}
			}
		}

		onPreferableComponentChanged: {
			switch(preferableComponent) {
				case Model.TextRiddle.PreferableComponent.TextField:
					sourceComponent = textFieldComponent;
					break;
				case Model.TextRiddle.PreferableComponent.TextArea:
					sourceComponent = textAreaComponent;
					break;
				default:
					sourceComponent = textFieldComponent;
			}
		}
	}
}
