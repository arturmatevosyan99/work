import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3

import Core.Test 0.1

import Repository.Model 0.1
import DrOblozhko.Model 0.1
import Mentalgami.Model 0.1

import "qrc:/Conversation/View/Application" as ConversationApplication
import "qrc:/Mentalgami/View/Application" as MentalgamiApplication

/**
 * @details
 * 1) Регистрация аватаром eureka через WelcomeQuiz
 * 2) Получение списка объектов домашней директории
 * 3) Проверка наличия чата (DrOblozhko.AppointmentChannel)
 * 4) Отправка команды через MCI "/echo Hello World" и ответ-возвращение EchoMessage обратно с туза
 * 5) Проверить, что последним сообщением в чате является сообщение с текстом, идентичным отправленному нами в пункте 4
 */
PetrI {
	id: petrI
	objectName: "Регистрация Eureka-пациентов и повторная отправка сообщений"

	Component.onCompleted: {

		console.log(petrI + ".autoFire:", petrI.autoFire);
		if (!petrI.autoFire) {
			return ;
		}
	}

	context: Context {
		objectName: new Date().toLocaleString(Qt.locale(), "hh.mm.ss_dd.MM.yyyy")

		property Avatar patient: Avatar {
			Component.onCompleted: {
				pointEnd(getEnvironmentVariable("ACE_ENDPOINT", "http://127.0.0.1:8080"));
			}

			property int rInt: Math.floor( Math.random()*9999999 )
			property string account: "patient" + "_" + context.objectName + "_"+ rInt + "@cadabra.local"
			property string code: "9999"
			property int channelFeedId

			property variant savedHomeIndex: []
			property int n
			property int iterationsCounter

			session.skeletonName: "eureka"
			realm.synchronizationMode: Realm.Lazy
			realm.objectName: "ladybug" + "_" + context.objectName

			property ConversationApplication.ChannelFeed channelFeed: ConversationApplication.ChannelFeed {

			}

			property MentalgamiApplication.AuthorizationQuiz authorizationQuiz: MentalgamiApplication.AuthorizationQuiz {
				avatar: context.patient
			}
		}

		property Avatar supervisor: Avatar {
			Component.onCompleted: {
				pointEnd(getEnvironmentVariable("ACE_ENDPOINT", "http://127.0.0.1:8080"));
			}
		}
	}

	tokens: [
			Token {
				positions: [petrI.preAssertion]
			}
/*
			/// Еще какой-то токен для ближайшего будущего
			Token {
				property alias context: petrI.context
			}
*/
	]

	preAssertion: Assertion {
		id: initialAssertion
		description: "3 preassertion"

		actions: [printDebugInformation]

		Assert {
			description: "Check before printing debug information"

			targetAction: printDebugInformation

			onActionPrepared: {
				console.assert(!!currentToken);
				console.assert(!!currentToken.description);
				console.assert(!!currentToken.positions);

				console.assert(!!petrI.context.patient);
				console.assert(!!petrI.context.patient.realm);
				console.assert(!!petrI.context.patient.realm.objectName);
				console.assert(!!petrI.context.patient.account);
				console.assert(!!petrI.context.patient.code);
			}
		}
	}

	preActions: [
		Action {
			id: printDebugInformation
			description: "Print debug information"

			assertions: [printDebugInformationCheck]

			onActed: {
				console.log("currentToken.description =", currentToken.description);
				console.log("currentToken.positions =", currentToken.positions);
				console.log("currentToken.account =", currentToken.account);
				console.log("currentToken.code =", currentToken.code);

				console.log("currentToken.avatar.realm.objectName =", currentToken.avatar.realm.objectName);
			}
		}
	]

	Assertion {
		id: printDebugInformationCheck
		description: "Leaf assertion"

		actions: []

		/// @ocd
		Assert {
			description: "Check after printing debug information"

			targetAction: printDebugInformation

			onActionCompleted: {
				console.log("The port model is ready to go");
			}
		}
	}

	Port {
		source: "qrc:/Mentalgami/Test/Integration/AuthorizationQuizFlow.qml"
	}

	Port {
		source: "qrc:/Mentalgami/Test/Integration/WelcomeQuizFlow.qml"
	}

	Port {
		source: "qrc:/Repository/Test/Integration/DirectoryIndex.qml"
	}

	Port {
		source: "qrc:/Repository/Test/Integration/GroupChannelEchoLoop.qml"
	}
}
