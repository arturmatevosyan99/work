CORE_RESOURCES = $$PWD/Core.qrc

CORE_HEADERS = \
	$$PWD/Core.hpp \
	$$PWD/Data/AbstractCommand.hpp \
	$$PWD/Data/AbstractEntity.hpp \
	$$PWD/Data/AbstractExpression.hpp \
	$$PWD/Data/AbstractPredicate.hpp \
	$$PWD/Data/Command/EntityClose.hpp \
	$$PWD/Data/Command/EntityCommand.hpp \
	$$PWD/Data/Command/EntityFetch.hpp \
	$$PWD/Data/Command/EntityMutate.hpp \
	$$PWD/Data/Command/EntityOpen.hpp \
	$$PWD/Data/Command/EntityReopen.hpp \
	$$PWD/Data/Command/ProviderClone.hpp \
	$$PWD/Data/Command/ProviderConnect.hpp \
	$$PWD/Data/Command/ProviderDisconnect.hpp \
	$$PWD/Data/Event.hpp \
	$$PWD/Data/Expression/FieldExpression.hpp \
	$$PWD/Data/Expression/RoundExpression.hpp \
	$$PWD/Data/Factory.hpp \
	$$PWD/Data/Field.hpp \
	$$PWD/Data/ForeignEntity.hpp \
	$$PWD/Data/ForeignKey.hpp \
	$$PWD/Data/Predicate/BetweenPredicate.hpp \
	$$PWD/Data/Predicate/BitflagPredicate.hpp \
	$$PWD/Data/Predicate/CompoundPredicate.hpp \
	$$PWD/Data/Predicate/ConditionPredicate.hpp \
	$$PWD/Data/Predicate/GroupPredicate.hpp \
	$$PWD/Data/Predicate/InEntityPredicate.hpp \
	$$PWD/Data/Predicate/InPredicate.hpp \
	$$PWD/Data/Predicate/JoinPredicate.hpp \
	$$PWD/Data/Predicate/LikePredicate.hpp \
	$$PWD/Data/Predicate/NotLikePredicate.hpp \
	$$PWD/Data/Predicate/OrderPredicate.hpp \
	$$PWD/Data/Predicate/WherePredicate.hpp \
	$$PWD/Data/Provider.hpp \
	$$PWD/Data/QueryStream.hpp \
	$$PWD/Data/Rowset.hpp \
	$$PWD/Data/Rowset/ShadedRowset.hpp \
	$$PWD/Data/Rowset/StaticRowset.hpp \
	$$PWD/Data/Rowset/TeleRowset.hpp \
	$$PWD/Data/Rowset/VersionedRowset.hpp \
	$$PWD/Meta/AbstractScheme.hpp \
	$$PWD/Meta/Duration.hpp \
	$$PWD/Meta/DynamicRole.hpp \
	$$PWD/Meta/Filter.hpp \
	$$PWD/Meta/ITunesItem.hpp \
	$$PWD/Meta/MapModel.hpp \
	$$PWD/Meta/ObjectList.hpp \
	$$PWD/Meta/Order.hpp \
	$$PWD/Meta/PropertyList.hpp \
	$$PWD/Meta/Qd.hpp \
	$$PWD/Meta/Font.hpp \
	$$PWD/Meta/Scheme.hpp \
	$$PWD/Meta/Scheme/RenderScheme.hpp \
	$$PWD/Meta/Scheme/TestScheme.hpp \
	$$PWD/Model/AbstractEvent.hpp \
	$$PWD/Model/Event.hpp \
	$$PWD/Model/MediaInfo.hpp \
	$$PWD/Model/MenuCallback.hpp \
	$$PWD/Model/SortFilterProxyModel.hpp \
	$$PWD/Net/AbstractResource.hpp \
	$$PWD/Net/Call/CRPCall.hpp \
	$$PWD/Net/Call/CRPDeferredCall.hpp \
	$$PWD/Net/Call/CRPDelay.hpp \
	$$PWD/Net/Call/CRPFuture.hpp \
	$$PWD/Net/Call/CRPPromise.hpp \
	$$PWD/Net/Call/CRPInterface.hpp \
	$$PWD/Net/Call/CRPReaction.hpp \
	$$PWD/Net/Call/CRPReactor.hpp \
	$$PWD/Net/Call/DiagnosticCall.hpp \
	$$PWD/Net/Call/File/CubeFile.hpp \
	$$PWD/Net/Call/File/QRCFile.hpp \
	$$PWD/Net/Call/FileCall.hpp \
	$$PWD/Net/Call/Grab/CubeGrab.hpp \
	$$PWD/Net/Call/Grab/YoutubeGrab.hpp \
	$$PWD/Net/Call/ITunesGrab.hpp \
	$$PWD/Net/Call/MediaGrab.hpp \
	$$PWD/Net/Endpoint.hpp \
	$$PWD/Net/Proto/Scheme.hpp \
	$$PWD/Net/ServiceCall.hpp \
	$$PWD/System/AudioFile.hpp \
	$$PWD/System/AudioRecorder.hpp \
	$$PWD/System/AudioRole.hpp \
	$$PWD/System/AudioStream.hpp \
	$$PWD/System/DiagnosticBundle.hpp \
	$$PWD/System/FitnessTracker.hpp \
	$$PWD/System/ImageThumbnail.hpp \
	$$PWD/System/Native.hpp \
	$$PWD/System/Alert.hpp \
	$$PWD/System/Alert/AlertOption.hpp \
	$$PWD/System/Alert/DialogAlert.hpp \
	$$PWD/System/Alert/StateAlert.hpp \
	$$PWD/System/Alert/InputAlert.hpp \
	$$PWD/System/Alert/ActionAlert.hpp \
	$$PWD/System/Alert/SoundAlert.hpp \
	$$PWD/System/Preference.hpp \
	$$PWD/System/NotificationService.hpp \
	$$PWD/System/NotificationCenter.hpp \
	$$PWD/System/UserTracker.hpp \
	$$PWD/Test/AbstractCase.hpp \
	$$PWD/Test/AbstractShocker.hpp \
	$$PWD/Test/Assert.hpp \
	$$PWD/Test/Case/GeneralCase.hpp \
	$$PWD/Test/Case/FirstCase.hpp \
	$$PWD/Test/Case/PetriCase.hpp \
	$$PWD/Test/Node.hpp \
	$$PWD/Test/Node/AbstractWaiter.hpp \
	$$PWD/Test/Node/Action.hpp \
	$$PWD/Test/Node/Assertion.hpp \
	$$PWD/Test/Node/Port.hpp \
	$$PWD/Test/Node/Teleport.hpp \
	$$PWD/Test/Node/Shock.hpp \
	$$PWD/Test/Node/Waiter/GroupWaiter.hpp \
	$$PWD/Test/Node/Waiter/ObjectWaiter.hpp \
	$$PWD/Test/Node/Waiter/SignalWaiter.hpp \
	$$PWD/Test/Node/Waiter/TimeWaiter.hpp \
	$$PWD/Test/Node/Waiter/WaiterCallback.hpp \
	$$PWD/Test/PetrI.hpp \
	$$PWD/Test/Context.hpp \
	$$PWD/Test/Token.hpp \
	$$PWD/View/AlertDialog.hpp \
	$$PWD/View/ProgressCircle.hpp \
	$$PWD/View/StatusBar.hpp \
	$$PWD/View/StateIsland.hpp \
	$$PWD/View/Theme.hpp \
	$$PWD/View/WindowManager.hpp

