import QtQml 2.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3

import Core.Test 0.1

import Repository.Model 0.1
import DrOblozhko.Model 0.1
import Mentalgami.Model 0.1

import Stress.Test 0.1

import "qrc:/Conversation/View/Application" as ConversationApplication
import "qrc:/Mentalgami/View/Application" as MentalgamiApplication

/**
 * @details
 * 1) Авторизовать Queen на Ace-стороне
 * 2) Закрешить Ace с помощью шокера
 * 4) Дождаться сетевой доступности Ace (через запрос версии расширений доступных на Ace)
 * 5) Вызывать verify
 * 6) Отрубить сеть включив шокером
 *.7) Новый идентификатор сессии уходит с Ace в "никуда"
 * 8) Вернуть сеть выключив шокер
 * 9) Вызывать verify
 * 10) Подтвердить возникновение revalidated на Queen
 * 11) Всех убил адвокат
 */
PetrI {
	id: petrI
	objectName: "Ревалидация сессии и повторный апплет-сервлет аттачмент после сбоя интернет-соединения"

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
		actions: [printAction]

		Assert {
			description: "Check all essentials before start"

			targetAction: printAction

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
			id: printAction
			description: "Print debug information"

			assertions: [semanticAssertion]

			onActed: {
				console.log("currentToken.description =", currentToken.description);
				console.log("currentToken.positions =", currentToken.positions);
				console.log("petrI.context.patient.account =", petrI.context.patient.account);
				console.log("petrI.context.patient.code =", petrI.context.patient.code);
				console.log("petrI.context.patient.realm.objectName =", petrI.context.patient.realm.objectName);
			}
		}
	]

	Assertion {
		id: semanticAssertion

		actions: []

		/// @ocd Semantic sugar :( ?
		Assert {
			description: "Ready? Steady? Go?"

			targetAction: printAction

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
		source: "qrc:/Repository/Test/Integration/IndexAndAceCrashDown.qml"
	}

	Port {
		source: "qrc:/Repository/Test/Integration/EndpointOffline.qml"
	}
}