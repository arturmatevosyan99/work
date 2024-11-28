import QtQml 2.15
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import Core.System 0.1

Rectangle {
	id: rootView
	objectName: "Push Service UI Test Suite"

	function test()
	{
		console.error("Ah!")
	}

	property var serviceList: [
								{
									code: NotificationCenter.APN,
									name: "APN"
								},
								{
									code: NotificationCenter.GCM,
									name: "GCM"
								},
								{
									code: NotificationCenter.FCM,
									name: "FCM"
								},
								{
									code: NotificationCenter.HCM,
									name: "HCM"
								},
								{
									code: NotificationCenter.RSM,
									name: "RSM"
								},
	]

	Column {
		id: buttonColumn

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: implicitHeight

		Repeater {
			model: serviceList

			Button {
				required property int index
				property string code: rootView.serviceList[index].code
				property string name: rootView.serviceList[index].name
				property var service: NotificationCenter.getService(code)
				property string token: service ? service.token : ""

				enabled: service
				width: buttonColumn.width
				text: "%1: %2".arg(name).arg(token ? (token.slice(0, 10) + "...") : "unknown")

				Connections {
					enabled: !!target
					target: service

					ignoreUnknownSignals: true

					function onTokenAcquired(old_token)
					{
						logText.text += name + " token acquired: " + token + "\n";
					}

					function onMessageReceived(title, icon, sound, body)
					{
						logText.text += "Message from " + name + " service received: " + title + "\n";
					}
				}

				onClicked: {
					if (service) {
						logText.text += name + " token requested for " + Native.bundleIdentifier + "...\n";
						service.requestToken();
					}
				}

				onPressAndHold: {
					Native.copyToClipboard(token);
					down = false;
					logText.text += name + " token copied to the clipboard!\n";
				}
			}
		}
	}

	Text {
		id: logText

		anchors.left: parent.left
		anchors.top: buttonColumn.bottom
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		wrapMode: Text.WrapAnywhere
	}
}