CORE_SOURCES = \
	$$PWD/Core.cpp \
	$$PWD/Data/AbstractCommand.cpp \
	$$PWD/Data/AbstractEntity.cpp \
	$$PWD/Data/AbstractExpression.cpp \
	$$PWD/Data/AbstractPredicate.cpp \
	$$PWD/Data/Command/EntityClose.cpp \
	$$PWD/Data/Command/EntityCommand.cpp \
	$$PWD/Data/Command/EntityFetch.cpp \
	$$PWD/Data/Command/EntityMutate.cpp \
	$$PWD/Data/Command/EntityOpen.cpp \
	$$PWD/Data/Command/EntityReopen.cpp \
	$$PWD/Data/Command/ProviderClone.cpp \
	$$PWD/Data/Command/ProviderConnect.cpp \
	$$PWD/Data/Command/ProviderDisconnect.cpp \
	$$PWD/Data/Event.cpp \
	$$PWD/Data/Expression/FieldExpression.cpp \
	$$PWD/Data/Expression/RoundExpression.cpp \
	$$PWD/Data/Factory.cpp \
	$$PWD/Data/Field.cpp \
	$$PWD/Data/ForeignEntity.cpp \
	$$PWD/Data/ForeignKey.cpp \
	$$PWD/Data/Predicate/BetweenPredicate.cpp \
	$$PWD/Data/Predicate/BitflagPredicate.cpp \
	$$PWD/Data/Predicate/CompoundPredicate.cpp \
	$$PWD/Data/Predicate/ConditionPredicate.cpp \
	$$PWD/Data/Predicate/GroupPredicate.cpp \
	$$PWD/Data/Predicate/InEntityPredicate.cpp \
	$$PWD/Data/Predicate/InPredicate.cpp \
	$$PWD/Data/Predicate/JoinPredicate.cpp \
	$$PWD/Data/Predicate/LikePredicate.cpp \
	$$PWD/Data/Predicate/NotLikePredicate.cpp \
	$$PWD/Data/Predicate/OrderPredicate.cpp \
	$$PWD/Data/Predicate/WherePredicate.cpp \
	$$PWD/Data/Provider.cpp \
	$$PWD/Data/QueryStream.cpp \
	$$PWD/Data/Rowset.cpp \
	$$PWD/Data/Rowset/ShadedRowset.cpp \
	$$PWD/Data/Rowset/StaticRowset.cpp \
	$$PWD/Data/Rowset/TeleRowset.cpp \
	$$PWD/Data/Rowset/VersionedRowset.cpp \
	$$PWD/Meta/AbstractScheme.cpp \
	$$PWD/Meta/Duration.cpp \
	$$PWD/Meta/DynamicRole.cpp \
	$$PWD/Meta/Filter.cpp \
	$$PWD/Meta/ITunesItem.cpp \
	$$PWD/Meta/MapModel.cpp \
	$$PWD/Meta/ObjectList.cpp \
	$$PWD/Meta/Order.cpp \
	$$PWD/Meta/PropertyList.cpp \
	$$PWD/Meta/Qd.cpp \
	$$PWD/Meta/Font.cpp \
	$$PWD/Meta/Scheme.cpp \
	$$PWD/Meta/Scheme/RenderScheme.cpp \
	$$PWD/Meta/Scheme/TestScheme.cpp \
	$$PWD/Model/AbstractEvent.cpp \
	$$PWD/Model/Event.cpp \
	$$PWD/Model/MediaInfo.cpp \
	$$PWD/Model/MenuCallback.cpp \
	$$PWD/Model/SortFilterProxyModel.cpp \
	$$PWD/Net/AbstractResource.cpp \
	$$PWD/Net/Call/CRPCall.cpp \
	$$PWD/Net/Call/CRPDeferredCall.cpp \
	$$PWD/Net/Call/CRPDelay.cpp \
	$$PWD/Net/Call/CRPFuture.cpp \
	$$PWD/Net/Call/CRPPromise.cpp \
	$$PWD/Net/Call/CRPInterface.cpp \
	$$PWD/Net/Call/CRPReaction.cpp \
	$$PWD/Net/Call/CRPReactor.cpp \
	$$PWD/Net/Call/DiagnosticCall.cpp \
	$$PWD/Net/Call/File/CubeFile.cpp \
	$$PWD/Net/Call/File/QRCFile.cpp \
	$$PWD/Net/Call/FileCall.cpp \
	$$PWD/Net/Call/Grab/CubeGrab.cpp \
	$$PWD/Net/Call/Grab/YoutubeGrab.cpp \
	$$PWD/Net/Call/ITunesGrab.cpp \
	$$PWD/Net/Call/MediaGrab.cpp \
	$$PWD/Net/Endpoint.cpp \
	$$PWD/Net/Proto/Scheme.cpp \
	$$PWD/Net/ServiceCall.cpp \
	$$PWD/System/AudioFile.cpp \
	$$PWD/System/AudioRecorder.cpp \
	$$PWD/System/AudioRole.cpp \
	$$PWD/System/AudioStream.cpp \
	$$PWD/System/DiagnosticBundle.cpp \
	$$PWD/System/FitnessTracker.cpp \
	$$PWD/System/ImageThumbnail.cpp \
	$$PWD/System/Native.cpp \
	$$PWD/System/Alert.cpp \
	$$PWD/System/Alert/AlertOption.cpp \
	$$PWD/System/Alert/DialogAlert.cpp \
	$$PWD/System/Alert/ActionAlert.cpp \
	$$PWD/System/Alert/StateAlert.cpp \
	$$PWD/System/Alert/InputAlert.cpp \
	$$PWD/System/Alert/SoundAlert.cpp \
	$$PWD/System/Preference.cpp \
	$$PWD/System/NotificationCenter.cpp \
	$$PWD/System/NotificationService.cpp \
	$$PWD/System/UserTracker.cpp \
	$$PWD/Test/AbstractCase.cpp \
	$$PWD/Test/AbstractShocker.cpp \
	$$PWD/Test/Assert.cpp \
	$$PWD/Test/Case/GeneralCase.cpp \
	$$PWD/Test/Case/FirstCase.cpp \
	$$PWD/Test/Case/PetriCase.cpp \
	$$PWD/Test/Node.cpp \
	$$PWD/Test/Node/AbstractWaiter.cpp \
	$$PWD/Test/Node/Action.cpp \
	$$PWD/Test/Node/Assertion.cpp \
	$$PWD/Test/Node/Port.cpp \
	$$PWD/Test/Node/Teleport.cpp \
	$$PWD/Test/Node/Shock.cpp \
	$$PWD/Test/Node/Waiter/GroupWaiter.cpp \
	$$PWD/Test/Node/Waiter/ObjectWaiter.cpp \
	$$PWD/Test/Node/Waiter/SignalWaiter.cpp \
	$$PWD/Test/Node/Waiter/TimeWaiter.cpp \
	$$PWD/Test/Node/Waiter/WaiterCallback.cpp \
	$$PWD/Test/PetrI.cpp \
	$$PWD/Test/Context.cpp \
	$$PWD/Test/Token.cpp \
	$$PWD/View/AlertDialog.cpp \
	$$PWD/View/ProgressCircle.cpp \
	$$PWD/View/StatusBar.cpp \
	$$PWD/View/StateIsland.cpp \
	$$PWD/View/Theme.cpp \
	$$PWD/View/WindowManager.cpp

