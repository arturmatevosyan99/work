import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import QtQml 2.15
import QtWebView 1.15 as QtWeb

import "./"

QtWeb.WebView {
	id: rootView

	signal loaded(int progress)
	signal failed(string message)
	signal bypassed(string extension)

	property url ual
	property string title: ""
	property var downloadList: ["pdf", "rtf", "doc", "docx", "xls", "xlsx"]
	property alias preset: rootView.presetLoader.item
	property bool presetDefault: false
	property string presetAlias: ""
	onPresetAliasChanged: {
		if (presetAlias.length <= 0) {
			return;
		} else if (rootView.presetLoader[presetAlias] instanceof Component) {
			rootView.presetLoader.sourceComponent = rootView.presetLoader[presetAlias];
		} else {
			rootView.presetLoader.sourceComponent = null;
			rootView.presetAlias = "";
		}
	}
	property Loader presetLoader: Loader {
		property Component ria: Component {
			Item {
				id: riaPreset

				onParentChanged: {
					if (!parent) {
						return ;
					} else if (!!parent.StackView && !!parent.StackView.view) {
						stackBar.parent = parent;
						parent.header = riaPreset.stackBar;
					}
				}

				property StackBar stackBar: StackBar {
					stackView: riaPreset.parent && riaPreset.parent.StackView && riaPreset.parent.StackView.view ? riaPreset.parent.StackView.view : null

					titleText: qsTr("Web Application")
					subtitleText: rootView.title

					beatMode: rootView.loading ? StackBar.BeatMode.Pulse : StackBar.BeatMode.Stable

					optionStyle: StackBar.OptionStyle.None

					backBehaviour: StackBar.BackBehaviour.Signal
					onBackClicked: {
						if (rootView.canGoBack) {
							rootView.goBack();
						} else {
							pop();
						}
					}
				}
			}
		}

		property Component notion: Component {
			Item {
				id: notionPreset

				onParentChanged: {
					if (!parent) {
						return ;
					} else if (!!parent.StackView && !!parent.StackView.view) {
						stackBar.parent = parent;
						parent.header = notionPreset.stackBar;
					}
				}

				property StackBar stackBar: StackBar {
					stackView: notionPreset.parent && notionPreset.parent.StackView && notionPreset.parent.StackView.view ? notionPreset.parent.StackView.view : null

					titleText: qsTr("Web Application")
					subtitleText: rootView.title
					beatMode: rootView.loading ? StackBar.BeatMode.Pulse : StackBar.BeatMode.Stable

					backBehaviour: StackBar.BackBehaviour.Signal
					onBackClicked: {
						if (rootView.canGoBack) {
							rootView.goBack();
						} else {
							pop();
						}
					}

					optionStyle: StackBar.OptionStyle.Magnifier
					optionEnabled: !rootView.loading
					optionOpacity: optionEnabled ? 1.0 : 0.5
					onOptionClicked: {
						rootView.runJavaScript("window.dispatchEvent(new KeyboardEvent('keydown', {'metaKey': true, 'keyCode': 80}))", function(result) { console.log(result); });
					}
				}
			}
		}
	}

	onUalChanged: {
		if (rootView.presetAlias.length <= 0) {
			const raw_url = rootView.ual.toString();
			if (raw_url.includes("notion.so/") || raw_url.includes("notion.site/")) {
				rootView.presetAlias = "notion";
			} else if (rootView.presetDefault) {
				rootView.presetAlias = "ria";
			}
		}
		rootView.url = ual;
	}

	onLoadingChanged: {
		var url = loadRequest.url.toString();
		if (loadRequest.status == WebView.LoadSucceededStatus) {
			console.log("WebView::onLoadingChanged: 100%! (" + url + ")");
			rootView.loaded(100);
			return ;
		} else if (loadRequest.status == WebView.LoadFailedStatus) {
			rootView.failed(loadRequest.errorString);
			console.warn("WebView::onLoadingChanged: " + loadRequest.errorString + " (" + url + ")");
			return ;
		}
		console.debug("WebView::onLoadingChanged: " + rootView.loadProgress + "% (" + url + ")");
		rootView.loaded(rootView.loadProgress);
		for (var i = 0; i < rootView.downloadList.length; i++) {
			if (url.endsWith("." + rootView.downloadList[i])) {
				console.log("WebView::onLoadingChanged: Treating as a file...");
				rootView.stop();
				rootView.bypassed(rootView.downloadList[i]);
				Qt.openUrlExternally(loadRequest.url);
			}
		}
	}
}
