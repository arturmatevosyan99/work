import QtQml 2.15
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtWebView 1.15

import Repository.View 0.1

import "../../View/Application"

Rectangle {
	id: rootView

	WebApplication {
		anchors.fill: parent

		Component.onCompleted: {
			model.accessed("https://quser.notion.site/5490df3c15b94f9ca63f6cc926e0ccfc", "")
		}
	}
}
