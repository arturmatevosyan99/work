import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Core.Meta 0.1
import Core.View 0.1
import Core.System 0.1

import Repository.Model 0.1 as Model

import "../../../Core/View/"

Page {
	id: rootView

	Component.onCompleted: {
		console.log("WebApplication::onCompleted");
	}

	Component.onDestruction: {
		console.log("WebApplication::onDestruction");
	}

	property string headerTitle: qsTr("Web Application")
	property Model.WebApplication model: Model.WebApplication {
	} onModelChanged: {
		console.debug("WebApplication::onModelChanged", rootView.model);
		modelConnection.target = model;
		rootView.model.opened();
	}

	Connections {
		id: modelConnection

		target: rootView.model

		function onOpened()
		{
			if (rootView.model.descriptor.objectName) {
				rootView.headerTitle = rootView.model.descriptor.objectName;
			}
			rootView.model.sign("");
		}

		function onAccessed(url, preset)
		{
			if (preset.length > 0) {
				webView.presetAlias = preset;
			}
			webView.ual = url;
		}

		function onDenied()
		{
			Native.alertAction(qsTr("Auch!"), qsTr("Web appliaction is not available for you by that time"), ["Ok"]);
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	WebView {
		id: webView

		anchors.fill: parent
		anchors.topMargin: rootView.implicitHeaderHeight
		anchors.bottomMargin: rootView.implicitFooterHeight

		title: rootView.headerTitle
		presetDefault: true

		onPresetChanged: {
			if (preset) {
				preset.parent = rootView;
			}
		}
	}
}
