import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Shapes 1.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtGraphicalEffects 1.0

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model
import Core.System 0.1

View.MessageView {
	id: rootView

	width: circleView.implicitWidth
	height: circleView.implicitHeight
	bubble: false

	property bool isAvaliable: false
	property bool isPlaying: false
	property bool isLoaded: false

	Component.onCompleted: {
	}

	Component.onDestruction: {
		mediaPlayer.stop()
	}

	function play(seek)
	{
		mediaPlayer.muted = false;
		if (seek === true) {
			mediaPlayer.seek(0);
		}
		mediaPlayer.play();
	}

	model: Model.VideoscopeMessage {
		id: message

		onImported: {
			if (Model.VideoscopeMessage.Cached === message.cacheState) {
				console.log("VideoScopeMessage::onImported ", message.cachePath);
				mediaPlayer.source = message.cachePath;
			}
			console.log("VideoScopeMessage::onImported ", message.fileName);
			message.cache();
		}

		onCached: {
			console.log("VideoScopeMessage::onCached ", message.cachePath);
			mediaPlayer.source = message.cachePath;
		}
	}

	Dial {
		id: circleView

		anchors.left: rootView.left
		anchors.top: rootView.top

		live: false
		from: 0
		to: mediaPlayer.duration

		onMoved: {
			mediaPlayer.seek(value);
		}

		background: Rectangle {
			color: "transparent"
			width: Math.min(circleView.width, circleView.height)
			height: width
			rotation: circleView.angle

			Rectangle {
				id: outerCircleRectangle

				anchors.fill: parent
				radius: Math.max(width, height) / 2
				color: "transparent"
				border.color: Material.accent
				border.width: 8
			}

			ConicalGradient {
				anchors.fill: parent
				source: outerCircleRectangle
				gradient: Gradient {
					GradientStop {
						position: 0.00
						color: "white"
					}
					GradientStop {
						position: circleView.position - 0.01
						color: "white"
					}
					GradientStop {
						position: circleView.position
						color: "transparent"
					}
					GradientStop {
						position: 1.00
						color: "transparent"
					}
				}
			}
		}

		handle: Shape {
			id: circleViewHandle

			width: 20
			height: 20
			x: circleView.background.x + circleView.background.width / 2 - width / 2
			y: circleView.background.y + circleView.background.height / 2 - height / 2

			opacity: 1

			ShapePath {
				strokeWidth: 0
				fillColor: outerCircleRectangle.border.color

				startX: 10
				startY: 16
				PathLine { x: 10; y: 16 }
				PathLine { x: 16; y: 4 }
				PathLine { x: 4; y: 4 }
			}

			transform: [
				Translate {
					y: -circleView.background.width * 0.7 + circleViewHandle.height / 2
				},
				Rotation {
					angle: circleView.angle
					origin.x: circleViewHandle.width / 2
					origin.y: circleViewHandle.height / 2
				}
			]
		}
	}

	Binding {
		target: circleView
		property: "value"
		value: mediaPlayer.position
	}

	MediaPlayer {
		id: mediaPlayer

		autoLoad: true
		autoPlay: true
		audioRole: MediaPlayer.VideoRole
		muted: true
		notifyInterval: 100

		onPlaybackRateChanged: {
			console.debug("VideoscopeMessage::playbackRate: " + playbackRate);
		}

		onPlaybackStateChanged: {
			console.debug("VideoscopeMessage::playbackState = " + mediaPlayer.playbackState);
			isPlaying = (mediaPlayer.playbackState === MediaPlayer.PlayingState);
		}

		onAvailabilityChanged: {
			console.debug("VideoscopeMessage::availability = " + mediaPlayer.availability);
			isAvaliable = (mediaPlayer.availability == mediaPlayer.Available);
		}

		onErrorChanged: {
			console.error("VideoscopeMessage::error = " + mediaPlayer.error);
			console.error("VideoscopeMessage::errorString = " + mediaPlayer.errorString);
		}

		onStatusChanged: {
			console.debug("VideoscopeMessage::status = " + mediaPlayer.status);
			if ((mediaPlayer.status == MediaPlayer.Loaded) || (mediaPlayer.status == MediaPlayer.Buffered) || (mediaPlayer.status == MediaPlayer.Buffering)) {
				isLoaded = true;
			} else if (mediaPlayer.status == MediaPlayer.EndOfMedia) {
				seek(0);
				pause();
			}
		}
	}

	VideoOutput {
		id: videoOutput

		source: mediaPlayer
		anchors.fill: parent
		anchors.margins: 3

		fillMode: VideoOutput.PreserveAspectFit

		layer.enabled: true
		layer.effect: OpacityMask {
			maskSource: Item {
				width: videoOutput.width
				height: videoOutput.height
				Rectangle {
					anchors.centerIn: parent
					width: videoOutput.width
					height: videoOutput.height
					radius: Math.min(width, height)
				}
			}
		}
	}

	BusyIndicator {
		id: busyIndicator

		anchors.fill: parent
		anchors.horizontalCenter: circleView.horizontalCenter
		anchors.verticalCenter: circleView.verticalCenter

		running: !isLoaded && !isAvaliable
		visible: running
	}

	Button {
		id: playButton

		anchors.fill: circleView

		enabled: mediaPlayer.hasVideo
		visible: rootView.isPlaying ? false : !busyIndicator.running

		icon.color: Material.accent
		icon.width: rootView.width * 0.3
		icon.source: (message.cacheState === VideoscopeMessage.Unknown) ? "/Conversation/Asset/Image/Cancel3.svg" : "/Conversation/Asset/Image/Play.svg"

		background: Rectangle {
			color: "transparent"
		}
	}

	MouseArea {
		anchors.fill: parent

		onClicked: {
			if (rootView.isPlaying) {
				if (!mediaPlayer.muted) {
					mediaPlayer.pause();
				} else {
					mediaPlayer.pause();
					rootView.play(true);
				}
			} else {
				rootView.play(false);
			}
		}
	}

	Label {
		anchors.left: circleView.left

		text: new Date(mediaPlayer.position).toLocaleTimeString(Qt.locale(), "mm:ss")
		color: "lightgrey"
		font.pointSize: 9
	}

	Label {
		anchors.left: circleView.left
		anchors.bottom: circleView.bottom

		text: mediaPlayer.muted ? "🔇" : "🔊"
		color: "lightgrey"
		font.pointSize: 9
	}
}
