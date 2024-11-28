import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import Mentalgami.View 0.1 as View
import Mentalgami.Model 0.1 as Model

import "../../../Core/View"

View.TrackStackview {
	id: rootView

	WebView {
		anchors.fill: rootView
		anchors.topMargin: rootView.implicitHeaderHeight
		anchors.bottomMargin: rootView.implicitFooterHeight

		title: model.title
		ual: model.file
		presetDefault: true

		onPresetChanged: {
			if (preset) {
				preset.parent = rootView;
			}
		}

		onLoaded: {
			rootView.model.progress(progress / 100);
		}
	}
}
