import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

import Core.View 0.1
import Core.Model 0.1
import Core.System 0.1

Page {
	id: rootView

	Component.onCompleted: {
		console.log("ImageViewer::onCompleted");
	}

	Component.onDestruction: {
		console.log("ImageViewer::onDestruction");
	}

	property bool isCinema: false
	property url source

	padding: 0
	leftInset: 0
	rightInset: 0

	Timer {
		id: cinemaTimer

		interval: 10000
		running: true
		repeat: false

		onTriggered: {
			console.log("Cinema time!")
			isCinema = true
		}
	}

	header: StackBar {
		id: navigationBar

		isHidden: isCinema

		titleText: qsTr("Image Viewer")
		subtitleText: title

		accentColor: "black"
		backgroundColor: "black"
		foregroundColor: "white"

		stackView: rootView.StackView.view

		optionStyle: StackBar.OptionStyle.None
	}

	background: Rectangle {
		anchors.fill: parent
		color: "black"

		Flickable {
			id: imageFlickable

			anchors.fill: parent
			contentWidth: imageContainer.width
			contentHeight: imageContainer.height
			clip: true

			onHeightChanged: {
				if (imagePreview.status === Image.Ready) {
					imagePreview.fitToScreen();
				}
			}

			Item {
				id: imageContainer
				width: Math.max(imagePreview.width * imagePreview.scale, imageFlickable.width)
				height: Math.max(imagePreview.height * imagePreview.scale, imageFlickable.height)

				Image {
					id: imagePreview

					property real prevScale

					function fitToScreen() {
						scale = Math.min(imageFlickable.width / width, imageFlickable.height / height, 1)
						pinchArea.minScale = scale
						prevScale = scale
					}

					anchors.centerIn: parent
					fillMode: Image.PreserveAspectFit
					autoTransform: true
					cache: false
					asynchronous: true
					source: rootView.source
					sourceSize.height: 1000
					smooth: !imageFlickable.moving

					onStatusChanged: {
						if (status == Image.Ready) {
							fitToScreen()
							loadedAnimation.start()
						}
					}

					onScaleChanged: {
						if ((width * scale) > imageFlickable.width) {
							var xoff = (imageFlickable.width / 2 + imageFlickable.contentX) * scale / prevScale;
							imageFlickable.contentX = xoff - imageFlickable.width / 2
						}
						if ((height * scale) > imageFlickable.height) {
							var yoff = (imageFlickable.height / 2 + imageFlickable.contentY) * scale / prevScale;
							imageFlickable.contentY = yoff - imageFlickable.height / 2
						}
						prevScale = scale
					}

					NumberAnimation {
						id: loadedAnimation
						target: imagePreview
						property: "opacity"
						duration: 250
						from: 0; to: 1
						easing.type: Easing.InOutQuad
					}
				}
			}

			PinchArea {
				id: pinchArea

				property real minScale: 1.0
				property real maxScale: 3.0

				anchors.fill: parent
				enabled: imagePreview.status === Image.Ready
				pinch.target: imagePreview
				pinch.minimumScale: minScale * 0.5 // This is to create "bounce back effect"
				pinch.maximumScale: maxScale * 1.5 // when over zoomed

				onPinchFinished: {
					imageFlickable.returnToBounds()
					if (imagePreview.scale < pinchArea.minScale) {
						bounceBackAnimation.to = pinchArea.minScale
						bounceBackAnimation.start()
					}
					else if (imagePreview.scale > pinchArea.maxScale) {
						bounceBackAnimation.to = pinchArea.maxScale
						bounceBackAnimation.start()
					}
				}

				NumberAnimation {
					id: bounceBackAnimation
					target: imagePreview
					duration: 250
					property: "scale"
					from: imagePreview.scale
				}

				MouseArea {
					anchors.fill: parent
					onClicked: {
						if (rootView.isCinema) {
							rootView.isCinema = false;
							cinemaTimer.start();
						} else {
							if (cinemaTimer.running) {
								cinemaTimer.running = false;
							}
							rootView.isCinema = true;
						}
					}
				}
			}
		}
	}

	Loader {
		anchors.centerIn: parent

		sourceComponent: {
			switch (imagePreview.status) {
				case Image.Loading:
					return loadingIndicator
				case Image.Error:
					return failedLoading
				default:
					return undefined
			}
		}

		Component {
			id: loadingIndicator

			Item {
				height: childrenRect.height
				width: rootView.width

				BusyIndicator {
					id: imageLoadingIndicator
					anchors.horizontalCenter: parent.horizontalCenter
					running: true
				}

				Text {
					anchors {
						horizontalCenter: parent.horizontalCenter
						top: imageLoadingIndicator.bottom
						topMargin: 10
					}

					color: "white"
					text: qsTr("Loading... %1%").arg(Math.round(imagePreview.progress * 100))
				}
			}
		}

		Component {
			id: failedLoading

			Text {
				text: qsTr("Loading error")
				color: "white"
			}
		}
	}
}
