TEMPLATE = app
TARGET = queen-vanilla
QMAKE_BUNDLE = queen-vanilla

DEFINES += DEFAULT_ACE_PORT=443
DEFINES += DEFAULT_ACE_SECURE=true
DEFINES += DEFAULT_ACE_HOST=\\\"101.realm.cadabra.cloud\\\"

#DEFINES += TKS_ASDK
#DEFINES += INAPP_SDK
DEFINES += FFMPEG

ios {
	OTHER_FILES += $$PWD/vanilla.ios-asset/Info.plist
	QMAKE_INFO_PLIST = $$PWD/vanilla.ios-asset/Info.plist
	DISTFILES += $$PWD/vanilla.ios-asset/Info.plist

	ENTITLEMENTS.name = CODE_SIGN_ENTITLEMENTS
	CONFIG(release, debug|release) {
		ENTITLEMENTS.value = $$PWD/vanilla.ios-asset/Production.entitlements
		DISTFILES += $$PWD/vanilla.ios-asset/Production.entitlements
	} else {
		ENTITLEMENTS.value = $$PWD/vanilla.ios-asset/Development.entitlements
		DISTFILES += $$PWD/vanilla.ios-asset/Development.entitlements
	}
	QMAKE_MAC_XCODE_SETTINGS += ENTITLEMENTS

	IOS_LAUNCH_IMAGES.files += $$PWD/vanilla.ios-asset/VanillaLauncher.png
	IOS_LAUNCH_IMAGES.files += $$PWD/vanilla.ios-asset/VanillaLauncher.xib
	QMAKE_BUNDLE_DATA += IOS_LAUNCH_IMAGES

	QMAKE_ASSET_CATALOGS_APP_ICON = "Icon"
	QMAKE_ASSET_CATALOGS = $$PWD/vanilla.ios-asset/Images.xcassets
} android {
	ANDROID_PACKAGE_SOURCE_DIR = $$PWD/vanilla.android
}

!include(Queen.pri) {
	error("Couldn't find the Queen.pri")
}

include(Qaos/Qaos.pri)
RESOURCES += $$QAOS_RESOURCES
HEADERS += $$QAOS_HEADERS
SOURCES += $$QAOS_SOURCES

include(Core/Core.pri)
RESOURCES += $$CORE_RESOURCES
HEADERS += $$CORE_HEADERS
SOURCES += $$CORE_SOURCES

include(Repository/Repository.pri)
RESOURCES += $$REPOSITORY_RESOURCES
HEADERS += $$REPOSITORY_HEADERS
SOURCES += $$REPOSITORY_SOURCES

include(Conversation/Conversation.pri)
RESOURCES += $$CONVERSATION_RESOURCES
HEADERS += $$CONVERSATION_HEADERS
SOURCES += $$CONVERSATION_SOURCES

include(ERP/ERP.pri)
RESOURCES += $$ERP_RESOURCES
HEADERS += $$ERP_HEADERS
SOURCES += $$ERP_SOURCES

include(SD/SD.pri)
RESOURCES += $$SD_RESOURCES
HEADERS += $$SD_HEADERS
SOURCES += $$SD_SOURCES

include(EC/EC.pri)
RESOURCES += $$EC_RESOURCES
HEADERS += $$EC_HEADERS
SOURCES += $$EC_SOURCES

include(Mentalgami/Mentalgami.pri)
RESOURCES += $$MENTALGAMI_RESOURCES
HEADERS += $$MENTALGAMI_HEADERS
SOURCES += $$MENTALGAMI_SOURCES

include(DrOblozhko/DrOblozhko.pri)
RESOURCES += $$DROBLOZHKO_RESOURCES
HEADERS += $$DROBLOZHKO_HEADERS
SOURCES += $$DROBLOZHKO_SOURCES

RESOURCES += Vanilla.qrc
SOURCES += Vanilla.cpp
HEADERS += Vanilla.hpp
