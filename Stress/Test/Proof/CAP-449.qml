import QtQml 2.15
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
	id: rootView
	objectName: "Emoji render defect"

	ListView {
		id: listView

		property int itemHeight: parent.height / 20
		property int itemWidth: parent.width

		anchors.fill: parent

		model: ListModel {
			ListElement {
				emoji: "😀"
				description: ""
			}
			ListElement {
				emoji: "😃"
				description: ""
			}
			ListElement {
				emoji: "😄"
				description: ""
			}
			ListElement {
				emoji: "😁"
				description: ""
			}
			ListElement {
				emoji: "😆"
				description: ""
			}
			ListElement {
				emoji: "😅"
				description: ""
			}
			ListElement {
				emoji: "🤣"
				description: ""
			}
			ListElement {
				emoji: "😂"
				description: ""
			}
			ListElement {
				emoji: "🙂"
				description: ""
			}
			ListElement {
				emoji: "🙃"
				description: ""
			}
			ListElement {
				emoji: "🫠"
				description: ""
			}
			ListElement {
				emoji: "😉"
				description: ""
			}
			ListElement {
				emoji: "😊"
				description: ""
			}
			ListElement {
				emoji: "😇"
				description: ""
			}
			ListElement {
				emoji: "🥰"
				description: ""
			}
			ListElement {
				emoji: "😍"
				description: ""
			}
			ListElement {
				emoji: "🤩"
				description: ""
			}
			ListElement {
				emoji: "😘"
				description: ""
			}
			ListElement {
				emoji: "😗"
				description: ""
			}
			ListElement {
				emoji: "😚"
				description: ""
			}
			ListElement {
				emoji: "😙"
				description: ""
			}
			ListElement {
				emoji: "🥲"
				description: ""
			}
			ListElement {
				emoji: "😋"
				description: ""
			}
			ListElement {
				emoji: "😛"
				description: ""
			}
			ListElement {
				emoji: "😜"
				description: ""
			}
			ListElement {
				emoji: "🤪"
				description: ""
			}
			ListElement {
				emoji: "😝"
				description: ""
			}
			ListElement {
				emoji: "🤑"
				description: ""
			}
			ListElement {
				emoji: "🤗"
				description: ""
			}
			ListElement {
				emoji: "🤭"
				description: ""
			}
			ListElement {
				emoji: "🫢"
				description: ""
			}
			ListElement {
				emoji: "🫣"
				description: ""
			}
			ListElement {
				emoji: "🤫"
				description: ""
			}
			ListElement {
				emoji: "🤔"
				description: ""
			}
			ListElement {
				emoji: "🫡"
				description: ""
			}
			ListElement {
				emoji: "🤐"
				description: ""
			}
			ListElement {
				emoji: "🤨"
				description: ""
			}
			ListElement {
				emoji: "😐"
				description: ""
			}
			ListElement {
				emoji: "😑"
				description: ""
			}
			ListElement {
				emoji: "😶"
				description: ""
			}
			ListElement {
				emoji: "🫥"
				description: ""
			}
			ListElement {
				emoji: "😶‍🌫️"
				description: ""
			}
			ListElement {
				emoji: "😏"
				description: ""
			}
			ListElement {
				emoji: "😒"
				description: ""
			}
			ListElement {
				emoji: "🙄"
				description: ""
			}
			ListElement {
				emoji: "😬"
				description: ""
			}
			ListElement {
				emoji: "😮‍💨"
				description: ""
			}
			ListElement {
				emoji: "🤥"
				description: ""
			}
			ListElement {
				emoji: "🫨"
				description: ""
			}
			ListElement {
				emoji: "😌"
				description: ""
			}
			ListElement {
				emoji: "😔"
				description: ""
			}
			ListElement {
				emoji: "😪"
				description: ""
			}
			ListElement {
				emoji: "🤤"
				description: ""
			}
			ListElement {
				emoji: "😴"
				description: ""
			}
			ListElement {
				emoji: "😷"
				description: ""
			}
			ListElement {
				emoji: "🤒"
				description: ""
			}
			ListElement {
				emoji: "🤕"
				description: ""
			}
			ListElement {
				emoji: "🤢"
				description: ""
			}
			ListElement {
				emoji: "🤮"
				description: ""
			}
			ListElement {
				emoji: "🤧"
				description: ""
			}
			ListElement {
				emoji: "🥵"
				description: ""
			}
			ListElement {
				emoji: "🥶"
				description: ""
			}
			ListElement {
				emoji: "🥴"
				description: ""
			}
			ListElement {
				emoji: "😵"
				description: ""
			}
			ListElement {
				emoji: "😵‍💫"
				description: ""
			}
			ListElement {
				emoji: "🤯"
				description: ""
			}
			ListElement {
				emoji: "🤠"
				description: ""
			}
			ListElement {
				emoji: "🥳"
				description: ""
			}
			ListElement {
				emoji: "🥸"
				description: ""
			}
			ListElement {
				emoji: "😎"
				description: ""
			}
			ListElement {
				emoji: "🤓"
				description: ""
			}
			ListElement {
				emoji: "🧐"
				description: ""
			}
			ListElement {
				emoji: "😕"
				description: ""
			}
			ListElement {
				emoji: "🫤"
				description: ""
			}
			ListElement {
				emoji: "😟"
				description: ""
			}
			ListElement {
				emoji: "🙁"
				description: ""
			}
			ListElement {
				emoji: "☹"
				description: ""
			}
			ListElement {
				emoji: "😮"
				description: ""
			}
			ListElement {
				emoji: "😯"
				description: ""
			}
			ListElement {
				emoji: "😲"
				description: ""
			}
			ListElement {
				emoji: "😳"
				description: ""
			}
			ListElement {
				emoji: "🥺"
				description: ""
			}
			ListElement {
				emoji: "🥹"
				description: ""
			}
			ListElement {
				emoji: "😦"
				description: ""
			}
			ListElement {
				emoji: "😧"
				description: ""
			}
			ListElement {
				emoji: "😨"
				description: ""
			}
			ListElement {
				emoji: "😰"
				description: ""
			}
			ListElement {
				emoji: "😥"
				description: ""
			}
			ListElement {
				emoji: "😢"
				description: ""
			}
			ListElement {
				emoji: "😭"
				description: ""
			}
			ListElement {
				emoji: "😱"
				description: ""
			}
			ListElement {
				emoji: "😖"
				description: ""
			}
			ListElement {
				emoji: "😣"
				description: ""
			}
			ListElement {
				emoji: "😞"
				description: ""
			}
			ListElement {
				emoji: "😓"
				description: ""
			}
			ListElement {
				emoji: "😩"
				description: ""
			}
			ListElement {
				emoji: "😫"
				description: ""
			}
			ListElement {
				emoji: "🥱"
				description: ""
			}
			ListElement {
				emoji: "😤"
				description: ""
			}
			ListElement {
				emoji: "😡"
				description: ""
			}
			ListElement {
				emoji: "😠"
				description: ""
			}
			ListElement {
				emoji: "🤬"
				description: ""
			}
			ListElement {
				emoji: "😈"
				description: ""
			}
			ListElement {
				emoji: "👿"
				description: ""
			}
			ListElement {
				emoji: "💀"
				description: ""
			}
			ListElement {
				emoji: "☠"
				description: ""
			}
			ListElement {
				emoji: "💩"
				description: ""
			}
			ListElement {
				emoji: "🤡"
				description: ""
			}
			ListElement {
				emoji: "👹"
				description: ""
			}
			ListElement {
				emoji: "👺"
				description: ""
			}
			ListElement {
				emoji: "👻"
				description: ""
			}
			ListElement {
				emoji: "👽"
				description: ""
			}
			ListElement {
				emoji: "👾"
				description: ""
			}
			ListElement {
				emoji: "🤖"
				description: ""
			}
			ListElement {
				emoji: "😺"
				description: ""
			}
			ListElement {
				emoji: "😸"
				description: ""
			}
			ListElement {
				emoji: "😹"
				description: ""
			}
			ListElement {
				emoji: "😻"
				description: ""
			}
			ListElement {
				emoji: "😼"
				description: ""
			}
			ListElement {
				emoji: "😽"
				description: ""
			}
			ListElement {
				emoji: "🙀"
				description: ""
			}
			ListElement {
				emoji: "😿"
				description: ""
			}
			ListElement {
				emoji: "😾"
				description: ""
			}
			ListElement {
				emoji: "🙈"
				description: ""
			}
			ListElement {
				emoji: "🙉"
				description: ""
			}
			ListElement {
				emoji: "🙊"
				description: ""
			}
			ListElement {
				emoji: "💌"
				description: ""
			}
			ListElement {
				emoji: "💘"
				description: ""
			}
			ListElement {
				emoji: "💝"
				description: ""
			}
			ListElement {
				emoji: "💖"
				description: ""
			}
			ListElement {
				emoji: "💗"
				description: ""
			}
			ListElement {
				emoji: "💓"
				description: ""
			}
			ListElement {
				emoji: "💞"
				description: ""
			}
			ListElement {
				emoji: "💕"
				description: ""
			}
			ListElement {
				emoji: "💟"
				description: ""
			}
			ListElement {
				emoji: "❣"
				description: ""
			}
			ListElement {
				emoji: "💔"
				description: ""
			}
			ListElement {
				emoji: "❤️‍🔥"
				description: ""
			}
			ListElement {
				emoji: "❤️‍🩹"
				description: ""
			}
			ListElement {
				emoji: "❤"
				description: ""
			}
			ListElement {
				emoji: "🩷"
				description: ""
			}
			ListElement {
				emoji: "🧡"
				description: ""
			}
			ListElement {
				emoji: "💛"
				description: ""
			}
			ListElement {
				emoji: "💚"
				description: ""
			}
			ListElement {
				emoji: "💙"
				description: ""
			}
			ListElement {
				emoji: "🩵"
				description: ""
			}
			ListElement {
				emoji: "💜"
				description: ""
			}
			ListElement {
				emoji: "🤎"
				description: ""
			}
			ListElement {
				emoji: "🖤"
				description: ""
			}
			ListElement {
				emoji: "🩶"
				description: ""
			}
			ListElement {
				emoji: "🤍"
				description: ""
			}
			ListElement {
				emoji: "💋"
				description: ""
			}
			ListElement {
				emoji: "💯"
				description: ""
			}
			ListElement {
				emoji: "💢"
				description: ""
			}
			ListElement {
				emoji: "💥"
				description: ""
			}
			ListElement {
				emoji: "💫"
				description: ""
			}
			ListElement {
				emoji: "💦"
				description: ""
			}
			ListElement {
				emoji: "💨"
				description: ""
			}
			ListElement {
				emoji: "🕳"
				description: ""
			}
			ListElement {
				emoji: "💬"
				description: ""
			}
			ListElement {
				emoji: "👁️‍🗨️"
				description: ""
			}
			ListElement {
				emoji: "🗨"
				description: ""
			}
			ListElement {
				emoji: "🗯"
				description: ""
			}
			ListElement {
				emoji: "💭"
				description: ""
			}
			ListElement {
				emoji: "💤"
				description: ""
			}
			ListElement {
				emoji: "👋"
				description: ""
			}
			ListElement {
				emoji: "🤚"
				description: ""
			}
			ListElement {
				emoji: "🖐"
				description: ""
			}
			ListElement {
				emoji: "✋"
				description: ""
			}
			ListElement {
				emoji: "🖖"
				description: ""
			}
			ListElement {
				emoji: "🫱"
				description: ""
			}
			ListElement {
				emoji: "🫲"
				description: ""
			}
			ListElement {
				emoji: "🫳"
				description: ""
			}
			ListElement {
				emoji: "🫴"
				description: ""
			}
			ListElement {
				emoji: "🫷"
				description: ""
			}
			ListElement {
				emoji: "🫸"
				description: ""
			}
			ListElement {
				emoji: "👌"
				description: ""
			}
			ListElement {
				emoji: "🤌"
				description: ""
			}
			ListElement {
				emoji: "🤏"
				description: ""
			}
			ListElement {
				emoji: "✌"
				description: ""
			}
			ListElement {
				emoji: "🤞"
				description: ""
			}
			ListElement {
				emoji: "🫰"
				description: ""
			}
			ListElement {
				emoji: "🤟"
				description: ""
			}
			ListElement {
				emoji: "🤘"
				description: ""
			}
			ListElement {
				emoji: "🤙"
				description: ""
			}
			ListElement {
				emoji: "👈"
				description: ""
			}
			ListElement {
				emoji: "👉"
				description: ""
			}
			ListElement {
				emoji: "👆"
				description: ""
			}
			ListElement {
				emoji: "🖕"
				description: ""
			}
			ListElement {
				emoji: "👇"
				description: ""
			}
			ListElement {
				emoji: "☝"
				description: ""
			}
			ListElement {
				emoji: "🫵"
				description: ""
			}
			ListElement {
				emoji: "👍"
				description: ""
			}
			ListElement {
				emoji: "👎"
				description: ""
			}
			ListElement {
				emoji: "✊"
				description: ""
			}
			ListElement {
				emoji: "👊"
				description: ""
			}
			ListElement {
				emoji: "🤛"
				description: ""
			}
			ListElement {
				emoji: "🤜"
				description: ""
			}
			ListElement {
				emoji: "👏"
				description: ""
			}
			ListElement {
				emoji: "🙌"
				description: ""
			}
			ListElement {
				emoji: "🫶"
				description: ""
			}
			ListElement {
				emoji: "👐"
				description: ""
			}
			ListElement {
				emoji: "🤲"
				description: ""
			}
			ListElement {
				emoji: "🤝"
				description: ""
			}
			ListElement {
				emoji: "🙏"
				description: ""
			}
			ListElement {
				emoji: "✍"
				description: ""
			}
			ListElement {
				emoji: "💅"
				description: ""
			}
			ListElement {
				emoji: "🤳"
				description: ""
			}
			ListElement {
				emoji: "💪"
				description: ""
			}
			ListElement {
				emoji: "🦾"
				description: ""
			}
			ListElement {
				emoji: "🦿"
				description: ""
			}
			ListElement {
				emoji: "🦵"
				description: ""
			}
			ListElement {
				emoji: "🦶"
				description: ""
			}
			ListElement {
				emoji: "👂"
				description: ""
			}
			ListElement {
				emoji: "🦻"
				description: ""
			}
			ListElement {
				emoji: "👃"
				description: ""
			}
			ListElement {
				emoji: "🧠"
				description: ""
			}
			ListElement {
				emoji: "🫀"
				description: ""
			}
			ListElement {
				emoji: "🫁"
				description: ""
			}
			ListElement {
				emoji: "🦷"
				description: ""
			}
			ListElement {
				emoji: "🦴"
				description: ""
			}
			ListElement {
				emoji: "👀"
				description: ""
			}
			ListElement {
				emoji: "👁"
				description: ""
			}
			ListElement {
				emoji: "👅"
				description: ""
			}
			ListElement {
				emoji: "👄"
				description: ""
			}
			ListElement {
				emoji: "🫦"
				description: ""
			}
			ListElement {
				emoji: "👶"
				description: ""
			}
			ListElement {
				emoji: "🧒"
				description: ""
			}
			ListElement {
				emoji: "👦"
				description: ""
			}
			ListElement {
				emoji: "👧"
				description: ""
			}
			ListElement {
				emoji: "🧑"
				description: ""
			}
			ListElement {
				emoji: "👱"
				description: ""
			}
			ListElement {
				emoji: "👨"
				description: ""
			}
			ListElement {
				emoji: "🧔"
				description: ""
			}
			ListElement {
				emoji: "🧔‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧔‍♀️"
				description: ""
			}
			ListElement {
				emoji: "👨‍🦰"
				description: ""
			}
			ListElement {
				emoji: "👨‍🦱"
				description: ""
			}
			ListElement {
				emoji: "👨‍🦳"
				description: ""
			}
			ListElement {
				emoji: "👨‍🦲"
				description: ""
			}
			ListElement {
				emoji: "👩"
				description: ""
			}
			ListElement {
				emoji: "👩‍🦰"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🦰"
				description: ""
			}
			ListElement {
				emoji: "👩‍🦱"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🦱"
				description: ""
			}
			ListElement {
				emoji: "👩‍🦳"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🦳"
				description: ""
			}
			ListElement {
				emoji: "👩‍🦲"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🦲"
				description: ""
			}
			ListElement {
				emoji: "👱‍♀️"
				description: ""
			}
			ListElement {
				emoji: "👱‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧓"
				description: ""
			}
			ListElement {
				emoji: "👴"
				description: ""
			}
			ListElement {
				emoji: "👵"
				description: ""
			}
			ListElement {
				emoji: "🙍"
				description: ""
			}
			ListElement {
				emoji: "🙍‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🙍‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🙎"
				description: ""
			}
			ListElement {
				emoji: "🙎‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🙎‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🙅"
				description: ""
			}
			ListElement {
				emoji: "🙅‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🙅‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🙆"
				description: ""
			}
			ListElement {
				emoji: "🙆‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🙆‍♀️"
				description: ""
			}
			ListElement {
				emoji: "💁"
				description: ""
			}
			ListElement {
				emoji: "💁‍♂️"
				description: ""
			}
			ListElement {
				emoji: "💁‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🙋"
				description: ""
			}
			ListElement {
				emoji: "🙋‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🙋‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧏"
				description: ""
			}
			ListElement {
				emoji: "🧏‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧏‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🙇"
				description: ""
			}
			ListElement {
				emoji: "🙇‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🙇‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤦"
				description: ""
			}
			ListElement {
				emoji: "🤦‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤦‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤷"
				description: ""
			}
			ListElement {
				emoji: "🤷‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤷‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧑‍⚕️"
				description: ""
			}
			ListElement {
				emoji: "👨‍⚕️"
				description: ""
			}
			ListElement {
				emoji: "👩‍⚕️"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🎓"
				description: ""
			}
			ListElement {
				emoji: "👨‍🎓"
				description: ""
			}
			ListElement {
				emoji: "👩‍🎓"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🏫"
				description: ""
			}
			ListElement {
				emoji: "👨‍🏫"
				description: ""
			}
			ListElement {
				emoji: "👩‍🏫"
				description: ""
			}
			ListElement {
				emoji: "🧑‍⚖️"
				description: ""
			}
			ListElement {
				emoji: "👨‍⚖️"
				description: ""
			}
			ListElement {
				emoji: "👩‍⚖️"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🌾"
				description: ""
			}
			ListElement {
				emoji: "👨‍🌾"
				description: ""
			}
			ListElement {
				emoji: "👩‍🌾"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🍳"
				description: ""
			}
			ListElement {
				emoji: "👨‍🍳"
				description: ""
			}
			ListElement {
				emoji: "👩‍🍳"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🔧"
				description: ""
			}
			ListElement {
				emoji: "👨‍🔧"
				description: ""
			}
			ListElement {
				emoji: "👩‍🔧"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🏭"
				description: ""
			}
			ListElement {
				emoji: "👨‍🏭"
				description: ""
			}
			ListElement {
				emoji: "👩‍🏭"
				description: ""
			}
			ListElement {
				emoji: "🧑‍💼"
				description: ""
			}
			ListElement {
				emoji: "👨‍💼"
				description: ""
			}
			ListElement {
				emoji: "👩‍💼"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🔬"
				description: ""
			}
			ListElement {
				emoji: "👨‍🔬"
				description: ""
			}
			ListElement {
				emoji: "👩‍🔬"
				description: ""
			}
			ListElement {
				emoji: "🧑‍💻"
				description: ""
			}
			ListElement {
				emoji: "👨‍💻"
				description: ""
			}
			ListElement {
				emoji: "👩‍💻"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🎤"
				description: ""
			}
			ListElement {
				emoji: "👨‍🎤"
				description: ""
			}
			ListElement {
				emoji: "👩‍🎤"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🎨"
				description: ""
			}
			ListElement {
				emoji: "👨‍🎨"
				description: ""
			}
			ListElement {
				emoji: "👩‍🎨"
				description: ""
			}
			ListElement {
				emoji: "🧑‍✈️"
				description: ""
			}
			ListElement {
				emoji: "👨‍✈️"
				description: ""
			}
			ListElement {
				emoji: "👩‍✈️"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🚀"
				description: ""
			}
			ListElement {
				emoji: "👨‍🚀"
				description: ""
			}
			ListElement {
				emoji: "👩‍🚀"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🚒"
				description: ""
			}
			ListElement {
				emoji: "👨‍🚒"
				description: ""
			}
			ListElement {
				emoji: "👩‍🚒"
				description: ""
			}
			ListElement {
				emoji: "👮"
				description: ""
			}
			ListElement {
				emoji: "👮‍♂️"
				description: ""
			}
			ListElement {
				emoji: "👮‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🕵"
				description: ""
			}
			ListElement {
				emoji: "🕵️‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🕵️‍♀️"
				description: ""
			}
			ListElement {
				emoji: "💂"
				description: ""
			}
			ListElement {
				emoji: "💂‍♂️"
				description: ""
			}
			ListElement {
				emoji: "💂‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🥷"
				description: ""
			}
			ListElement {
				emoji: "👷"
				description: ""
			}
			ListElement {
				emoji: "👷‍♂️"
				description: ""
			}
			ListElement {
				emoji: "👷‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🫅"
				description: ""
			}
			ListElement {
				emoji: "🤴"
				description: ""
			}
			ListElement {
				emoji: "👸"
				description: ""
			}
			ListElement {
				emoji: "👳"
				description: ""
			}
			ListElement {
				emoji: "👳‍♂️"
				description: ""
			}
			ListElement {
				emoji: "👳‍♀️"
				description: ""
			}
			ListElement {
				emoji: "👲"
				description: ""
			}
			ListElement {
				emoji: "🧕"
				description: ""
			}
			ListElement {
				emoji: "🤵"
				description: ""
			}
			ListElement {
				emoji: "🤵‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤵‍♀️"
				description: ""
			}
			ListElement {
				emoji: "👰"
				description: ""
			}
			ListElement {
				emoji: "👰‍♂️"
				description: ""
			}
			ListElement {
				emoji: "👰‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤰"
				description: ""
			}
			ListElement {
				emoji: "🫃"
				description: ""
			}
			ListElement {
				emoji: "🫄"
				description: ""
			}
			ListElement {
				emoji: "🤱"
				description: ""
			}
			ListElement {
				emoji: "👩‍🍼"
				description: ""
			}
			ListElement {
				emoji: "👨‍🍼"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🍼"
				description: ""
			}
			ListElement {
				emoji: "👼"
				description: ""
			}
			ListElement {
				emoji: "🎅"
				description: ""
			}
			ListElement {
				emoji: "🤶"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🎄"
				description: ""
			}
			ListElement {
				emoji: "🦸"
				description: ""
			}
			ListElement {
				emoji: "🦸‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🦸‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🦹"
				description: ""
			}
			ListElement {
				emoji: "🦹‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🦹‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧙"
				description: ""
			}
			ListElement {
				emoji: "🧙‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧙‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧚"
				description: ""
			}
			ListElement {
				emoji: "🧚‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧚‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧛"
				description: ""
			}
			ListElement {
				emoji: "🧛‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧛‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧜"
				description: ""
			}
			ListElement {
				emoji: "🧜‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧝"
				description: ""
			}
			ListElement {
				emoji: "🧝‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧝‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧞"
				description: ""
			}
			ListElement {
				emoji: "🧞‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧞‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧟"
				description: ""
			}
			ListElement {
				emoji: "🧟‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧟‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧌"
				description: ""
			}
			ListElement {
				emoji: "💆"
				description: ""
			}
			ListElement {
				emoji: "💆‍♂️"
				description: ""
			}
			ListElement {
				emoji: "💆‍♀️"
				description: ""
			}
			ListElement {
				emoji: "💇"
				description: ""
			}
			ListElement {
				emoji: "💇‍♂️"
				description: ""
			}
			ListElement {
				emoji: "💇‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🚶"
				description: ""
			}
			ListElement {
				emoji: "🚶‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🚶‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧍"
				description: ""
			}
			ListElement {
				emoji: "🧍‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧍‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧎"
				description: ""
			}
			ListElement {
				emoji: "🧎‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧎‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🦯"
				description: ""
			}
			ListElement {
				emoji: "👨‍🦯"
				description: ""
			}
			ListElement {
				emoji: "👩‍🦯"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🦼"
				description: ""
			}
			ListElement {
				emoji: "👨‍🦼"
				description: ""
			}
			ListElement {
				emoji: "👩‍🦼"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🦽"
				description: ""
			}
			ListElement {
				emoji: "👨‍🦽"
				description: ""
			}
			ListElement {
				emoji: "👩‍🦽"
				description: ""
			}
			ListElement {
				emoji: "🏃"
				description: ""
			}
			ListElement {
				emoji: "🏃‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🏃‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🏃‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🕺"
				description: ""
			}
			ListElement {
				emoji: "🕴"
				description: ""
			}
			ListElement {
				emoji: "👯"
				description: ""
			}
			ListElement {
				emoji: "👯‍♂️"
				description: ""
			}
			ListElement {
				emoji: "👯‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧖"
				description: ""
			}
			ListElement {
				emoji: "🧖‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧖‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧗"
				description: ""
			}
			ListElement {
				emoji: "🧗‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧗‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤺"
				description: ""
			}
			ListElement {
				emoji: "🏇"
				description: ""
			}
			ListElement {
				emoji: "⛷"
				description: ""
			}
			ListElement {
				emoji: "🏂"
				description: ""
			}
			ListElement {
				emoji: "🏌"
				description: ""
			}
			ListElement {
				emoji: "🏌️‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🏌️‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🏄"
				description: ""
			}
			ListElement {
				emoji: "🏄‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🏄‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🚣"
				description: ""
			}
			ListElement {
				emoji: "🚣‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🚣‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🏊"
				description: ""
			}
			ListElement {
				emoji: "🏊‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🏊‍♀️"
				description: ""
			}
			ListElement {
				emoji: "⛹"
				description: ""
			}
			ListElement {
				emoji: "⛹️‍♂️"
				description: ""
			}
			ListElement {
				emoji: "⛹️‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🏋"
				description: ""
			}
			ListElement {
				emoji: "🏋️‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🏋️‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🚴"
				description: ""
			}
			ListElement {
				emoji: "🚴‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🚴‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🚵"
				description: ""
			}
			ListElement {
				emoji: "🚵‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🚵‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤸"
				description: ""
			}
			ListElement {
				emoji: "🤸‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤸‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤼"
				description: ""
			}
			ListElement {
				emoji: "🤼‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤼‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤽"
				description: ""
			}
			ListElement {
				emoji: "🤽‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤽‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤾"
				description: ""
			}
			ListElement {
				emoji: "🤾‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤾‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🤹"
				description: ""
			}
			ListElement {
				emoji: "🤹‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🤹‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🧘"
				description: ""
			}
			ListElement {
				emoji: "🧘‍♂️"
				description: ""
			}
			ListElement {
				emoji: "🧘‍♀️"
				description: ""
			}
			ListElement {
				emoji: "🛀"
				description: ""
			}
			ListElement {
				emoji: "🛌"
				description: ""
			}
			ListElement {
				emoji: "🧑‍🤝‍🧑"
				description: ""
			}
			ListElement {
				emoji: "👭"
				description: ""
			}
			ListElement {
				emoji: "👫"
				description: ""
			}
			ListElement {
				emoji: "👬"
				description: ""
			}
			ListElement {
				emoji: "💏"
				description: ""
			}
			ListElement {
				emoji: "👩‍❤️‍💋‍👨"
				description: ""
			}
			ListElement {
				emoji: "👨‍❤️‍💋‍👨"
				description: ""
			}
			ListElement {
				emoji: "👩‍❤️‍💋‍👩"
				description: ""
			}
			ListElement {
				emoji: "💑"
				description: ""
			}
			ListElement {
				emoji: "👩‍❤️‍👨"
				description: ""
			}
			ListElement {
				emoji: "👨‍❤️‍👨"
				description: ""
			}
			ListElement {
				emoji: "👩‍❤️‍👩"
				description: ""
			}
			ListElement {
				emoji: "👨‍👩‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👩‍👧"
				description: ""
			}
			ListElement {
				emoji: "👨‍👩‍👧‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👩‍👦‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👩‍👧‍👧"
				description: ""
			}
			ListElement {
				emoji: "👨‍👨‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👨‍👧"
				description: ""
			}
			ListElement {
				emoji: "👨‍👨‍👧‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👨‍👦‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👨‍👧‍👧"
				description: ""
			}
			ListElement {
				emoji: "👩‍👩‍👦"
				description: ""
			}
			ListElement {
				emoji: "👩‍👩‍👧"
				description: ""
			}
			ListElement {
				emoji: "👩‍👩‍👧‍👦"
				description: ""
			}
			ListElement {
				emoji: "👩‍👩‍👦‍👦"
				description: ""
			}
			ListElement {
				emoji: "👩‍👩‍👧‍👧"
				description: ""
			}
			ListElement {
				emoji: "👨‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👦‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👧"
				description: ""
			}
			ListElement {
				emoji: "👨‍👧‍👦"
				description: ""
			}
			ListElement {
				emoji: "👨‍👧‍👧"
				description: ""
			}
			ListElement {
				emoji: "👩‍👦"
				description: ""
			}
			ListElement {
				emoji: "👩‍👦‍👦"
				description: ""
			}
			ListElement {
				emoji: "👩‍👧"
				description: ""
			}
			ListElement {
				emoji: "👩‍👧‍👦"
				description: ""
			}
			ListElement {
				emoji: "👩‍👧‍👧"
				description: ""
			}
			ListElement {
				emoji: "🗣"
				description: ""
			}
			ListElement {
				emoji: "👤"
				description: ""
			}
			ListElement {
				emoji: "👥"
				description: ""
			}
			ListElement {
				emoji: "🫂"
				description: ""
			}
			ListElement {
				emoji: "👪"
				description: ""
			}
			ListElement {
				emoji: "👣"
				description: ""
			}
			ListElement {
				emoji: "🦰"
				description: ""
			}
			ListElement {
				emoji: "🦱"
				description: ""
			}
			ListElement {
				emoji: "🦳"
				description: ""
			}
			ListElement {
				emoji: "🦲"
				description: ""
			}
			ListElement {
				emoji: "🐵"
				description: ""
			}
			ListElement {
				emoji: "🐒"
				description: ""
			}
			ListElement {
				emoji: "🦍"
				description: ""
			}
			ListElement {
				emoji: "🦧"
				description: ""
			}
			ListElement {
				emoji: "🐶"
				description: ""
			}
			ListElement {
				emoji: "🐕"
				description: ""
			}
			ListElement {
				emoji: "🦮"
				description: ""
			}
			ListElement {
				emoji: "🐕‍🦺"
				description: ""
			}
			ListElement {
				emoji: "🐩"
				description: ""
			}
			ListElement {
				emoji: "🐺"
				description: ""
			}
			ListElement {
				emoji: "🦊"
				description: ""
			}
			ListElement {
				emoji: "🦝"
				description: ""
			}
			ListElement {
				emoji: "🐱"
				description: ""
			}
			ListElement {
				emoji: "🐈"
				description: ""
			}
			ListElement {
				emoji: "🐈‍⬛"
				description: ""
			}
			ListElement {
				emoji: "🦁"
				description: ""
			}
			ListElement {
				emoji: "🐯"
				description: ""
			}
			ListElement {
				emoji: "🐅"
				description: ""
			}
			ListElement {
				emoji: "🐆"
				description: ""
			}
			ListElement {
				emoji: "🐴"
				description: ""
			}
			ListElement {
				emoji: "🫎"
				description: ""
			}
			ListElement {
				emoji: "🫏"
				description: ""
			}
			ListElement {
				emoji: "🐎"
				description: ""
			}
			ListElement {
				emoji: "🦄"
				description: ""
			}
			ListElement {
				emoji: "🦓"
				description: ""
			}
			ListElement {
				emoji: "🦌"
				description: ""
			}
			ListElement {
				emoji: "🦬"
				description: ""
			}
			ListElement {
				emoji: "🐮"
				description: ""
			}
			ListElement {
				emoji: "🐂"
				description: ""
			}
			ListElement {
				emoji: "🐃"
				description: ""
			}
			ListElement {
				emoji: "🐄"
				description: ""
			}
			ListElement {
				emoji: "🐷"
				description: ""
			}
			ListElement {
				emoji: "🐖"
				description: ""
			}
			ListElement {
				emoji: "🐗"
				description: ""
			}
			ListElement {
				emoji: "🐽"
				description: ""
			}
			ListElement {
				emoji: "🐏"
				description: ""
			}
			ListElement {
				emoji: "🐑"
				description: ""
			}
			ListElement {
				emoji: "🐐"
				description: ""
			}
			ListElement {
				emoji: "🐪"
				description: ""
			}
			ListElement {
				emoji: "🐫"
				description: ""
			}
			ListElement {
				emoji: "🦙"
				description: ""
			}
			ListElement {
				emoji: "🦒"
				description: ""
			}
			ListElement {
				emoji: "🐘"
				description: ""
			}
			ListElement {
				emoji: "🦣"
				description: ""
			}
			ListElement {
				emoji: "🦏"
				description: ""
			}
			ListElement {
				emoji: "🦛"
				description: ""
			}
			ListElement {
				emoji: "🐭"
				description: ""
			}
			ListElement {
				emoji: "🐁"
				description: ""
			}
			ListElement {
				emoji: "🐀"
				description: ""
			}
			ListElement {
				emoji: "🐹"
				description: ""
			}
			ListElement {
				emoji: "🐰"
				description: ""
			}
			ListElement {
				emoji: "🐇"
				description: ""
			}
			ListElement {
				emoji: "🐿"
				description: ""
			}
			ListElement {
				emoji: "🦫"
				description: ""
			}
			ListElement {
				emoji: "🦔"
				description: ""
			}
			ListElement {
				emoji: "🦇"
				description: ""
			}
			ListElement {
				emoji: "🐻"
				description: ""
			}
			ListElement {
				emoji: "🐻‍❄️"
				description: ""
			}
			ListElement {
				emoji: "🐨"
				description: ""
			}
			ListElement {
				emoji: "🐼"
				description: ""
			}
			ListElement {
				emoji: "🦥"
				description: ""
			}
			ListElement {
				emoji: "🦦"
				description: ""
			}
			ListElement {
				emoji: "🦨"
				description: ""
			}
			ListElement {
				emoji: "🦘"
				description: ""
			}
			ListElement {
				emoji: "🦡"
				description: ""
			}
			ListElement {
				emoji: "🐾"
				description: ""
			}
			ListElement {
				emoji: "🦃"
				description: ""
			}
			ListElement {
				emoji: "🐔"
				description: ""
			}
			ListElement {
				emoji: "🐓"
				description: ""
			}
			ListElement {
				emoji: "🐣"
				description: ""
			}
			ListElement {
				emoji: "🐤"
				description: ""
			}
			ListElement {
				emoji: "🐥"
				description: ""
			}
			ListElement {
				emoji: "🐦"
				description: ""
			}
			ListElement {
				emoji: "🐧"
				description: ""
			}
			ListElement {
				emoji: "🕊"
				description: ""
			}
			ListElement {
				emoji: "🦅"
				description: ""
			}
			ListElement {
				emoji: "🦆"
				description: ""
			}
			ListElement {
				emoji: "🦢"
				description: ""
			}
			ListElement {
				emoji: "🦉"
				description: ""
			}
			ListElement {
				emoji: "🦤"
				description: ""
			}
			ListElement {
				emoji: "🪶"
				description: ""
			}
			ListElement {
				emoji: "🦩"
				description: ""
			}
			ListElement {
				emoji: "🦚"
				description: ""
			}
			ListElement {
				emoji: "🦜"
				description: ""
			}
			ListElement {
				emoji: "🪽"
				description: ""
			}
			ListElement {
				emoji: "🐦‍⬛"
				description: ""
			}
			ListElement {
				emoji: "🪿"
				description: ""
			}
			ListElement {
				emoji: "🐸"
				description: ""
			}
			ListElement {
				emoji: "🐊"
				description: ""
			}
			ListElement {
				emoji: "🐢"
				description: ""
			}
			ListElement {
				emoji: "🦎"
				description: ""
			}
			ListElement {
				emoji: "🐍"
				description: ""
			}
			ListElement {
				emoji: "🐲"
				description: ""
			}
			ListElement {
				emoji: "🐉"
				description: ""
			}
			ListElement {
				emoji: "🦕"
				description: ""
			}
			ListElement {
				emoji: "🦖"
				description: ""
			}
			ListElement {
				emoji: "🐳"
				description: ""
			}
			ListElement {
				emoji: "🐋"
				description: ""
			}
			ListElement {
				emoji: "🐬"
				description: ""
			}
			ListElement {
				emoji: "🦭"
				description: ""
			}
			ListElement {
				emoji: "🐟"
				description: ""
			}
			ListElement {
				emoji: "🐠"
				description: ""
			}
			ListElement {
				emoji: "🐡"
				description: ""
			}
			ListElement {
				emoji: "🦈"
				description: ""
			}
			ListElement {
				emoji: "🐙"
				description: ""
			}
			ListElement {
				emoji: "🐚"
				description: ""
			}
			ListElement {
				emoji: "🪸"
				description: ""
			}
			ListElement {
				emoji: "🪼"
				description: ""
			}
			ListElement {
				emoji: "🐌"
				description: ""
			}
			ListElement {
				emoji: "🦋"
				description: ""
			}
			ListElement {
				emoji: "🐛"
				description: ""
			}
			ListElement {
				emoji: "🐜"
				description: ""
			}
			ListElement {
				emoji: "🐝"
				description: ""
			}
			ListElement {
				emoji: "🪲"
				description: ""
			}
			ListElement {
				emoji: "🐞"
				description: ""
			}
			ListElement {
				emoji: "🦗"
				description: ""
			}
			ListElement {
				emoji: "🪳"
				description: ""
			}
			ListElement {
				emoji: "🕷"
				description: ""
			}
			ListElement {
				emoji: "🕸"
				description: ""
			}
			ListElement {
				emoji: "🦂"
				description: ""
			}
			ListElement {
				emoji: "🦟"
				description: ""
			}
			ListElement {
				emoji: "🪰"
				description: ""
			}
			ListElement {
				emoji: "🪱"
				description: ""
			}
			ListElement {
				emoji: "🦠"
				description: ""
			}
			ListElement {
				emoji: "💐"
				description: ""
			}
			ListElement {
				emoji: "🌸"
				description: ""
			}
			ListElement {
				emoji: "💮"
				description: ""
			}
			ListElement {
				emoji: "🪷"
				description: ""
			}
			ListElement {
				emoji: "🏵"
				description: ""
			}
			ListElement {
				emoji: "🌹"
				description: ""
			}
			ListElement {
				emoji: "🥀"
				description: ""
			}
			ListElement {
				emoji: "🌺"
				description: ""
			}
			ListElement {
				emoji: "🌻"
				description: ""
			}
			ListElement {
				emoji: "🌼"
				description: ""
			}
			ListElement {
				emoji: "🌷"
				description: ""
			}
			ListElement {
				emoji: "🪻"
				description: ""
			}
			ListElement {
				emoji: "🌱"
				description: ""
			}
			ListElement {
				emoji: "🪴"
				description: ""
			}
			ListElement {
				emoji: "🌲"
				description: ""
			}
			ListElement {
				emoji: "🌳"
				description: ""
			}
			ListElement {
				emoji: "🌴"
				description: ""
			}
			ListElement {
				emoji: "🌵"
				description: ""
			}
			ListElement {
				emoji: "🌾"
				description: ""
			}
			ListElement {
				emoji: "🌿"
				description: ""
			}
			ListElement {
				emoji: "☘"
				description: ""
			}
			ListElement {
				emoji: "🍀"
				description: ""
			}
			ListElement {
				emoji: "🍁"
				description: ""
			}
			ListElement {
				emoji: "🍂"
				description: ""
			}
			ListElement {
				emoji: "🍃"
				description: ""
			}
			ListElement {
				emoji: "🪹"
				description: ""
			}
			ListElement {
				emoji: "🪺"
				description: ""
			}
			ListElement {
				emoji: "🍄"
				description: ""
			}
			ListElement {
				emoji: "🍇"
				description: ""
			}
			ListElement {
				emoji: "🍈"
				description: ""
			}
			ListElement {
				emoji: "🍉"
				description: ""
			}
			ListElement {
				emoji: "🍊"
				description: ""
			}
			ListElement {
				emoji: "🍋"
				description: ""
			}
			ListElement {
				emoji: "🍌"
				description: ""
			}
			ListElement {
				emoji: "🍍"
				description: ""
			}
			ListElement {
				emoji: "🥭"
				description: ""
			}
			ListElement {
				emoji: "🍎"
				description: ""
			}
			ListElement {
				emoji: "🍏"
				description: ""
			}
			ListElement {
				emoji: "🍐"
				description: ""
			}
			ListElement {
				emoji: "🍑"
				description: ""
			}
			ListElement {
				emoji: "🍒"
				description: ""
			}
			ListElement {
				emoji: "🍓"
				description: ""
			}
			ListElement {
				emoji: "🫐"
				description: ""
			}
			ListElement {
				emoji: "🥝"
				description: ""
			}
			ListElement {
				emoji: "🍅"
				description: ""
			}
			ListElement {
				emoji: "🫒"
				description: ""
			}
			ListElement {
				emoji: "🥥"
				description: ""
			}
			ListElement {
				emoji: "🥑"
				description: ""
			}
			ListElement {
				emoji: "🍆"
				description: ""
			}
			ListElement {
				emoji: "🥔"
				description: ""
			}
			ListElement {
				emoji: "🥕"
				description: ""
			}
			ListElement {
				emoji: "🌽"
				description: ""
			}
			ListElement {
				emoji: "🌶"
				description: ""
			}
			ListElement {
				emoji: "🫑"
				description: ""
			}
			ListElement {
				emoji: "🥒"
				description: ""
			}
			ListElement {
				emoji: "🥬"
				description: ""
			}
			ListElement {
				emoji: "🥦"
				description: ""
			}
			ListElement {
				emoji: "🧄"
				description: ""
			}
			ListElement {
				emoji: "🧅"
				description: ""
			}
			ListElement {
				emoji: "🥜"
				description: ""
			}
			ListElement {
				emoji: "🫘"
				description: ""
			}
			ListElement {
				emoji: "🌰"
				description: ""
			}
			ListElement {
				emoji: "🫚"
				description: ""
			}
			ListElement {
				emoji: "🫛"
				description: ""
			}
			ListElement {
				emoji: "🍞"
				description: ""
			}
			ListElement {
				emoji: "🥐"
				description: ""
			}
			ListElement {
				emoji: "🥖"
				description: ""
			}
			ListElement {
				emoji: "🫓"
				description: ""
			}
			ListElement {
				emoji: "🥨"
				description: ""
			}
			ListElement {
				emoji: "🥯"
				description: ""
			}
			ListElement {
				emoji: "🥞"
				description: ""
			}
			ListElement {
				emoji: "🧇"
				description: ""
			}
			ListElement {
				emoji: "🧀"
				description: ""
			}
			ListElement {
				emoji: "🍖"
				description: ""
			}
			ListElement {
				emoji: "🍗"
				description: ""
			}
			ListElement {
				emoji: "🥩"
				description: ""
			}
			ListElement {
				emoji: "🥓"
				description: ""
			}
			ListElement {
				emoji: "🍔"
				description: ""
			}
			ListElement {
				emoji: "🍟"
				description: ""
			}
			ListElement {
				emoji: "🍕"
				description: ""
			}
			ListElement {
				emoji: "🌭"
				description: ""
			}
			ListElement {
				emoji: "🥪"
				description: ""
			}
			ListElement {
				emoji: "🌮"
				description: ""
			}
			ListElement {
				emoji: "🌯"
				description: ""
			}
			ListElement {
				emoji: "🫔"
				description: ""
			}
			ListElement {
				emoji: "🥙"
				description: ""
			}
			ListElement {
				emoji: "🧆"
				description: ""
			}
			ListElement {
				emoji: "🥚"
				description: ""
			}
			ListElement {
				emoji: "🍳"
				description: ""
			}
			ListElement {
				emoji: "🥘"
				description: ""
			}
			ListElement {
				emoji: "🍲"
				description: ""
			}
			ListElement {
				emoji: "🫕"
				description: ""
			}
			ListElement {
				emoji: "🥣"
				description: ""
			}
			ListElement {
				emoji: "🥗"
				description: ""
			}
			ListElement {
				emoji: "🍿"
				description: ""
			}
			ListElement {
				emoji: "🧈"
				description: ""
			}
			ListElement {
				emoji: "🧂"
				description: ""
			}
			ListElement {
				emoji: "🥫"
				description: ""
			}
			ListElement {
				emoji: "🍱"
				description: ""
			}
			ListElement {
				emoji: "🍘"
				description: ""
			}
			ListElement {
				emoji: "🍙"
				description: ""
			}
			ListElement {
				emoji: "🍚"
				description: ""
			}
			ListElement {
				emoji: "🍛"
				description: ""
			}
			ListElement {
				emoji: "🍜"
				description: ""
			}
			ListElement {
				emoji: "🍝"
				description: ""
			}
			ListElement {
				emoji: "🍠"
				description: ""
			}
			ListElement {
				emoji: "🍢"
				description: ""
			}
			ListElement {
				emoji: "🍣"
				description: ""
			}
			ListElement {
				emoji: "🍤"
				description: ""
			}
			ListElement {
				emoji: "🍥"
				description: ""
			}
			ListElement {
				emoji: "🥮"
				description: ""
			}
			ListElement {
				emoji: "🍡"
				description: ""
			}
			ListElement {
				emoji: "🥟"
				description: ""
			}
			ListElement {
				emoji: "🥠"
				description: ""
			}
			ListElement {
				emoji: "🥡"
				description: ""
			}
			ListElement {
				emoji: "🦀"
				description: ""
			}
			ListElement {
				emoji: "🦞"
				description: ""
			}
			ListElement {
				emoji: "🦐"
				description: ""
			}
			ListElement {
				emoji: "🦑"
				description: ""
			}
			ListElement {
				emoji: "🦪"
				description: ""
			}
			ListElement {
				emoji: "🍦"
				description: ""
			}
			ListElement {
				emoji: "🍧"
				description: ""
			}
			ListElement {
				emoji: "🍨"
				description: ""
			}
			ListElement {
				emoji: "🍩"
				description: ""
			}
			ListElement {
				emoji: "🍪"
				description: ""
			}
			ListElement {
				emoji: "🎂"
				description: ""
			}
			ListElement {
				emoji: "🍰"
				description: ""
			}
			ListElement {
				emoji: "🧁"
				description: ""
			}
			ListElement {
				emoji: "🥧"
				description: ""
			}
			ListElement {
				emoji: "🍫"
				description: ""
			}
			ListElement {
				emoji: "🍬"
				description: ""
			}
			ListElement {
				emoji: "🍭"
				description: ""
			}
			ListElement {
				emoji: "🍮"
				description: ""
			}
			ListElement {
				emoji: "🍯"
				description: ""
			}
			ListElement {
				emoji: "🍼"
				description: ""
			}
			ListElement {
				emoji: "🥛"
				description: ""
			}
			ListElement {
				emoji: "☕"
				description: ""
			}
			ListElement {
				emoji: "🫖"
				description: ""
			}
			ListElement {
				emoji: "🍵"
				description: ""
			}
			ListElement {
				emoji: "🍶"
				description: ""
			}
			ListElement {
				emoji: "🍾"
				description: ""
			}
			ListElement {
				emoji: "🍷"
				description: ""
			}
			ListElement {
				emoji: "🍸"
				description: ""
			}
			ListElement {
				emoji: "🍹"
				description: ""
			}
			ListElement {
				emoji: "🍺"
				description: ""
			}
			ListElement {
				emoji: "🍻"
				description: ""
			}
			ListElement {
				emoji: "🥂"
				description: ""
			}
			ListElement {
				emoji: "🥃"
				description: ""
			}
			ListElement {
				emoji: "🫗"
				description: ""
			}
			ListElement {
				emoji: "🥤"
				description: ""
			}
			ListElement {
				emoji: "🧋"
				description: ""
			}
			ListElement {
				emoji: "🧃"
				description: ""
			}
			ListElement {
				emoji: "🧉"
				description: ""
			}
			ListElement {
				emoji: "🧊"
				description: ""
			}
			ListElement {
				emoji: "🥢"
				description: ""
			}
			ListElement {
				emoji: "🍽"
				description: ""
			}
			ListElement {
				emoji: "🍴"
				description: ""
			}
			ListElement {
				emoji: "🥄"
				description: ""
			}
			ListElement {
				emoji: "🔪"
				description: ""
			}
			ListElement {
				emoji: "🫙"
				description: ""
			}
			ListElement {
				emoji: "🏺"
				description: ""
			}
			ListElement {
				emoji: "🌍"
				description: ""
			}
			ListElement {
				emoji: "🌎"
				description: ""
			}
			ListElement {
				emoji: "🌏"
				description: ""
			}
			ListElement {
				emoji: "🌐"
				description: ""
			}
			ListElement {
				emoji: "🗺"
				description: ""
			}
			ListElement {
				emoji: "🗾"
				description: ""
			}
			ListElement {
				emoji: "🧭"
				description: ""
			}
			ListElement {
				emoji: "🏔"
				description: ""
			}
			ListElement {
				emoji: "⛰"
				description: ""
			}
			ListElement {
				emoji: "🌋"
				description: ""
			}
			ListElement {
				emoji: "🗻"
				description: ""
			}
			ListElement {
				emoji: "🏕"
				description: ""
			}
			ListElement {
				emoji: "🏖"
				description: ""
			}
			ListElement {
				emoji: "🏜"
				description: ""
			}
			ListElement {
				emoji: "🏝"
				description: ""
			}
			ListElement {
				emoji: "🏞"
				description: ""
			}
			ListElement {
				emoji: "🏟"
				description: ""
			}
			ListElement {
				emoji: "🏛"
				description: ""
			}
			ListElement {
				emoji: "🏗"
				description: ""
			}
			ListElement {
				emoji: "🧱"
				description: ""
			}
			ListElement {
				emoji: "🪨"
				description: ""
			}
			ListElement {
				emoji: "🪵"
				description: ""
			}
			ListElement {
				emoji: "🛖"
				description: ""
			}
			ListElement {
				emoji: "🏘"
				description: ""
			}
			ListElement {
				emoji: "🏚"
				description: ""
			}
			ListElement {
				emoji: "🏠"
				description: ""
			}
			ListElement {
				emoji: "🏡"
				description: ""
			}
			ListElement {
				emoji: "🏢"
				description: ""
			}
			ListElement {
				emoji: "🏣"
				description: ""
			}
			ListElement {
				emoji: "🏤"
				description: ""
			}
			ListElement {
				emoji: "🏥"
				description: ""
			}
			ListElement {
				emoji: "🏦"
				description: ""
			}
			ListElement {
				emoji: "🏨"
				description: ""
			}
			ListElement {
				emoji: "🏩"
				description: ""
			}
			ListElement {
				emoji: "🏪"
				description: ""
			}
			ListElement {
				emoji: "🏫"
				description: ""
			}
			ListElement {
				emoji: "🏬"
				description: ""
			}
			ListElement {
				emoji: "🏭"
				description: ""
			}
			ListElement {
				emoji: "🏯"
				description: ""
			}
			ListElement {
				emoji: "🏰"
				description: ""
			}
			ListElement {
				emoji: "💒"
				description: ""
			}
			ListElement {
				emoji: "🗼"
				description: ""
			}
			ListElement {
				emoji: "🗽"
				description: ""
			}
			ListElement {
				emoji: "⛪"
				description: ""
			}
			ListElement {
				emoji: "🕌"
				description: ""
			}
			ListElement {
				emoji: "🛕"
				description: ""
			}
			ListElement {
				emoji: "🕍"
				description: ""
			}
			ListElement {
				emoji: "⛩"
				description: ""
			}
			ListElement {
				emoji: "🕋"
				description: ""
			}
			ListElement {
				emoji: "⛲"
				description: ""
			}
			ListElement {
				emoji: "⛺"
				description: ""
			}
			ListElement {
				emoji: "🌁"
				description: ""
			}
			ListElement {
				emoji: "🌃"
				description: ""
			}
			ListElement {
				emoji: "🏙"
				description: ""
			}
			ListElement {
				emoji: "🌄"
				description: ""
			}
			ListElement {
				emoji: "🌅"
				description: ""
			}
			ListElement {
				emoji: "🌆"
				description: ""
			}
			ListElement {
				emoji: "🌇"
				description: ""
			}
			ListElement {
				emoji: "🌉"
				description: ""
			}
			ListElement {
				emoji: "♨"
				description: ""
			}
			ListElement {
				emoji: "🎠"
				description: ""
			}
			ListElement {
				emoji: "🛝"
				description: ""
			}
			ListElement {
				emoji: "🎡"
				description: ""
			}
			ListElement {
				emoji: "🎢"
				description: ""
			}
			ListElement {
				emoji: "💈"
				description: ""
			}
			ListElement {
				emoji: "🎪"
				description: ""
			}
			ListElement {
				emoji: "🚂"
				description: ""
			}
			ListElement {
				emoji: "🚃"
				description: ""
			}
			ListElement {
				emoji: "🚄"
				description: ""
			}
			ListElement {
				emoji: "🚅"
				description: ""
			}
			ListElement {
				emoji: "🚆"
				description: ""
			}
			ListElement {
				emoji: "🚇"
				description: ""
			}
			ListElement {
				emoji: "🚈"
				description: ""
			}
			ListElement {
				emoji: "🚉"
				description: ""
			}
			ListElement {
				emoji: "🚊"
				description: ""
			}
			ListElement {
				emoji: "🚝"
				description: ""
			}
			ListElement {
				emoji: "🚞"
				description: ""
			}
			ListElement {
				emoji: "🚋"
				description: ""
			}
			ListElement {
				emoji: "🚌"
				description: ""
			}
			ListElement {
				emoji: "🚍"
				description: ""
			}
			ListElement {
				emoji: "🚎"
				description: ""
			}
			ListElement {
				emoji: "🚐"
				description: ""
			}
			ListElement {
				emoji: "🚑"
				description: ""
			}
			ListElement {
				emoji: "🚒"
				description: ""
			}
			ListElement {
				emoji: "🚓"
				description: ""
			}
			ListElement {
				emoji: "🚔"
				description: ""
			}
			ListElement {
				emoji: "🚕"
				description: ""
			}
			ListElement {
				emoji: "🚖"
				description: ""
			}
			ListElement {
				emoji: "🚗"
				description: ""
			}
			ListElement {
				emoji: "🚘"
				description: ""
			}
			ListElement {
				emoji: "🚙"
				description: ""
			}
			ListElement {
				emoji: "🛻"
				description: ""
			}
			ListElement {
				emoji: "🚚"
				description: ""
			}
			ListElement {
				emoji: "🚛"
				description: ""
			}
			ListElement {
				emoji: "🚜"
				description: ""
			}
			ListElement {
				emoji: "🏎"
				description: ""
			}
			ListElement {
				emoji: "🏍"
				description: ""
			}
			ListElement {
				emoji: "🛵"
				description: ""
			}
			ListElement {
				emoji: "🦽"
				description: ""
			}
			ListElement {
				emoji: "🦼"
				description: ""
			}
			ListElement {
				emoji: "🛺"
				description: ""
			}
			ListElement {
				emoji: "🚲"
				description: ""
			}
			ListElement {
				emoji: "🛴"
				description: ""
			}
			ListElement {
				emoji: "🛹"
				description: ""
			}
			ListElement {
				emoji: "🛼"
				description: ""
			}
			ListElement {
				emoji: "🚏"
				description: ""
			}
			ListElement {
				emoji: "🛣"
				description: ""
			}
			ListElement {
				emoji: "🛤"
				description: ""
			}
			ListElement {
				emoji: "🛢"
				description: ""
			}
			ListElement {
				emoji: "⛽"
				description: ""
			}
			ListElement {
				emoji: "🛞"
				description: ""
			}
			ListElement {
				emoji: "🚨"
				description: ""
			}
			ListElement {
				emoji: "🚥"
				description: ""
			}
			ListElement {
				emoji: "🚦"
				description: ""
			}
			ListElement {
				emoji: "🛑"
				description: ""
			}
			ListElement {
				emoji: "🚧"
				description: ""
			}
			ListElement {
				emoji: "⚓"
				description: ""
			}
			ListElement {
				emoji: "🛟"
				description: ""
			}
			ListElement {
				emoji: "⛵"
				description: ""
			}
			ListElement {
				emoji: "🛶"
				description: ""
			}
			ListElement {
				emoji: "🚤"
				description: ""
			}
			ListElement {
				emoji: "🛳"
				description: ""
			}
			ListElement {
				emoji: "⛴"
				description: ""
			}
			ListElement {
				emoji: "🛥"
				description: ""
			}
			ListElement {
				emoji: "🚢"
				description: ""
			}
			ListElement {
				emoji: "✈"
				description: ""
			}
			ListElement {
				emoji: "🛩"
				description: ""
			}
			ListElement {
				emoji: "🛫"
				description: ""
			}
			ListElement {
				emoji: "🛬"
				description: ""
			}
			ListElement {
				emoji: "🪂"
				description: ""
			}
			ListElement {
				emoji: "💺"
				description: ""
			}
			ListElement {
				emoji: "🚁"
				description: ""
			}
			ListElement {
				emoji: "🚟"
				description: ""
			}
			ListElement {
				emoji: "🚠"
				description: ""
			}
			ListElement {
				emoji: "🚡"
				description: ""
			}
			ListElement {
				emoji: "🛰"
				description: ""
			}
			ListElement {
				emoji: "🚀"
				description: ""
			}
			ListElement {
				emoji: "🛸"
				description: ""
			}
			ListElement {
				emoji: "🛎"
				description: ""
			}
			ListElement {
				emoji: "🧳"
				description: ""
			}
			ListElement {
				emoji: "⌛"
				description: ""
			}
			ListElement {
				emoji: "⏳"
				description: ""
			}
			ListElement {
				emoji: "⌚"
				description: ""
			}
			ListElement {
				emoji: "⏰"
				description: ""
			}
			ListElement {
				emoji: "⏱"
				description: ""
			}
			ListElement {
				emoji: "⏲"
				description: ""
			}
			ListElement {
				emoji: "🕰"
				description: ""
			}
			ListElement {
				emoji: "🕛"
				description: ""
			}
			ListElement {
				emoji: "🕧"
				description: ""
			}
			ListElement {
				emoji: "🕐"
				description: ""
			}
			ListElement {
				emoji: "🕜"
				description: ""
			}
			ListElement {
				emoji: "🕑"
				description: ""
			}
			ListElement {
				emoji: "🕝"
				description: ""
			}
			ListElement {
				emoji: "🕒"
				description: ""
			}
			ListElement {
				emoji: "🕞"
				description: ""
			}
			ListElement {
				emoji: "🕓"
				description: ""
			}
			ListElement {
				emoji: "🕟"
				description: ""
			}
			ListElement {
				emoji: "🕔"
				description: ""
			}
			ListElement {
				emoji: "🕠"
				description: ""
			}
			ListElement {
				emoji: "🕕"
				description: ""
			}
			ListElement {
				emoji: "🕡"
				description: ""
			}
			ListElement {
				emoji: "🕖"
				description: ""
			}
			ListElement {
				emoji: "🕢"
				description: ""
			}
			ListElement {
				emoji: "🕗"
				description: ""
			}
			ListElement {
				emoji: "🕣"
				description: ""
			}
			ListElement {
				emoji: "🕘"
				description: ""
			}
			ListElement {
				emoji: "🕤"
				description: ""
			}
			ListElement {
				emoji: "🕙"
				description: ""
			}
			ListElement {
				emoji: "🕥"
				description: ""
			}
			ListElement {
				emoji: "🕚"
				description: ""
			}
			ListElement {
				emoji: "🕦"
				description: ""
			}
			ListElement {
				emoji: "🌑"
				description: ""
			}
			ListElement {
				emoji: "🌒"
				description: ""
			}
			ListElement {
				emoji: "🌓"
				description: ""
			}
			ListElement {
				emoji: "🌔"
				description: ""
			}
			ListElement {
				emoji: "🌕"
				description: ""
			}
			ListElement {
				emoji: "🌖"
				description: ""
			}
			ListElement {
				emoji: "🌗"
				description: ""
			}
			ListElement {
				emoji: "🌘"
				description: ""
			}
			ListElement {
				emoji: "🌙"
				description: ""
			}
			ListElement {
				emoji: "🌚"
				description: ""
			}
			ListElement {
				emoji: "🌛"
				description: ""
			}
			ListElement {
				emoji: "🌜"
				description: ""
			}
			ListElement {
				emoji: "🌡"
				description: ""
			}
			ListElement {
				emoji: "☀"
				description: ""
			}
			ListElement {
				emoji: "🌝"
				description: ""
			}
			ListElement {
				emoji: "🌞"
				description: ""
			}
			ListElement {
				emoji: "🪐"
				description: ""
			}
			ListElement {
				emoji: "⭐"
				description: ""
			}
			ListElement {
				emoji: "🌟"
				description: ""
			}
			ListElement {
				emoji: "🌠"
				description: ""
			}
			ListElement {
				emoji: "🌌"
				description: ""
			}
			ListElement {
				emoji: "☁"
				description: ""
			}
			ListElement {
				emoji: "⛅"
				description: ""
			}
			ListElement {
				emoji: "⛈"
				description: ""
			}
			ListElement {
				emoji: "🌤"
				description: ""
			}
			ListElement {
				emoji: "🌥"
				description: ""
			}
			ListElement {
				emoji: "🌦"
				description: ""
			}
			ListElement {
				emoji: "🌧"
				description: ""
			}
			ListElement {
				emoji: "🌨"
				description: ""
			}
			ListElement {
				emoji: "🌩"
				description: ""
			}
			ListElement {
				emoji: "🌪"
				description: ""
			}
			ListElement {
				emoji: "🌫"
				description: ""
			}
			ListElement {
				emoji: "🌬"
				description: ""
			}
			ListElement {
				emoji: "🌀"
				description: ""
			}
			ListElement {
				emoji: "🌈"
				description: ""
			}
			ListElement {
				emoji: "🌂"
				description: ""
			}
			ListElement {
				emoji: "☂"
				description: ""
			}
			ListElement {
				emoji: "☔"
				description: ""
			}
			ListElement {
				emoji: "⛱"
				description: ""
			}
			ListElement {
				emoji: "⚡"
				description: ""
			}
			ListElement {
				emoji: "❄"
				description: ""
			}
			ListElement {
				emoji: "☃"
				description: ""
			}
			ListElement {
				emoji: "⛄"
				description: ""
			}
			ListElement {
				emoji: "☄"
				description: ""
			}
			ListElement {
				emoji: "🔥"
				description: ""
			}
			ListElement {
				emoji: "💧"
				description: ""
			}
			ListElement {
				emoji: "🌊"
				description: ""
			}
			ListElement {
				emoji: "🎃"
				description: ""
			}
			ListElement {
				emoji: "🎄"
				description: ""
			}
			ListElement {
				emoji: "🎆"
				description: ""
			}
			ListElement {
				emoji: "🎇"
				description: ""
			}
			ListElement {
				emoji: "🧨"
				description: ""
			}
			ListElement {
				emoji: "✨"
				description: ""
			}
			ListElement {
				emoji: "🎈"
				description: ""
			}
			ListElement {
				emoji: "🎉"
				description: ""
			}
			ListElement {
				emoji: "🎊"
				description: ""
			}
			ListElement {
				emoji: "🎋"
				description: ""
			}
			ListElement {
				emoji: "🎍"
				description: ""
			}
			ListElement {
				emoji: "🎎"
				description: ""
			}
			ListElement {
				emoji: "🎏"
				description: ""
			}
			ListElement {
				emoji: "🎐"
				description: ""
			}
			ListElement {
				emoji: "🎑"
				description: ""
			}
			ListElement {
				emoji: "🧧"
				description: ""
			}
			ListElement {
				emoji: "🎀"
				description: ""
			}
			ListElement {
				emoji: "🎁"
				description: ""
			}
			ListElement {
				emoji: "🎗"
				description: ""
			}
			ListElement {
				emoji: "🎟"
				description: ""
			}
			ListElement {
				emoji: "🎫"
				description: ""
			}
			ListElement {
				emoji: "🎖"
				description: ""
			}
			ListElement {
				emoji: "🏆"
				description: ""
			}
			ListElement {
				emoji: "🏅"
				description: ""
			}
			ListElement {
				emoji: "🥇"
				description: ""
			}
			ListElement {
				emoji: "🥈"
				description: ""
			}
			ListElement {
				emoji: "🥉"
				description: ""
			}
			ListElement {
				emoji: "⚽"
				description: ""
			}
			ListElement {
				emoji: "⚾"
				description: ""
			}
			ListElement {
				emoji: "🥎"
				description: ""
			}
			ListElement {
				emoji: "🏀"
				description: ""
			}
			ListElement {
				emoji: "🏐"
				description: ""
			}
			ListElement {
				emoji: "🏈"
				description: ""
			}
			ListElement {
				emoji: "🏉"
				description: ""
			}
			ListElement {
				emoji: "🎾"
				description: ""
			}
			ListElement {
				emoji: "🥏"
				description: ""
			}
			ListElement {
				emoji: "🎳"
				description: ""
			}
			ListElement {
				emoji: "🏏"
				description: ""
			}
			ListElement {
				emoji: "🏑"
				description: ""
			}
			ListElement {
				emoji: "🏒"
				description: ""
			}
			ListElement {
				emoji: "🥍"
				description: ""
			}
			ListElement {
				emoji: "🏓"
				description: ""
			}
			ListElement {
				emoji: "🏸"
				description: ""
			}
			ListElement {
				emoji: "🥊"
				description: ""
			}
			ListElement {
				emoji: "🥋"
				description: ""
			}
			ListElement {
				emoji: "🥅"
				description: ""
			}
			ListElement {
				emoji: "⛳"
				description: ""
			}
			ListElement {
				emoji: "⛸"
				description: ""
			}
			ListElement {
				emoji: "🎣"
				description: ""
			}
			ListElement {
				emoji: "🤿"
				description: ""
			}
			ListElement {
				emoji: "🎽"
				description: ""
			}
			ListElement {
				emoji: "🎿"
				description: ""
			}
			ListElement {
				emoji: "🛷"
				description: ""
			}
			ListElement {
				emoji: "🥌"
				description: ""
			}
			ListElement {
				emoji: "🎯"
				description: ""
			}
			ListElement {
				emoji: "🪀"
				description: ""
			}
			ListElement {
				emoji: "🪁"
				description: ""
			}
			ListElement {
				emoji: "🔫"
				description: ""
			}
			ListElement {
				emoji: "🎱"
				description: ""
			}
			ListElement {
				emoji: "🔮"
				description: ""
			}
			ListElement {
				emoji: "🪄"
				description: ""
			}
			ListElement {
				emoji: "🎮"
				description: ""
			}
			ListElement {
				emoji: "🕹"
				description: ""
			}
			ListElement {
				emoji: "🎰"
				description: ""
			}
			ListElement {
				emoji: "🎲"
				description: ""
			}
			ListElement {
				emoji: "🧩"
				description: ""
			}
			ListElement {
				emoji: "🧸"
				description: ""
			}
			ListElement {
				emoji: "🪅"
				description: ""
			}
			ListElement {
				emoji: "🪩"
				description: ""
			}
			ListElement {
				emoji: "🪆"
				description: ""
			}
			ListElement {
				emoji: "♠"
				description: ""
			}
			ListElement {
				emoji: "♥"
				description: ""
			}
			ListElement {
				emoji: "♦"
				description: ""
			}
			ListElement {
				emoji: "♣"
				description: ""
			}
			ListElement {
				emoji: "♟"
				description: ""
			}
			ListElement {
				emoji: "🃏"
				description: ""
			}
			ListElement {
				emoji: "🀄"
				description: ""
			}
			ListElement {
				emoji: "🎴"
				description: ""
			}
			ListElement {
				emoji: "🎭"
				description: ""
			}
			ListElement {
				emoji: "🖼"
				description: ""
			}
			ListElement {
				emoji: "🎨"
				description: ""
			}
			ListElement {
				emoji: "🧵"
				description: ""
			}
			ListElement {
				emoji: "🪡"
				description: ""
			}
			ListElement {
				emoji: "🧶"
				description: ""
			}
			ListElement {
				emoji: "🪢"
				description: ""
			}
			ListElement {
				emoji: "👓"
				description: ""
			}
			ListElement {
				emoji: "🕶"
				description: ""
			}
			ListElement {
				emoji: "🥽"
				description: ""
			}
			ListElement {
				emoji: "🥼"
				description: ""
			}
			ListElement {
				emoji: "🦺"
				description: ""
			}
			ListElement {
				emoji: "👔"
				description: ""
			}
			ListElement {
				emoji: "👕"
				description: ""
			}
			ListElement {
				emoji: "👖"
				description: ""
			}
			ListElement {
				emoji: "🧣"
				description: ""
			}
			ListElement {
				emoji: "🧤"
				description: ""
			}
			ListElement {
				emoji: "🧥"
				description: ""
			}
			ListElement {
				emoji: "🧦"
				description: ""
			}
			ListElement {
				emoji: "👗"
				description: ""
			}
			ListElement {
				emoji: "👘"
				description: ""
			}
			ListElement {
				emoji: "🥻"
				description: ""
			}
			ListElement {
				emoji: "🩱"
				description: ""
			}
			ListElement {
				emoji: "🩲"
				description: ""
			}
			ListElement {
				emoji: "🩳"
				description: ""
			}
			ListElement {
				emoji: "👙"
				description: ""
			}
			ListElement {
				emoji: "👚"
				description: ""
			}
			ListElement {
				emoji: "🪭"
				description: ""
			}
			ListElement {
				emoji: "👛"
				description: ""
			}
			ListElement {
				emoji: "👜"
				description: ""
			}
			ListElement {
				emoji: "👝"
				description: ""
			}
			ListElement {
				emoji: "🛍"
				description: ""
			}
			ListElement {
				emoji: "🎒"
				description: ""
			}
			ListElement {
				emoji: "🩴"
				description: ""
			}
			ListElement {
				emoji: "👞"
				description: ""
			}
			ListElement {
				emoji: "👟"
				description: ""
			}
			ListElement {
				emoji: "🥾"
				description: ""
			}
			ListElement {
				emoji: "🥿"
				description: ""
			}
			ListElement {
				emoji: "👠"
				description: ""
			}
			ListElement {
				emoji: "👡"
				description: ""
			}
			ListElement {
				emoji: "🩰"
				description: ""
			}
			ListElement {
				emoji: "👢"
				description: ""
			}
			ListElement {
				emoji: "🪮"
				description: ""
			}
			ListElement {
				emoji: "👑"
				description: ""
			}
			ListElement {
				emoji: "👒"
				description: ""
			}
			ListElement {
				emoji: "🎩"
				description: ""
			}
			ListElement {
				emoji: "🎓"
				description: ""
			}
			ListElement {
				emoji: "🧢"
				description: ""
			}
			ListElement {
				emoji: "🪖"
				description: ""
			}
			ListElement {
				emoji: "⛑"
				description: ""
			}
			ListElement {
				emoji: "📿"
				description: ""
			}
			ListElement {
				emoji: "💄"
				description: ""
			}
			ListElement {
				emoji: "💍"
				description: ""
			}
			ListElement {
				emoji: "💎"
				description: ""
			}
			ListElement {
				emoji: "🔇"
				description: ""
			}
			ListElement {
				emoji: "🔈"
				description: ""
			}
			ListElement {
				emoji: "🔉"
				description: ""
			}
			ListElement {
				emoji: "🔊"
				description: ""
			}
			ListElement {
				emoji: "📢"
				description: ""
			}
			ListElement {
				emoji: "📣"
				description: ""
			}
			ListElement {
				emoji: "📯"
				description: ""
			}
			ListElement {
				emoji: "🔔"
				description: ""
			}
			ListElement {
				emoji: "🔕"
				description: ""
			}
			ListElement {
				emoji: "🎼"
				description: ""
			}
			ListElement {
				emoji: "🎵"
				description: ""
			}
			ListElement {
				emoji: "🎶"
				description: ""
			}
			ListElement {
				emoji: "🎙"
				description: ""
			}
			ListElement {
				emoji: "🎚"
				description: ""
			}
			ListElement {
				emoji: "🎛"
				description: ""
			}
			ListElement {
				emoji: "🎤"
				description: ""
			}
			ListElement {
				emoji: "🎧"
				description: ""
			}
			ListElement {
				emoji: "📻"
				description: ""
			}
			ListElement {
				emoji: "🎷"
				description: ""
			}
			ListElement {
				emoji: "🪗"
				description: ""
			}
			ListElement {
				emoji: "🎸"
				description: ""
			}
			ListElement {
				emoji: "🎹"
				description: ""
			}
			ListElement {
				emoji: "🎺"
				description: ""
			}
			ListElement {
				emoji: "🎻"
				description: ""
			}
			ListElement {
				emoji: "🪕"
				description: ""
			}
			ListElement {
				emoji: "🥁"
				description: ""
			}
			ListElement {
				emoji: "🪘"
				description: ""
			}
			ListElement {
				emoji: "🪇"
				description: ""
			}
			ListElement {
				emoji: "🪈"
				description: ""
			}
			ListElement {
				emoji: "📱"
				description: ""
			}
			ListElement {
				emoji: "📲"
				description: ""
			}
			ListElement {
				emoji: "☎"
				description: ""
			}
			ListElement {
				emoji: "📞"
				description: ""
			}
			ListElement {
				emoji: "📟"
				description: ""
			}
			ListElement {
				emoji: "📠"
				description: ""
			}
			ListElement {
				emoji: "🔋"
				description: ""
			}
			ListElement {
				emoji: "🪫"
				description: ""
			}
			ListElement {
				emoji: "🔌"
				description: ""
			}
			ListElement {
				emoji: "💻"
				description: ""
			}
			ListElement {
				emoji: "🖥"
				description: ""
			}
			ListElement {
				emoji: "🖨"
				description: ""
			}
			ListElement {
				emoji: "⌨"
				description: ""
			}
			ListElement {
				emoji: "🖱"
				description: ""
			}
			ListElement {
				emoji: "🖲"
				description: ""
			}
			ListElement {
				emoji: "💽"
				description: ""
			}
			ListElement {
				emoji: "💾"
				description: ""
			}
			ListElement {
				emoji: "💿"
				description: ""
			}
			ListElement {
				emoji: "📀"
				description: ""
			}
			ListElement {
				emoji: "🧮"
				description: ""
			}
			ListElement {
				emoji: "🎥"
				description: ""
			}
			ListElement {
				emoji: "🎞"
				description: ""
			}
			ListElement {
				emoji: "📽"
				description: ""
			}
			ListElement {
				emoji: "🎬"
				description: ""
			}
			ListElement {
				emoji: "📺"
				description: ""
			}
			ListElement {
				emoji: "📷"
				description: ""
			}
			ListElement {
				emoji: "📸"
				description: ""
			}
			ListElement {
				emoji: "📹"
				description: ""
			}
			ListElement {
				emoji: "📼"
				description: ""
			}
			ListElement {
				emoji: "🔍"
				description: ""
			}
			ListElement {
				emoji: "🔎"
				description: ""
			}
			ListElement {
				emoji: "🕯"
				description: ""
			}
			ListElement {
				emoji: "💡"
				description: ""
			}
			ListElement {
				emoji: "🔦"
				description: ""
			}
			ListElement {
				emoji: "🏮"
				description: ""
			}
			ListElement {
				emoji: "🪔"
				description: ""
			}
			ListElement {
				emoji: "📔"
				description: ""
			}
			ListElement {
				emoji: "📕"
				description: ""
			}
			ListElement {
				emoji: "📖"
				description: ""
			}
			ListElement {
				emoji: "📗"
				description: ""
			}
			ListElement {
				emoji: "📘"
				description: ""
			}
			ListElement {
				emoji: "📙"
				description: ""
			}
			ListElement {
				emoji: "📚"
				description: ""
			}
			ListElement {
				emoji: "📓"
				description: ""
			}
			ListElement {
				emoji: "📒"
				description: ""
			}
			ListElement {
				emoji: "📃"
				description: ""
			}
			ListElement {
				emoji: "📜"
				description: ""
			}
			ListElement {
				emoji: "📄"
				description: ""
			}
			ListElement {
				emoji: "📰"
				description: ""
			}
			ListElement {
				emoji: "🗞"
				description: ""
			}
			ListElement {
				emoji: "📑"
				description: ""
			}
			ListElement {
				emoji: "🔖"
				description: ""
			}
			ListElement {
				emoji: "🏷"
				description: ""
			}
			ListElement {
				emoji: "💰"
				description: ""
			}
			ListElement {
				emoji: "🪙"
				description: ""
			}
			ListElement {
				emoji: "💴"
				description: ""
			}
			ListElement {
				emoji: "💵"
				description: ""
			}
			ListElement {
				emoji: "💶"
				description: ""
			}
			ListElement {
				emoji: "💷"
				description: ""
			}
			ListElement {
				emoji: "💸"
				description: ""
			}
			ListElement {
				emoji: "💳"
				description: ""
			}
			ListElement {
				emoji: "🧾"
				description: ""
			}
			ListElement {
				emoji: "💹"
				description: ""
			}
			ListElement {
				emoji: "✉"
				description: ""
			}
			ListElement {
				emoji: "📧"
				description: ""
			}
			ListElement {
				emoji: "📨"
				description: ""
			}
			ListElement {
				emoji: "📩"
				description: ""
			}
			ListElement {
				emoji: "📤"
				description: ""
			}
			ListElement {
				emoji: "📥"
				description: ""
			}
			ListElement {
				emoji: "📦"
				description: ""
			}
			ListElement {
				emoji: "📫"
				description: ""
			}
			ListElement {
				emoji: "📪"
				description: ""
			}
			ListElement {
				emoji: "📬"
				description: ""
			}
			ListElement {
				emoji: "📭"
				description: ""
			}
			ListElement {
				emoji: "📮"
				description: ""
			}
			ListElement {
				emoji: "🗳"
				description: ""
			}
			ListElement {
				emoji: "✏"
				description: ""
			}
			ListElement {
				emoji: "✒"
				description: ""
			}
			ListElement {
				emoji: "🖋"
				description: ""
			}
			ListElement {
				emoji: "🖊"
				description: ""
			}
			ListElement {
				emoji: "🖌"
				description: ""
			}
			ListElement {
				emoji: "🖍"
				description: ""
			}
			ListElement {
				emoji: "📝"
				description: ""
			}
			ListElement {
				emoji: "💼"
				description: ""
			}
			ListElement {
				emoji: "📁"
				description: ""
			}
			ListElement {
				emoji: "📂"
				description: ""
			}
			ListElement {
				emoji: "🗂"
				description: ""
			}
			ListElement {
				emoji: "📅"
				description: ""
			}
			ListElement {
				emoji: "📆"
				description: ""
			}
			ListElement {
				emoji: "🗒"
				description: ""
			}
			ListElement {
				emoji: "🗓"
				description: ""
			}
			ListElement {
				emoji: "📇"
				description: ""
			}
			ListElement {
				emoji: "📈"
				description: ""
			}
			ListElement {
				emoji: "📉"
				description: ""
			}
			ListElement {
				emoji: "📊"
				description: ""
			}
			ListElement {
				emoji: "📋"
				description: ""
			}
			ListElement {
				emoji: "📌"
				description: ""
			}
			ListElement {
				emoji: "📍"
				description: ""
			}
			ListElement {
				emoji: "📎"
				description: ""
			}
			ListElement {
				emoji: "🖇"
				description: ""
			}
			ListElement {
				emoji: "📏"
				description: ""
			}
			ListElement {
				emoji: "📐"
				description: ""
			}
			ListElement {
				emoji: "✂"
				description: ""
			}
			ListElement {
				emoji: "🗃"
				description: ""
			}
			ListElement {
				emoji: "🗄"
				description: ""
			}
			ListElement {
				emoji: "🗑"
				description: ""
			}
			ListElement {
				emoji: "🔒"
				description: ""
			}
			ListElement {
				emoji: "🔓"
				description: ""
			}
			ListElement {
				emoji: "🔏"
				description: ""
			}
			ListElement {
				emoji: "🔐"
				description: ""
			}
			ListElement {
				emoji: "🔑"
				description: ""
			}
			ListElement {
				emoji: "🗝"
				description: ""
			}
			ListElement {
				emoji: "🔨"
				description: ""
			}
			ListElement {
				emoji: "🪓"
				description: ""
			}
			ListElement {
				emoji: "⛏"
				description: ""
			}
			ListElement {
				emoji: "⚒"
				description: ""
			}
			ListElement {
				emoji: "🛠"
				description: ""
			}
			ListElement {
				emoji: "🗡"
				description: ""
			}
			ListElement {
				emoji: "⚔"
				description: ""
			}
			ListElement {
				emoji: "💣"
				description: ""
			}
			ListElement {
				emoji: "🪃"
				description: ""
			}
			ListElement {
				emoji: "🏹"
				description: ""
			}
			ListElement {
				emoji: "🛡"
				description: ""
			}
			ListElement {
				emoji: "🪚"
				description: ""
			}
			ListElement {
				emoji: "🔧"
				description: ""
			}
			ListElement {
				emoji: "🪛"
				description: ""
			}
			ListElement {
				emoji: "🔩"
				description: ""
			}
			ListElement {
				emoji: "⚙"
				description: ""
			}
			ListElement {
				emoji: "🗜"
				description: ""
			}
			ListElement {
				emoji: "⚖"
				description: ""
			}
			ListElement {
				emoji: "🦯"
				description: ""
			}
			ListElement {
				emoji: "🔗"
				description: ""
			}
			ListElement {
				emoji: "⛓"
				description: ""
			}
			ListElement {
				emoji: "🪝"
				description: ""
			}
			ListElement {
				emoji: "🧰"
				description: ""
			}
			ListElement {
				emoji: "🧲"
				description: ""
			}
			ListElement {
				emoji: "🪜"
				description: ""
			}
			ListElement {
				emoji: "⚗"
				description: ""
			}
			ListElement {
				emoji: "🧪"
				description: ""
			}
			ListElement {
				emoji: "🧫"
				description: ""
			}
			ListElement {
				emoji: "🧬"
				description: ""
			}
			ListElement {
				emoji: "🔬"
				description: ""
			}
			ListElement {
				emoji: "🔭"
				description: ""
			}
			ListElement {
				emoji: "📡"
				description: ""
			}
			ListElement {
				emoji: "💉"
				description: ""
			}
			ListElement {
				emoji: "🩸"
				description: ""
			}
			ListElement {
				emoji: "💊"
				description: ""
			}
			ListElement {
				emoji: "🩹"
				description: ""
			}
			ListElement {
				emoji: "🩼"
				description: ""
			}
			ListElement {
				emoji: "🩺"
				description: ""
			}
			ListElement {
				emoji: "🩻"
				description: ""
			}
			ListElement {
				emoji: "🚪"
				description: ""
			}
			ListElement {
				emoji: "🛗"
				description: ""
			}
			ListElement {
				emoji: "🪞"
				description: ""
			}
			ListElement {
				emoji: "🪟"
				description: ""
			}
			ListElement {
				emoji: "🛏"
				description: ""
			}
			ListElement {
				emoji: "🛋"
				description: ""
			}
			ListElement {
				emoji: "🪑"
				description: ""
			}
			ListElement {
				emoji: "🚽"
				description: ""
			}
			ListElement {
				emoji: "🪠"
				description: ""
			}
			ListElement {
				emoji: "🚿"
				description: ""
			}
			ListElement {
				emoji: "🛁"
				description: ""
			}
			ListElement {
				emoji: "🪤"
				description: ""
			}
			ListElement {
				emoji: "🪒"
				description: ""
			}
			ListElement {
				emoji: "🧴"
				description: ""
			}
			ListElement {
				emoji: "🧷"
				description: ""
			}
			ListElement {
				emoji: "🧹"
				description: ""
			}
			ListElement {
				emoji: "🧺"
				description: ""
			}
			ListElement {
				emoji: "🧻"
				description: ""
			}
			ListElement {
				emoji: "🪣"
				description: ""
			}
			ListElement {
				emoji: "🧼"
				description: ""
			}
			ListElement {
				emoji: "🫧"
				description: ""
			}
			ListElement {
				emoji: "🪥"
				description: ""
			}
			ListElement {
				emoji: "🧽"
				description: ""
			}
			ListElement {
				emoji: "🧯"
				description: ""
			}
			ListElement {
				emoji: "🛒"
				description: ""
			}
			ListElement {
				emoji: "🚬"
				description: ""
			}
			ListElement {
				emoji: "⚰"
				description: ""
			}
			ListElement {
				emoji: "🪦"
				description: ""
			}
			ListElement {
				emoji: "⚱"
				description: ""
			}
			ListElement {
				emoji: "🧿"
				description: ""
			}
			ListElement {
				emoji: "🪬"
				description: ""
			}
			ListElement {
				emoji: "🗿"
				description: ""
			}
			ListElement {
				emoji: "🪧"
				description: ""
			}
			ListElement {
				emoji: "🪪"
				description: ""
			}
			ListElement {
				emoji: "🏧"
				description: ""
			}
			ListElement {
				emoji: "🚮"
				description: ""
			}
			ListElement {
				emoji: "🚰"
				description: ""
			}
			ListElement {
				emoji: "♿"
				description: ""
			}
			ListElement {
				emoji: "🚹"
				description: ""
			}
			ListElement {
				emoji: "🚺"
				description: ""
			}
			ListElement {
				emoji: "🚻"
				description: ""
			}
			ListElement {
				emoji: "🚼"
				description: ""
			}
			ListElement {
				emoji: "🚾"
				description: ""
			}
			ListElement {
				emoji: "🛂"
				description: ""
			}
			ListElement {
				emoji: "🛃"
				description: ""
			}
			ListElement {
				emoji: "🛄"
				description: ""
			}
			ListElement {
				emoji: "🛅"
				description: ""
			}
			ListElement {
				emoji: "⚠"
				description: ""
			}
			ListElement {
				emoji: "🚸"
				description: ""
			}
			ListElement {
				emoji: "⛔"
				description: ""
			}
			ListElement {
				emoji: "🚫"
				description: ""
			}
			ListElement {
				emoji: "🚳"
				description: ""
			}
			ListElement {
				emoji: "🚭"
				description: ""
			}
			ListElement {
				emoji: "🚯"
				description: ""
			}
			ListElement {
				emoji: "🚱"
				description: ""
			}
			ListElement {
				emoji: "🚷"
				description: ""
			}
			ListElement {
				emoji: "📵"
				description: ""
			}
			ListElement {
				emoji: "🔞"
				description: ""
			}
			ListElement {
				emoji: "☢"
				description: ""
			}
			ListElement {
				emoji: "☣"
				description: ""
			}
			ListElement {
				emoji: "⬆"
				description: ""
			}
			ListElement {
				emoji: "↗"
				description: ""
			}
			ListElement {
				emoji: "➡"
				description: ""
			}
			ListElement {
				emoji: "↘"
				description: ""
			}
			ListElement {
				emoji: "⬇"
				description: ""
			}
			ListElement {
				emoji: "↙"
				description: ""
			}
			ListElement {
				emoji: "⬅"
				description: ""
			}
			ListElement {
				emoji: "↖"
				description: ""
			}
			ListElement {
				emoji: "↕"
				description: ""
			}
			ListElement {
				emoji: "↔"
				description: ""
			}
			ListElement {
				emoji: "↩"
				description: ""
			}
			ListElement {
				emoji: "↪"
				description: ""
			}
			ListElement {
				emoji: "⤴"
				description: ""
			}
			ListElement {
				emoji: "⤵"
				description: ""
			}
			ListElement {
				emoji: "🔃"
				description: ""
			}
			ListElement {
				emoji: "🔄"
				description: ""
			}
			ListElement {
				emoji: "🔙"
				description: ""
			}
			ListElement {
				emoji: "🔚"
				description: ""
			}
			ListElement {
				emoji: "🔛"
				description: ""
			}
			ListElement {
				emoji: "🔜"
				description: ""
			}
			ListElement {
				emoji: "🔝"
				description: ""
			}
			ListElement {
				emoji: "🛐"
				description: ""
			}
			ListElement {
				emoji: "⚛"
				description: ""
			}
			ListElement {
				emoji: "🕉"
				description: ""
			}
			ListElement {
				emoji: "✡"
				description: ""
			}
			ListElement {
				emoji: "☸"
				description: ""
			}
			ListElement {
				emoji: "☯"
				description: ""
			}
			ListElement {
				emoji: "✝"
				description: ""
			}
			ListElement {
				emoji: "☦"
				description: ""
			}
			ListElement {
				emoji: "☪"
				description: ""
			}
			ListElement {
				emoji: "☮"
				description: ""
			}
			ListElement {
				emoji: "🕎"
				description: ""
			}
			ListElement {
				emoji: "🔯"
				description: ""
			}
			ListElement {
				emoji: "🪯"
				description: ""
			}
			ListElement {
				emoji: "♈"
				description: ""
			}
			ListElement {
				emoji: "♉"
				description: ""
			}
			ListElement {
				emoji: "♊"
				description: ""
			}
			ListElement {
				emoji: "♋"
				description: ""
			}
			ListElement {
				emoji: "♌"
				description: ""
			}
			ListElement {
				emoji: "♍"
				description: ""
			}
			ListElement {
				emoji: "♎"
				description: ""
			}
			ListElement {
				emoji: "♏"
				description: ""
			}
			ListElement {
				emoji: "♐"
				description: ""
			}
			ListElement {
				emoji: "♑"
				description: ""
			}
			ListElement {
				emoji: "♒"
				description: ""
			}
			ListElement {
				emoji: "♓"
				description: ""
			}
			ListElement {
				emoji: "⛎"
				description: ""
			}
			ListElement {
				emoji: "🔀"
				description: ""
			}
			ListElement {
				emoji: "🔁"
				description: ""
			}
			ListElement {
				emoji: "🔂"
				description: ""
			}
			ListElement {
				emoji: "▶"
				description: ""
			}
			ListElement {
				emoji: "⏩"
				description: ""
			}
			ListElement {
				emoji: "⏭"
				description: ""
			}
			ListElement {
				emoji: "⏯"
				description: ""
			}
			ListElement {
				emoji: "◀"
				description: ""
			}
			ListElement {
				emoji: "⏪"
				description: ""
			}
			ListElement {
				emoji: "⏮"
				description: ""
			}
			ListElement {
				emoji: "🔼"
				description: ""
			}
			ListElement {
				emoji: "⏫"
				description: ""
			}
			ListElement {
				emoji: "🔽"
				description: ""
			}
			ListElement {
				emoji: "⏬"
				description: ""
			}
			ListElement {
				emoji: "⏸"
				description: ""
			}
			ListElement {
				emoji: "⏹"
				description: ""
			}
			ListElement {
				emoji: "⏺"
				description: ""
			}
			ListElement {
				emoji: "⏏"
				description: ""
			}
			ListElement {
				emoji: "🎦"
				description: ""
			}
			ListElement {
				emoji: "🔅"
				description: ""
			}
			ListElement {
				emoji: "🔆"
				description: ""
			}
			ListElement {
				emoji: "📶"
				description: ""
			}
			ListElement {
				emoji: "🛜"
				description: ""
			}
			ListElement {
				emoji: "📳"
				description: ""
			}
			ListElement {
				emoji: "📴"
				description: ""
			}
			ListElement {
				emoji: "♀"
				description: ""
			}
			ListElement {
				emoji: "♂"
				description: ""
			}
			ListElement {
				emoji: "⚧"
				description: ""
			}
			ListElement {
				emoji: "✖"
				description: ""
			}
			ListElement {
				emoji: "➕"
				description: ""
			}
			ListElement {
				emoji: "➖"
				description: ""
			}
			ListElement {
				emoji: "➗"
				description: ""
			}
			ListElement {
				emoji: "🟰"
				description: ""
			}
			ListElement {
				emoji: "♾"
				description: ""
			}
			ListElement {
				emoji: "‼"
				description: ""
			}
			ListElement {
				emoji: "⁉"
				description: ""
			}
			ListElement {
				emoji: "❓"
				description: ""
			}
			ListElement {
				emoji: "❔"
				description: ""
			}
			ListElement {
				emoji: "❕"
				description: ""
			}
			ListElement {
				emoji: "❗"
				description: ""
			}
			ListElement {
				emoji: "〰"
				description: ""
			}
			ListElement {
				emoji: "💱"
				description: ""
			}
			ListElement {
				emoji: "💲"
				description: ""
			}
			ListElement {
				emoji: "⚕"
				description: ""
			}
			ListElement {
				emoji: "♻"
				description: ""
			}
			ListElement {
				emoji: "⚜"
				description: ""
			}
			ListElement {
				emoji: "🔱"
				description: ""
			}
			ListElement {
				emoji: "📛"
				description: ""
			}
			ListElement {
				emoji: "🔰"
				description: ""
			}
			ListElement {
				emoji: "⭕"
				description: ""
			}
			ListElement {
				emoji: "✅"
				description: ""
			}
			ListElement {
				emoji: "☑"
				description: ""
			}
			ListElement {
				emoji: "✔"
				description: ""
			}
			ListElement {
				emoji: "❌"
				description: ""
			}
			ListElement {
				emoji: "❎"
				description: ""
			}
			ListElement {
				emoji: "➰"
				description: ""
			}
			ListElement {
				emoji: "➿"
				description: ""
			}
			ListElement {
				emoji: "〽"
				description: ""
			}
			ListElement {
				emoji: "✳"
				description: ""
			}
			ListElement {
				emoji: "✴"
				description: ""
			}
			ListElement {
				emoji: "❇"
				description: ""
			}
			ListElement {
				emoji: "©"
				description: ""
			}
			ListElement {
				emoji: "®"
				description: ""
			}
			ListElement {
				emoji: "™"
				description: ""
			}
			ListElement {
				emoji: "#️⃣"
				description: ""
			}
			ListElement {
				emoji: "*️⃣"
				description: ""
			}
			ListElement {
				emoji: "0️⃣"
				description: ""
			}
			ListElement {
				emoji: "1️⃣"
				description: ""
			}
			ListElement {
				emoji: "2️⃣"
				description: ""
			}
			ListElement {
				emoji: "3️⃣"
				description: ""
			}
			ListElement {
				emoji: "4️⃣"
				description: ""
			}
			ListElement {
				emoji: "5️⃣"
				description: ""
			}
			ListElement {
				emoji: "6️⃣"
				description: ""
			}
			ListElement {
				emoji: "7️⃣"
				description: ""
			}
			ListElement {
				emoji: "8️⃣"
				description: ""
			}
			ListElement {
				emoji: "9️⃣"
				description: ""
			}
			ListElement {
				emoji: "🔟"
				description: ""
			}
			ListElement {
				emoji: "🔠"
				description: ""
			}
			ListElement {
				emoji: "🔡"
				description: ""
			}
			ListElement {
				emoji: "🔢"
				description: ""
			}
			ListElement {
				emoji: "🔣"
				description: ""
			}
			ListElement {
				emoji: "🔤"
				description: ""
			}
			ListElement {
				emoji: "🅰"
				description: ""
			}
			ListElement {
				emoji: "🆎"
				description: ""
			}
			ListElement {
				emoji: "🅱"
				description: ""
			}
			ListElement {
				emoji: "🆑"
				description: ""
			}
			ListElement {
				emoji: "🆒"
				description: ""
			}
			ListElement {
				emoji: "🆓"
				description: ""
			}
			ListElement {
				emoji: "ℹ"
				description: ""
			}
			ListElement {
				emoji: "🆔"
				description: ""
			}
			ListElement {
				emoji: "Ⓜ"
				description: ""
			}
			ListElement {
				emoji: "🆕"
				description: ""
			}
			ListElement {
				emoji: "🆖"
				description: ""
			}
			ListElement {
				emoji: "🅾"
				description: ""
			}
			ListElement {
				emoji: "🆗"
				description: ""
			}
			ListElement {
				emoji: "🅿"
				description: ""
			}
			ListElement {
				emoji: "🆘"
				description: ""
			}
			ListElement {
				emoji: "🆙"
				description: ""
			}
			ListElement {
				emoji: "🆚"
				description: ""
			}
			ListElement {
				emoji: "🈁"
				description: ""
			}
			ListElement {
				emoji: "🈂"
				description: ""
			}
			ListElement {
				emoji: "🈷"
				description: ""
			}
			ListElement {
				emoji: "🈶"
				description: ""
			}
			ListElement {
				emoji: "🈯"
				description: ""
			}
			ListElement {
				emoji: "🉐"
				description: ""
			}
			ListElement {
				emoji: "🈹"
				description: ""
			}
			ListElement {
				emoji: "🈚"
				description: ""
			}
			ListElement {
				emoji: "🈲"
				description: ""
			}
			ListElement {
				emoji: "🉑"
				description: ""
			}
			ListElement {
				emoji: "🈸"
				description: ""
			}
			ListElement {
				emoji: "🈴"
				description: ""
			}
			ListElement {
				emoji: "🈳"
				description: ""
			}
			ListElement {
				emoji: "㊗"
				description: ""
			}
			ListElement {
				emoji: "㊙"
				description: ""
			}
			ListElement {
				emoji: "🈺"
				description: ""
			}
			ListElement {
				emoji: "🈵"
				description: ""
			}
			ListElement {
				emoji: "🔴"
				description: ""
			}
			ListElement {
				emoji: "🟠"
				description: ""
			}
			ListElement {
				emoji: "🟡"
				description: ""
			}
			ListElement {
				emoji: "🟢"
				description: ""
			}
			ListElement {
				emoji: "🔵"
				description: ""
			}
			ListElement {
				emoji: "🟣"
				description: ""
			}
			ListElement {
				emoji: "🟤"
				description: ""
			}
			ListElement {
				emoji: "⚫"
				description: ""
			}
			ListElement {
				emoji: "⚪"
				description: ""
			}
			ListElement {
				emoji: "🟥"
				description: ""
			}
			ListElement {
				emoji: "🟧"
				description: ""
			}
			ListElement {
				emoji: "🟨"
				description: ""
			}
			ListElement {
				emoji: "🟩"
				description: ""
			}
			ListElement {
				emoji: "🟦"
				description: ""
			}
			ListElement {
				emoji: "🟪"
				description: ""
			}
			ListElement {
				emoji: "🟫"
				description: ""
			}
			ListElement {
				emoji: "⬛"
				description: ""
			}
			ListElement {
				emoji: "⬜"
				description: ""
			}
			ListElement {
				emoji: "◼"
				description: ""
			}
			ListElement {
				emoji: "◻"
				description: ""
			}
			ListElement {
				emoji: "◾"
				description: ""
			}
			ListElement {
				emoji: "◽"
				description: ""
			}
			ListElement {
				emoji: "▪"
				description: ""
			}
			ListElement {
				emoji: "▫"
				description: ""
			}
			ListElement {
				emoji: "🔶"
				description: ""
			}
			ListElement {
				emoji: "🔷"
				description: ""
			}
			ListElement {
				emoji: "🔸"
				description: ""
			}
			ListElement {
				emoji: "🔹"
				description: ""
			}
			ListElement {
				emoji: "🔺"
				description: ""
			}
			ListElement {
				emoji: "🔻"
				description: ""
			}
			ListElement {
				emoji: "💠"
				description: ""
			}
			ListElement {
				emoji: "🔘"
				description: ""
			}
			ListElement {
				emoji: "🔳"
				description: ""
			}
			ListElement {
				emoji: "🔲"
				description: ""
			}
			ListElement {
				emoji: "🏁"
				description: ""
			}
			ListElement {
				emoji: "🚩"
				description: ""
			}
			ListElement {
				emoji: "🎌"
				description: ""
			}
			ListElement {
				emoji: "🏴"
				description: ""
			}
			ListElement {
				emoji: "🏳"
				description: ""
			}
			ListElement {
				emoji: "🏳️‍🌈"
				description: ""
			}
			ListElement {
				emoji: "🏳️‍⚧️"
				description: ""
			}
			ListElement {
				emoji: "🏴‍☠️"
				description: ""
			}
			ListElement {
				emoji: "🇦🇨"
				description: ""
			}
			ListElement {
				emoji: "🇦🇩"
				description: ""
			}
			ListElement {
				emoji: "🇦🇪"
				description: ""
			}
			ListElement {
				emoji: "🇦🇫"
				description: ""
			}
			ListElement {
				emoji: "🇦🇬"
				description: ""
			}
			ListElement {
				emoji: "🇦🇮"
				description: ""
			}
			ListElement {
				emoji: "🇦🇱"
				description: ""
			}
			ListElement {
				emoji: "🇦🇲"
				description: ""
			}
			ListElement {
				emoji: "🇦🇴"
				description: ""
			}
			ListElement {
				emoji: "🇦🇶"
				description: ""
			}
			ListElement {
				emoji: "🇦🇷"
				description: ""
			}
			ListElement {
				emoji: "🇦🇸"
				description: ""
			}
			ListElement {
				emoji: "🇦🇹"
				description: ""
			}
			ListElement {
				emoji: "🇦🇺"
				description: ""
			}
			ListElement {
				emoji: "🇦🇼"
				description: ""
			}
			ListElement {
				emoji: "🇦🇽"
				description: ""
			}
			ListElement {
				emoji: "🇦🇿"
				description: ""
			}
			ListElement {
				emoji: "🇧🇦"
				description: ""
			}
			ListElement {
				emoji: "🇧🇧"
				description: ""
			}
			ListElement {
				emoji: "🇧🇩"
				description: ""
			}
			ListElement {
				emoji: "🇧🇪"
				description: ""
			}
			ListElement {
				emoji: "🇧🇫"
				description: ""
			}
			ListElement {
				emoji: "🇧🇬"
				description: ""
			}
			ListElement {
				emoji: "🇧🇭"
				description: ""
			}
			ListElement {
				emoji: "🇧🇮"
				description: ""
			}
			ListElement {
				emoji: "🇧🇯"
				description: ""
			}
			ListElement {
				emoji: "🇧🇱"
				description: ""
			}
			ListElement {
				emoji: "🇧🇲"
				description: ""
			}
			ListElement {
				emoji: "🇧🇳"
				description: ""
			}
			ListElement {
				emoji: "🇧🇴"
				description: ""
			}
			ListElement {
				emoji: "🇧🇶"
				description: ""
			}
			ListElement {
				emoji: "🇧🇷"
				description: ""
			}
			ListElement {
				emoji: "🇧🇸"
				description: ""
			}
			ListElement {
				emoji: "🇧🇹"
				description: ""
			}
			ListElement {
				emoji: "🇧🇻"
				description: ""
			}
			ListElement {
				emoji: "🇧🇼"
				description: ""
			}
			ListElement {
				emoji: "🇧🇾"
				description: ""
			}
			ListElement {
				emoji: "🇧🇿"
				description: ""
			}
			ListElement {
				emoji: "🇨🇦"
				description: ""
			}
			ListElement {
				emoji: "🇨🇨"
				description: ""
			}
			ListElement {
				emoji: "🇨🇩"
				description: ""
			}
			ListElement {
				emoji: "🇨🇫"
				description: ""
			}
			ListElement {
				emoji: "🇨🇬"
				description: ""
			}
			ListElement {
				emoji: "🇨🇭"
				description: ""
			}
			ListElement {
				emoji: "🇨🇮"
				description: ""
			}
			ListElement {
				emoji: "🇨🇰"
				description: ""
			}
			ListElement {
				emoji: "🇨🇱"
				description: ""
			}
			ListElement {
				emoji: "🇨🇲"
				description: ""
			}
			ListElement {
				emoji: "🇨🇳"
				description: ""
			}
			ListElement {
				emoji: "🇨🇴"
				description: ""
			}
			ListElement {
				emoji: "🇨🇵"
				description: ""
			}
			ListElement {
				emoji: "🇨🇷"
				description: ""
			}
			ListElement {
				emoji: "🇨🇺"
				description: ""
			}
			ListElement {
				emoji: "🇨🇻"
				description: ""
			}
			ListElement {
				emoji: "🇨🇼"
				description: ""
			}
			ListElement {
				emoji: "🇨🇽"
				description: ""
			}
			ListElement {
				emoji: "🇨🇾"
				description: ""
			}
			ListElement {
				emoji: "🇨🇿"
				description: ""
			}
			ListElement {
				emoji: "🇩🇪"
				description: ""
			}
			ListElement {
				emoji: "🇩🇬"
				description: ""
			}
			ListElement {
				emoji: "🇩🇯"
				description: ""
			}
			ListElement {
				emoji: "🇩🇰"
				description: ""
			}
			ListElement {
				emoji: "🇩🇲"
				description: ""
			}
			ListElement {
				emoji: "🇩🇴"
				description: ""
			}
			ListElement {
				emoji: "🇩🇿"
				description: ""
			}
			ListElement {
				emoji: "🇪🇦"
				description: ""
			}
			ListElement {
				emoji: "🇪🇨"
				description: ""
			}
			ListElement {
				emoji: "🇪🇪"
				description: ""
			}
			ListElement {
				emoji: "🇪🇬"
				description: ""
			}
			ListElement {
				emoji: "🇪🇭"
				description: ""
			}
			ListElement {
				emoji: "🇪🇷"
				description: ""
			}
			ListElement {
				emoji: "🇪🇸"
				description: ""
			}
			ListElement {
				emoji: "🇪🇹"
				description: ""
			}
			ListElement {
				emoji: "🇪🇺"
				description: ""
			}
			ListElement {
				emoji: "🇫🇮"
				description: ""
			}
			ListElement {
				emoji: "🇫🇯"
				description: ""
			}
			ListElement {
				emoji: "🇫🇰"
				description: ""
			}
			ListElement {
				emoji: "🇫🇲"
				description: ""
			}
			ListElement {
				emoji: "🇫🇴"
				description: ""
			}
			ListElement {
				emoji: "🇫🇷"
				description: ""
			}
			ListElement {
				emoji: "🇬🇦"
				description: ""
			}
			ListElement {
				emoji: "🇬🇧"
				description: ""
			}
			ListElement {
				emoji: "🇬🇩"
				description: ""
			}
			ListElement {
				emoji: "🇬🇪"
				description: ""
			}
			ListElement {
				emoji: "🇬🇫"
				description: ""
			}
			ListElement {
				emoji: "🇬🇬"
				description: ""
			}
			ListElement {
				emoji: "🇬🇭"
				description: ""
			}
			ListElement {
				emoji: "🇬🇮"
				description: ""
			}
			ListElement {
				emoji: "🇬🇱"
				description: ""
			}
			ListElement {
				emoji: "🇬🇲"
				description: ""
			}
			ListElement {
				emoji: "🇬🇳"
				description: ""
			}
			ListElement {
				emoji: "🇬🇵"
				description: ""
			}
			ListElement {
				emoji: "🇬🇶"
				description: ""
			}
			ListElement {
				emoji: "🇬🇷"
				description: ""
			}
			ListElement {
				emoji: "🇬🇸"
				description: ""
			}
			ListElement {
				emoji: "🇬🇹"
				description: ""
			}
			ListElement {
				emoji: "🇬🇺"
				description: ""
			}
			ListElement {
				emoji: "🇬🇼"
				description: ""
			}
			ListElement {
				emoji: "🇬🇾"
				description: ""
			}
			ListElement {
				emoji: "🇭🇰"
				description: ""
			}
			ListElement {
				emoji: "🇭🇲"
				description: ""
			}
			ListElement {
				emoji: "🇭🇳"
				description: ""
			}
			ListElement {
				emoji: "🇭🇷"
				description: ""
			}
			ListElement {
				emoji: "🇭🇹"
				description: ""
			}
			ListElement {
				emoji: "🇭🇺"
				description: ""
			}
			ListElement {
				emoji: "🇮🇨"
				description: ""
			}
			ListElement {
				emoji: "🇮🇩"
				description: ""
			}
			ListElement {
				emoji: "🇮🇪"
				description: ""
			}
			ListElement {
				emoji: "🇮🇱"
				description: ""
			}
			ListElement {
				emoji: "🇮🇲"
				description: ""
			}
			ListElement {
				emoji: "🇮🇳"
				description: ""
			}
			ListElement {
				emoji: "🇮🇴"
				description: ""
			}
			ListElement {
				emoji: "🇮🇶"
				description: ""
			}
			ListElement {
				emoji: "🇮🇷"
				description: ""
			}
			ListElement {
				emoji: "🇮🇸"
				description: ""
			}
			ListElement {
				emoji: "🇮🇹"
				description: ""
			}
			ListElement {
				emoji: "🇯🇪"
				description: ""
			}
			ListElement {
				emoji: "🇯🇲"
				description: ""
			}
			ListElement {
				emoji: "🇯🇴"
				description: ""
			}
			ListElement {
				emoji: "🇯🇵"
				description: ""
			}
			ListElement {
				emoji: "🇰🇪"
				description: ""
			}
			ListElement {
				emoji: "🇰🇬"
				description: ""
			}
			ListElement {
				emoji: "🇰🇭"
				description: ""
			}
			ListElement {
				emoji: "🇰🇮"
				description: ""
			}
			ListElement {
				emoji: "🇰🇲"
				description: ""
			}
			ListElement {
				emoji: "🇰🇳"
				description: ""
			}
			ListElement {
				emoji: "🇰🇵"
				description: ""
			}
			ListElement {
				emoji: "🇰🇷"
				description: ""
			}
			ListElement {
				emoji: "🇰🇼"
				description: ""
			}
			ListElement {
				emoji: "🇰🇾"
				description: ""
			}
			ListElement {
				emoji: "🇰🇿"
				description: ""
			}
			ListElement {
				emoji: "🇱🇦"
				description: ""
			}
			ListElement {
				emoji: "🇱🇧"
				description: ""
			}
			ListElement {
				emoji: "🇱🇨"
				description: ""
			}
			ListElement {
				emoji: "🇱🇮"
				description: ""
			}
			ListElement {
				emoji: "🇱🇰"
				description: ""
			}
			ListElement {
				emoji: "🇱🇷"
				description: ""
			}
			ListElement {
				emoji: "🇱🇸"
				description: ""
			}
			ListElement {
				emoji: "🇱🇹"
				description: ""
			}
			ListElement {
				emoji: "🇱🇺"
				description: ""
			}
			ListElement {
				emoji: "🇱🇻"
				description: ""
			}
			ListElement {
				emoji: "🇱🇾"
				description: ""
			}
			ListElement {
				emoji: "🇲🇦"
				description: ""
			}
			ListElement {
				emoji: "🇲🇨"
				description: ""
			}
			ListElement {
				emoji: "🇲🇩"
				description: ""
			}
			ListElement {
				emoji: "🇲🇪"
				description: ""
			}
			ListElement {
				emoji: "🇲🇫"
				description: ""
			}
			ListElement {
				emoji: "🇲🇬"
				description: ""
			}
			ListElement {
				emoji: "🇲🇭"
				description: ""
			}
			ListElement {
				emoji: "🇲🇰"
				description: ""
			}
			ListElement {
				emoji: "🇲🇱"
				description: ""
			}
			ListElement {
				emoji: "🇲🇲"
				description: ""
			}
			ListElement {
				emoji: "🇲🇳"
				description: ""
			}
			ListElement {
				emoji: "🇲🇴"
				description: ""
			}
			ListElement {
				emoji: "🇲🇵"
				description: ""
			}
			ListElement {
				emoji: "🇲🇶"
				description: ""
			}
			ListElement {
				emoji: "🇲🇷"
				description: ""
			}
			ListElement {
				emoji: "🇲🇸"
				description: ""
			}
			ListElement {
				emoji: "🇲🇹"
				description: ""
			}
			ListElement {
				emoji: "🇲🇺"
				description: ""
			}
			ListElement {
				emoji: "🇲🇻"
				description: ""
			}
			ListElement {
				emoji: "🇲🇼"
				description: ""
			}
			ListElement {
				emoji: "🇲🇽"
				description: ""
			}
			ListElement {
				emoji: "🇲🇾"
				description: ""
			}
			ListElement {
				emoji: "🇲🇿"
				description: ""
			}
			ListElement {
				emoji: "🇳🇦"
				description: ""
			}
			ListElement {
				emoji: "🇳🇨"
				description: ""
			}
			ListElement {
				emoji: "🇳🇪"
				description: ""
			}
			ListElement {
				emoji: "🇳🇫"
				description: ""
			}
			ListElement {
				emoji: "🇳🇬"
				description: ""
			}
			ListElement {
				emoji: "🇳🇮"
				description: ""
			}
			ListElement {
				emoji: "🇳🇱"
				description: ""
			}
			ListElement {
				emoji: "🇳🇴"
				description: ""
			}
			ListElement {
				emoji: "🇳🇵"
				description: ""
			}
			ListElement {
				emoji: "🇳🇷"
				description: ""
			}
			ListElement {
				emoji: "🇳🇺"
				description: ""
			}
			ListElement {
				emoji: "🇳🇿"
				description: ""
			}
			ListElement {
				emoji: "🇴🇲"
				description: ""
			}
			ListElement {
				emoji: "🇵🇦"
				description: ""
			}
			ListElement {
				emoji: "🇵🇪"
				description: ""
			}
			ListElement {
				emoji: "🇵🇫"
				description: ""
			}
			ListElement {
				emoji: "🇵🇬"
				description: ""
			}
			ListElement {
				emoji: "🇵🇭"
				description: ""
			}
			ListElement {
				emoji: "🇵🇰"
				description: ""
			}
			ListElement {
				emoji: "🇵🇱"
				description: ""
			}
			ListElement {
				emoji: "🇵🇲"
				description: ""
			}
			ListElement {
				emoji: "🇵🇳"
				description: ""
			}
			ListElement {
				emoji: "🇵🇷"
				description: ""
			}
			ListElement {
				emoji: "🇵🇸"
				description: ""
			}
			ListElement {
				emoji: "🇵🇹"
				description: ""
			}
			ListElement {
				emoji: "🇵🇼"
				description: ""
			}
			ListElement {
				emoji: "🇵🇾"
				description: ""
			}
			ListElement {
				emoji: "🇶🇦"
				description: ""
			}
			ListElement {
				emoji: "🇷🇪"
				description: ""
			}
			ListElement {
				emoji: "🇷🇴"
				description: ""
			}
			ListElement {
				emoji: "🇷🇸"
				description: ""
			}
			ListElement {
				emoji: "🇷🇺"
				description: ""
			}
			ListElement {
				emoji: "🇷🇼"
				description: ""
			}
			ListElement {
				emoji: "🇸🇦"
				description: ""
			}
			ListElement {
				emoji: "🇸🇧"
				description: ""
			}
			ListElement {
				emoji: "🇸🇨"
				description: ""
			}
			ListElement {
				emoji: "🇸🇩"
				description: ""
			}
			ListElement {
				emoji: "🇸🇪"
				description: ""
			}
			ListElement {
				emoji: "🇸🇬"
				description: ""
			}
			ListElement {
				emoji: "🇸🇭"
				description: ""
			}
			ListElement {
				emoji: "🇸🇮"
				description: ""
			}
			ListElement {
				emoji: "🇸🇯"
				description: ""
			}
			ListElement {
				emoji: "🇸🇰"
				description: ""
			}
			ListElement {
				emoji: "🇸🇱"
				description: ""
			}
			ListElement {
				emoji: "🇸🇲"
				description: ""
			}
			ListElement {
				emoji: "🇸🇳"
				description: ""
			}
			ListElement {
				emoji: "🇸🇴"
				description: ""
			}
			ListElement {
				emoji: "🇸🇷"
				description: ""
			}
			ListElement {
				emoji: "🇸🇸"
				description: ""
			}
			ListElement {
				emoji: "🇸🇹"
				description: ""
			}
			ListElement {
				emoji: "🇸🇻"
				description: ""
			}
			ListElement {
				emoji: "🇸🇽"
				description: ""
			}
			ListElement {
				emoji: "🇸🇾"
				description: ""
			}
			ListElement {
				emoji: "🇸🇿"
				description: ""
			}
			ListElement {
				emoji: "🇹🇦"
				description: ""
			}
			ListElement {
				emoji: "🇹🇨"
				description: ""
			}
			ListElement {
				emoji: "🇹🇩"
				description: ""
			}
			ListElement {
				emoji: "🇹🇫"
				description: ""
			}
			ListElement {
				emoji: "🇹🇬"
				description: ""
			}
			ListElement {
				emoji: "🇹🇭"
				description: ""
			}
			ListElement {
				emoji: "🇹🇯"
				description: ""
			}
			ListElement {
				emoji: "🇹🇰"
				description: ""
			}
			ListElement {
				emoji: "🇹🇱"
				description: ""
			}
			ListElement {
				emoji: "🇹🇲"
				description: ""
			}
			ListElement {
				emoji: "🇹🇳"
				description: ""
			}
			ListElement {
				emoji: "🇹🇴"
				description: ""
			}
			ListElement {
				emoji: "🇹🇷"
				description: ""
			}
			ListElement {
				emoji: "🇹🇹"
				description: ""
			}
			ListElement {
				emoji: "🇹🇻"
				description: ""
			}
			ListElement {
				emoji: "🇹🇼"
				description: ""
			}
			ListElement {
				emoji: "🇹🇿"
				description: ""
			}
			ListElement {
				emoji: "🇺🇦"
				description: ""
			}
			ListElement {
				emoji: "🇺🇬"
				description: ""
			}
			ListElement {
				emoji: "🇺🇲"
				description: ""
			}
			ListElement {
				emoji: "🇺🇳"
				description: ""
			}
			ListElement {
				emoji: "🇺🇸"
				description: ""
			}
			ListElement {
				emoji: "🇺🇾"
				description: ""
			}
			ListElement {
				emoji: "🇺🇿"
				description: ""
			}
			ListElement {
				emoji: "🇻🇦"
				description: ""
			}
			ListElement {
				emoji: "🇻🇨"
				description: ""
			}
			ListElement {
				emoji: "🇻🇪"
				description: ""
			}
			ListElement {
				emoji: "🇻🇬"
				description: ""
			}
			ListElement {
				emoji: "🇻🇮"
				description: ""
			}
			ListElement {
				emoji: "🇻🇳"
				description: ""
			}
			ListElement {
				emoji: "🇻🇺"
				description: ""
			}
			ListElement {
				emoji: "🇼🇫"
				description: ""
			}
			ListElement {
				emoji: "🇼🇸"
				description: ""
			}
			ListElement {
				emoji: "🇽🇰"
				description: ""
			}
			ListElement {
				emoji: "🇾🇪"
				description: ""
			}
			ListElement {
				emoji: "🇾🇹"
				description: ""
			}
			ListElement {
				emoji: "🇿🇦"
				description: ""
			}
			ListElement {
				emoji: "🇿🇲"
				description: ""
			}
			ListElement {
				emoji: "🇿🇼"
				description: ""
			}
			ListElement {
				emoji: "🏴󠁧󠁢󠁥󠁮󠁧󠁿"
				description: ""
			}
			ListElement {
				emoji: "🏴󠁧󠁢󠁳󠁣󠁴󠁿"
				description: ""
			}
			ListElement {
				emoji: "🏴󠁧󠁢󠁷󠁬󠁳󠁿"
				description: ""
			}
		}

		delegate: Rectangle {
			height: listView.itemHeight
			width: listView.itemWidth

			color: (index & 1) ? "lightgrey" : "white"

			Label {
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.horizontalCenter
				anchors.bottom: parent.bottom

				text: emoji
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignHCenter
			}

			Text {
				anchors.top: parent.top
				anchors.left: parent.horizontalCenter
				anchors.right: parent.right
				anchors.bottom: parent.bottom

				text: emoji
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignHCenter
			}
		}
	}
}
