import QtQuick 2.15
import QtQml 2.15
import QtQml.Models 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import QtMultimedia 5.15

import Core.View 0.1
import Core.System 0.1
import Core.Model 0.1
import Core.Meta 0.1

import Repository.Controller 0.1
import Repository.Model 0.1

import Conversation.View 0.1
import Conversation.Model 0.1

import "../../../Core/View"

import "../../../Conversation/View"

/// @todo DrOblozhko.PatientChatKit: steps, license,
/// @todo DrOblozhko.DoctorChatKit: license, license patch, dynamic,
/// @todo DrOblozhko.ManagerChatKit: etc
Page {
	id: application

	enum MenuMode {
		Custom = 0,
		Collapse = 1,
		Profile = 2,
		Party = 4,
		Media = 8,
		Search = 16
	}

	/// @brief Scroll on tap
	property int tapCount: 10
	/// @brief Message side padding
	property int sideGap: 10
	/// @brief Green-line(Heartbeat signal) in Stackbar to indication connection loss
	property bool syncAvailable: false
	/// @brief Pushing progress (0-100)
	property double pushProgress: 0

	property alias headerShield: shieldDrawer.contentItem
	property alias headerBadge: stackBar.badgeText
	property alias headerTitle: stackBar.titleText
	property alias headerSubtitle: stackBar.subtitleText
	property alias headerIcon: stackBar.iconSource

	Component.onCompleted: {
		console.log("ChannelFeed::onCompleted");
	}

	Component.onDestruction: {
		console.log("ChannelFeed::onDestruction");
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		titleText: qsTr("Chat")

		stackView: application.StackView.view

		beatMode: syncAvailable ? StackBar.BeatMode.Stable : StackBar.BeatMode.Pulse
		keyboardCounterflow: true

		onBackClicked: {
		}

//		optionStyle: shieldDrawer.contentItem ? StackBar.OptionStyle.Stack : StackBar.OptionStyle.None
		onOptionClicked: {
			(shieldDrawer.visible) ? shieldDrawer.close() : shieldDrawer.open();
		}

		onTitleClicked: {
			/// @note jump up
		}

		onTitleDoubleClicked: {
			application.StackView.view.push(profileComponent, {"model": application.model});
		}
	}

	/**
	 * @brief Chat controller
	 */
	property BusyBoard controller: BusyBoard {
		/**
		 * @brief Push a text message to the chat
		 * @param type: string message Text value
		 * @return type: boolean True if success
		 */
		function text(message)
		{
			return (
					application.model &&
					application.model.isOpened &&
					application.model.push(message)
			);
		}

		/**
		 * @brief Push a command message to the chat
		 * @param type: string name Command name
		 * @param type: array argument_list(optional) List of arguments
		 * @return type: boolean True if success
		 */
		function command(name, argument_list)
		{
			return (
					application.model &&
					application.model.isOpened &&
					application.model.push("/" + name + " " + (argument_list ?? []).map(a => a.trim()).join(" "))
			);
		}

		function closeMenu()
		{
			shieldDrawer.close();
			return true;
		}

		function openProfile()
		{
			application.StackView.view.push(profileComponent, {"model": application.model});
			return true;
		}

		function openParty()
		{
			application.StackView.view.push(partyComponent, {"model": application.model});
			return true;
		}

		function openMedia()
		{
			application.StackView.view.push(mediaComponent, {"model": application.model});
			return true;
		}

		function openSearch()
		{
	//		application.StackView.view.push(mediaComponent, {"model": application.model});
			return false;
		}

		function doMode(enumber)
		{
			switch (enumber) {
				case ChannelFeed.MenuMode.Collapse: return closeMenu();
				case ChannelFeed.MenuMode.Profile:  return openProfile();
				case ChannelFeed.MenuMode.Party: return openParty();
				case ChannelFeed.MenuMode.Media: return openMedia();
				case ChannelFeed.MenuMode.Search: return openSearch();
				default: return false;
			}
		}

		/**
		 * @brief Open and stack another application
		 * @todo Needs to be implemented on a regular(system) basis via signal
		 * @param type: Core.Meta.Qd Object Id
		 * @param type: string application_scheme Object will be opened as Application with such scheme hash
		 * @param type: component_parameter Component parameters
		 * @param type: application_option Application options
		 * @return type: boolean True if success
		 */
		function openApplication(id, application_scheme, component_parameter, application_option)
		{
			var render = SchemeFactory.load("Repository.Meta.ApplicationRender", application_scheme);
			if (render) {
				var component = Qt.createComponent(render.makeURL());
				if (component) {
					var view = application.StackView.view.push(component, component_parameter, StackView.PushTransition);
					if (view) {
						application.model.session.attach(view.model, id, true, application_option);
						return true;
					} else {
						console.log("Could not push Component to the StackView");
					}
				} else {
					console.log("Could not create Component for scheme:", application_scheme);
				}
			}
			return false;
		}
	}

	/// @brief Complex data model with chat feed, party and sync log
	property GroupChannel model: GroupChannel {
	} onModelChanged: {
		console.debug("ChannelFeed::onModelChanged", application.model);
		modelConnection.target = model;
		if (modelConnection.target.isOpened) {
			modelConnection.onOpened();
		}
	}

	Connections {
		id: syncConnection

		target: application.model.session
		enabled: false

		function onUpstreamed(onoff)
		{
			application.syncAvailable = application.model.session.hasUpstream;
		}
	}

	Connections {
		id: modelConnection

		property var indexPromise: undefined

		target: application.model

		function promiseIndex()
		{
			if (modelConnection.target && (!indexPromise || indexPromise.isIdle || indexPromise.duration() > 60)) {
				indexPromise = modelConnection.target.index();
				return true;
			}
			return false;
		}

		function onOpened()
		{
			console.log("ChannelFeed.modelConnection::onOpened");

			syncConnection.enabled = true;
			syncAvailable = application.model.session.hasUpstream;

			if (application.headerIcon.length == 0) {
				application.headerIcon = application.model.descriptor.objectPicture;
			}
			/*if (application.headerBadge.length === 0) {
				application.headerBadge = ("§%1").arg(application.model.descriptor.objectId);
			}*/
			if (application.headerSubtitle.length === 0) {
/*
				application.headerSubtitle = ("§%1: %2")
					.arg(application.model.descriptor.objectId)
					.arg(application.model.descriptor.objectSimplifiedName)
				;
*/
			}
		}

		function onInitialized(dump, success)
		{
			console.log("ChannelFeed.modelConnection::onInitialized");
		}

		function onPulled(upper_log_bound, lower_log_bound, flags)
		{
			console.log("ChannelFeed.modelConnection::onPulled");
			if (flags === GroupChannel.PushMessage) {
				Alert.sound("qrc:/Conversation/Asset/Sound/ChannelPush.wav");
/*
				if (!pushSound.playing && !pullSound.playing) {
					pushSound.play();
				}
*/
			} else if (flags === GroupChannel.ReceiveMessage) {
				Alert.sound("qrc:/Conversation/Asset/Sound/ChannelPull.wav");
/*
				if (!pushSound.playing && !pullSound.playing) {
					pullSound.play();
				}
*/
			}
		}

		function onProgressed(value)
		{
			application.pushProgress = value;
			console.log("ChannelFeed.modelConnection::onProgressed", application.pushProgress);
		}
	}

	Component {
		id: profileComponent
		ChannelProfile {
		}
	}

	Component {
		id: mediaComponent
		ChannelMedia {
		}
	}

	Component {
		id: partyComponent
		ChannelParty {
		}
	}

	Component {
		id: cameraComponent

		Camera {
			id: camerView

			onCaptured: {
				imageThumbnail.enqueue(filename, true);
				application.StackView.view.pop(StackView.PushTransition);
			}
		}
	}

	SoundEffect {
		id: pushSound

		loops: 0
		source: "/Conversation/Asset/Sound/ChannelPush.wav"
	}

	SoundEffect {
		id: pullSound

		loops: 0
		source: "/Conversation/Asset/Sound/ChannelPull.wav"
	}

	Drawer {
		id: shieldDrawer

		edge: Qt.RightEdge
		width: application.width * 0.80
		height: application.height
		topPadding: WindowManager.topSafePadding
		background: Item {
			Rectangle {
				id: shieldDrawerToggler

				anchors.top: parent.top
				anchors.bottom: parent.bottom
				anchors.left: parent.left
				width: 20

				radius: 10

				Rectangle {
					anchors.centerIn: parent
					anchors.margins: 6
					width: 6
					height: 20

					radius: 2

					color: Material.color(Material.Grey, Material.Shade200)
				}
			}
			Rectangle {
				anchors.top: parent.top
				anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.leftMargin: shieldDrawerToggler.radius
				anchors.right: parent.right

				color: "#FFFFFF" /// @??? Dark | Light
			}
		}

		onAboutToShow: {
			if (contentItem instanceof ChannelShield) {
				modelConnection.promiseIndex();
				contentItem.revealed();
			}
		}

		onContentItemChanged: {
			if (contentItem instanceof ChannelShield) {
				contentItem.channelFeed = application;
				shieldDrawerConnections.target = contentItem
			}
		}

		Connections {
			id: shieldDrawerConnections

			enabled: !!target
			ignoreUnknownSignals: true

			function onModeRequested(enumber)
			{
				Qt.callLater(shieldDrawer.close);
				Qt.callLater(application.controller.doMode, enumber);
			}

			function onApplicationRequested(object_id, application_scheme, option_map)
			{
				Qt.callLater(shieldDrawer.close);
				Qt.callLater(application.controller.openApplication, object_id, application_scheme, {}, option_map);
			}

			function onCustomComponentRequested(component)
			{
				Qt.callLater(shieldDrawer.close);
				if (application.StackView && application.StackView.view) {
					application.StackView.view.push(component, {}, StackView.PushTransition);
				}
			}
		}
	}

	ImageThumbnail {
		id: imageThumbnail

		format: ImageThumbnail.JPEG
		limit: 1280
		quality: 100

		onStarted: {

		}

		onFinished: {
			if (imageThumbnail.flush()) {
				application.model.push(imageThumbnail.buffer, GroupChannel.ImageBinary);
			} else {
				Native.alertAction(qsTr("Oh!"), qsTr("Image processing error, fell free to contact us"), ["Ok"]);
			}
		}
	}

	AudioRecorder {
		id: audioRecorder

		onStateChanged: {
			switch (value) {
				case AudioRecorder.On:
					composeLayout.forceState = "record";
					break;

				case AudioRecorder.Off:
					composeLayout.forceState = "";
					if (audioRecorder.flush()) {
						application.model.push(audioRecorder.buffer, GroupChannel.AudioBinary);
					} else {
						Native.alertAction(qsTr("Oh!"), qsTr("Audio processing error, fell free to contact us"), ["Ok"]);
					}
					break;
			}
		}

		onDurationChanged: {
			console.log("ChannelFeed::AudioRecorder::onDurationChanged");
		}
	}

	Connections {
		id: albumConnection

		target: Native

		function onAlbumItemPicked(filename) {
			imageThumbnail.enqueue(filename, false);
		}
/*
		function onCameraPictureTaked() {

		}
*/
	}

	Loader {
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.left: parent.left
		anchors.leftMargin: inputPane.leftPadding + application.sideGap
		anchors.bottom: inputPane.top

		asynchronous: ("systems.cadabra.queen.ladybug" != Native.bundleIdentifier)

		sourceComponent: FeedView {
			id: feedView

			channelModel: modelConnection.target
			messageMargin: inputPane.leftPadding + application.sideGap
			messageMinimumWidth: feedView.width - inputPane.leftPadding - application.sideGap
			messageMinimumHeight: (feedView.height - stackBar.contentHeight - inputPane.height) * 0.6

			focus: true

			onLoadRequested: {
				application.model.fetch(GroupChannel.FeedRotate, end);
			}

			onRedactionRequested: {
				inputPane.activeMessage = model;
			}

			onModifyRequested: {
				application.model.modify(meta);
			}

			onRepushRequested: {
				application.model.repush(uid);
			}

			onWithdrawRequested: {
				application.model.withdraw(qd);
			}

			onDiscardRequested: {
				application.model.discard(uid);
			}

			onPopupRequested: {
				if (!component) {
					var object_id = properties.objectId;
					var application_scheme = properties.applicationScheme;
					var attach_option = properties.attachOption ?? {}
					if (!object_id || !application_scheme) {
						console.error("Object id and/or application scheme is/are unspecified in the popup request");
						return ;
					}
					delete properties.objectId;
					delete properties.applicationScheme;
					delete properties.attachOption;
					application.controller.openApplication(object_id, application_scheme, properties, attach_option);
				} else {
					application.StackView.view.push(component, properties, StackView.Transition);
				}
			}

			onMessageRead: {
				if (
					application.model.isOpened
//					&&
//					application.model.hasUnreadMessages
				) {
					application.model.read(offset, GroupChannel.FeedOffset);
				}
			}

			onMovementStarted: {
				if (!activeFocus) {
					forceActiveFocus();
				}
			}

			Connections {
				id: statusbarConnection

				target: StatusBar
				enabled: true

				function onTouched()
				{
					if (feedView.count <= 0) {
						return ;
					}

					var index = feedView.lastVisibleIndex + application.tapCount;
					if (index >= feedView.count) {
						index = feedView.count - 1;
					}
					feedView.scroll(index)
				}
			}
		}
	}

	Pane {
		id: inputPane

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		focusPolicy: Qt.WheelFocus

		onContentHeightChanged: {
//			console.warn("ChannelFeed.inputPane::contentHeight=", contentHeight);
		}

		enabled: application.model.isOpened
		background: Rectangle {
			color: "white"
			opacity: 0.95

			Rectangle {
				width: parent.width
				height: 1
				anchors.top: parent.top
				color: "transparent"
				border.color: Material.accent
			}
		}

		property real messageImageHeight: messageText.font.pixelSize + (application.sideGap * 2)
		property var activeMessage: undefined
		onActiveMessageChanged: {
			if (activeMessage) {
				messageText.text = activeMessage.previewText;
				messageText.focus = true;
			}
		}

		RowLayout {
			id: composeLayout

			property string forceState: ""

			state: (forceState.length > 0) ? forceState : "input"
			states: [
				State {
					name: "input"
					PropertyChanges {
						target: messageText
						enabled: true
						placeholderText: qsTr("Message")
						text: ""
						durationState: 0
						colorState: "transparent"
					}
				},
				State {
					name: "prerecord"
					PropertyChanges {
						target: messageText
						enabled: true
						placeholderText: qsTr("Record is starting...")
						text: ""
						durationState: sendArea.pressAndHoldInterval - 300
						colorState: "transparent"
					}
				},
				State {
					name: "postrecord"
					PropertyChanges {
						target: messageText
						enabled: true
						placeholderText: qsTr("Record is processing...")
						text: ""
						durationState: 0
						colorState: "transparent"
					}
				},
				State {
					name: "record"
					PropertyChanges {
						target: messageText
						enabled: false
						placeholderText: audioRecorder.isEmpty ? "○ " + qsTr("Recording...") : "● " + qsTr("Recording") + ": " + audioRecorder.duration.toLocaleTimeString(Qt.locale(), "mm:ss")
						text: ""
						durationState: sendArea.holdLimit - 300
						colorState: Material.accent
					}
				}
			]
			anchors.fill: parent
			spacing: 2

			MouseArea {
				Layout.preferredHeight: optionIcon.implicitHeight + (application.sideGap / 2)
				Layout.preferredWidth: messageText.activeFocus && optionIcon.state !== "cancel" ? 0 : optionIcon.implicitWidth + 5
				Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

				Behavior on Layout.preferredWidth  {
					NumberAnimation {
						duration: 150
						easing.type: Easing.InOutQuad
					}
				}

				onClicked: {
					if (optionIcon.state === "attachment") {
						attachmentMenu.open()
					} else if (optionIcon.state === "cancel") {
						inputPane.activeMessage = undefined;
						messageText.clear();
					}
				}

				Image {
					id: optionIcon

					anchors.centerIn: parent

					fillMode: Image.PreserveAspectFit
					sourceSize.height: inputPane.messageImageHeight
					sourceSize.width: inputPane.messageImageHeight

					visible: parent.Layout.preferredWidth > 0

					state: inputPane.activeMessage ? "cancel" : "attachment"
					states: [
						State {
								name: "attachment"
								PropertyChanges {
									target: optionIcon
									source: "/Conversation/Asset/Image/Attachment2.svg"
								}
						},
						State {
								name: "cancel"
								PropertyChanges {
									target: optionIcon
									source: "/Conversation/Asset/Image/Cancel2.svg"
								}
						}
					]

					Menu {
						id: attachmentMenu

						width: composeLayout.width

						Action {
							text: qsTr("Album")
							onTriggered: {
								Native.openAlbum();
							}
						}
						Action {
							text: qsTr("Camera")
							onTriggered: {
								var view = application.StackView.view.push(cameraComponent, {}, StackView.PushTransition); /// Native.openCamera();
							}
						}
						MenuSeparator {

						}
						Action {
							text: qsTr("Close")
							onTriggered: {
								attachmentMenu.close();
							}
						}
					}
				}
			}

			ScrollView {
				id: messageScroll

				Layout.fillWidth: true
				Layout.maximumHeight: Screen.desktopAvailableHeight / 3
				Layout.preferredHeight: messageText.implicitHeight
				Layout.alignment: Qt.AlignVCenter

				ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
				ScrollBar.horizontal.interactive: false

				onImplicitHeightChanged: {
					console.log("ChannelFeed.messageScroll.onImplicitHeightChanged", implicitHeight);
				}

				onHeightChanged: {
					console.log("ChannelFeed.messageScroll.onHeightChanged", height);
					/// @hack Some large text was copy&pasted
					if (messageText.cursorUnreachablePosition > -Infinity && messageText.length - messageText.cursorUnreachablePosition > 50) {
						messageText.cursorPosition = messageText.cursorUnreachablePosition;
						messageText.cursorUnreachablePosition = -Infinity;
					}
				}

				onFocusChanged: {
					console.log("ChannelFeed.messageScroll.onFocusChanged", focus, focusReason);
				}

				Layout.onPreferredHeightChanged: {
					console.log("Layout.onPreferredHeightChanged", Layout.preferredHeight);
				}

				Layout.onMaximumHeightChanged: {
					console.log("Layout.onMaximumHeightChanged", Layout.maximumHeight);
				}

				TextArea {
					id: messageText

					property alias positionGradient: textBackground.gradientStopPosition
					property alias durationState: stateAnimation.duration
					property color colorState: "transparent"
					property bool empty: preeditText.length === 0 && text.length === 0

					verticalAlignment: TextEdit.AlignTop
					wrapMode: TextArea.Wrap
					topPadding: application.sideGap + 3
					leftPadding: application.sideGap
					rightPadding: application.sideGap
					bottomPadding: application.sideGap
					background: null
					focus: true

					Keys.onReturnPressed: {
						if (event.modifiers & Qt.ControlModifier) {
							messageText.text += "\n";
							return ;
						}

						Qt.inputMethod.commit();
						if (inputPane.activeMessage) {
							inputPane.activeMessage.text = messageText.text;
							application.model.modify(inputPane.activeMessage.exportMeta());
							inputPane.activeMessage = undefined;
						} else {
							application.model.push(messageText.text);
						}
						messageText.clear();
					}

					onEditingFinished: {
						console.log("ChannelFeed.messageText::onEditingFinished");
						Qt.inputMethod.commit();
					}

					onReleased: {
						console.log("ChannelFeed.messageText::onReleased");
					}

					onActiveFocusChanged: {
						console.log("ChannelFeed.messageText::onActiveFocusChanged", activeFocus);
					}

					onFocusChanged: {
						console.log("ChannelFeed.messageText::onFocusChanged", focus, focusReason);
					}

					onInputMethodComposingChanged: {
						console.log("onInputMethodComposingChanged: ", inputMethodComposing);
					}

					onTextChanged: {
						console.log("ChannelFeed.messageText::onTextChanged", text.length);
					}

					onPreeditTextChanged: {
						console.log("ChannelFeed.messageText::onPreeditTextChanged: ", preeditText.length);
					}

					onCursorDelegateChanged: {
						console.log("ChannelFeed.messageText::onCursorDelegateChanged", cursorDelegate);
					}

					property int cursorUnreachablePosition: -Infinity
					onCursorPositionChanged: {
						/// @hack Some large text was copy&pasted
						cursorUnreachablePosition = (text.length < cursorPosition) ? text.length : -Infinity;
//						console.log("ChannelFeed.messageText::onCursorPositionChanged", cursorPosition);
					}

					onCursorRectangleChanged: {
//						console.log("ChannelFeed.messageText::onCursorRectangleChanged", cursorRectangle);
					}

					onCursorVisibleChanged: {
//						console.log("ChannelFeed.messageText::onCursorVisibleChanged", cursorVisible);
					}
				}

				background: Rectangle {
					id: textBackground

					radius: 10
					border.width: 1
					border.color: "whitesmoke"

					property double gradientStopPosition: 0.999
					gradient: Gradient {
						orientation: Gradient.Horizontal

						GradientStop {
							position: 0.0
							color: messageText.colorState
						}
						GradientStop {
							position: textBackground.gradientStopPosition  - 0.001
							color: messageText.colorState
						}
						GradientStop {
							position: textBackground.gradientStopPosition  + 0.001
							color: "whitesmoke"
						}
						GradientStop {
							position: 1.0
							color: "whitesmoke"
						}
					}

					NumberAnimation {
						id: stateAnimation

						target: textBackground
						property: "gradientStopPosition"
						running: false
						from: 0.999
						to: 0
						duration: 0
						onDurationChanged: {
							running = false;
							if (duration > 0) {
								if (textBackground.gradientStopPosition > 0) {
									textBackground.gradientStopPosition = 0.999;
									from = textBackground.gradientStopPosition;
									to = 0;
								} else {
									textBackground.gradientStopPosition = 0;
									from = 0;
									to = 0.999;
								}
								running = true;
							} else {
								textBackground.gradientStopPosition = 0.999;
							}
						}
					}
				}
			}

			MouseArea {
				id: sendArea

				Layout.preferredHeight: messageIcon.implicitHeight + (application.sideGap / 2)
				Layout.preferredWidth: messageIcon.implicitWidth + 5
				Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

				onClicked: {
					console.log("MouseArea::onClicked");
					if (messageIcon.state == "edit" || messageIcon.state == "input") {
						Qt.inputMethod.commit();
						if (inputPane.activeMessage) {
							inputPane.activeMessage.text = messageText.text;
							application.model.modify(inputPane.activeMessage.exportMeta());
							inputPane.activeMessage = undefined;
						} else {
							application.model.push(messageText.text);
						}
						messageText.clear();
					}
				}

				onPressed: {
					console.log("MouseArea::onPressed");
					if (messageIcon.state == "voice") {
						composeLayout.forceState = "prerecord";
					}
				}

				property int holdLimit: 30000
				pressAndHoldInterval: 1500
				onPressAndHold: {
					console.log("MouseArea::onPressAndHold");
					if (messageIcon.state != "voice") {
						return;
					}
					audioRecorder.start(holdLimit);
				}

				onReleased: {
					console.log("MouseArea::onReleased");
					if (messageIcon.state != "voice") {
						return;
					}
					composeLayout.forceState = "postrecord";
					if (!audioRecorder.stop()) {
						composeLayout.forceState = "";
					}
				}

				Image {
					id: messageIcon

					anchors.centerIn: parent

					fillMode: Image.PreserveAspectFit
					sourceSize.height: inputPane.messageImageHeight
					sourceSize.width: inputPane.messageImageHeight

					state: messageText.empty ? "voice" : inputPane.activeMessage ? "edit" : "input"
					states: [
						State {
							name: "edit"
							PropertyChanges {
								target: messageIcon
								source: "/Conversation/Asset/Image/Ok.svg"
							}
						},
						State {
							name: "input"
							PropertyChanges {
								target: messageIcon
								source: "/Conversation/Asset/Image/ArrowUp.svg"
							}
						},
						State {
							name: "voice"
							PropertyChanges {
								target: messageIcon
								source: "/Conversation/Asset/Image/Microphone3.svg"
							}
						}
					]
				}
			}
		}
	}
}
