import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQml.Models 2.15
import QtGraphicalEffects 1.15

import Core.View 0.1
import Core.System 0.1
import Core.Meta 0.1

import EC.Global 0.1
import EC.Model 0.1
import EC.Data 0.1

import SD.Global 0.1

import "../../../Core/View"

import "../../../Repository/View"

Page {
	id: application

	property OnlineShop model: OnlineShop {
/*
		showroom: [
			Showcase {
				id: historyShowcase

				catalogue: Showcase.Order
				filter: []
			}
		]
*/
	} onModelChanged: {
		console.debug("CustomerAccount::onModelChanged", application.model);
		modelConnection.target = model;
		application.model.opened();
	}

	Connections {
		id: modelConnection
		target: application.model

		function onOpened() {
			application.model.extractAccount(application.model.descriptor.opt("customer_subject_id"));
		}

		function onShowcaseLoaded(success) {

		}

		function onAccountExtracted(account) {
			if (account.subjectQd === 0) {
				listView.model.setProperty(0, "caption", qsTr("Could not load :("));
				return ;
			}

			listView.model.clear();
			/// @brief Subject
			listView.model.append({"section": qsTr("Subject"), "caption": "@%1".arg(account.subjectQd), "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// < at icon
			if (account.subjectName != account.subjectQd) {
				listView.model.append({"section": qsTr("Subject"), "caption": "@%1".arg(account.subjectName), "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// < at icon
			}
			if (account.subjectDescription.length > 0) {
				listView.model.append({"section": qsTr("Subject"), "caption": account.subjectDescription, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"});
			}
			if (account.subjectTimezone.length > 0) {
				listView.model.append({"section": qsTr("Subject"), "caption": account.subjectTimezone, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// < timezone icon
			}
			if (account.subjectLocale.length > 0) {
				listView.model.append({"section": qsTr("Subject"), "caption": account.subjectLocale, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// < locale icon
			}
			/// @brief Contacts
			if (account.legalType !== SD.UndefinedPerson) {
				var legal_type = qsTr("Counterparty");
				switch (account.legalType) {
					case SD.PhysicalPerson:
						legal_type = qsTr("Physical person");
						break;
					case SD.JuridicalPerson:
						legal_type = qsTr("Juridical person");
						break;
					case SD.LimitedLabilityCompany:
						legal_type = qsTr("Limited lability company");
						break;
					case SD.Company:
						legal_type = qsTr("Company");
						break;
				}
				listView.model.append({"section": legal_type, "caption": account.legalName, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"});
				if (account.legalPhoneNumber.length > 0) {
					listView.model.append({"section": legal_type, "caption": account.legalPhoneNumber, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// < phone icon
				}
				if (account.legalEmailAddress.length > 0) {
					listView.model.append({"section": legal_type, "caption": account.legalEmailAddress, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// < email icon
				}
				if (account.legalComment.length > 0) {
					listView.model.append({"section": legal_type, "caption": account.legalComment, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// <  comment icon
				}
				if (account.legalCountryCode.length > 0) {
					listView.model.append({"section": legal_type, "caption": account.legalCountryCode, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// < country flag icon
				}
				if (account.legalPostCode.length > 0) {
					listView.model.append({"section": legal_type, "caption": account.legalPostCode, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// <  zip icon
				}
				if (account.legalPostAddress1.length > 0) {
					listView.model.append({"section": legal_type, "caption": account.legalPostAddress1, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// <  address icon
				}
				if (account.legalPostAddress2.length > 0) {
					listView.model.append({"section": legal_type, "caption": account.legalPostAddress2, "ico": "/DrOblozhko/Asset/Image/InfoCircle.svg"}); /// <  address icon
				}
			}

			/// @todo Counteryparty contacts
			/// @todo Licenses (Activate, Prolongate, Cancel)
			/// @todo Orders (Last 10 Purchase Orders)
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		titleText: qsTr("Customer Account")

		stackView: application.StackView.view

		onBackClicked: {
		}

		optionStyle: StackBar.OptionStyle.None
		onOptionClicked: {
			optionMenu.open();
		}

		Menu {
			id: optionMenu

			parent: stackBar.optionButton
			topMargin: stackBar.optionButton.height

			Component.onCompleted: {
			}

			MenuItem {
				text: qsTr("Close")
				onTriggered: {
					optionMenu.close();
				}
			}
		}
	}

	ListView {
		id: listView

		anchors.left: parent.left
		anchors.top: parent.top
		width: parent.width
		height: parent.height

		orientation: Qt.Vertical

		model: ListModel {
			ListElement {
				caption: qsTr("Loading...")
				ico: "/DrOblozhko/Asset/Image/InfoCircle.svg"
				section: ""
			}
		}

		delegate: CallbackDelegate {
			required property string caption
			required property string ico
			required property string section

			height: implicitContentHeight * 2
			width: listView.width

			iconText: caption
			iconSource: ico
		}

		section {
			property: "section"
			criteria: ViewSection.FullString
			delegate: Rectangle {
				required property string section

				width: ListView.view.width
				height: childrenRect.height
				color: "transparent"
				border.width: 0

				Text {
					text: parent.section
					font.bold: true
					font.pixelSize: 16
					color: "lightsteelblue"
					padding: 4
				}
			}
		}
	}
}
