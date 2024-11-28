import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Core.View 0.1
import Core.System 0.1

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model

View.MessageView {
	id: rootView

	contentWidth: childrenRect.width
	contentHeight: childrenRect.height

	model: Model.TextMessage {
		id: message

		onImported: {
			switch (message.format) {
				case Model.TextMessage.Plain:
					label.textFormat = Text.PlainText;
					break;

				case Model.TextMessage.Styled:
					label.textFormat = Text.StyledText;
					break;

				case Model.TextMessage.Rich:
					label.textFormat = Text.RichText;
					break;

				case Model.TextMessage.Markdown:
					label.textFormat = Text.MarkdownText;
					break;

				default:
					label.textFormat = Text.AutoText;
			}

			if (label.text.length > 0) {
				var height = label.height / label.lineCount;
				label.text = "";
				label.height = height;
				label.text = message.text;
			} else {
				label.text = message.text;
			}
		}
	}

	Label {
		id: label

		anchors.left: rootView.left
		anchors.top: rootView.top
		anchors.margins: 12

		text: "{...}"
		font: Theme.body1Font
		color: fontColor
		linkColor: fontColor
		wrapMode: Label.Wrap

		onContentHeightChanged: {
			height = label.contentHeight;
		}

		onContentWidthChanged: {
			width = Math.min(rootView.minimumWidth, label.contentWidth);
		}

		onHoveredLinkChanged: {
			console.log("TextMessage::onHoveredLinkChanged", link);
		}

		onLinkActivated: {
			console.log("MessageText::onLinkActivated", new Date().getTime());
		}

		MouseArea {
			id: hyperlinkArea

			anchors.fill: parent
			propagateComposedEvents: true
			pressAndHoldInterval: rootView.holdThreshold

			property string candidate: ""

			onPressAndHold: {
				console.log("TextMessage::onPressAndHold");
				mouse.accepted = true;
				candidate = "";
				var point = hyperlinkArea.mapToGlobal(mouse.x, mouse.y);
				rootView.held(Qt.point(point.x, point.y));
			}

			onPressed: {
				console.log("TextMessage::onPressed");
				candidate = label.linkAt(mouse.x, mouse.y);
				if (candidate) {
					mouse.accepted = true;
				} else {
					mouse.accepted = false;
				}
			}

			onPositionChanged: {
				console.log("TextMessage::onPositionChanged", mouse);
				if (candidate.length > 0) {
					mouse.accepted = true;
				}
			}

			onReleased: {
				console.log("TextMessage::onReleased");
				mouse.accepted = true;
				if (candidate.length > 0) {
					Qt.openUrlExternally(candidate);
					candidate = "";
				}
			}
		}
	}
}
