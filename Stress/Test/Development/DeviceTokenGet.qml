import QtQml 2.15
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import Core.Meta 0.1
import Core.View 0.1

import Conversation.View 0.1 as ConversationView

import "../../../Conversation/View/Message" as ConversationMessageView

Rectangle {
	id: rootView

	ListView {
		id: listView

		anchors.fill: parent

		model: ObjectModel {
			id: objectModel

			ItemDelegate {
				id: text1Delegate

				width: listView.width
				height: textDelegate1Column.height

				Column {
					id: textDelegate1Column

					width: parent.width

					Text {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Text1(System): Съешь же ещё этих мягких <u>французских булок</u>, да выпей чаю! <b>The quick brown</b> fox jumps <s>over</s> the lazy dog."

					}

					Text {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Text1(Guiness): Съешь же ещё этих мягких <u>французских булок</u>, да выпей чаю! <b>The quick brown</b> fox jumps <s>over</s> the lazy dog."
						font: Theme.body1Font
					}
				}
			}

			ItemDelegate {
				id: text2Delegate

				width: listView.width
				height: text2DelegateColumn.height

				Column {
					id: text2DelegateColumn

					width: parent.width

					Text {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Text2(System): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."

					}

					Text {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Text2(Guiness): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."
						font: Theme.body1Font
					}
				}
			}

			ItemDelegate {
				id: label1Delegate

				width: listView.width
				height: label1DelegateColumn.height

				Column {
					id: label1DelegateColumn

					width: parent.width

					Label {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Label1(System): Съешь же ещё этих мягких <u>французских булок</u>, да выпей чаю! <b>The quick brown</b> fox jumps <s>over</s> the lazy dog."

					}

					Label {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Label1(Guiness): Съешь же ещё этих мягких <u>французских булок</u>, да выпей чаю! <b>The quick brown</b> fox jumps <s>over</s> the lazy dog."
						font: Theme.body1Font
					}
				}
			}

			ItemDelegate {
				id: label2Delegate

				width: listView.width
				height: label2DelegateColumn.height

				Column {
					id: label2DelegateColumn

					width: parent.width

					Label {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Label2(System): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."

					}

					Label {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.Label2(Guiness): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."
						font: Theme.body1Font
					}
				}
			}

			ItemDelegate {
				id: label3Delegate

				width: listView.width
				height: label3DelegateColumn.height

				Column {
					id: label3DelegateColumn

					width: parent.width

					Label {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						font.bold: true
						font.italic: true
						text: "QtQuick.Label3(System): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."
					}

					Label {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						font: Theme.customizeFont(Theme.body1Font).setUnderline(true).setBold(true)
						text: "QtQuick.Label3(Guiness): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."
					}
				}
			}

			ItemDelegate {
				id: label4Delegate

				width: listView.width
				height: label4DelegateColumn.height

				Column {
					id: label4DelegateColumn

					width: parent.width

					Label {
						id: system4Label

						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "Здравствуйте! Меня зовут Ангелина, я бот-помощница Сергея Обложко. Необходимо провести небольшое анкетирование, перед тем как мы продолжим. Какой у вас сейчас вес?"
					}

					Label {
						id: guiness4Label

						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						font: Theme.body1Font
						text: "Здравствуйте! Меня зовут Ангелина, я бот-помощница Сергея Обложко. Необходимо провести небольшое анкетирование, перед тем как мы продолжим. Какой у вас сейчас вес?"
					}
				}

				onClicked: {
					console.log(system4Label.font);
					console.log(guiness4Label.font);
				}
			}

			ItemDelegate {
				id: textFieldDelegate

				width: listView.width
				height: textFieldColumn.height

				Column {
					id: textFieldColumn

					width: parent.width

					TextField {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.TextField(System): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."
					}

					TextField {
						width: parent.width
						height: contentHeight

						wrapMode: Text.WordWrap
						text: "QtQuick.TextField(Guiness): Съешь же ещё этих мягких французских булок, да выпей чаю! The quick brown fox jumps over the lazy dog."
						font: Theme.body1Font
					}
				}
			}

			ItemDelegate {
				id: textMessage1Delegate

				width: listView.width
				height: textMessage1Layout.viewMinimumHeight

				ConversationView.MessageLayout {
					id: textMessage1Layout

					width:listView.width
					viewMinimumWidth: parent.width * 0.6
					viewMinimumHeight: view ? view.contentHeight : 100

					side: ConversationView.MessageLayout.Left

					Component.onCompleted: {
						render(SchemeFactory.ident("Conversation.Meta.MessageRender", "Conversation", "TextMessage"), ConversationView.FeedLayout);
					}

					onRendered: {
						model.text = "Conversation.TextMessage1(Guiness): Съешь же ещё этих мягких <u>французских булок</u>, да выпей чаю! <b>The quick brown</b> fox jumps <s>over</s> the lazy dog.";
						model.imported();
					}
				}
			}

			ItemDelegate {
				id: textMessage2Delegate

				width: listView.width
				height: textMessage2Layout.viewMinimumHeight

				ConversationView.MessageLayout {
					id: textMessage2Layout

					width:listView.width
					viewMinimumWidth: parent.width * 0.6
					viewMinimumHeight: view ? view.contentHeight : 100

					side: ConversationView.MessageLayout.Left

					Component.onCompleted: {
						render(SchemeFactory.ident("Conversation.Meta.MessageRender", "Conversation", "TextMessage"), ConversationView.FeedLayout);
					}

					onRendered: {
						model.text = "Здравствуйте! Меня зовут Ангелина, я бот-помощница Сергея Обложко. Необходимо провести небольшое анкетирование, перед тем как мы продолжим. Какой у вас сейчас вес?";
						model.imported();
					}
				}
			}
		}
	}
}
