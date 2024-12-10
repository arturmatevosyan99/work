TEMPLATE = app
TARGET = queen-ladybug
QMAKE_BUNDLE = queen-ladybug

DEFINES += DEFAULT_ACE_PORT=80
DEFINES += DEFAULT_ACE_SECURE=false
DEFINES += DEFAULT_ACE_HOST=\\\"realm0.cadabra.systems\\\"

DEFINES += FFMPEG

ios {
	OTHER_FILES += $$PWD/ladybug.ios-asset/Info.plist
	QMAKE_INFO_PLIST = $$PWD/ladybug.ios-asset/Info.plist
	DISTFILES += $$PWD/ladybug.ios-asset/Info.plist

	ENTITLEMENTS.name = CODE_SIGN_ENTITLEMENTS
	CONFIG(release, debug|release) {
		ENTITLEMENTS.value = $$PWD/ladybug.ios-asset/Production.entitlements
		DISTFILES += $$PWD/ladybug.ios-asset/Production.entitlements
	} else {
		ENTITLEMENTS.value = $$PWD/ladybug.ios-asset/Development.entitlements
		DISTFILES += $$PWD/ladybug.ios-asset/Development.entitlements
	}
	QMAKE_MAC_XCODE_SETTINGS += ENTITLEMENTS
	message($$ENTITLEMENTS.name $$ENTITLEMENTS.value)

	IOS_LAUNCH_IMAGES.files += $$PWD/ladybug.ios-asset/LadybugLauncher.png
	IOS_LAUNCH_IMAGES.files += $$PWD/ladybug.ios-asset/LadybugLauncher.xib
	QMAKE_BUNDLE_DATA += IOS_LAUNCH_IMAGES

	QMAKE_ASSET_CATALOGS_APP_ICON = "Icon"
	QMAKE_ASSET_CATALOGS = $$PWD/ladybug.ios-asset/Images.xcassets
} android {
	ANDROID_PACKAGE_SOURCE_DIR = $$PWD/ladybug.android
}

QT += testlib gui
CONFIG += qml_debug
CONFIG += testcase

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

include(SmartCare/SmartCare.pri)
RESOURCES += $$SMARTCARE_RESOURCES
HEADERS += $$SMARTCARE_HEADERS
SOURCES += $$SMARTCARE_SOURCES

include(Mentalgami/Mentalgami.pri)
RESOURCES += $$MENTALGAMI_RESOURCES
HEADERS += $$MENTALGAMI_HEADERS
SOURCES += $$MENTALGAMI_SOURCES

include(DrOblozhko/DrOblozhko.pri)
RESOURCES += $$DROBLOZHKO_RESOURCES
HEADERS += $$DROBLOZHKO_HEADERS
SOURCES += $$DROBLOZHKO_SOURCES

include(Stress/Stress.pri)
RESOURCES += $$STRESS_RESOURCES
HEADERS += $$STRESS_HEADERS
SOURCES += $$STRESS_SOURCES

RESOURCES += Ladybug.qrc
SOURCES += Ladybug.cpp
HEADERS += Ladybug.hpp
