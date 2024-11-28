import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Mentalgami.View 0.1 as View
import Mentalgami.Model 0.1 as Model
import Mentalgami.Meta 0.1

View.RiddleView {
	id: rootView

	focus: SwipeView.isCurrentItem

	model: Model.NumericRiddle {

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
			id: tumblerDelegateComponent

			Text {
				text: modelData

				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				font: Tumbler.tumbler.font
				opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
			}
		}

		Component {
			id: tumblerComponent

			Tumbler {
				id: tumbler

				property alias answerPlace: tumbler.currentIndex

				model: numbers
				enabled: !rootView.model.isReadOnly

				delegate: tumblerDelegateComponent
				currentIndex: rootView.model.validator.defaultValue /// @note Не всегда корректно для Tumbler
				visibleItemCount: 5

				onCurrentIndexChanged: {
					validateAnswer(currentIndex);
				}

				readonly property var numbers: (function() {
					var array = [];

					if (rootView.model.validator.stepSize !== 0) {
						for (var i = rootView.model.validator.minimum; i <= rootView.model.validator.maximum; i = i + rootView.model.validator.stepSize) {
							array.push(i);
						}
					}

					return array;
				})()
			}
		}

		Component {
			id: spinBoxComponent

			SpinBox {
				id: spinBox

				property alias answerPlace: spinBox.value

				from: rootView.model.validator.minimum
				value: rootView.model.validator.defaultValue
				to: rootView.model.validator.maximum

				stepSize: Math.ceil(rootView.model.validator.stepSize)
				enabled: !rootView.model.isReadOnly

				onValueChanged: {
					validateAnswer(value);
				}
			}
		}

		Component {
			id: sliderComponent

			Column {
				property alias answerPlace: slider.value

				Label {
					id: currentValueLabel

					anchors.horizontalCenter: parent.horizontalCenter
				}

				Slider {
					id: slider

					anchors.left: parent.left
					anchors.right: parent.right

					from: rootView.model.validator.minimum
					value: rootView.model.validator.defaultValue
					to: rootView.model.validator.maximum

					stepSize: rootView.model.validator.stepSize
					enabled: !rootView.model.isReadOnly

					function countDecimalDigits(stepSize) {
						const stringStepSize = stepSize.toString();

						if (stringStepSize.includes('.')) {
							return stringStepSize.split('.')[1].length;
						}

						return 0;
					}

					onValueChanged: {
						currentValueLabel.text = value.toFixed(countDecimalDigits(rootView.model.validator.stepSize));
						validateAnswer(value);
					}
				}

				RowLayout {
					id: rowLayout

					width: parent.width

					Label {
						id: minimumlabel

						text: rootView.model.validator.minimum
						Layout.alignment: Qt.AlignLeft
					}

					Label {
						id: maximumlabel

						text: rootView.model.validator.maximum
						Layout.alignment: Qt.AlignRight
					}
				}
			}
		}

		Component {
			id: textFieldComponent

			TextField {
				id: textField

				focus: SwipeView.isCurrentItem

				property alias answerPlace: textField.text

				enabled: !rootView.model.isReadOnly
				text: rootView.model.validator.defaultValue
				inputMethodHints: Qt.ImhFormattedNumbersOnly

				validator: DoubleValidator {
					bottom: rootView.model.validator.minimum
					top: rootView.model.validator.maximum

					decimals: 1
					notation: DoubleValidator.StandardNotation
				}

				onTextChanged: {
					validateAnswer(text);
				}
			}
		}

		onPreferableComponentChanged: {
			switch(preferableComponent) {
				case Model.NumericRiddle.PreferableComponent.Tumbler:
					sourceComponent = tumblerComponent;
					break;
				case Model.NumericRiddle.PreferableComponent.SpinBox:
					sourceComponent = spinBoxComponent;
					break;
				case Model.NumericRiddle.PreferableComponent.Slider:
					sourceComponent = sliderComponent;
					break;
				case Model.NumericRiddle.PreferableComponent.TextField:
					sourceComponent = textFieldComponent;
					break;
				default:
					sourceComponent = tumblerComponent;
			}
		}
	}
}
