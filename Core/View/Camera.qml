import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtSensors 5.15
import QtPositioning 5.15

import Core.View 0.1
import Core.Model 0.1
import Core.System 0.1

Rectangle {
	id: rootView

	signal captured(url filename)

	color: "black"

	Component.onCompleted: {
		for (var c=0; c<QtMultimedia.availableCameras.length; c++) {
			console.log("QtMultimedia.availableCameras["+c+"]", QtMultimedia.availableCameras[c].deviceId, QtMultimedia.availableCameras[c].displayName, QtMultimedia.availableCameras[c].position, QtMultimedia.availableCameras[c].orientation);
		}

		if (!orientationSensor.start()) {
			console.log("Could not start OrientationSensor", orientationSensor.error);
		}
		/// @brief "gps" location provider requires ACCESS_FINE_LOCATION permission.
/*
		if (!positionSource.start()) {
			console.log("Could not activate PositionSource: ", positionSource.error);
		}
*/

//		orientate();
	}

	Component.onDestruction: {
		orientationSensor.stop();
//		positionSource.stop();
	}

	property bool isPortrait: Screen.primaryOrientation === Qt.PortraitOrientation || Screen.primaryOrientation === Qt.InvertedPortraitOrientation

	function exit() {
		if (rootView.StackView && rootView.StackView.view) {
			rootView.StackView.view.pop();
		}
	}

	function preview() {		
/*
		var coordinate = positionSource.position.coordinate;		
		camera.imageCapture.setMetadata("GPSLatitude", coordinate.latitude)
		camera.imageCapture.setMetadata("GPSLongitude", coordinate.longitude)
		camera.imageCapture.setMetadata("GPSAltitude", coordinate.altitude)
		camera.imageCapture.setMetadata("Date", positionSource.position.timestamp);
*/
/*
		camera.imageCapture.setMetadata("Orientation", WindowManager.screenRotationAngle);
*/
		camera.imageCapture.setMetadata("Date", new Date());
		camera.imageCapture.capture();
	}

	function orientate() {
/*
		var orientation = orientationSensor.reading.orientation;
		switch (orientation) {
			case OrientationReading.RightUp:
				camera.imageCapture.setMetadata("Orientation", 0);
				break;
			case OrientationReading.TopUp:
				camera.imageCapture.setMetadata("Orientation", 270);
				break;
			case OrientationReading.LeftUp:
				camera.imageCapture.setMetadata("Orientation", 180);
				break;
			case OrientationReading.TopDown:
				camera.imageCapture.setMetadata("Orientation", 90);
				break;
		}
*/

		console.log("Camera::orientate screenRotationAngle =", WindowManager.screenRotationAngle);
		console.log("Camera::orientate camera.orientation =", camera.orientation);
		console.log("Camera::orientate camera.metaData.orientation =", camera.metaData.orientation);
		console.log("Camera::orientate videoOutput.orientation =", videoOutput.orientation);
	}

	function rotate() {
		camera.position = Camera.BackFace === camera.position ? Camera.FrontFace : Camera.BackFace;
	}

	OrientationSensor {
		id: orientationSensor

		active: false

		onReadingChanged: {
			console.log("Camera::OrientationSensor has been changed: ", orientationSensor.reading.orientation);
			orientate();
		}
	}
/*
	PositionSource {
		id: positionSource

		updateInterval: 3000
		active: false

		property var lastCoordinate: QtPositioning.coordinate(90, 0)
		onPositionChanged: {
			positionSource.lastCoordinate.latitude = coordinate.latitude;
			positionSource.lastCoordinate.longitude = coordinate.longitude;
		}
	}
*/
	Camera {
		id: camera

		position: Camera.BackFace
		captureMode: Camera.CaptureStillImage
		flash.mode: Camera.FlashAuto
		imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash
		focus.focusMode: CameraFocus.FocusContinuous

		property url capturedImage: "file://";
/*
		exposure {
			exposureCompensation: -1.0
			exposureMode: Camera.ExposurePortrait
		}
*/
		focus {
			focusPointMode: Camera.FocusPointAuto
		}

		imageCapture.onImageCaptured: {
			orientate();
			console.log("Capture preview: ", preview);
			camera.unlock();
/*
			if (Qt.platform.os === "ios") {
				if (camera.position === Camera.BackFace) {
					photoPreview.rotation = (360 - camera.orientation + WindowManager.screenRotationAngle) % 360;
				} else {
					photoPreview.rotation = (camera.orientation - WindowManager.screenRotationAngle) % 360;
				}				
			}
*/
			photoPreview.source = preview;
			stackLayout.state = "preview";
			orientate();
		}

		imageCapture.onImageSaved: {
			console.log("Capture image: ", path);
			capturedImage = Qt.resolvedUrl("file://" + path);
			photoPreview.source = capturedImage;
		}

		imageCapture.onImageMetadataAvailable: {
			console.log("Camera metadata available: ", key, value)
		}

		onOrientationChanged: {
			console.log("Camera::Camera orientation has been changed=", camera.orientation, videoOutput.orientation);
//			orientate();
		}
	}

	StackLayout {
		id: stackLayout
		anchors.fill: parent

		state: "finder"
		states: [
			State {
				name: "finder"
				PropertyChanges {
					target: stackLayout
					currentIndex: 0
				}
			},
			State {
				name: "preview"
				PropertyChanges {
					target: stackLayout
					currentIndex: 1
				}
			}
		]

		Item {
			Layout.fillWidth: true
			Layout.fillHeight: true

			VideoOutput {
				id: videoOutput

				source: camera
				focus : visible
				fillMode: VideoOutput.PreserveAspectFit
				autoOrientation: true
				rotation: (Qt.platform.os === "ios" && camera.position === Camera.FrontFace) ? 180 : 0

				anchors.fill: parent

				MouseArea {

					anchors.fill: parent

					onClicked: {
						camera.focus.focusPointMode = Camera.FocusPointCustom;
						camera.focus.customFocusPoint = Qt.point(mouse.x / rootView.width, mouse.y / rootView.height);
						console.log("Camera::VideoOutput focus =", camera.focus.customFocusPoint);
					}
				}
			}

			Rectangle {
				id: stopBar

				color: "#242424"
				radius: 15
				height: 50
				width: 50

				anchors.left: parent.left
				anchors.leftMargin: 20
				anchors.top: parent.top
				anchors.topMargin: WindowManager.topSafePadding

				Image {
					id: cancelIcon

					anchors.fill: parent
					anchors.margins: 15

					fillMode: Image.PreserveAspectFit
					sourceSize.height: height
					sourceSize.width: width
					source: "/Core/Asset/Image/Cancel.svg"
				}
				MouseArea {
					anchors.fill: parent
					enabled: true
					onClicked: {
						exit();
					}
				}
			}

			Rectangle {
				id: flipBar

				color: "#242424"

				radius: 15
				height: 50
				width: 50

				anchors.right: parent.right
				anchors.rightMargin: 20
				anchors.top: parent.top
				anchors.topMargin: WindowManager.topSafePadding

				Image {
					id: flipIcon

					anchors.fill: parent
					anchors.margins: 15

					fillMode: Image.PreserveAspectFit
					sourceSize.height: height
					sourceSize.width: width
					source: "/Core/Asset/Image/Camera.svg"
				}
				MouseArea {
					anchors.fill: parent
					enabled: true
					onClicked: {
						rotate();
					}
				}
			}

			Item {
				id: controlBar

				height: childrenRect.height
	/*
				color: "#242424"
				radius: 15
	*/
				anchors.left: parent.left
				anchors.leftMargin: 10
				anchors.right: parent.right
				anchors.rightMargin: 10
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 10

				RowLayout {
					height: childrenRect.height + 40

					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: parent.top

					spacing: 1

					Rectangle {
						id: captureButton

						Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
						Layout.preferredHeight: 80
						Layout.preferredWidth: 80

						color: "white"
						border.width: 1
						border.color: "white"
						radius: 40

						Rectangle {
							anchors.fill: parent
							anchors.margins: 5

							color: "transparent"
							border.color: "black"
							border.width: 1
							radius: height * 0.5

							MouseArea {
								anchors.fill: parent
								onClicked: {
									preview();
								}
							}
						}
					}
				}
			}
		}

		Item {
			Layout.fillWidth: true
			Layout.fillHeight: true

			Image {
				id: photoPreview

				anchors.fill: parent				
				fillMode: Image.PreserveAspectFit
				autoTransform: true
				mirror: camera.position === Camera.FrontFace
/*
				autoTransform: Qt.platform.os !== "ios"
				rotation: 0
*/
				Rectangle {
					id: backBar

					color: "transparent"

//					radius: 15
					height: 50
					width: 50

					anchors.left: parent.left
					anchors.leftMargin: 20
					anchors.bottom: parent.bottom
					anchors.bottomMargin: 20

					Image {
						id: backIcon

						anchors.fill: parent
						anchors.margins: 15

						fillMode: Image.PreserveAspectFit
						sourceSize.height: height
						sourceSize.width: width
						source: "/Core/Asset/Image/Back.svg"
					}
					MouseArea {
						anchors.fill: parent
						enabled: true
						onClicked: {
							/// @todo remove temp file
							stackLayout.state = "finder";
						}
					}
				}

				Rectangle {
					id: pushBar

					color: "transparent"

//					radius: 15
					height: 50
					width: 50

					anchors.right: parent.right
					anchors.rightMargin: 20
					anchors.bottom: parent.bottom
					anchors.bottomMargin: 20

					Image {
						id: pushIcon

						anchors.fill: parent
						anchors.margins: 15

						fillMode: Image.PreserveAspectFit
						sourceSize.height: height
						sourceSize.width: width
						source: "/Core/Asset/Image/ArrowUp.svg"
					}
					MouseArea {
						anchors.fill: parent
						enabled: true
						onClicked: {
							captured(camera.capturedImage);
							console.log(camera.capturedImage);
						}
					}
				}
			}
		}
	}
}
