import QtQml 2.15
import QtQuick 2.15

import Core.Test 0.1

import Repository.Model 0.1
import DrOblozhko.Model 0.1
import Mentalgami.Model 0.1

import "qrc:/Mentalgami/View/Application" as MentalgamiView

/**
 * @author Anna Edakina
 *
 * @brief Вечная симуляция всего
 */
PetrI {
	id: petrI

	property Component tokenComponent: Component {
		Token {
			id: token

			property string account	
			property string code

			property bool isRegistered

			property var session_id
			property var session_handle
			property var session_scheme

			property int quiz_id
			property int directory_id

			property Realm realm: Realm {
				synchronizationMode: Realm.Lazy
			}

			property Session session: Session {
				skeletonName: "eureka"
			}

			property MentalgamiView.PatientQuiz welcomeQuiz: MentalgamiView.PatientQuiz {

			}

			property Directory directory: Directory {

			}
		}
	}

	Component.onCompleted: {
		console.log(petrI + ".autoFire:", petrI.autoFire);
		if (!petrI.autoFire) {
			return ;
		}

		for (var p = 0; p < getEnvironmentVariable("PATIENT_COUNT", 1); p++) {
			petrI.tokens.push(
								petrI.tokenComponent.createObject(
																	petrI,
																	{
																		"description": "Patient #" + p,
																		"positions": [petrI.preAssertion],
																		"account": "patient+date" + p + "@cadabra.local",
																		"code": 9999
																	}
								)
			);
			/// @note Задаем endpoint WelcomeService(Будет рефакторинг) и Realm
			petrI.tokens[p].welcomeQuiz.application.setEndpoint(petrI.tokens[p].realm.aceEndpoint, getEnvironmentVariable("ACE_ENDPOINT", "http://127.0.0.1:8080"));
		}

		console.log(petrI.tokens.length + " patients were tokenized");
	}

	/// @brief Авторизация через регистрацию или восстановление доступа
	Port {
		source: "qrc:/Mentalgami/Test/Integration/UnifiedLogInCase.qml"
	}

	/// ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ-ВСЁ
}
