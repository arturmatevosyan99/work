import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15

import "../../../Core/View"

Page {
	id: application

	Component.onCompleted: {
		console.log("SmartDiary::onCompleted");
	}

	Component.onDestruction: {
		console.log("SmartDiary::onDestruction");
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	/// @brief Smart diary (Allergy, Fitness Tracker, Feed diary, ...)
	header: StackBar {
		id: stackBar

		titleText: qsTr("Smart Diary")

		stackView: application.StackView.view
//		keyboardCounterflow: true

		onBackClicked: {
		}

		optionStyle: StackBar.OptionStyle.None
		onOptionClicked: {
		}
	}
}
