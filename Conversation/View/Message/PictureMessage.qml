import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Conversation.View 0.1 as View
import Conversation.Model 0.1 as Model
import Core.View 0.1

import "../../../Core/View"

View.MessageView {
	id: rootView

	bubble: true

	model: Model.PictureMessage {
		id: message

		onImported: {
			if (message.thumbnailSize.height > 0 && message.thumbnailSize.width > 0) {
				var allowed_width = rootView.minimumWidth * 0.5;
				if (message.thumbnailSize.width <= allowed_width) {
					rootView.contentWidth = message.thumbnailSize.width - rootView.gap;
					rootView.contentHeight = message.thumbnailSize.height - rootView.gap;
				} else {
					rootView.contentWidth = allowed_width - rootView.gap;
					rootView.contentHeight = (message.thumbnailSize.height * (allowed_width / message.thumbnailSize.width)) - rootView.gap;
				}
			} else {
				rootView.contentWidth = cacheIndicator.width;
				rootView.contentHeight = cacheIndicator.height;
			}

			if (Model.PictureMessage.Cached === message.cacheState) {
				console.log("PictureMessage::onImported ", message.cachePath);
				rootView.loadBackgroundURLImage(message.cachePath, 0.5);
			} else if (message.hasThumbnail) {
				rootView.loadBackgroundBase64Image(message.thumbnailBase64, 0.5);
			}
			console.log("PictureMessage::onImported ", message.fileName);
			if (message.cache()) {
				cacheIndicator.running = true;
			}
		}

		onCached: {
			console.log("PictureMessage::onCached ", message.cachePath);
			cacheIndicator.running = false;
			rootView.loadBackgroundURLImage(message.cachePath, 0.5);
			console.log("PictureMessage::onCached (B)");
		}
	}

	Component {
		id: imageViewerComponent

		ImageViewer {
			id: viewer

			Component.onCompleted: {
			}
		}
	}

	BusyIndicator {
		id: cacheIndicator

		anchors.centerIn: parent
		visible: running
		running: false
	}

	onHeld: {
		if (canceled && message.isImported) {
			layout.popup(
							imageViewerComponent,
							{
								source: message.cachePath,
								title: message.fileName
							}
			);
		}
	}
}
