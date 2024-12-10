import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.System 0.1
import SmartCare.View 0.1 as View
import SmartCare.Model 0.1 as Model

import "../../../Core/View"

View.TrackView {
	id: rootView

	MultimediaPlayer {
		anchors.fill: parent

		title: model.title
		source: model.file

		onSourceChanged: {
			console.debug("VideoTrack2::MultimediaPlayer::onSourceChanged", source);
		}

		onStopped: {
			rootView.model.progress(playback_progress / 100);
			rootView.StackView.view.pop();
		}

		onPaused: {
			rootView.model.progress(playback_progress / 100);
		}
	}
}
