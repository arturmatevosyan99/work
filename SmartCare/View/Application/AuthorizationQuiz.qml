import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Core.Meta 0.1
import Core.Model 0.1
import Core.System 0.1
import Core.View 0.1

import Repository.Model 0.1

import SmartCare.Controller 0.1
import SmartCare.Model 0.1
import SmartCare.View 0.1
import SmartCare.Meta 0.1

import "../../View"
import "../../View/Riddle"

import "../../../Core/View"

PatientQuiz {
	id: application

	Component.onCompleted: {
		if (1 == 2) {
			/// @todo (CAP-451): Search for a bundled inapp receipt and try to authorize with
		} else {
			application.model.riddleList.compose(welcomeRiddleComponent);
			application.model.riddleList.compose(authorizationRiddleComponent);
			application.model.riddleList.compose(codeVerificationRiddleComponent);
		}
	}

	onFinished: {
		if (!NotificationCenter.hasPermission) {
			application.model.riddleList.compose(notificationRiddleComponent);
		}
		application.model.riddleList.compose(finalRiddleComponent);
		application.controller.nextRequested();
	}

	/// @note We are done!
	signal finalized();
	/// @note We are ready to start
	signal welcomed(var qd);

	/// @note Email address or Phone number
	property string account: ""
	/// @note Knot of Session and Realm
	property alias avatar: welcomeQuiz.avatar

	property ActionAlert accountMissedAlert: ActionAlert {
		objectName: "ACCOUNT_MISSED"

		title: qsTr("The account you specified does not exist")
		text: qsTr("Do you want to register a new account?")

		onYes: {
			authorizationRiddleModel.signUp(application.account);
		}

		AlertOption {
			flag: AlertOption.Yes

			onOpted: {
//				application.controller.delegate.currentItem.model.signUp(account ?? application.account);
			}
		}

		AlertOption {
			flag: AlertOption.No
		}
	}

	title: qsTr("Welcome!")
	header: null
	topPadding: WindowManager.topSafePadding

	Connections {
		id: modelConnections

		target: application.model

		function onAsked(success)
		{
			authorizationRiddleModel.completeBusynessPhase(AuthorizationRiddle.RiddleAsking);
		}

		function onAskFaulted(message)
		{
			authorizationRiddleModel.stopBusyness();
		}
	}

	AuthorizationRiddle {
		id: authorizationRiddleModel

		welcomeQuiz: application.model
	}

	model: WelcomeQuiz {
		id: welcomeQuiz

		/**
		 * @note Возможные переходы между фазами:
		 * SignIn(1) -> CodeIn(3)
		 * SignIn(1) -> SignUp(2)
		 * SignUp(2) -> CodeUp(4)
		 * CodeIn(3) -> SignIn(1)
		 * CodeIn(3) -> OnBoard(5)
		 * CodeUp(4) -> SignIn(1)
		 * CodeUp(4) -> OnBoard(5)
		 * OnBoard(5) -> ...
		 */

		onOpened: {
			authorizationRiddleModel.completeBusynessPhase(AuthorizationRiddle.QuizOpening);
		}

		onUnopened: {
			authorizationRiddleModel.stopBusyness();
		}

		onInvited: {
			if (listModel.rowCount() > 1) {
				/// @todo Show a list(if options more than one) of welcome services to be choosen by user manually
			} else {
				var qd = listModel.data(listModel.index(0, 0), MapModel.Key);
				authorizationRiddleModel.completeBusynessPhase(AuthorizationRiddle.Invitation);

				if (welcomeQuiz.attach(qd)) {
					application.welcomed(qd);
					return ;
				} else {
					/// @xxx WTF?
				}
			}

			authorizationRiddleModel.stopBusyness();
		}

		onLocked: {
			console.warn("Locked!");
			Alert.action("ENDPOINT_LOCKED", qsTr("Service is unavailable"), qsTr("We will be back soon"), AlertOption.Ok);
			authorizationRiddleModel.stopBusyness();
		}

		onRejected: {
			console.warn("Rejected:", message);
			Alert.action("ENDPOINT_REJECTED", qsTr("Service is unavailable"), message, AlertOption.Ok);
			authorizationRiddleModel.stopBusyness();
		}
	}

	Component {
		id: welcomeRiddleComponent

		/// @todo Need an Image
		RiddleView {
			id: welcomeRiddleView

			question: qsTr("Welcome!")
			description: qsTr("We are very happy to see you")
			specifications: RiddleInformation.BackwardAvailable | RiddleInformation.ForwardAvailable | RiddleInformation.ForwardActive

			onAnswered: {
				QuizFlow.nextRequested();
			}

			InputAlert {
				id: aceEndpointAlert
				objectName: "CUSTOM_ENDPOINT"

				title: qsTr("Custom endpoint")
				placeholder: "https://123.realm.cadabra.cloud"
				flags: InputAlert.CancelOption

				onCompleted: {
					avatar.pointEnd(aceEndpointAlert.text);
				}
			}

			MouseArea {
				anchors.fill: parent

				pressAndHoldInterval: 4000
				onPressAndHold:	aceEndpointAlert.warn()
			}
		}
	}

	Component {
		id: authorizationRiddleComponent

		/// @todo Need an Image
		RiddleView {
			id: authorizationRiddleView

			property alias currentRiddleItem: authorizationStackLayout.currentItem

			model: authorizationRiddleModel
			Connections {
				target: model

				function onAccounted(onoff)
				{
					if (onoff) {
						QuizFlow.nextRequested();
					} else {
						Qt.callLater(application.accountMissedAlert.warn);
					}
				}
			}

			QuizFlow.onActiveChanged: {
				if (QuizFlow.active) {
					model.text = Qt.binding(function() { return authorizationRiddleView.currentRiddleItem ? authorizationRiddleView.currentRiddleItem.question : "" });
					model.description = Qt.binding(function() { return authorizationRiddleView.currentRiddleItem ? authorizationRiddleView.currentRiddleItem.description : "" });
					model.specifications = Qt.binding(function() { return authorizationRiddleView.currentRiddleItem ? authorizationRiddleView.currentRiddleItem.specifications : RiddleInformation.Empty });

					welcomeQuiz.phase = WelcomeQuiz.Phase.SignIn;
				}
			}

			onAnswered: {
				application.account = currentRiddleItem.answer;
				model.signIn(application.account);
			}

			onAnswerReceived: {
				currentRiddleItem.answer = answer;
			}

			StackLayout {
				id: authorizationStackLayout

				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: authorizationTabBar.top

				currentIndex: authorizationTabBar.currentIndex
				property var currentItem: currentIndex < 0 ? undefined : children[currentIndex]

				EmailAddressRiddle {
					id: emailAddressRiddle

					Layout.fillWidth: true
					Layout.fillHeight: true

					focus: QuizFlow.active

					onAnswered: {
						authorizationRiddleView.answered(answer);
					}
				}

				PhoneNumberRiddle {
					id: phoneNumberRiddleView

					Layout.fillWidth: true
					Layout.fillHeight: true

					focus: QuizFlow.active

					onAnswered: {
						authorizationRiddleView.answered(answer);
					}
				}
			}

			TabBar {
				id: authorizationTabBar

				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom

				background: Rectangle {
					color: "transparent"
				}

				TabButton {
					text: qsTr("Email")
				}
				TabButton {
					text: qsTr("Phone")
				}
			}
		}
	}

	Component {
		id: codeVerificationRiddleComponent

		RiddleView {
			id: codeVerificationRiddleWrapper

			model: authorizationRiddleModel

			QuizFlow.onActiveChanged: {
				if (QuizFlow.active) {
					model.text = Qt.binding(function() { return codeVerificationRiddleView ? codeVerificationRiddleView.question : "" });
					model.description = Qt.binding(function() { return codeVerificationRiddleView ? codeVerificationRiddleView.description : "" });
					model.specifications = Qt.binding(function() { return codeVerificationRiddleView ? codeVerificationRiddleView.specifications : RiddleInformation.Empty });

					codeVerificationRiddleView.clear();
				}
			}

			onAnswered: {
				model.verify(account, codeVerificationRiddleView.answer);
			}

			onAnswerReceived: {
				codeVerificationRiddleView.answer = answer;
			}

			CodeVerificationRiddle {
				id: codeVerificationRiddleView

				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom

				Layout.fillWidth: true
				Layout.fillHeight: true

				focus: QuizFlow.active

				Connections {
					id: welcomeQuizConnections

					target: application.controller
					enabled: welcomeQuiz.phase === WelcomeQuiz.Phase.OnBoard

					function onStacked(view, index)
					{
						application.controller.delegate.currentIndex = index;
						enabled = false;

						authorizationRiddleModel.completeBusynessPhase(AuthorizationRiddle.Completion);
					}
				}

				onAnswered: {
					codeVerificationRiddleWrapper.answered(answer);
				}
			}
		}
	}

	Component {
		id: notificationRiddleComponent

		RiddleView {
			id: notificationRiddleView

			Layout.fillWidth: true
			Layout.fillHeight: true

			question: qsTr("Allow us to send you notifications")
			description: qsTr("We promise to bother you only in extreme cases")
			specifications: RiddleInformation.BackwardAvailable | RiddleInformation.ForwardAvailable | RiddleInformation.BackwardActive | RiddleInformation.ForwardActive

			onAnswered: {
				NotificationCenter.requestToken();
				QuizFlow.nextRequested();
			}
			/// @todo Реализовать переход к следующему вопросу только после нажатия на кнопку либо "разрешить", либо "запретить"
/*
			Connections {
				id: notificationCenterConnections

				target: NotificationCenter
				enabled: QuizFlow.active

				function onRegistered(family, token)
				{
					QuizFlow.nextRequested();
				}
			}
*/
		}
	}

	Component {
		id: finalRiddleComponent

		RiddleView {
			id: finalRiddleView

			Layout.fillWidth: true
			Layout.fillHeight: true

			QuizFlow.onActiveChanged: {
				if (QuizFlow.active) {
					welcomeQuiz.phase = WelcomeQuiz.Phase.OnBoard;
					application.finalized();
				}
			}

			BusyIndicator {
				id: busyIndicator

				anchors.top: parent.top
				anchors.topMargin: parent.height / 5
				anchors.left: parent.left
				anchors.leftMargin: 10
				anchors.right: parent.right
				anchors.rightMargin: 10
			}
		}
	}
}
