import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import SmartCare.Meta 0.1
import SmartCare.Model 0.1 as Model
import SmartCare.View 0.1 as View

View.RiddleView {
	id: rootView

	focus: true

	model: Model.DateRiddle {

	}

	property var locale: Qt.locale()

	function validateAnswer(answer) {
		if (rootView.model.validate(answer)) {
			rootView.specifications |= RiddleInformation.ForwardActive;
		} else {
			rootView.specifications &= ~RiddleInformation.ForwardActive;
		}
	}

	function recalculateDateAnswer() {
		if (dayTumbler.completed && monthTumbler.completed && yearTumbler.completed) {
			datePicker.currentDate.setDate(dayTumbler.model[dayTumbler.currentIndex], monthTumbler.model[monthTumbler.currentIndex], yearTumbler.model[yearTumbler.currentIndex]);
			validateAnswer(datePicker.currentDate);
		}
	}

	function getDaysInMonth(month, year) {
		return new Date(year, month, 0).getDate();
	}

	onAnswerReceived: {
		validateAnswer(answer);
	}

	Component {
		id: dayAndYearDelegateComponent

		Text {
			text: modelData

			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter

			font: Tumbler.tumbler.font
			opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
		}
	}

	Component {
		id: monthDelegateComponent

		Text {
			text: locale.monthName(modelData - 1, Locale.ShortFormat)

			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter

			font: Tumbler.tumbler.font
			opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
		}
	}

	RowLayout {
		id: datePicker

		property var currentDate: rootView.model.makeDefault()

		property alias dayTumbler: dayTumbler
		property alias monthTumbler: monthTumbler
		property alias yearTumbler: yearTumbler

		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
		anchors.horizontalCenter: parent.horizontalCenter

		Tumbler {
			id: dayTumbler

			property int defaultDay: rootView.model.validator.defaultValue.day
			property bool completed: false

			visible: rootView.model.validator.minimum.day !== -1 && rootView.model.validator.maximum.day !== -1
			enabled: !rootView.model.isReadOnly

			model: days()
			delegate: dayAndYearDelegateComponent
			visibleItemCount: 3

			onDefaultDayChanged: {
				if (rootView.model.validator.defaultValue.day !== -1) {
					currentIndex = rootView.model.validator.defaultValue.day - 1;
					completed = true;
				}
			}

			onCompletedChanged: {
				if (completed) {
					recalculateDateAnswer();
				}
			}

			onMovingChanged: {
				if (!moving) {
					recalculateDateAnswer();
				} else {
					rootView.specifications &= ~RiddleInformation.ForwardActive;
				}
			}

			function days() {
				var array = [];

				for (var i = 1; i <= 31; i++) {
					array.push(i);
				}

				return array;
			}

			function updateDayModel() {
				if (monthTumbler.completed && yearTumbler.completed) {
					var daysInMonth = getDaysInMonth(monthTumbler.model[monthTumbler.currentIndex], yearTumbler.model[yearTumbler.currentIndex]);

					if (dayTumbler.model.length !== daysInMonth) {
						var lastDayIndex = dayTumbler.currentIndex;
						var newDayModel = [];

						for (var i = 1; i <= daysInMonth; ++i) {
							newDayModel.push(i);
						}

						dayTumbler.model = newDayModel;
						var newDayIndex = Math.min(daysInMonth - 1, lastDayIndex);
						dayTumbler.positionViewAtIndex(newDayIndex, Tumbler.Center);
					}
				}
			}
		}

		Tumbler {
			id: monthTumbler

			property int defaultMonth: rootView.model.validator.defaultValue.month
			property bool completed: false

			visible: rootView.model.validator.minimum.month !== -1 && rootView.model.validator.maximum.month !== -1
			enabled: !rootView.model.isReadOnly

			model: months()
			delegate: monthDelegateComponent
			visibleItemCount: 3

			onDefaultMonthChanged: {
				if (rootView.model.validator.defaultValue.month !== -1) {
					currentIndex = rootView.model.validator.defaultValue.month - 1;
					completed = true;
				}
			}

			onCompletedChanged: {
				if (completed) {
					dayTumbler.updateDayModel();
					recalculateDateAnswer();
				}
			}

			onMovingChanged: {
				if (!moving) {
					dayTumbler.updateDayModel();
					recalculateDateAnswer();
				} else {
					rootView.specifications &= ~RiddleInformation.ForwardActive;
				}
			}

			function months() {
				var array = [];

				for (var i = 1; i <= 12; i++) {
					array.push(i);
				}

				return array;
			}
		}

		Tumbler {
			id: yearTumbler

			property bool isModelDataValid: rootView.model.validator.minimum.year !== -1 && rootView.model.validator.maximum.year !== -1
			property int defaultYear: rootView.model.validator.defaultValue.year
			property bool completed: false

			visible: isModelDataValid
			enabled: !rootView.model.isReadOnly

			model: isModelDataValid ? years() : []
			delegate: dayAndYearDelegateComponent
			visibleItemCount: 3

			onDefaultYearChanged: {
				if (rootView.model.validator.defaultValue.year !== -1) {
					currentIndex = rootView.model.validator.defaultValue.year - rootView.model.validator.minimum.year;
					completed = true;
				}
			}

			onCompletedChanged: {
				if (completed) {
					dayTumbler.updateDayModel();
					recalculateDateAnswer();
				}
			}

			onMovingChanged: {
				if (!moving) {
					dayTumbler.updateDayModel();
					recalculateDateAnswer();
				} else {
					rootView.specifications &= ~RiddleInformation.ForwardActive;
				}
			}

			function years() {
				var array = [];

				for (var i = rootView.model.validator.minimum.year; i <= rootView.model.validator.maximum.year; i++) {
					array.push(i);
				}

				return array;
			}
		}
	}
}
