import QtQml 2.12
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.3

import Core.Global 0.1
import Core.View 0.1
import Core.System 0.1
import EC.Model 0.1
import DrOblozhko.Model 0.1

import "../../Core/View"

FocusScope {
	id: rootScope

	enum AccountType {
		None,
		EmailAddress,
		PhoneNumber,
		FacebookToken
	}

	Material.theme:	 Material.Light
	Material.accent: Material.Blue
	Material.primary: Material.White

	signal enrollmentRequested(string name)
	signal recognitionRequested(string account)
	signal recoveryRequested(string account)
	signal restorationRequested(string code)
	signal ratificationRequested(string code)
	signal boardingFinished()
	signal timeoutExpired()

	property string endpoint: ""

	property alias activeAction: registrationScope.isBusy

	property int gap: 20
	property int cornerRadius: 10
	property int fontPointSize: 22

	property int nextButtonHeight: 45
	property int nextButtonFontSize: 30
	property int nextButtonRadius: 10

	property int topMargin: Math.max(WindowManager.safeAreaInset.top, rootScope.gap)
	property int bottomMargin: Math.max(WindowManager.safeAreaInset.bottom, rootScope.gap + 5)
	property int keyboardHeight: 0

	property string textColor: "white"

	property string nextButtonText: qsTr("Next") + "  ►"
	property string nextButtonTextColor: "white"

	StackView.onActivated: {
		StatusBar.theme = StatusBar.Dark;
		WindowManager.orientScreen(Qt.PortraitOrientation);
		swipeView.forceActiveFocus();
	}

	StackView.onDeactivated: {
		WindowManager.orientScreen(Qt.PrimaryOrientation);
	}

	property Appointment model: Appointment {
	} onModelChanged: {
		modelConnection.target = model;
	}

	Connections {
		id: modelConnection

		target: model

		function onRejected()
		{
			rootScope.activeAction = false;
		}

		function onBlocked()
		{
			console.log("Tour::Appointment::onBlocked");
			Native.alertAction(qsTr("Hm..."), qsTr("Incorrect code, check code again or request a new one"), ["Ok"]);
			rootScope.activeAction = false;
		}

		function onLocked(success)
		{
			console.log("Tour::Appointment::onLocked");
			UserTracker.activate();
			if (success) { /// < Существующий аккаунт успешно заблокирован для восстановления и нужно его верифицировать
				registrationScope.isRecovery = true;
				if ("confirmation2" !== registrationBackground.state) {
					registrationBackground.state = "confirmation1";
				}
			} else { /// < Такой аккаунт не зарегистрирован и восстановить его, соответственно, нельзя
				Native.alertAction(qsTr("Hm..."), qsTr("Specified account does not exist"), ["Ok"]);
			}
			rootScope.activeAction = false;
		}

		function onArranged(success)
		{
			console.log("Tour::Appointment::onArranged");
			UserTracker.activate();
			if (success) { /// < Подготовлено к регистрации, но нужно верифицировать аккаунт
				registrationScope.isRecovery = false;
				if ("confirmation2" !== registrationBackground.state) {
					registrationBackground.state = "confirmation1";
				}
			} else { /// < Аккаунт не может быть восстановлен или зарегистрирован
				Native.alertAction(qsTr("Hm..."), qsTr("Please, provide valid email or phone number"), ["Ok"]);
			}
			rootScope.activeAction = false;
		}

		function onVerified(success)
		{
			console.log("Tour::Appointment::onVerified");
			if (success) { /// < Верификация нового аккаунта пройдена
				registrationBackground.state = (NotificationCenter.hasPermission ? "name" : "notificationCenterRequest");
			} else { /// < Неверный код
				Native.alertAction(qsTr("Hm..."), qsTr("Incorrect code, check code again or request a new one"), ["Ok"]);
			}
			rootScope.activeAction = false;
		}

		function onIssued(session_id, session_handle, session_scheme, channel_id, channel_scheme)
		{
			console.log("Tour::Appointment::onIssued");
			if (registrationScope.isRecovery && !NotificationCenter.hasPermission) { /// < Нужно получить разрешения на пуши и(потом) разрешение на доступ к Здоровью
				registrationBackground.state = "notificationCenterRequest";
				rootScope.activeAction = false;
			} else {
				rootScope.activeAction = true;
				rootScope.boardingFinished();
			}
		}
	}

	Connections {
		target: Native

		function onInputAlertDone(value, success)
		{
			if (success) {
				rootScope.endpoint = value;
				registrationScope.action();
			} else {
				rootScope.endpoint = "";
			}
		}

		function onApplicationStateChanged(previous, current)
		{
			if (current === Core.ActiveApplication && (pinInput.activeFocus || accountInput.activeFocus)) {
				actionButton.focus = true;
			}
		}

		function onScreenKeyboardChanged()
		{
			if (rootScope.keyboardHeight < Native.screenKeyboardScreenHeight) {
				rootScope.keyboardHeight = Native.screenKeyboardScreenHeight;
			}
		}
	}

	Timer {
		repeat: false
		running: rootScope.keyboardHeight == 0 && swipeView.currentItem == registrationScope
		interval: 1500

		onTriggered: {
			if (rootScope.keyboardHeight == 0) {
				rootScope.keyboardHeight = rootScope.height / 2;
			}
		}
	}

	onRecoveryRequested: {
		console.log("Tour::onRecoveryRequested");
		model.repointed(endpoint);
		activeAction = true;
		model.recover(account);
	}
	onRecognitionRequested: {
		console.log("Tour::onRecognizeRequested");
		model.repointed(endpoint);
		activeAction = true;
		model.recognize(account);
	}
	onRestorationRequested: {
		console.log("Tour::onRestorationRequested");
		activeAction = true;
		model.restore(code);
	}
	onRatificationRequested: {
		console.log("Tour::onRatificationRequested");
		activeAction = true;
		model.ratify(code);
	}
	onEnrollmentRequested: {
		console.log("Tour::onEnrollmentRequested");
		activeAction = true;
		model.enroll(name);
	}

	SwipeView {
		id: swipeView

		currentIndex: 0
		anchors.fill: parent

		interactive: currentIndex < 4

		background: Rectangle {
			gradient: Gradient {
				GradientStop {
					position: 0.0
					color: "#EC6F8C"
				}
				GradientStop {
					position: 1.0
					color: "#B72446"
				}
			}
		}

		Item {
			Rectangle {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				gradient: Gradient {
					GradientStop {
						position: 0.0
						color: "#31A9C7"
					}
					GradientStop {
						position: 1.0
						color: "#135B6D"
					}
				}
			}
			Image {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				source: "../Asset/Image/Tour1@1x.png"
				fillMode: Image.PreserveAspectCrop
			}
			Label {
				y: 0
				text: qsTr("«Metabolic Plate» is an app that can help change nutrition habits.")
				color: textColor
				wrapMode: Text.WordWrap

				verticalAlignment: Text.AlignVCenter
//				horizontalAlignment: Text.AlignHCenter

				anchors.top: parent.top
				anchors.topMargin: topMargin
				anchors.leftMargin: gap
				anchors.rightMargin: gap

				anchors.left: parent.left
				anchors.right: parent.right

				leftPadding: gap
				rightPadding: gap
				bottomPadding: 0
				topPadding: gap

				font.pointSize: fontPointSize
			}
			ActionButton {
				anchors.bottom: parent.bottom
				anchors.bottomMargin: rootScope.bottomMargin
				anchors.left: parent.left
				anchors.leftMargin: gap
				anchors.right: parent.right
				anchors.rightMargin: gap
				height: nextButtonHeight
				text: nextButtonText
				font.pointSize: nextButtonFontSize
				radius: nextButtonRadius
				textColor: nextButtonTextColor
				color: "#f17066"
				onClicked: {
					swipeView.currentIndex += 1;
				}
			}
		}

		Item {
			Rectangle {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				gradient: Gradient {
					GradientStop {
						position: 0.0
						color: "#F17066"
					}
					GradientStop {
						position: 1.0
						color: "#B3362D"
					}
				}
			}
			Image {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				source: "../Asset/Image/Tour3@1x.png"
				fillMode: Image.PreserveAspectCrop
				cache: false
			}
			Label {
				y: 0
				text: qsTr("«Metabolic Plate» is a free course that will help you analyze and understand your nutrition habits.")
				wrapMode: Text.WordWrap

				verticalAlignment: Text.AlignVCenter
//				horizontalAlignment: Text.AlignHCenter

				anchors.top: parent.top
				anchors.topMargin: topMargin
				anchors.leftMargin: gap
				anchors.rightMargin: gap

				anchors.left: parent.left
				anchors.right: parent.right

				leftPadding: gap
				rightPadding: gap
				bottomPadding: 0
				topPadding: gap

				font.pointSize: fontPointSize
				color: textColor
			}
			ActionButton {
				anchors.bottom: parent.bottom
				anchors.bottomMargin: rootScope.bottomMargin
				anchors.left: parent.left
				anchors.leftMargin: gap
				anchors.right: parent.right
				anchors.rightMargin: gap
				height: nextButtonHeight
				text: nextButtonText
				font.pointSize: nextButtonFontSize
				radius: nextButtonRadius
				textColor: nextButtonTextColor
				color: "#1a86a1"
				onClicked: {
					swipeView.currentIndex += 1;
				}
			}
		}

		Item {
			Rectangle {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				gradient: Gradient {
					GradientStop {
						position: 0.0
						color: "#C7A5E3"
					}
					GradientStop {
						position: 1.0
						color: "#7F6099"
					}
				}
			}
			Image {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				source: "../Asset/Image/Tour2@1x.png"
				fillMode: Image.PreserveAspectCrop
				cache: false
			}
			Label {
				y: 0
				text: qsTr("«Metabolic Plate» is an interactive coaching course with personalized premium support and care.")
				wrapMode: Text.WordWrap

				verticalAlignment: Text.AlignVCenter
//				horizontalAlignment: Text.AlignHCenter

				anchors.top: parent.top
				anchors.topMargin: topMargin
				anchors.leftMargin: gap
				anchors.rightMargin: gap

				anchors.left: parent.left
				anchors.right: parent.right

				leftPadding: gap
				rightPadding: gap
				bottomPadding: 0
				topPadding: gap

				font.pointSize: fontPointSize
				color: textColor
			}
			ActionButton {
				anchors.bottom: parent.bottom
				anchors.bottomMargin: rootScope.bottomMargin
				anchors.left: parent.left
				anchors.leftMargin: gap
				anchors.right: parent.right
				anchors.rightMargin: gap
				height: nextButtonHeight
				text: nextButtonText
				font.pointSize: nextButtonFontSize
				radius: nextButtonRadius
				textColor: nextButtonTextColor
				color: "#f17066"
				onClicked: {
					swipeView.currentIndex += 1;
				}
			}
		}

		Item {
			Rectangle {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				gradient: Gradient {
					GradientStop {
						position: 0.0
						color: "#EC6F8C"
					}
					GradientStop {
						position: 1.0
						color: "#B72446"
					}
				}
			}
			Image {
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 0
				anchors.top: parent.top
				anchors.topMargin: 0
				source: "../Asset/Image/Tour4@1x.png"
				fillMode: Image.PreserveAspectCrop
				cache: false;
			}
			Label {
				y: 0
				text: qsTr("«Metabolic Plate» helped more than 15,000 people who have changed their nutrition habits.")
				wrapMode: Text.WordWrap

				verticalAlignment: Text.AlignVCenter
//				horizontalAlignment: Text.AlignHCenter

				anchors.top: parent.top
				anchors.topMargin: topMargin
				anchors.leftMargin: gap
				anchors.rightMargin: gap

				anchors.left: parent.left
				anchors.right: parent.right

				leftPadding: gap
				rightPadding: gap
				bottomPadding: 0
				topPadding: gap

				font.pointSize: fontPointSize
				color: textColor
			}
			ActionButton {
				anchors.bottom: parent.bottom
				anchors.bottomMargin: rootScope.bottomMargin
				anchors.left: parent.left
				anchors.leftMargin: gap
				anchors.right: parent.right
				anchors.rightMargin: gap
				height: nextButtonHeight
				text: nextButtonText
				font.pointSize: nextButtonFontSize
				radius: nextButtonRadius
				textColor: nextButtonTextColor
				color: "#7f6099"
				onClicked: {
					swipeView.currentIndex += 1;
				}
			}
		}

		FocusScope {
			id: registrationScope

			focus: SwipeView.isCurrentItem

			property string	lastAccountPhoneEmail:	""

			property int accountType: Tour.AccountType.None
			property bool isRecovery: false
			property bool isBusy: false
			onIsBusyChanged: {
				actionButton.enabled = isBusy ? false : isActionable();
				actionButton.progress = isBusy ? -1 : 0;
				accountInput.enabled = !isBusy;
				topLabel.enabled = !isBusy;
				bottomLabel.enabled	= !isBusy;
			}

			function action()
			{
				Qt.inputMethod.commit();
				if ((registrationBackground.state === "recovery") || (registrationBackground.state === "recognition")) {
					registrationAlterConnections.enabled = true;
					if (accountInput.text.indexOf('@') >= 0) {
						accountType = Tour.AccountType.EmailAddress;
						Native.alertAction(
											qsTr("Please confirm the email: ") + "\n" + accountInput.text,
											qsTr("We are going to email a verification code, please make sure you specified the right address and mailbox is accessible"),
											[
												qsTr("Edit email address"),
												qsTr("Yes, send")
											]
						);
					} else if (accountInput.text[0] === '+') {
						accountType = Tour.AccountType.PhoneNumber;
						Native.alertAction(
											qsTr("Please confirm the number: ") + "\n" + accountInput.text,
											qsTr("We are about to send a verification code, please make sure you specified the right phone number"),
											[
												qsTr("Edit number"),
												qsTr("Yes, send text to this number")
											]
						);
					}
				} else if ((registrationBackground.state === "confirmation1") || (registrationBackground.state === "confirmation2")) {
					rootScope.restorationRequested(pinInput.text);
				} else if (registrationBackground.state === "notificationCenterRequest") {
					pushExampleImage.visible = false;
					if (isRecovery) {
						rootScope.activeAction = true;
						/// @todo Здесь будет переход к получению разрешения на доступ к Здоровью
						rootScope.boardingFinished();
					} else {
						registrationBackground.state = "name";
					}
					NotificationCenter.requestToken();
				} /* else if (registrationBackground.state === "healthAccessRequest") {
					healthAccessRequested();
				} */ else if (registrationBackground.state === "name") {
					rootScope.enrollmentRequested(accountInput.text);
				}
			}

			function isActionable()
			{
				if ((registrationBackground.state === "recognition") || (registrationBackground.state === "recovery") || (registrationBackground.state === "name")) {
					return (accountInput.isAcceptable && (accountInput.text != ""));
				} else if ((registrationBackground.state === "confirmation1") || (registrationBackground.state === "confirmation2")) {
					return (pinInput.text.length == 4 ? true : false);
				} else if (registrationBackground.state === "notificationCenterRequest") {
					return true;
				}
				return false;
			}

			Connections {
				id: registrationAlterConnections

				target: Native
				enabled: false

				function onActionAlertDone(index)
				{
					registrationScope.lastAccountPhoneEmail = accountInput.text.trim().toLowerCase();
					enabled = false;
					if (!index) {
						accountInput.focus = true;
						return ;
					} else if (rootScope.endpoint.length == 0) {
						/**
						  * @brief Quite possible that for moderation purposes
						  * we will use different endpoints for each in the future
						  */
						if (registrationScope.lastAccountPhoneEmail.endsWith('@t.est')) {
							rootScope.endpoint = "https://realm13.cadabra.systems";
						} else if (registrationScope.lastAccountPhoneEmail.endsWith('@apple.com')) {
							rootScope.endpoint = "https://realm13.cadabra.systems";
						} else if (registrationScope.lastAccountPhoneEmail.endsWith('@google.com')) {
							rootScope.endpoint = "https://realm13.cadabra.systems";
						} else if (registrationScope.lastAccountPhoneEmail.endsWith('@huawei.com')) {
							rootScope.endpoint = "https://realm13.cadabra.systems";
						} else if (registrationScope.lastAccountPhoneEmail.endsWith('@microsoft.com')) {
							rootScope.endpoint = "https://realm13.cadabra.systems";
						} else if (registrationScope.lastAccountPhoneEmail.endsWith('@rustore.ru')) {
							rootScope.endpoint = "https://realm13.cadabra.systems";
						}
					}

					if (registrationBackground.state === "recovery") {
						registrationScope.isRecovery = true;
						rootScope.recoveryRequested(accountInput.text);
					} else if (registrationBackground.state === "recognition") {
						registrationScope.isRecovery = false;
						rootScope.recognitionRequested(accountInput.text);
					}
				}
			}

			Rectangle {
				id: registrationBackground

				anchors.fill: parent
				gradient: Gradient {
					GradientStop {
						position: 0.0
						color: "#A8616C"
					}
					GradientStop {
						position: 1.0
						color: "#8B3543"
					}
				}
				opacity: rootScope.keyboardHeight > 0 ? 1 : 0
				Behavior on opacity {
					NumberAnimation {
						duration: 500
					}
				}

				onStateChanged: {
					actionButton.enabled = registrationScope.isActionable();
				}

				Label {
					id: topLabel

					anchors.top: parent.top
					anchors.topMargin: rootScope.topMargin
					anchors.left: parent.left
					anchors.leftMargin: rootScope.gap
					anchors.right: parent.right
					anchors.rightMargin: rootScope.gap
					anchors.bottom: accountInput.top
					anchors.bottomMargin: rootScope.gap

					color: "white"
					text: ""
					textFormat: Text.StyledText
					wrapMode: TextEdit.WordWrap
					verticalAlignment: Text.AlignTop

					fontSizeMode: Text.Fit
					minimumPointSize: 14
					font.pointSize: rootScope.fontPointSize

					onLinkActivated: {
						registrationBackground.state = link;
					}
				}

				AccountInput {
					id: accountInput

					focus: visible
					height: 50
					width: parent.width - gap -gap
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.bottom: actionButton.top
					anchors.bottomMargin: gap

					color: "white"
					radius: cornerRadius
					textFontPointSize: fontPointSize
					placeholderText: qsTr("Phone number with ' + ' or email")

					onTextChanged: {
						actionButton.enabled = registrationScope.isActionable();
					}

					onEditingFinished: {
						actionButton.click();
					}
				}

				PinInput {
					id: pinInput

					anchors.horizontalCenter: parent.horizontalCenter
					anchors.bottom: actionButton.top
					anchors.bottomMargin: gap

					height: 50
					width: parent.width - gap - gap

					focus: visible

					fontPointSize: 32
					sectionRadius: cornerRadius
					sectionWidth: 50
					sectionGap: gap
					sectionBorderinlineColor: "#f17066"
					sectionBorderoutlineColor: "transparent"
					sectionBackgroundColor: "white"

					onTextChanged: {
						actionButton.enabled = registrationScope.isActionable() && !registrationScope.isBusy;
					}

					onInputCompleted: {
						registrationScope.action();
					}
				}

				Timer {
					id: actionTimer

					running: actionButton.progress < 0
					repeat: false
					triggeredOnStart: false
					interval: 15000

					onTriggered: {
						rootScope.timeoutExpired();
					}
				}

				ActionButton {
					id: actionButton

					height: 50
					width: parent.width - gap - gap
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.bottom: bottomLabel.top
					anchors.bottomMargin: gap
					enabled: false
					text: ""
					font.pointSize: fontPointSize
					radius:	cornerRadius
					textColor: "white"
					color: "#f17066"

					onClicked: {
						rootScope.endpoint = "";
						registrationScope.action();
					}

					onPressAndHold: {
						Native.alertInput(qsTr("Custom endpoint"),  "https://ace1.hudeem-online.ru", Native.ActionCancel);
					}
				}

				Label {
					id: bottomLabel

					anchors.left: parent.left
					anchors.leftMargin: rootScope.gap
					anchors.right: parent.right
					anchors.rightMargin: rootScope.gap
					anchors.bottom: parent.bottom
					anchors.bottomMargin: rootScope.keyboardHeight > 0 ? rootScope.bottomMargin + rootScope.keyboardHeight : rootScope.height

					text: ""
					color: "white"
					linkColor: "white"
					font.pointSize: fontPointSize
					horizontalAlignment: Text.AlignHCenter

					onLinkActivated: { /// < обработчик ссылки из нижнего текста
						if ((registrationBackground.state === "confirmation1")) {
							if (registrationScope.isRecovery) {
								rootScope.recoveryRequested(registrationScope.lastAccountPhoneEmail);
							} else {
								rootScope.recognitionRequested(registrationScope.lastAccountPhoneEmail);
							}
						}
						registrationBackground.state = link
					}
				}

				Image {
					id: pushExampleImage

					visible: false
					anchors.top: actionButton.bottom
					anchors.topMargin: rootScope.gap
/*
					anchors.left: parent.left
					anchors.leftMargin: rootScope.gap
					anchors.right: parent.right
					anchors.rightMargin: rootScope.gap
*/
					anchors.bottom: parent.bottom
					anchors.bottomMargin: rootScope.gap
					anchors.horizontalCenter: parent.horizontalCenter

					source: "qrc:/DrOblozhko/Asset/Image/PushRequest.png"
					fillMode: Image.PreserveAspectFit

					MouseArea {
						anchors.fill: parent
						onClicked: {
							registrationScope.action();
						}
					}
				}

				state: "recognition"
				states: [
					State { name: "recognition"
						PropertyChanges {
							target: topLabel
							text: qsTr("Please provide your mobile phone number or email address:")
						}
						PropertyChanges {
							target: accountInput
							visible: true
							validator: AccountInput.ValidatorType.EmailOrPhone
						}
						PropertyChanges {
							target: pinInput
							visible: false
						}
						PropertyChanges {
							target: bottomLabel
							text: "<a href=\"recovery\">" + qsTr("I already have an account") + "</a>"
							visible: true
						}
						PropertyChanges {
							target: pushExampleImage
							visible: false
						}
						PropertyChanges {
							target: actionButton
							text: qsTr("Send")
						}
					},
					State { name: "recovery"
						PropertyChanges {
							target: topLabel
							text: qsTr("Please provide a phone number or email you previously registered with and account will be fully restored:")
						}
						PropertyChanges {
							target: accountInput
							validator: AccountInput.ValidatorType.EmailOrPhone
							visible: true
						}
						PropertyChanges {
							target: pinInput
							visible: false
						}
						PropertyChanges {
							target: bottomLabel
							text: "<a href=\"recognition\">" + qsTr("Create a new account") + "</a>"
							visible: true
						}
						PropertyChanges {
							target: pushExampleImage
							visible: false
						}
						PropertyChanges {
							target: actionButton
							text: qsTr("Send")
						}
					},
					State { name: "confirmation1"
						PropertyChanges {
							target: topLabel
							text: (
									registrationScope.accountType === Tour.AccountType.PhoneNumber
									? qsTr("We have sent verification code to\n<br>%1 <a href=\"%2\"><font color=\"white\" size=\"-1\">change</font></a><br>please enter the code in the field below (4 digits)")
									: qsTr("We just emailed verification code to\n<br>%1 <a href=\"%2\"><font color=\"white\" size=\"-1\">change</font></a><br>please enter verification code below (Also, check your junk folder to make sure it did not end up there)")
							).arg(accountInput.text).arg(registrationScope.isRecovery ? "recovery" : "recognition")
						}
						PropertyChanges {
							target: accountInput
							visible: false
						}
						PropertyChanges {
							target: pinInput
							visible: true
							text: ""
						}
						PropertyChanges {
							target: bottomLabel
							text: "<a href=\"confirmation2\">" + qsTr("Re-send code") + "</a>"
							visible: true
						}
						PropertyChanges {
							target: pushExampleImage
							visible: false
						}
						PropertyChanges {
							target: pinInput
							visible: true
						}
						PropertyChanges {
							target: actionButton
							text: qsTr("Confirm")
						}
					},
					State { name: "confirmation2"
						PropertyChanges {
							target: topLabel
							text: (
									registrationScope.accountType === Tour.AccountType.PhoneNumber
									? qsTr("We have sent verification code again to %1\n<br>please enter code in the field below (4 digits)")
									: qsTr("We emailed verification code again to %1\n<br>please enter the code in the field below (Also, check your junk folder to make sure it did not end up there)")
							).arg(accountInput.text)
						}
						PropertyChanges {
							target: accountInput
							visible: false
						}
						PropertyChanges {
							target: pinInput
							visible: true
							text: ""
						}
						PropertyChanges {
							target: bottomLabel
							text: "<a href=\"recognition\">" + qsTr("No code again") + "</a>"
							visible: true
						}
						PropertyChanges {
							target: pushExampleImage
							visible: false
						}
						PropertyChanges {
							target: actionButton
							text: qsTr("Confirm")
						}
					},
					State { name: "notificationCenterRequest"
						PropertyChanges {
							target: topLabel
							text: qsTr("The app needs permission to send notifications when a new message or video is delivered")
						}
						PropertyChanges {
							target: accountInput
							visible: false
						}
						PropertyChanges {
							target: pinInput
							visible: false
							text: ""
						}
						PropertyChanges {
							target: bottomLabel
							visible: false
						}
						PropertyChanges {
							target: pushExampleImage
							visible: true
						}
						PropertyChanges {
							target: actionButton
							text: qsTr("Give permission")
							enabled: true
						}
					},
					State { name: "name"
						PropertyChanges {
							target: registrationBackground
							color: "#a8616c"
						}
						PropertyChanges {
							target: topLabel
							text: qsTr("Finally, what is your name?")
						}
						PropertyChanges {
							target: accountInput
							validator: AccountInput.ValidatorType.None
							visible: true
							text: ""
							placeholderText: qsTr("Name")
						}
						PropertyChanges {
							target: pinInput
							visible: false
							text: ""
						}
						PropertyChanges {
							target: bottomLabel
							visible: false
						}
						PropertyChanges {
							target: pushExampleImage
							visible: false
						}
						PropertyChanges {
							target: actionButton
							text: qsTr("Save")
							enabled: false
						}
					}/*,
					State { name: stHealth
						PropertyChanges {
							target: registrationBackground
							color: "#a22c58"
						}
						PropertyChanges {
							target: topLabel
							text: qsTr("Also we need access to \"Health\", to analyze your daily activity automatically")
						}
						PropertyChanges {
							target: accountInput
							visible: false
						}
						PropertyChanges {
							target: actionButton
							text: qsTr("Allow access")
							enabled: true
						}
						PropertyChanges {
							target: pinInput
							visible: false
							text: ""
						}
						PropertyChanges {
							target: bottomLabel
							visible: false
						}
						PropertyChanges {
							target: pushExampleImage
							visible: true
						}
					}*/
				]
			} /// < registrationBackground
		} /// < registrationScope
	}
}
