import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14

import SmartCare.Model 0.1
import "../../View"

Rectangle {
	id: root
	width: parent.width
	height: parent.height
	color: Material.background

	SmartBook {
		id: application
		Component.onCompleted: {
			setSource("file:///home/arturvm/workspace/MDF/MetabolicPlate")
			console.log(application.index.data(application.index.index(0,0), "filePath"))
		}
	}

	MarkdownView {
		id: markdownDocument
		model: application.index.rootDocument
	}
}
