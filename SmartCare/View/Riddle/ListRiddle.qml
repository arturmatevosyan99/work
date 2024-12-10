import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import SmartCare.View 0.1 as View
import SmartCare.Model 0.1 as Model
import SmartCare.Meta 0.1

View.RiddleView {
	id: rootView

	focus: true

	model: Model.ListRiddle {

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

	ListView {
		id: answerField

		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.bottom: parent.bottom
		anchors.bottomMargin: parent.height / 5

		property var answers: []

		ScrollIndicator.vertical: ScrollIndicator {

		}

		clip: true
		focus: true

		model: rootView.model.answerModel

		ButtonGroup {
			id: exclusiveButtonGroup

			exclusive: true
		}

		delegate: Component {
			Loader {
				property int preferableComponent: rootView.model.component

				Component {
					id: textComponent

					Text {
						text: answerValue
					}
				}

				Component {
					id: checkBoxComponent

					CheckBox {
						id: checkBox

						text: answerValue
						anchors.verticalCenter: parent.verticalCenter

						enabled: !rootView.model.isReadOnly

						onCheckedChanged: {
							if (checked) {
								answerField.answers.push(answerKey);
							} else {
								const index = answerField.answers.indexOf(answerKey);

								if (index > -1) {
									answerField.answers.splice(index, 1);
								}
							}

							validateAnswer(answerField.answers);
						}
					}
				}

				Component {
					id: radioButtonComponent

					RadioButton {
						id: radioButton

						text: answerValue
						anchors.verticalCenter: parent.verticalCenter
						ButtonGroup.group: exclusiveButtonGroup

						enabled: !rootView.model.isReadOnly

						onCheckedChanged: {
							if (checked) {
								answerField.answers.push(answerKey);
							} else {
								/// @note Предполагается, что ListRiddle с radioButtonComponent отвечает за хранение только одного ключа
								answerField.answers = [];
							}

							validateAnswer(answerField.answers);
						}
					}
				}

				onPreferableComponentChanged: {
					switch(preferableComponent) {
						case Model.ListRiddle.PreferableComponent.Empty:
							sourceComponent = textComponent;
							break;
						case Model.ListRiddle.PreferableComponent.CheckBox:
							sourceComponent = checkBoxComponent;
							break;
						case Model.ListRiddle.PreferableComponent.RadioButton:
							sourceComponent = radioButtonComponent;
							break;
						default:
							sourceComponent = textComponent;
					}
				}
			}
		}
	}
}
