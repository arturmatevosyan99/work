import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import SmartCare.View 0.1 as View

View.RiddleView {
	id: rootView

	question: qsTr("Something went wrong")
	description: qsTr("This type of question is unsupported, please check if application is up to date")
}
