import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import Core.View 0.1
import DrOblozhko.Model 0.1 as Model

import "../../../Core/View"

Loader {
	id: application

	property Model.Webinar model: Model.Webinar {
		onOpened: {
			page();
		}

		onFetched: {
			switch (application.model.type) {
				case Webinar.Unknown:
					application.sourceComponent = unknownComponent;
					break;

				case Webinar.HyperLink:
					application.sourceComponent = hyperlinkComponent;
					break;

				default:
					application.sourceComponent = multimediaPlayerComponent;
			}
		}
	} onModelChanged: {
		console.debug("Webinar::onModelChanged", application.model);
		if (application.model.isOpened) {
			application.model.onOpened();
		}
	}

	Component {
		id: multimediaPlayerComponent

		MultimediaPlayer {
			id: videoPlayer

			source: application.model.source
			title: application.model.title

			onStopped: {
				application.StackView.view.pop();
			}
		}
	}

	Component {
		id: hyperlinkComponent

		Page {
			anchors.fill: parent

			padding: 0
			leftInset: 0
			rightInset: 0

			header: StackBar {
				id: navigationBar

				titleText: qsTr("Webinar")
				subtitleLabel: application.model.title
				stackView: application.StackView.view

				optionStyle: StackBar.OptionStyle.None
			}

			Timer {
				repeat: false
				interval: 2000
				running: true
				onTriggered: {
					Qt.openUrlExternally(application.model.source);
				}
			}

			Text {
				id: hyperlinkLabel

				anchors.left: parent.left
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter

				height: contentHeight

				padding: 10
				wrapMode: Text.WordWrap
				horizontalAlignment: Text.AlignHCenter

				text: qsTr("Live broadcast will open in a browser shortly or press button here if that did not happen automatically")
			}

			Button {
				id: hyperlinkButton

				anchors.top: hyperlinkLabel.bottom
				anchors.topMargin: 5
				anchors.horizontalCenter: hyperlinkLabel.horizontalCenter

				onClicked: {
					Qt.openUrlExternally(application.model.source);
				}

				contentItem: Text {
					text: qsTr("Live!")
					font.weight: Font.ExtraBold
					font.pixelSize: 19
					color: parent.down ? Qt.darker(navigationBar.accentColor, 2.0) : navigationBar.accentColor
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
				}

				background: Rectangle {
					border.width: 1
					border.color: navigationBar.accentColor
					radius: 4
				}
			}
		}
	}

	Component {
		id: unknownComponent

		Page {
			anchors.fill: parent

			padding: 0
			leftInset: 0
			rightInset: 0

			header: StackBar {
				id: navigationBar

				titleText: qsTr("Webinar")
				subtitleText: qsTr("Error")
				stackView: application.StackView.view
				optionStyle: StackBar.OptionStyle.None
			}

			Text {
				id: errorLabel

				anchors.fill: parent

				padding: 10
				wrapMode: Text.WordWrap
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignHCenter

				text: qsTr("Webinar is not available for you by that time, feel free to contact us if you believe this is in error")
			}
		}
	}
}
