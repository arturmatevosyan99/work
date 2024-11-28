VERSION = 24.11.0

QT += qml
QT += quick
QT += quickcontrols2
QT += concurrent
QT += webview
QT += websockets
QT += sensors
QT += positioning
QT += multimedia
QT += sql
QT += svg
QT += xml
CONFIG += sdk_no_version_check
CONFIG += c++11
CONFIG += c++1z
CONFIG += c++17
CONFIG += lrelease
CONFIG -= -std=gnu++11
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-private-field

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QUEEN_FRAMEWORK_VERSION=$$shell_quote(\"0.$$VERSION\")
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000	# disables all the APIs deprecated before Qt 6.0.0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML2_IMPORT_PATH += $$PWD

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH += $$PWD

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

defineReplace(androidVersionCode) {
	segments = $$split(1, ".")
	for (segment, segments): vCode = "$$first(vCode)$$format_number($$segment, width=3 zeropad)"

	suffix = 0

#	contains(ANDROID_TARGET_ARCH, armeabi-v7a): suffix = 0
#	else:contains(ANDROID_TARGET_ARCH, arm64-v8a): suffix = 1
#	else:contains(ANDROID_TARGET_ARCH, x86): suffix = 2
#	else:contains(ANDROID_TARGET_ARCH, x86_64): suffix = 3

	return($$first(vCode)$$first(suffix))
}

defineReplace(iosShortVersion) {
	segments = $$split(1, ".")

	return($$member(segments, 0).$$member(segments, 1))
}

defineReplace(androidArch) {
	contains(ANDROID_TARGET_ARCH, armeabi-v7a): retval = arm
	else:contains(ANDROID_TARGET_ARCH, arm64-v8a): retval = arm64
	else:contains(ANDROID_TARGET_ARCH, x86): retval = x86
	else:contains(ANDROID_TARGET_ARCH, x86_64): retval = x86_64

	return($$retval)
}

defineReplace(packageDotNotationName) {
	segments = $$split(1, "-")

	return($$member(segments, 0).$$member(segments, 1))
}

CONFIG(debug, debug|release) {
	QML_IMPORT_TRACE = 1
	QUEEN_BUILD_TYPE = Debug
} else {
	QUEEN_BUILD_TYPE = Release
}

OBJECTS_DIR = ./$$TARGET-$$ANDROID_TARGET_ARCH-build/obj
MOC_DIR = ./$$TARGET-$$ANDROID_TARGET_ARCH-build/moc
RCC_DIR = ./$$TARGET-$$ANDROID_TARGET_ARCH-build/rcc
UI_DIR = ./$$TARGET-$$ANDROID_TARGET_ARCH-build/ui

RESOURCES += $$PWD/Queen.qrc

TRANSLATIONS += \
	$$PWD/English.ts \
	$$PWD/Russian.ts

HEADERS += \
	$$PWD/Queen.hpp \
	$$PWD/Skeleton.hpp \
	$$PWD/Bone.hpp \
	$$PWD/Body.hpp \
	$$PWD/Cupboard.hpp \
	$$PWD/Environment.hpp

SOURCES += \
	$$PWD/Queen.cpp \
	$$PWD/Skeleton.cpp \
	$$PWD/Bone.cpp \
	$$PWD/Body.cpp \
	$$PWD/Cupboard.cpp \
	$$PWD/Environment.cpp

QMAKE_TARGET_BUNDLE_PREFIX = "systems.cadabra"

ios {
	message(Building iOS app for `$$QMAKE_HOST.arch/$$QT_ARCH` platform...)
	DESTDIR = ./
	CONFIG(iphoneos, iphoneos|iphonesimulator) {
		QUEEN_ARCH = $$QMAKE_HOST.arch
		QUEEN_IOS_SDK = iphoneos
	} CONFIG(iphonesimulator, iphoneos|iphonesimulator) {
		QUEEN_ARCH = $$QMAKE_HOST.arch
		QUEEN_IOS_SDK = iphonesimulator
		QMAKE_APPLE_DEVICE_ARCHS = $$QMAKE_HOST.arch
	}

	QMAKE_FULL_VERSION = $$VERSION
	QMAKE_SHORT_VERSION = $$VERSION # $$iosShortVersion($$QMAKE_FULL_VERSION)

	QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1
	QMAKE_IOS_DEPLOYMENT_TARGET = 12.0

	defined(APPLE_PROVISIONING_PROFILE_UID, var) {
		QPROFILE.name = PROVISIONING_PROFILE_SPECIFIER
		QPROFILE.value = $$APPLE_PROVISIONING_PROFILE_UID
		QMAKE_MAC_XCODE_SETTINGS += QPROFILE
	}

	defined(APPLE_DEVELOPMENT_TEAM_KEY, var) {
		QTEAM.name = DEVELOPMENT_TEAM
		QTEAM.value = $$APPLE_DEVELOPMENT_TEAM_KEY
		QMAKE_MAC_XCODE_SETTINGS += QTEAM
	}

	defined(APPLE_CODE_SIGN_IDENTITY, var) {
		QSIGN.name = CODE_SIGN_IDENTITY
		QSIGN.value = $$APPLE_CODE_SIGN_IDENTITY
		QMAKE_MAC_XCODE_SETTINGS += QSIGN
	}

	CONFIG -= bitcode

	QMAKE_RPATHDIR += @executable_path/Frameworks

	LIBS += -lz
	LIBS += -framework IOKit
	LIBS += -framework UIKit
	LIBS += -framework HealthKit
	LIBS += -framework StoreKit
	LIBS += -framework AudioToolbox
} android {
	message(Building Android app for `$$QMAKE_HOST.arch/$$QT_ARCH` platform...)
	DESTDIR = ./$$TARGET-$$ANDROID_TARGET_ARCH-build/
	QUEEN_ARCH = $$ANDROID_TARGET_ARCH

	QUEEN_ANDROID_NDK_PATH = $$(ANDROID_NDK_ROOT)

	ANDROID_APPLICATION_ID = $$packageDotNotationName("$${QMAKE_TARGET_BUNDLE_PREFIX}.$${QMAKE_BUNDLE}")
	ANDROID_VERSION_NAME = $$VERSION
	ANDROID_VERSION_CODE = $$androidVersionCode($$ANDROID_VERSION_NAME)
	ANDROID_API_VERSION = 13 # 12 = 31, 13 = 33
	ANDROID_MIN_SDK_VERSION = 23
	ANDROID_TARGET_SDK_VERSION = 33

	mkpath($$OUT_PWD/android-build)
	QUEEN_PROP = ext { "queenApplicationId = '$$ANDROID_APPLICATION_ID'" "queenVersionCode = '$$ANDROID_VERSION_CODE'" "queenVersionName = '$$ANDROID_VERSION_NAME'" }
	QUEEN_PROP_FILE = $$absolute_path($$OUT_PWD/android-build/queen.properties)
	!write_file($$QUEEN_PROP_FILE, QUEEN_PROP) {
		warning(Could not create version file)
	}

	CONFIG += object_parallel_to_source
	QT += androidextras
	LIBS += -lz

	QUEEN_JAVA_SOURCES.files = $$files($$PWD/queen.android/src/systems/cadabra/queen/*.java)
	QUEEN_JAVA_SOURCES.path = /java/systems/cadabra/queen
	INSTALLS += QUEEN_JAVA_SOURCES
} macx {
	message(Building macOS app for `$$QMAKE_HOST.arch/$$QT_ARCH` platform...)
	DESTDIR = ./
	QUEEN_ARCH = $$QMAKE_HOST.arch

	QMAKE_FULL_VERSION = $$VERSION
	QMAKE_SHORT_VERSION = $$VERSION # $$iosShortVersion($$QMAKE_FULL_VERSION)

	QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 13.0

	defined(APPLE_PROVISIONING_PROFILE_UID, var) {
		QPROFILE.name = PROVISIONING_PROFILE_SPECIFIER
		QPROFILE.value = $$APPLE_PROVISIONING_PROFILE_UID
		QMAKE_MAC_XCODE_SETTINGS += QPROFILE
	}

	defined(APPLE_DEVELOPMENT_TEAM_KEY, var) {
		QTEAM.name = DEVELOPMENT_TEAM
		QTEAM.value = $$APPLE_DEVELOPMENT_TEAM_KEY
		QMAKE_MAC_XCODE_SETTINGS += QTEAM
	}

	defined(APPLE_CODE_SIGN_IDENTITY, var) {
		QSIGN.name = CODE_SIGN_IDENTITY
		QSIGN.value = $$APPLE_CODE_SIGN_IDENTITY
		QMAKE_MAC_XCODE_SETTINGS += QSIGN
	}

	CONFIG -= bitcode
	CONFIG += object_parallel_to_source

	QMAKE_RPATHDIR += @executable_path/Frameworks

	LIBS += -lz
	LIBS += -framework CoreFoundation
	LIBS += -framework Security
	LIBS += -framework VideoToolbox
	LIBS += -framework CoreMedia
	LIBS += -framework CoreVideo
} linux:!android {
	message(Building linux app for `$$QMAKE_HOST.arch/$$QT_ARCH` platform...)
	DESTDIR = ./
	QUEEN_ARCH = $$QMAKE_HOST.arch

	QMAKE_FULL_VERSION = $$VERSION
	QMAKE_SHORT_VERSION = $$VERSION

	CONFIG += object_parallel_to_source

	QMAKE_RPATHDIR += @executable_path/Frameworks

	LIBS += -lz
}