contains(QT, testlib) {
#	CORE_RESOURCES += $$PWD/Test/CoreTest.qrc
	CORE_SOURCES += $$PWD/Test/CoreTest.cpp

#	CORE_HEADERS += $$files($$PWD/Test/*.hpp, true)
#	CORE_SOURCES += $$files($$PWD/Test/*.cpp, true)

#	CORE_HEADERS += $$files($$PWD/Shocker/*.hpp, true)
#	CORE_SOURCES += $$files($$PWD/Shocker/*.cpp, true)
}

ios {
	CUSTOM_CORE_FONTS.files += $$PWD/Asset/Font/Inter.Variable.ttf
	QMAKE_BUNDLE_DATA += CUSTOM_CORE_FONTS

	OBJECTIVE_HEADERS += \
				$$PWD/System/Native/iOS.h \
				$$PWD/System/NotificationService/APNService.h

	OBJECTIVE_SOURCES += \
				$$PWD/System/Native/iOS.mm \
				$$PWD/System/Alert/iOSAlert.mm \
				$$PWD/System/AudioRole/iOSAudioRole.mm \
				$$PWD/System/NotificationService/APNService.mm \
				$$PWD/System/FitnessTracker/HealthKit.mm \
				$$PWD/View/StatusBar/iOSStatusBar.mm \
				$$PWD/View/WindowManager/iOSWindowManager.mm

	LIBS += -framework UserNotifications
} android {
	CORE_HEADERS += \
			$$PWD/System/Native/Android.hpp \
			$$PWD/System/NotificationService/AndroidNotificationService.hpp \
			$$PWD/System/NotificationService/FCMService.hpp \
			$$PWD/System/NotificationService/HCMService.hpp \
			$$PWD/System/NotificationService/RSMService.hpp

	CORE_SOURCES += \
			$$PWD/System/Native/Android.cpp \
			$$PWD/System/Alert/AndroidAlert.cpp \
			$$PWD/System/AudioRole/AndroidAudioRole.cpp \
			$$PWD/System/NotificationService/AndroidNotificationService.cpp \
			$$PWD/System/NotificationService/FCMService.cpp \
			$$PWD/System/NotificationService/HCMService.cpp \
			$$PWD/System/NotificationService/RSMService.cpp \
			$$PWD/System/FitnessTracker/GoogleFit.cpp \
			$$PWD/View/StatusBar/AndroidStatusBar.cpp \
			$$PWD/View/WindowManager/AndroidWindowManager.cpp
} linux:!android {
	CORE_HEADERS += \
			$$PWD/System/Native/Linux.hpp

	CORE_SOURCES += \
			$$PWD/System/Native/Linux.cpp \
			$$PWD/System/Alert/LinuxAlert.cpp \
			$$PWD/System/AudioRole/LinuxAudioRole.cpp \
			$$PWD/System/FitnessTracker/FitGami.cpp \
			$$PWD/View/WindowManager/LinuxWindowManager.cpp \
			$$PWD/View/StatusBar/LinuxStatusBar.cpp
} macx {
	CUSTOM_CORE_FONTS.files += $$PWD/Asset/Font/Inter.Variable.ttf
	QMAKE_BUNDLE_DATA += CUSTOM_CORE_FONTS

#	CORE_HEADERS += \

	CORE_SOURCES += \
			$$PWD/System/FitnessTracker/FitGami.cpp

	OBJECTIVE_HEADERS += \
				$$PWD/System/Native/macOS.h

	OBJECTIVE_SOURCES += \
				$$PWD/System/Native/macOS.mm \
				$$PWD/System/Alert/macOSAlert.mm \
				$$PWD/System/AudioRole/macOSAudioRole.mm \
				$$PWD/View/WindowManager/macOSWindowManager.mm \
				$$PWD/View/StatusBar/macOSStatusBar.mm
} bsd {
	CORE_HEADERS += \
			$$PWD/System/Native/BSD.hpp

	CORE_SOURCES += \
			$$PWD/System/Native/BSD.cpp \
			$$PWD/System/Alert/BSDAlert.cpp
} windows {
	CORE_HEADERS += \
			$$PWD/System/Native/Windows.hpp

	CORE_SOURCES += \
			$$PWD/System/Native/Windows.cpp \
			$$PWD/System/Alert/WindowsAlert.cpp \
			$$PWD/System/FitnessTracker/FitGami.cpp
}

