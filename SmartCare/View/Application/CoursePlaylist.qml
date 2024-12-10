import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15
import QtQml 2.15 /// < don't move up because of QTBUG-81787

import Core.Global 0.1
import Core.Meta 0.1
import Core.System 0.1
import Core.View 0.1

import Repository.Controller 0.1

import SmartCare.Global 0.1
import SmartCare.View 0.1
import SmartCare.Model 0.1

import "../../View"

import "../../../Core/View"

import "../../../Qaos/QtQuick/ListView.js" as ListViewScript

Page {
	id: application

	Component.onCompleted: {
		console.log("CoursePlaylist::onCompleted");
	}

	Component.onDestruction: {
		console.log("CoursePlaylist::onDestruction");
	}

	property SmartCourse model: SmartCourse {
	} onModelChanged: {
		console.debug("CoursePlaylist::onModelChanged", application.model);
		modelConnection.target = model;
		if (modelConnection.target.isOpened) {
			modelConnection.onOpened();
		}
	}
	Connections {
		id: modelConnection

		target: model

		function onOpened()
		{
			console.log("CoursePlaylist.modelConnection::onOpened", "overview size: " + application.model.overviewSize, "index size: " + application.model.indexSize);
		}

		function onExceptionThrew(level, message)
		{
			if (level >= Core.FatalMessage) {
				Native.alertAction(qsTr("Error"), message, ["Ok"]);
			}
		}
	}

	/**
	 * @brief Progress controller
	 */
	property BusyBoard controller: BusyBoard {
	}

	Popup {
		id: helpPopup

		y: 0
		x: Math.round((parent.width - width) / 2)
		width: parent.width
		height: parent.height
		verticalPadding: 0
		horizontalPadding: 0

		parent: Overlay.overlay

		background: Rectangle {
			anchors.fill: parent
			color: "black"
			opacity: 0.8
		}

		visible: false
		onVisibleChanged: {
			if (!visible) {
				return ;
			}
			var option_button_point = stackBar.optionButton.mapToGlobal(0, 0);
			refreshButton.x = option_button_point.x /*+ ((stackBar.optionButton.width - refreshButton.width) / 2)*/;
			refreshButton.y = option_button_point.y /*+ ((stackBar.optionButton.height - refreshButton.height) / 2)*/;
		}

		Label {
			anchors.verticalCenter: refreshButton.verticalCenter
			anchors.left: descriptionLabel.left
			anchors.leftMargin: 4
			anchors.right: refreshButton.left
			anchors.rightMargin: 4

			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignRight

			font.pointSize: descriptionLabel.font.pointSize
			minimumPointSize: Theme.defaultFont.pointSize * 0.7
			fontSizeMode: Text.HorizontalFit
			elide: Text.ElideMiddle
			wrapMode: Text.WordWrap
			color: "white"

			text: qsTr("Items become available automatically but just in case here is a refresh button") + " →"
		}

		RoundButton {
			id: refreshButton

			padding: 0
			topInset: 0
			leftInset: 0
			bottomInset: 0
			rightInset: 0

			icon: stackBar.optionButton.icon

			onClicked: {
				if (modelConnection.target) {
					modelConnection.target.progress();
				}
				helpPopup.close();
			}
		}

		Label {
			id: descriptionLabel

			anchors.fill: parent

			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
			wrapMode: Text.WordWrap
			color: "white"

			text: listView.headerItem.currentItem ? listView.headerItem.currentItem.description : qsTr("Loading...")
		}

		RoundButton {
			text: "\u2713"

			anchors.bottom: parent.bottom
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.margins: 40

			onPressAndHold: {
				console.log("Hold on!");
			}

			onClicked: {
				helpPopup.close();
			}
		}
	}

	padding: 0
	leftInset: 0
	rightInset: 0

	header: StackBar {
		id: stackBar

		stackView: application.StackView.view

		titleLabel.visible: !listView.visibleHeader
		titleLabel.text: qsTr("Smart Course")

		subtitleLabel.visible: !listView.visibleHeader
		subtitleLabel.text: listView.headerItem.currentItem
							? listView.headerItem.currentItem.title
							: modelConnection.target.descriptor.objectSimplifiedName

		beatMode: (!modelConnection.target || listView.visibleHeader)
					? StackBar.BeatMode.None
					: (
						SmartCourse.Committing === (modelConnection.target.progressState & SmartCourse.Committing)
						? StackBar.BeatMode.Pulse
						: StackBar.BeatMode.Stable
					)
		accentColor: (!modelConnection.target || listView.visibleHeader)
					 ? Material.accentColor
					 : (
						SmartCourse.Modified === modelConnection.target.progressState
						? Material.color(Material.Red)
						: Material.accentColor
					 )
		backgroundGlass: !listView.visibleHeader

		onBackClicked: {
		}

		optionStyle: (!listView.visibleHeader || helpPopup.visible) ? StackBar.OptionStyle.Refresh : StackBar.OptionStyle.Help
		onOptionClicked: {
			if (StackBar.OptionStyle.Refresh  === optionStyle && modelConnection.target) {
				modelConnection.target.progress();
			} else {
				helpPopup.open();
			}
		}
	}

	background: Rectangle {
		color: header.backgroundColor
	}

	ListView {
		id: listView

		anchors.fill: parent
//		anchors.margins: 4

		readonly property int tracksProactive: 5
		readonly property real offsetSize: 50

		property int firstVisibleIndex: 0
		property int lastVisibleIndex: 0
		readonly property bool isReady: (!!application.model && application.model.isOpened && (listView.lastVisibleIndex - listView.firstVisibleIndex) > 1)

		property bool atPreactiveArea: false
		onAtPreactiveAreaChanged: {
			console.debug("CoursePlaylist.listView::onAtPreactiveAreaChanged", atPreactiveArea);
			/// @note Все нужное делаем в onAtSpareAreaChanged
		} Binding on atPreactiveArea {
			when: listView.isReady
			restoreMode: Binding.RestoreNone

			value: (listView.lastVisibleIndex - listView.firstVisibleIndex) <= listView.tracksProactive
		}

		property bool atProactiveArea: false
		onAtProactiveAreaChanged: {
			console.debug("CoursePlaylist.listView::onAtProactiveAreaChanged", atProactiveArea);
			if (atProactiveArea && application.model.indexState === SmartCourse.Partial && !application.model.index()) {
				console.log("CoursePlaylist.listView::onAtProactiveAreaChanged", "Index error");
			}
		} Binding on atProactiveArea {
			when: listView.isReady
			restoreMode: Binding.RestoreNone

			value: (listView.count - listView.lastVisibleIndex) <= listView.tracksProactive
		}

		property real spareY: 0
		onAtYBeginningChanged: {
			spareY = atYBeginning ? contentY : 0
		}

		property bool atSpareArea: false
		onAtSpareAreaChanged: {
			console.debug("CoursePlaylist.listView::onAtSpareAreaChanged", atSpareArea);
			if (atSpareArea && application.model.progressState === SmartCourse.Modified && !application.model.progress()) {
				console.log("CoursePlaylist.listView::onAtSpareAreaChanged", "Progress error");
			}
		} Binding on atSpareArea {
			when: listView.isReady
			restoreMode: Binding.RestoreNone

			value: listView.spareY < 0 && (listView.spareY - listView.contentY) > listView.offsetSize
		}

		property real extraY: 0
		onAtYEndChanged: {
			extraY = atYEnd ? contentY : 0
		}

		property bool atExtraArea: false
		onAtExtraAreaChanged: {
			console.debug("CoursePlaylist.listView::onAtExtraAreaChanged", atExtraArea);
			/// @note Все нужное делаем в onAtProactiveAreaChanged
		} Binding on atExtraArea {
			when: listView.isReady
			restoreMode: Binding.RestoreNone

			value: listView.extraY > 0 && listView.contentY - listView.extraY > listView.offsetSize
		}

		spacing: 8

		currentIndex: -1
		highlight: Rectangle {
			width: 200
			height: 50
			y: listView.currentItem.y
			z: 1

			color: Material.color(Material.Blue, Material.Shade100)
			opacity: 0.3

			Behavior on y {
				SpringAnimation {
					spring: 2
					damping: 0.1
				}
			}
		}

		readonly property bool visibleHeader: contentY < -20
		headerPositioning: ListView.InlineHeader
		header: SwipeView {
			interactive: false
			orientation: Qt.Horizontal
			//visible: application.model.briefModel

			Repeater {
				model: application.model.briefModel
				delegate: Column {

					required property url artwork
					required property string title
					required property string author
					required property string copyright
					required property string description
					required property date release_datetime
					required property int revision_code
					required property int language_code

					height: 100 // implicitHeight
					width: listView.width

					Item {
						id: coverItem

						anchors.horizontalCenter: parent.horizontalCenter

						width: listView.width * 0.7
						height: width

						Image {
							anchors.fill: parent
							anchors.horizontalCenter: parent.horizontalCenter
							sourceSize.height: height

							source: "/SmartCare/Asset/Image/CourseCover.png"
							visible: !coverImage.visible
							fillMode: Image.PreserveAspectFit
							verticalAlignment: Image.AlignVCenter
							horizontalAlignment: Image.AlignHCenter
						}

						Image {
							id: coverImage

							anchors.fill: parent
							anchors.horizontalCenter: parent.horizontalCenter
							sourceSize.height: height

							source: artwork
							visible: status === Image.Ready
							fillMode: Image.PreserveAspectFit
							verticalAlignment: Image.AlignVCenter
							horizontalAlignment: Image.AlignHCenter

							layer.enabled: true
							layer.effect: OpacityMask {
								maskSource: Item {
									width: coverImage.width
									height: coverImage.height
									Rectangle {
										anchors.fill: parent
										radius: 4
									}
								}
							}
						}
					}

					Label {
						width: parent.width - 20
						anchors.horizontalCenter: parent.horizontalCenter

						text: title
						elide: Text.ElideRight
						wrapMode: Text.WordWrap
						maximumLineCount: 3
						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter
//						fontSizeMode: Text.HorizontalFit
						font.weight: Font.ExtraBold
						font.pointSize: Theme.defaultFont.pointSize
					}

					Label {
						width: parent.width
						height: contentHeight
						leftPadding: 8
						rightPadding: 8

						text: author
						elide: Text.ElideRight
						wrapMode: Text.NoWrap
						color: "darkgrey"
						horizontalAlignment: Text.AlignHCenter
						font.weight: Font.Normal
						font.capitalization: Font.Capitalize
						font.pointSize: Theme.defaultFont.pointSize * 0.9
					}

					Label {
						id: authorLabel

						width: parent.width
						height: contentHeight

						property string accuracyCode: ""

						text: "%1 %2-%3-%4-%5-%6 %7".arg(copyright).arg(release_datetime.getFullYear()).arg(release_datetime.getMonth()).arg(release_datetime.getDate()).arg(revision_code).arg(language_code).arg(accuracyCode)
						elide: Text.ElideMiddle
						wrapMode: Text.NoWrap
						color: "darkgrey"
						horizontalAlignment: Text.AlignHCenter
						font.weight: Font.Light
						font.capitalization: Font.AllUppercase
						font.pointSize: Theme.defaultFont.pointSize * 0.6

						states: [
							State {
								name: "inactive"
								when: !application.model.activeLicense
								PropertyChanges {
									target: authorLabel
									accuracyCode: "\u2612" // ☒
								}
							},
							State {
								name: "busy"
								when: !application.model || !application.model.isOpened || SmartCourse.Committing === (application.model.progressState & SmartCourse.Committing)
								PropertyChanges {
									target: authorLabel
									accuracyCode: "\u27F3" // ⟳
								}
							},
							State {
								name: "idle"
								when: !!application.model && application.model.isOpened && SmartCourse.Actual === (application.model.progressState & SmartCourse.Modified)
								PropertyChanges {
									target: authorLabel
									accuracyCode: "\u00A9" // ©
								}
							},
							State {
								name: "ready"
								when: !!application.model && application.model.isOpened && (SmartCourse.Modified === application.model.progressState)
								PropertyChanges {
									target: authorLabel
									accuracyCode: "\u21BA" // ↺
								}
							}
						]
					}
				}
			}
		}

		footerPositioning: ListView.InlineFooter
		footer: Item {
			height: footerLabel.implicitHeight
			width: listView.width

			Label {
				id: footerLabel

				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right

				padding: 10

				horizontalAlignment: Text.AlignHCenter
				color: "lightgrey"
				font.weight: Font.Light
				font.pointSize: Theme.defaultFont.pointSize * 0.7

				states: [
					State {
						name: "inactive"
						when: !application.model.activeLicense
						PropertyChanges {
							target: footerLabel
							text: qsTr("Not available")
						}
					},
					State {
						name: "busy"
						when: !application.model || !application.model.isOpened || SmartCourse.Loading === (application.model.indexState & SmartCourse.Loading)
						PropertyChanges {
							target: footerLabel
							text: qsTr("Loading...")
						}
					},
					State {
						name: "idle"
						when: !!application.model && application.model.isOpened && SmartCourse.Complete === application.model.indexState
						PropertyChanges {
							target: footerLabel
							text: qsTr("This is the end")
						}
					},
					State {
						name: "ready"
						when: !!application.model && application.model.isOpened && SmartCourse.Partial === application.model.indexState
						PropertyChanges {
							target: footerLabel
							text: qsTr("Pull up to load")
						}
					}
				]
			}
		}

		model: application.model.trackModel

		delegate: ItemDelegate {
			id: trackItem

			width: listView.width
			height: implicitContentHeight

			enabled: application.model.activeLicense && !isFileDenied
			opacity: enabled ? 1 : 0.4
			Behavior on opacity {
				NumberAnimation {
					duration: 500
				}
			}

			readonly property bool isFileSigned: file.length > 0 && life_timestamp > new Date()
			readonly property bool isFileDenied: flag.includes('D') || flag.includes('d')

			property var file_promise: undefined

			required property int index

			required property variant subject_id

			required property string key
			required property int revision_code
			required property int language_code

			required property string title
			required property string description
			required property string artwork

			required property string flag
			required property variant weight

			required property string content_type
			required property int content_length
			required property int content_size

			required property string file
			required property date file_timestamp
			required property date life_timestamp

			required property variant meta
			required property date meta_timestamp

			required property double local_progress
			required property date local_progress_timestamp

			required property double remote_progress
			required property date remote_progress_timestamp

			onRemote_progressChanged: progress()
			onRemote_progress_timestampChanged: progress()

			onIsFileSignedChanged: {
				if (!file_promise) {
					return;
				} else if (!isFileSigned) {
					Native.alertAction(qsTr("Ah..."), qsTr("Access is denied because license is over, feel free to contact us."), ["Ok"]);
				} else  if (!trackLayout.model.sign(file, life_timestamp)) {
					Native.alertAction(qsTr("Ah..."), qsTr("License is expired, feel free to contact us."), ["Ok"]);
				} else if (!stack()) {
					Native.alertAction(qsTr("Ah..."), qsTr("Could not open a file, feel free to contact us."), ["Ok"]);
				}
				file_promise = undefined;
			}

			onClicked: {
				let promise = file_promise;
				listView.currentIndex = index;
				if (!!promise && promise.isBusy) {
					return ;
				} else if (!isFileSigned) {
					file_promise = application.model.file(key);
				} else if (!stack()) {
					Native.alertAction(qsTr("Ah..."), qsTr("Could not open a file, feel free to contact us."), ["Ok"]);
				}
			}

			function progress()
			{
				return !trackLayout.model || !trackLayout.model.isImported
						? false
						: trackLayout.model.update(remote_progress, remote_progress_timestamp)
				;
			}

			function stack()
			{
				var render = SchemeFactory.load("SmartCare.Meta.TrackRender", trackLayout.scheme);
				if (!render) {
					return false;
				}
				let component_url = undefined;
				component_url = render.makeURL(SmartCare.StackviewLayout);
				if (component_url.toString().length > 0) {
					var component = Qt.createComponent(component_url);
					if (component) {
						var view = application.StackView.view.push(component, {model: trackLayout.model}, StackView.PushTransition);
						if (view) {
							return true;
						}
					}
					console.error("Could not stack a file at " + trackLayout.model.file + " with component from " + component_url);
					return false;
				}
				component_url = render.makeURL(SmartCare.PopupviewLayout);
				if (component_url.toString().length > 0) {
					console.error("Could not popup a file at " + trackLayout.model.file + " with component from " + component_url);
					return false;
				} else if (!Qt.openUrlExternally(trackLayout.model.file)) {
					console.error("Could not open a file at " + trackLayout.model.file);
					return false;
				} else if (!trackLayout.model.progress(1)) {
					console.debug("Progress is ignored because is less than local or remote ones")
				}
				return true;
			}

			indicator: Image {
				id: trackImage

				anchors.top: parent.top
				anchors.left: parent.left
				anchors.leftMargin: listView.spacing
				anchors.bottom: parent.bottom
				width: trackItem.implicitContentHeight

				source: artwork
				sourceSize.width: width
				fillMode: Image.PreserveAspectFit
				verticalAlignment: Image.AlignVCenter
				horizontalAlignment: Image.AlignHCenter
				opacity: !!trackItem.file_promise ? 0.5 : 1
				layer.enabled: true
				layer.effect: OpacityMask {
					maskSource: Item {
						width: trackImage.width
						height: trackImage.height
						Rectangle {
							anchors.fill: parent
							radius: 4
						}
					}
				}

				BusyIndicator {
					anchors.verticalCenter: parent.verticalCenter
					anchors.horizontalCenter: parent.horizontalCenter
					width: parent.width * 0.4

					running: trackImage.opacity < 1
					visible: running
				}
			}

			contentItem: Item {
				anchors.left: parent.left
				anchors.leftMargin: (listView.spacing * 2) + indicator.width
				anchors.top: parent.top
				anchors.right: parent.right
				anchors.rightMargin: listView.spacing

				implicitHeight: trackLabel.implicitHeight + trackLayout.implicitHeight + listView.spacing

				Label {
					id: trackLabel

					anchors.left: parent.left
					anchors.top: parent.top
					anchors.right: parent.right
					height: implicitHeight

					text: title
					elide: Text.ElideRight

					font.pointSize: Theme.defaultFont.pointSize * 0.8
				}

				TrackLayout {
					id: trackLayout

					anchors.left: parent.left
					anchors.top: trackLabel.bottom
					anchors.right: parent.right
					height: implicitHeight

					Component.onCompleted: {
						represent(content_type, SmartCare.PlaylistLayout);
					}

					Connections {
						target: trackLayout.model
						enabled: !!target

						function onProgressed()
						{
							if (!application.model.progress(target.key, target.localProgress)) {
								console.debug("Could not update a progress of track " + target.key);
							}
						}
					}

					onRendered: {
						visible = hasModel && model.import(
															application.model,
															key, revision_code, language_code,
															title, description, artwork,
															flag, weight,
															content_type, content_size, content_length,
															life_timestamp, file, file_timestamp,
															meta, meta_timestamp,
															remote_progress, remote_progress_timestamp,
															local_progress, local_progress_timestamp
						);
					}
				}
			}

			background: Rectangle {
				height: trackItem.height
				width: trackItem.width

//				radius: 8
				opacity: enabled ? 1 : 0.3
				color: Material.color(Material.Grey, Material.Shade100)

				Label {
					anchors.top: parent.top
					anchors.right: parent.right
					anchors.bottom: parent.bottom
					topPadding: 0
					leftPadding: 0
					rightPadding: 0
					bottomPadding: 1

					text: (index + 1)
					fontSizeMode: Text.VerticalFit
					font.pointSize: Theme.defaultFont.pointSize * 10
					font.weight: Font.Black
					color: Material.color(Material.Grey, Material.Shade300)
					verticalAlignment: Text.AlignTop
					horizontalAlignment: Text.AlignRight
				}
				Rectangle {
					anchors.bottom: parent.bottom
					anchors.left: parent.left
					anchors.leftMargin: trackItem.indicator.width
					anchors.right: parent.right
					anchors.rightMargin: parent.radius
					height: 1

					color: Material.color(Material.Grey, Material.Shade300)
				}
			}
		}

		add: Transition {
			enabled: !(listView.moving || listView.dragging || listView.flicking)
			NumberAnimation {
				property: "x"
				duration: 250
				easing.type: Easing.OutBounce
			}
		}

		displaced: Transition {
			enabled: !(listView.moving || listView.dragging || listView.flicking)
			NumberAnimation {
				properties: "x"
				duration: 250
				easing.type: Easing.OutBounce
			}
		}

		onContentYChanged: {
			if (count <= 0) {
				return;
			}
			var index_map = ListViewScript.traverse(listView);
			[firstVisibleIndex, lastVisibleIndex] = [index_map[ListView.Beginning], index_map[ListView.End]];
		}
	}
}
