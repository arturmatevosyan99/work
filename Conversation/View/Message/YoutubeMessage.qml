import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import Core.View 0.1
import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

import "../../../Core/View"

View.MessageView {
	id: rootView

	bubble: false
	contentWidth: thumbnailImage.implicitWidth
	contentHeight: thumbnailImage.implicitHeight

	model: Model.YoutubeMessage {
		id: message

		onImported: {			
			console.log("YoutubeMessage::onImported " + message.thumbnailMQ.toString());
			thumbnailImage.source = message.thumbnailMQ.toString();
		}
	}

	Component {
		id: multimediaPlayerComponent

		MultimediaPlayer {
			id: player

			Component.onCompleted: {
			}
		}
	}

	Image {
		id: thumbnailImage

		asynchronous: true
		anchors.fill: rootView
		source: "/Conversation/Asset/Image/YoutubeMQDefaultThumbnail.jpg"
		fillMode: Image.PreserveAspectCrop

		onSourceChanged: {
//			resize(thumbnailImage.sourceSize.width, thumbnailImage.sourceSize.height);
		}

		onStatusChanged: {
			if (status == Image.Ready) {
//				resize(thumbnailImage.sourceSize.width, thumbnailImage.sourceSize.height);
			}
		}

		layer.enabled: true

		layer.effect: OpacityMask {
			maskSource: Item {
				width: thumbnailImage.width
				height: thumbnailImage.height
				Rectangle {
					anchors.centerIn: parent
					width: thumbnailImage.width
					height: thumbnailImage.height
					radius: 10
				}
			}
		}

		Image {
			id: playImage
			visible: message.isImported
			anchors.horizontalCenter: thumbnailImage.horizontalCenter
			anchors.verticalCenter: thumbnailImage.verticalCenter

			source: "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAABgCAMAAADVRocKAAAB7FBMVEVHcEz/PQD/RgD/PQD/PQD/PQD/PQD/PQD/PQD/PQD/AAD/PAD/PQD/QAD/PAD/PQD/PQD/MwD/OwD/PQD/PQD/PQD/PAD/QAD/PQD/PQD/PQD/PQD/PAD/PQD/PQD/QAD/OgD/PAD/MwD/PgD/PAD/PQD/KwD/PQD/PgD/PgD/OQD/PgD/PAD/PQD/PgD/PAD/PAD/QAD/PQD/PQD/PQD/PgD/PwD/PAD/PQD/PAD/PAD/PQD/PQD/PQD/PQD/PgD/PAD/PAD/PQD/QAD/PQD/PQD/PAD/OQD/PQD/PAD/PQD/PQD/PgD/PgD/PQD/PQD/PQD/VQD/PQD/PwD/PQD/PgD/PQD/PQD/PAD/PQD/PQD/PgD/PQD/OwD/PQD/PgD/NwD/QAD/PQD/PgD/RAD/PgD/OgD/PQD/PAD/PQD/PQD/PQD/PgD/OwD/PAD/PQD/PQD/PQD/PQD/PAD/PQD/QAD/PQD/AAD/PAD/PQD/PgD/SQD/PQD/PQD/PgD/PQD/PAD/PAD/PQD/PQD/PAD/PgD/PQD/////+/r/2Mz/PwL/q5H/aTr//v7/Qgf/9/T/noD/y7v/SA7/i2b/SA//kW//Xiv/7ef/8ez/Ux3/vqr/d0z/uKL/49r/Txj/flX/5Nv/x7X/XSr/WiZsHYP7AAAAhnRSTlMA+guisPvM3e5xAuj3BGHs5gon4OXQJiCS1cDUckPDGCN/BW8i9QY2X3MSoRH+kUSHJP2gxFI90+J7vvGPcLOZUZS9LOGjPAllwqjrsjHw0ZMD1kW8IcHqg++xOpcr32MXEILPD04Wli/e44FnNGLO8i60kLkomgEzGUIHpOctimap9tzXhJT1wIgAAAKJSURBVGje7dj3U9swFAdwIE5khwwC2YsAhTLKKrMUKKt777333nvQhUR3S/de/2h/6l0ayZbkPF/vOH1/tt/nzn6WpVdSoqKioqKiMutTFu3tOdO0bd3iC92eBEIVuu7D2KfrYYRSnuywK37ACIwsyS2VrVub23hq8lby4TksmO2JMVfHsswcv0Dx600acmObcSMtXW1Zvvmo7eJ/Uzo6z7z++T0YIMfHzep3lmOQrKxk12/ow0BZ28AE0hgsQ6z66/vhgAkvA5iLAXOaARiQgMEAFkECVxjAFkggywDKIYEgY+XEoGmmgMuwwGoKaOfc8eqdFNBDAQHOHWT6yRsJIE0BC3gAIR/ePhYG4hRwkQ8Q8uipKHCPAlpEAPLs00cxYIAChoUAQl68fC4C9FFAtyBAyI/vAsBtCkgIA4QItKz7aiFQJQGItGyoEKiQAQRaltpbBOUAbstS+6OFsgCnZWOFgE8asG5Zausy3wZg1bLHYICvn82uv+P0I9oM8JK/fLO4/ojTbVpW5If2/rX1h9bqLXKpmOFcXk8tdimZxe4Xd7ELU0BWGDBvzbxEKGAH7A9nEwV0if0yLVszLysoYBD2pz9qZ9vCa8381FBAI3/jNYPFE6CADOeOn7+lto4ZCqiE3fxGHd6+t4YcPoCEGSecJCQw4PQhcNf/OMaCHsSvMQDvBFz9KtYoAfIZ1TGnLf6zUPUjteyBUQ7oU9gaNRt5jSOI+g92mw/t9sdLix4Kth2ynDtWGxqyjbiRNrRPYHbqj93YO9VyKRUULbxmw5ir42670GD2n9yM7TzcWFcz2KUlPQcR6tf1eozv6/oJhCKe5atcbSeNwEhvZ0gN4VVUVFRUZn/+APwGAa5o2hibAAAAAElFTkSuQmCC"
			//source: "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHg9IjBweCIgeT0iMHB4Igp3aWR0aD0iNDgiIGhlaWdodD0iNDgiCnZpZXdCb3g9IjAgMCA0OCA0OCIKc3R5bGU9IiBmaWxsOiMwMDAwMDA7Ij48cGF0aCBmaWxsPSIjRkYzRDAwIiBkPSJNNDMuMiwzMy45Yy0wLjQsMi4xLTIuMSwzLjctNC4yLDRjLTMuMywwLjUtOC44LDEuMS0xNSwxLjFjLTYuMSwwLTExLjYtMC42LTE1LTEuMWMtMi4xLTAuMy0zLjgtMS45LTQuMi00QzQuNCwzMS42LDQsMjguMiw0LDI0YzAtNC4yLDAuNC03LjYsMC44LTkuOWMwLjQtMi4xLDIuMS0zLjcsNC4yLTRDMTIuMyw5LjYsMTcuOCw5LDI0LDljNi4yLDAsMTEuNiwwLjYsMTUsMS4xYzIuMSwwLjMsMy44LDEuOSw0LjIsNGMwLjQsMi4zLDAuOSw1LjcsMC45LDkuOUM0NCwyOC4yLDQzLjYsMzEuNiw0My4yLDMzLjl6Ij48L3BhdGg+PHBhdGggZmlsbD0iI0ZGRiIgZD0iTTIwIDMxTDIwIDE3IDMyIDI0eiI+PC9wYXRoPjwvc3ZnPg=="
		}

		Label {
			text: message.note
			anchors.top: playImage.bottom
			anchors.bottom: thumbnailImage.bottom
			anchors.left: thumbnailImage.left
			anchors.right: thumbnailImage.right
			padding: 8
			font.pointSize: 10
			elide: Text.ElideRight
			wrapMode: Label.WordWrap
			color: "white"
			background: Rectangle {
				implicitWidth: parent.width
				implicitHeight: parent.height
				color: "black"
				opacity: 0.4
//				radius:
			}
		}

		MouseArea {
			anchors.fill: thumbnailImage
			enabled: message.isImported
			onClicked: {
				layout.popup(
							multimediaPlayerComponent,
							{
								source: "https://www.youtube.com/watch?v=" + message.videoId,
								title: message.note
							}
				);
			}
		}
	}
}
