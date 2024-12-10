import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14

import SmartCare.Model 0.1
import "../"

Page {
	id: application

	property SmartBook name: SmartBook {
		// Set source if needed, e.g.:
		// setSource("file:///path/to/your/directory")
	}

	MarkdownView {
		id: markdownDocument
	}
}