android {
	!defined(ANDROID_LIBOPENSSL_PATH, var) {
		ANDROID_LIBOPENSSL_PATH = $$OUT_PWD/android-build/openssl
		!exists($${ANDROID_LIBOPENSSL_PATH}/$${ANDROID_TARGET_ARCH}/lib*) {
			system(../3party/ssl/android-build.sh $$ANDROID_TARGET_ARCH $$ANDROID_LIBOPENSSL_PATH $$QUEEN_ANDROID_NDK_PATH):SSL_BUILD=TRUE
			!equals(SSL_BUILD,TRUE) {
				error("OpenSSL for Android does not exist at $$ANDROID_LIBOPENSSL_PATH")
			}
		}
	}
	ANDROID_ABIS = $${ANDROID_ABIS}
	for(ANDROID_ABI, ANDROID_ABIS) {
		ANDROID_EXTRA_LIBS += \
				$$ANDROID_LIBOPENSSL_PATH/$${ANDROID_ABI}/libcrypto_1_1.so \
				$$ANDROID_LIBOPENSSL_PATH/$${ANDROID_ABI}/libssl_1_1.so
	}
}

android {
	defined(FCM_SDK, var) {
		DEFINES += FCM_SDK=\\\"$${FCM_SDK}\\\"
	}
	defined(HCM_SDK, var) {
		DEFINES += HCM_SDK=\\\"$${HCM_SDK}\\\"
	}
	defined(RSM_SDK, var) {
		DEFINES += RSM_SDK=\\\"$${RSM_SDK}\\\"
	}
} ios {
	defined(APS_KIT, var) {
		DEFINES += APS_KIT=\\\"$${APS_KIT}\\\"
	}
}

