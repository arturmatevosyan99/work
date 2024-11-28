import QtMultimedia 5.15
import QtGraphicalEffects 1.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import Core.View 0.1
import Core.Model 0.1
import Core.System 0.1

import "../../Qaos/QtQuick/DateTime.js" as DateTimeScript

Rectangle {
	id: rootView

//	property alias audio: audioOutput /// < avaliable since qt6
	property alias video: videoOutput
	property bool isCinema: false
	property bool isAvaliable: false
	property bool isPlaying: false
	property bool isLoaded: false

	property double progress: (mediaPlayer.position * 100) / mediaPlayer.duration
	property alias title: titleLabel.text
	property alias source: mediaInfo.rawSource

	signal stopped(double playback_progress)
	signal paused(double playback_progress)
	signal resumed(double playback_progress)

	Component.onCompleted: {
		WindowManager.keepScreen(true);
		AudioRole.mode = AudioRole.Playback;
		AudioRole.options = AudioRole.Stream;
		AudioRole.monitor(true);
	}

	Component.onDestruction: {
		mediaPlayer.stop();
		WindowManager.keepScreen(false);
		AudioRole.monitor(false);
		AudioRole.restore();
	}

	function stop()
	{
		var progress_backup = isNaN(rootView.progress) ? 0 : rootView.progress;
		mediaPlayer.stop();
		rootView.stopped(progress_backup);
		if (rootView.StackView && rootView.StackView.view) {
			rootView.StackView.view.pop();
		}
	}

	function seek(offset)
	{
		mediaPlayer.seek(offset);
	}

	function play()
	{
		var progress_backup = isNaN(rootView.progress) ? 0 : rootView.progress;
		if (rootView.isPlaying) {
			mediaPlayer.pause();
			rootView.paused(progress_backup);
		} else {
			mediaPlayer.play();
			rootView.resumed(progress_backup);
		}
	}

	Connections {
		target: Native

		function onBackRequested()
		{
			rootView.stop();
		}
	}

	Timer {
		id: cinemaTimer

		interval: 10000
		running: true
		repeat: false

		onTriggered: {
			isCinema = true;
		}
	}

	MediaInfo {
		id: mediaInfo

		onGrabbed: {
			console.log("MediaInfo::defaultSource", defaultSource);
			console.log("MediaInfo::rowCount", rowCount());
		}

		onModelReset: {
			console.log("MediaInfo::defaultSource", defaultSource);
			console.log("MediaInfo::rowCount", rowCount());
		}

		onDefaultSourceChanged: {
			var offset = mediaPlayer.position;
			mediaPlayer.source = defaultSource;
			if (offset > 100) {
				mediaPlayer.seek(offset);
			}
		}
	}

	MediaPlayer {
		id: mediaPlayer

		autoLoad: true
		autoPlay: true
		audioRole: MediaPlayer.VideoRole
		videoOutput: rootView.video
//		source: mediaInfo.defaultSource

		onSourceChanged: {
			console.debug("MediaPlayer::source = " + source);
		}

		onPlaybackRateChanged: {
			console.debug("MediaPlayer::paybackRate = " + playbackRate);
		}

		onPlaybackStateChanged: {
			console.debug("MediaPlayer::playbackState = " + playbackState);
			rootView.isPlaying = (MediaPlayer.PlayingState === playbackState);
		}

		onAvailabilityChanged: {
			console.debug("MediaPlayer::availability = " + availability);
			rootView.isAvaliable = (MediaPlayer.Available === availability);
		}

		onErrorChanged: {
			console.debug("MediaPlayer::error = " + mediaPlayer.error);
		}

		onStatusChanged: {
			console.debug("MediaPlayer::status = " + status);
			if ([MediaPlayer.Loaded, MediaPlayer.Buffered, MediaPlayer.Buffering].indexOf(status) > -1){
				rootView.isLoaded = true;
				rootView.isAvaliable = true;
			} else if (status == MediaPlayer.Loading) {
				rootView.isAvaliable = true; /// @hack just for now (28.12.2019)
			}/* else if (mediaPlayer.status == mediaPlayer.Stalled && mediaPlayer.playbackState != mediaPlayer.PlayingState) {
				rootView.isLoaded = false;
			}*/
		}

		onBufferProgressChanged: {
			if (mediaPlayer.bufferProgress < 0) {
				console.debug("MediaPlayer::bufferProgress = ", mediaPlayer.bufferProgress);
			}
		}
	}

	VideoOutput {
		id: videoOutput

		anchors.fill: parent
		fillMode: VideoOutput.PreserveAspectFit
		flushMode: VideoOutput.EmptyFrame

		MouseArea {
			anchors.fill: parent
			enabled: true
			onClicked: {
				if (isCinema) {
					isCinema = false;
					cinemaTimer.start();
				} else if (cinemaTimer.running) {
					cinemaTimer.running = false;
					isCinema = true;
				} else {
					isCinema = true;
				}
			}
		}

		Rectangle {
			id: stopBar

			anchors.left: parent.left
			anchors.leftMargin: 20
			anchors.top: parent.top
			anchors.topMargin: WindowManager.topSafePadding

			radius: 15
			height: 50
			width: 50

			visible: opacity != 0.0
			color: "#242424"

			states: [
				State {
					when: !isCinema
					PropertyChanges {
						target: stopBar
						opacity: 1
					}
				},
				State {
					when: isCinema
					PropertyChanges {
						target: stopBar
						opacity: 0.0
					}
				}
			]

			transitions: Transition {
				NumberAnimation {
					property: "opacity"
					duration: 500
				}
			}

			Image {
				id: cancelIcon

				anchors.fill: parent
				anchors.margins: 15

				fillMode: Image.PreserveAspectFit
				sourceSize.height: height
				source: "/Core/Asset/Image/Cancel.svg"
			}
			MouseArea {
				anchors.fill: parent
				enabled: true
				onClicked: {
					stop();
				}
			}
		}

		ComboBox {
			id: definitionBox

			anchors.left: stopBar.right
			anchors.leftMargin: 1
			anchors.top: stopBar.top

			width: stopBar.width
			height: stopBar.height

			visible: opacity != 0.0

			topInset: 0
			bottomInset: 0
			leftInset: 0
			rightInset: 0

			font.italic: false
			font.weight: Font.Black

			valueRole: "alias"
			textRole: valueRole

			states: [
					State {
						when: definitionBox.count > 1 && !isCinema
						PropertyChanges {
							target: definitionBox
							opacity: 1
						}
					},
					State {
						when: definitionBox.count < 2 || isCinema
						PropertyChanges {
							target: definitionBox
							opacity: 0.0
						}
					}
			]

			transitions: Transition {
				NumberAnimation {
					property: "opacity"
					duration: 500
				}
			}
			model: SortFilterProxyModel {
				sourceModel: mediaInfo
				sortRole: getRoleHashKey(definitionBox.valueRole)
			}

			delegate: ItemDelegate {
				required property string alias

				width: definitionBox.width
				height: width

				highlighted: definitionBox.highlightedIndex === index

				contentItem: Image {
					anchors.fill: parent
					anchors.margins: 0

					fillMode: Image.PreserveAspectFit
					sourceSize.height: height
					source: "/Core/Asset/Image/VideoResolution%1.svg".arg(parent.alias)
				}
			}

			contentItem: Image {
				anchors.fill: parent
				anchors.margins: 0

				fillMode: Image.PreserveAspectFit
				sourceSize.height: height
				source: "/Core/Asset/Image/VideoResolution%1.svg".arg(definitionBox.currentValue ?? "")
				layer.effect: OpacityMask {
					maskSource: Item {
						width: definitionBox.contentItem.width
						height: definitionBox.contentItem.height
						Rectangle {
							anchors.fill: parent
							radius: definitionBox.background.radius
						}
					}
				}
			}

			indicator: null

			background: Rectangle {
				implicitWidth: definitionBox.width
				implicitHeight: definitionBox.height
				border.width: 0
				radius: 15
				color: "#242424"
			}

			popup: Popup {
				y: 0
				width: definitionBox.width
				implicitHeight: contentItem.implicitHeight
				padding: 1

				contentItem: ListView {
					clip: true
					width: parent.width
					implicitHeight: contentHeight
					model: definitionBox.popup.visible ? definitionBox.delegateModel : null
					currentIndex: definitionBox.highlightedIndex

					ScrollIndicator.vertical: ScrollIndicator { }
				}

				background: Rectangle {
					color: "#242424"
					border.width: 0
					radius: 15
				}
			}

			onCountChanged: if (count > 0) {
				currentIndex = indexOfValue(mediaInfo.defaultAlias);
			}

			onActivated: {
				mediaInfo.defaultAlias = currentValue;
			}
		}

		Rectangle {
			id: muteBar

			visible: opacity != 0.0
			color: "#242424"

			radius: 15
			height: 50
			width: 50

			anchors.right: parent.right
			anchors.rightMargin: 20
			anchors.top: stopBar.top

			states: [
					State {
						when: !isCinema
						PropertyChanges {
							target: muteBar
							opacity: 1
						}
					},
					State {
						when: isCinema
						PropertyChanges {
							target: muteBar
							opacity: 0.0
						}
					}
			]

			transitions: Transition {
				NumberAnimation {
					property: "opacity"
					duration: 500
				}
			}

			Image {
				id: muteIcon

				anchors.fill: parent
				anchors.margins: 15

				fillMode: Image.PreserveAspectFit
				sourceSize.height: height
//				sourceSize.width: width
				source: "/Core/Asset/Image/Amplify.svg"

				state: mediaPlayer.muted ? "amplify" : "mute"
				states: [
						State {
								name: "mute"
								PropertyChanges {
									target: muteIcon
									source: "/Core/Asset/Image/Amplify.svg"
								}
						},
						State {
								name: "amplify"
								PropertyChanges {
									target: muteIcon
									source: "/Core/Asset/Image/Mute.svg"
								}
						}
				]
			}
			MouseArea {
				anchors.fill: parent
				enabled: true

				onClicked: {
					mediaPlayer.muted = !mediaPlayer.muted;
				}
			}
		}

		Label {
			id: titleLabel

			anchors.bottom: controlBar.top
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.topMargin: 50
			anchors.leftMargin: 5
			anchors.bottomMargin: 5
			anchors.rightMargin: 5

			verticalAlignment: Text.AlignBottom
			horizontalAlignment: Text.AlignHCenter

			color: "white"
			wrapMode: Text.WordWrap
	//		fontSizeMode: Text.Fit
			elide: Label.ElideRight

			font.pointSize: 20
			font.family: "Helvetica Neue"

			states: [
					State {
						when: !isCinema || !isLoaded
						PropertyChanges {
							target: titleLabel
							opacity: 1.0
						}
					},
					State {
						when: isCinema
						PropertyChanges {
							target: titleLabel
							opacity: 0.0
						}
					}
			]

			transitions: Transition {
				NumberAnimation {
					property: "opacity"
					duration: 1000
				}
			}
		}

		Rectangle {
			id: controlBar

			color: "#242424"
			visible: opacity != 0.0

			radius: 15

			anchors.left: parent.left
			anchors.leftMargin: 10
			anchors.right: parent.right
			anchors.rightMargin: 10
			anchors.bottom: parent.bottom
			anchors.bottomMargin: 10

			height: childrenRect.height

			states: [
						State {
							when: !isCinema
							PropertyChanges {
								target: controlBar
								opacity: 1.0
							}
						},
						State {
							when: isCinema
							PropertyChanges {
								target: controlBar
								opacity: 0.0
							}
						}
			]

			transitions: Transition {
				NumberAnimation {
					property: "opacity"
					duration: 1000
				}
			}

			Column {
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top
				height: childrenRect.height + 40
				topPadding: 20
				bottomPadding: 20

				spacing: 1

				Slider {
					id: timelineSlider

					anchors.left: parent.left
					anchors.right: parent.right
					Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
					anchors.leftMargin: 20
					anchors.rightMargin: 20

					from: 0
					to: mediaPlayer.duration
					value: mediaPlayer.position

					onMoved: {
						mediaPlayer.seek(value);
					}

					background: Rectangle {
						x: timelineSlider.leftPadding
						y: timelineSlider.topPadding + timelineSlider.availableHeight / 2 - height / 2
						implicitWidth: parent.width
						implicitHeight: 4
						width: timelineSlider.availableWidth
						height: implicitHeight
						radius: 2
						color: "#bdbebf"

						Rectangle {
							width: timelineSlider.visualPosition * parent.width
							height: parent.height
							color: Material.color(Material.Blue)
							radius: 2
						}
					}

					handle: Rectangle {
						x: timelineSlider.leftPadding + timelineSlider.visualPosition * (timelineSlider.availableWidth - width)
						y: timelineSlider.topPadding + timelineSlider.availableHeight / 2 - height / 2
						implicitWidth: 14
						implicitHeight: 14
						radius: 7
						color: timelineSlider.pressed ? "#f0f0f0" : "#f6f6f6"
						border.color: "#bdbebf"
					}
				}

				RowLayout {
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.leftMargin: 20
					anchors.rightMargin: 20

					Label {
						id: currentTimeLabel

						Layout.alignment: Qt.AlignLeft | Qt.AlignTop
						Layout.minimumWidth: contentWidth

						color: "#f0f0f0"
						text: DateTimeScript.formatTimePoint(mediaPlayer.position)
					}
					Item {
						Layout.fillWidth: true
					}
					Label {
						id: finishTimeLabel

						Layout.alignment: Qt.AlignRight| Qt.AlignTop
						Layout.minimumWidth: contentWidth

						color: "#f0f0f0"
						text: DateTimeScript.formatTimePoint(mediaPlayer.duration)
					}
				}

				RowLayout {
					anchors.left: parent.left
					anchors.right: parent.right
					Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
					spacing: 5
					ToolButton {
						id: stopButton
						onClicked: {
							stop();
						}
						Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
						Layout.preferredHeight: 40
						Layout.preferredWidth: 40
						Image {
							id: stopImage
							width: 40
							height: 40
							sourceSize.height: height
//							sourceSize.width: width
							fillMode: Image.PreserveAspectFit
							source: "/Core/Asset/Image/Stop.svg" /// @note source: https://icons8.ru/icon/pack/media-controls/win8
						}
					}
					ToolButton {
						id: playButton

						Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
						Layout.preferredHeight: 40
						Layout.preferredWidth: 40

						enabled: isLoaded

						Image {
							id: playImage
							width: 40
							height: 40
							sourceSize.height: height
//							sourceSize.width: width
							fillMode: Image.PreserveAspectFit
						}

						state: isPlaying ? "pause" : "play"
						states: [
							State {
								name: "pause"
								PropertyChanges {
									target: playImage
									source: "/Core/Asset/Image/Pause.svg"
								}
							},
							State {
								name: "play"
								PropertyChanges {
									target: playImage
									source: "/Core/Asset/Image/Play.svg"
								}
							}
						]

						onClicked: {
							play();
						}
					}
				}
			}
		}
	}

	BusyIndicator {
		id: busyIndicator

		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		running: !isLoaded && (mediaPlayer.playbackState != mediaPlayer.PlayingState)
	}

	color: "black"
}
