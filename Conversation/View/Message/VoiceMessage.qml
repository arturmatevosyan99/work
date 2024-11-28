import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtGraphicalEffects 1.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	height: 60
	width: 60
	bubble: false

	property bool isAvaliable: false
	property bool isPlaying: false
	property bool isLoaded: false

	Component.onDestruction: {
		mediaPlayer.stop()
	}

	model: Model.VoiceMessage {
		id: message

		onImported: {
			if (VoiceMessage.Cached === message.cacheState) {
				console.log("VoiceMessage::onImported ", message.cachePath);
				mediaPlayer.source = message.cachePath;
			}
			console.log("VoiceMessage::onImported ", message.fileName);
			message.cache();
		}

		onCached: {
			console.log("VoiceMessage::onCached ", message.cachePath);
			mediaPlayer.source = message.cachePath;
		}
	}

	Rectangle {
		id: circleView
		anchors.fill: parent
		radius: Math.min(width, height)
		border.color: Material.primary
		border.width: 2

		MediaPlayer {
			id: mediaPlayer
			autoLoad: true
			autoPlay: false
			muted : false

			onPlaybackRateChanged: {
				console.debug("VoiceMessage::playbackRate: " + playbackRate);
			}

			onPlaybackStateChanged: {
				console.debug("VoiceMessage::playbackState = " + mediaPlayer.playbackState);
				isPlaying = (mediaPlayer.playbackState === MediaPlayer.PlayingState);
			}

			onAvailabilityChanged: {
				console.debug("VoiceMessage::availability = " + mediaPlayer.availability);
				isAvaliable = (mediaPlayer.availability == mediaPlayer.Available);
			}

			onErrorChanged: {
				console.error("VoiceMessage::error = " + mediaPlayer.error);
				console.error("VoiceMessage::errorString = " + mediaPlayer.errorString);
			}

			onStatusChanged: {
				console.debug("VoiceMessage::status = " + mediaPlayer.status);
				if ((mediaPlayer.status == MediaPlayer.Loaded) || (mediaPlayer.status == MediaPlayer.Buffered)) {
					isLoaded = true;
					//now that media is loaded, we should know its size, and should request a resize
					//if we are not fullscreen, then the window should resize to the native size of the video
					//TODO: automatically resize video window to native move size
				}
			}
		}

		MouseArea {
			anchors.fill: parent
			enabled: mediaPlayer.hasAudio
			onClicked: {
				if (isPlaying ) {
					if (!mediaPlayer.muted) {
						mediaPlayer.pause();
					} else {
						mediaPlayer.pause();
						mediaPlayer.muted = false;
						mediaPlayer.seek(0);
						mediaPlayer.play();
					}
				} else {
					mediaPlayer.muted = false;
					mediaPlayer.play();
				}
			}
		}

		BusyIndicator {
			id: busyIndicator
			anchors.fill: parent
			anchors.horizontalCenter: circleView.horizontalCenter
			anchors.verticalCenter: circleView.verticalCenter
			running: !isLoaded && !isAvaliable
		}

		Image {
			id: playImage
			visible: (isPlaying) ? false : !busyIndicator.running
			state: (message.cacheState === VoiceMessage.Unknown) ? "invalid" : "ready"
			anchors.horizontalCenter: circleView.horizontalCenter
			anchors.verticalCenter: circleView.verticalCenter

			states: [
						State {
								name: "ready"
								PropertyChanges {
													target: playImage
													source: "data:image/png;base64,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"
								}
						},
						State {
								name: "invalid"
								PropertyChanges {
													target: playImage
													source: "data:image/png;base64,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"
								}
						}
			]
		}

		Label {
			text: new Date(mediaPlayer.position).toLocaleTimeString(Qt.locale(), "mm:ss")
			color: "lightgrey"
			font.pointSize: 9
			anchors.left: circleView.left
		}
	}
}