contains(DEFINES, USERTRACKER_SDK) {
	USERTRACKERSDK_BUILD=TRUE
	ios {
		OBJECTIVE_SOURCES += $$PWD/System/UserTracker/iOSUserTracker.mm
	} else:android {
		CORE_SOURCES += $$PWD/System/UserTracker/AndroidUserTracker.cpp
	} else {
		USERTRACKERSDK_BUILD=FALSE
		unset(USERTRACKER_SDK)
	}
}

contains(DEFINES, FFMPEG) {
	# @todo Check path according to $$QUEEN_ARCH
	FFMPEG_BUILD=TRUE
	ios {
		!defined(FFMPEG_BUILD_PATH, var) {
			FFMPEG_BUILD_PATH=$${OUT_PWD}/ffmpeg
			!exists($${FFMPEG_BUILD_PATH}/*.framework) {
				message(FFmpeg building...):FFMPEG_BUILD=FALSE
				system(../3party/ffmpeg/ios-build.sh $$QUEEN_ARCH $$QUEEN_IOS_SDK $$FFMPEG_BUILD_PATH):FFMPEG_BUILD=TRUE
			}
		}
	} android {
		!defined(FFMPEG_BUILD_PATH, var) {
			FFMPEG_BUILD_PATH=$${OUT_PWD}/android-build/ffmpeg
			!exists($${FFMPEG_BUILD_PATH}/$${ANDROID_TARGET_ARCH}/lib/lib*.so) {
				message(FFmpeg building...):FFMPEG_BUILD=FALSE
				system(../3party/ffmpeg/android-build.sh $$ANDROID_TARGET_ARCH $$FFMPEG_BUILD_PATH $$QUEEN_ANDROID_NDK_PATH):FFMPEG_BUILD=TRUE
			}
		}
	} macx {
		!defined(FFMPEG_BUILD_PATH, var) {
			FFMPEG_BUILD_PATH=$${OUT_PWD}/ffmpeg
			!exists($${FFMPEG_BUILD_PATH}/*.framework) {
				message(FFmpeg building...):FFMPEG_BUILD=FALSE
				system(../3party/ffmpeg/macos-build.sh $$QUEEN_ARCH $$QUEEN_IOS_SDK $$FFMPEG_BUILD_PATH):FFMPEG_BUILD=TRUE
			}
		}
	} linux:!android {
#		!defined(FFMPEG_BUILD_PATH, var) {
#			FFMPEG_BUILD_PATH=$${OUT_PWD}/ffmpeg
#			!exists($${FFMPEG_BUILD_PATH}/$${QUEEN_ARCH}/lib/lib*.so) {
#				message(FFmpeg building...):FFMPEG_BUILD=FALSE
#				system(../3party/ffmpeg/linux-build.sh $$FFMPEG_BUILD_PATH):FFMPEG_BUILD=TRUE
#			}
#		}
	}

	!equals(FFMPEG_BUILD,TRUE) {
		error(FFmpeg does not exist at $$FFMPEG_BUILD_PATH)
	} else {
		message(FFmpeg Ok)
	}

	ios {
		LIBS += -liconv
		INCLUDEPATH += "$${FFMPEG_BUILD_PATH}/$${QUEEN_ARCH}/include"
		LIBS += -F$${FFMPEG_BUILD_PATH}/
		LIBS += -framework libavcodec
		LIBS += -framework libavdevice
		LIBS += -framework libavfilter
		LIBS += -framework libavformat
		LIBS += -framework libavutil
		LIBS += -framework libswresample
		LIBS += -framework libswscale
	} android {
		INCLUDEPATH += "$${FFMPEG_BUILD_PATH}/$${ANDROID_TARGET_ARCH}/include"
		LIBS += -L$${FFMPEG_BUILD_PATH}/$${ANDROID_TARGET_ARCH}/lib/
		LIBS += -lavcodec
		LIBS += -lavdevice
		LIBS += -lavfilter
		LIBS += -lavformat
		LIBS += -lavutil
		LIBS += -lswresample
		LIBS += -lswscale

		ANDROID_ABIS = $${ANDROID_ABIS}
		for(ANDROID_ABI, ANDROID_ABIS) {
			ANDROID_EXTRA_LIBS += $${FFMPEG_BUILD_PATH}/$${ANDROID_ABI}/lib/libavcodec.so
			ANDROID_EXTRA_LIBS += $${FFMPEG_BUILD_PATH}/$${ANDROID_ABI}/lib/libavdevice.so
			ANDROID_EXTRA_LIBS += $${FFMPEG_BUILD_PATH}/$${ANDROID_ABI}/lib/libavfilter.so
			ANDROID_EXTRA_LIBS += $${FFMPEG_BUILD_PATH}/$${ANDROID_ABI}/lib/libavformat.so
			ANDROID_EXTRA_LIBS += $${FFMPEG_BUILD_PATH}/$${ANDROID_ABI}/lib/libavutil.so
			ANDROID_EXTRA_LIBS += $${FFMPEG_BUILD_PATH}/$${ANDROID_ABI}/lib/libswresample.so
			ANDROID_EXTRA_LIBS += $${FFMPEG_BUILD_PATH}/$${ANDROID_ABI}/lib/libswscale.so
		}
	} macx {
		LIBS += -liconv
		INCLUDEPATH += "$${FFMPEG_BUILD_PATH}/$${QUEEN_ARCH}/include"
		LIBS += -F$${FFMPEG_BUILD_PATH}/
		LIBS += -framework libavcodec
		LIBS += -framework libavdevice
		LIBS += -framework libavfilter
		LIBS += -framework libavformat
		LIBS += -framework libavutil
		LIBS += -framework libswresample
		LIBS += -framework libswscale
	} linux:!android {
		LIBS += -lavcodec
		LIBS += -lavdevice
		LIBS += -lavfilter
		LIBS += -lavformat
		LIBS += -lavutil
		LIBS += -lswresample
		LIBS += -lswscale
	}
}
