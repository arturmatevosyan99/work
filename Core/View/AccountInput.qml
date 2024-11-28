import QtQuick 2.15
import QtQuick.Controls 2.15

FocusScope {
	id: rootScope

	enum ValidatorType {
		None,
		EmailOrPhone,
		EmailAddress,
		PhoneNumber
	}

	signal editingFinished(string value)

	property alias isAcceptable: textField.acceptableInput
	property alias radius: backgroundRectangle.radius
	property alias color: backgroundRectangle.color
	property alias text: textField.text
	property alias placeholderText: textField.placeholderText
	property alias textFontPointSize: textField.font.pointSize

	property int validator: AccountInput.ValidatorType.None
	onValidatorChanged: {
		switch (validator) {
			case AccountInput.ValidatorType.None:
				textField.validator.regularExpression = /.*/;
				break;

			case AccountInput.ValidatorType.EmailOrPhone:
				textField.validator.regularExpression = /(\+[0-9\-(). ]+)|(\w+([\-+_.]\w+)*@\w+([\-+_.]\w+)*)/;
				break;

			case AccountInput.ValidatorType.EmailAddress:
				textField.validator.regularExpression = /(\w+([\-+_.]\w+)*@\w+([\-+_.]\w+)*)/;
				break;

			case AccountInput.ValidatorType.PhoneNumber:
				textField.validator.regularExpression = /(\+[0-9\-(). ]+)/;
				break;

			default:
				textField.validator.regularExpression = /.*/;
		}
	}

	TextField {
		id: textField

		focus: true
		anchors.fill: parent
		topPadding: 6
		bottomPadding: 6
		rightPadding: 4
		leftPadding: 4
		maximumLength: 100
		placeholderText: parent.placeholderText

		validator: RegularExpressionValidator {
		}

		background: Rectangle {
			id: backgroundRectangle

			color: "white"
			border.color: "transparent"
		}

		onTextEdited: {
			if (text.length <= 0) {
				inputMethodHints = Qt.ImhNone;
			} else if (text.charAt(0) == '+' && (rootScope.validator === AccountInput.ValidatorType.EmailOrPhone || rootScope.validator === AccountInput.ValidatorType.PhoneNumber)) {
				inputMethodHints = Qt.ImhDialableCharactersOnly;
			} else if (rootScope.validator === AccountInput.ValidatorType.EmailOrPhone || rootScope.validator === AccountInput.ValidatorType.EmailAddress) {
				inputMethodHints = Qt.ImhEmailCharactersOnly;
			} else {
				inputMethodHints =  Qt.ImhNone;
			}
		}

		onPreeditTextChanged: {
			Qt.inputMethod.commit();
		}

		onAccepted: {
			rootScope.editingFinished(textField.text);
		}
	}
}
